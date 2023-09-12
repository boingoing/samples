#include <cstddef>
#include <cstdint>
#include <iostream>
#include <unordered_set>
#include <vector>

#include "test/TestCase.h"

// Implement packet-level buffer for unreliable network

#define packet_byte_size 2
#define data_type uint8_t

struct Packet {
  uint32_t sequence_number;
  data_type data[packet_byte_size];
};

uint32_t window_start_sequence_number = 0;
uint32_t latest_received_sequence_number = 0;
uint32_t contiguous_sequence_number_count = 0;
std::vector<data_type> data_buffer;
std::unordered_set<uint32_t> received_sequence_numbers;

class UdpBufferTest : public TestCase {
 protected:
  void InitBuffer() {
    data_buffer.resize(256);
  }

  void UpdateWindowContiguousSequenceNumbers() {
    // We currently say |window_start_sequence_number + contiguous_sequence_number_count| is the end of contiguous data in the data buffer.
    // Starting at the next sequence number, increment |contiguous_sequence_number_count| for each received contiguous packet.
    for (auto next = window_start_sequence_number + contiguous_sequence_number_count; next <= latest_received_sequence_number; next++) {
      // Finding sequence number in |received_sequence_numbers| means we have received that packet.
      if (received_sequence_numbers.find(next) == received_sequence_numbers.cend()) {
        return;
      }
      // We found the next sequence number, otherwise we would have returned.
      contiguous_sequence_number_count++;
    }
  }

  void ReceiveFromNetwork(Packet& p) {
    const auto data_index = (p.sequence_number - window_start_sequence_number) * std::size(p.data);
    for (size_t i = 0; i < std::size(p.data); i++) {
      data_buffer[data_index + i] = p.data[i];
    }
    received_sequence_numbers.insert(p.sequence_number);
    latest_received_sequence_number = std::max(latest_received_sequence_number, p.sequence_number);

    UpdateWindowContiguousSequenceNumbers();
  }

  void SendToListener(std::vector<data_type>& data) {
    if (contiguous_sequence_number_count == 0) {
      return;
    }

    const auto bytes_to_read = contiguous_sequence_number_count * packet_byte_size;

    // Copy the contiguous data from our buffer.
    // We copy n bytes starting at the beginning of the buffer where n is the number of
    // pending contiguous received packets times the size of the data from those packets.
    data.assign(data_buffer.cbegin(), data_buffer.cbegin() + bytes_to_read);

    // Remove the data we copied for the listener from our buffer
    data_buffer.erase(data_buffer.cbegin(), data_buffer.cbegin() + bytes_to_read);

    // Move forward the sequence number at which the window starts
    window_start_sequence_number += contiguous_sequence_number_count;

    // We now have no contiguous data in the buffer (it's been flushed)
    contiguous_sequence_number_count = 0;
  }

  void ReceiveTestPacketFromNetwork(uint32_t sequence_number) {
    Packet p;
    p.sequence_number = sequence_number;
    // TODO: memset?
    for (size_t i = 0; i < std::size(p.data); i++) {
      p.data[i] = static_cast<data_type>(sequence_number);
    }
    ReceiveFromNetwork(p);
  }

  void PrintDataBuffer() {
    trace << "Window starting sequence number: " << window_start_sequence_number << std::endl;
    trace << "Latest received sequence number: " << latest_received_sequence_number << std::endl;
    trace << "Available contiguous packets: " << contiguous_sequence_number_count << std::endl;
    trace << "Data: " << std::endl;
    trace << "{ ";
    if (latest_received_sequence_number >= window_start_sequence_number && received_sequence_numbers.find(latest_received_sequence_number) != received_sequence_numbers.cend()) {
      for (auto iter = data_buffer.cbegin(); iter != data_buffer.cbegin() + (((latest_received_sequence_number - window_start_sequence_number) + 1) * packet_byte_size); iter++) {
        trace << static_cast<int>(*iter) << ", ";
      }
    }
    trace << "}" << std::endl;
  }

  void VerifyDataBuffer(std::vector<data_type> expected) {
    // data_buffer is always allocated, we only want to verify expected.size() bytes.
    const std::vector<data_type> trunc_data_buffer(data_buffer.cbegin(), data_buffer.cbegin() + expected.size());
    assert.equal(trunc_data_buffer, expected);
  }

  void VerifySequenceNumbers(uint32_t window_start, uint32_t latest_received, uint32_t available_contiguous) {
    assert.equal(window_start_sequence_number, window_start);
    assert.equal(latest_received_sequence_number, latest_received);
    assert.equal(contiguous_sequence_number_count, available_contiguous);
  }
};

TEST_CASE(UdpBufferTest, base_test) {
  InitBuffer();

  trace << "Initial buffer state:" << std::endl;
  PrintDataBuffer();
  VerifySequenceNumbers(0, 0, 0);
  VerifyDataBuffer({});

  ReceiveTestPacketFromNetwork(0);
  ReceiveTestPacketFromNetwork(1);
  ReceiveTestPacketFromNetwork(2);
  ReceiveTestPacketFromNetwork(3);
  trace << "After receiving packets 0-3:" << std::endl;
  PrintDataBuffer();
  VerifySequenceNumbers(0, 3, 4);
  VerifyDataBuffer({0, 0, 1, 1, 2, 2, 3, 3});

  std::vector<data_type> out;
  SendToListener(out);
  trace << "After delivering packets up to 3:" << std::endl;
  PrintDataBuffer();
  VerifySequenceNumbers(4, 3, 0);
  VerifyDataBuffer({});

  ReceiveTestPacketFromNetwork(4);
  trace << "After receiving packet 4:" << std::endl;
  PrintDataBuffer();
  VerifySequenceNumbers(4, 4, 1);
  VerifyDataBuffer({4, 4});

  ReceiveTestPacketFromNetwork(6);
  trace << "After receiving packet 6:" << std::endl;
  PrintDataBuffer();
  VerifySequenceNumbers(4, 6, 1);
  VerifyDataBuffer({4, 4, 0, 0, 6, 6});

  ReceiveTestPacketFromNetwork(8);
  trace << "After receiving packet 8:" << std::endl;
  PrintDataBuffer();
  VerifySequenceNumbers(4, 8, 1);
  VerifyDataBuffer({4, 4, 0, 0, 6, 6, 0, 0, 8, 8});

  ReceiveTestPacketFromNetwork(7);
  trace << "After receiving packet 7:" << std::endl;
  PrintDataBuffer();
  VerifySequenceNumbers(4, 8, 1);
  VerifyDataBuffer({4, 4, 0, 0, 6, 6, 7, 7, 8, 8});

  ReceiveTestPacketFromNetwork(5);
  trace << "After receiving packet 5:" << std::endl;
  PrintDataBuffer();
  VerifySequenceNumbers(4, 8, 5);
  VerifyDataBuffer({4, 4, 5, 5, 6, 6, 7, 7, 8, 8});

  SendToListener(out);
  trace << "After delivering packets up to 8:" << std::endl;
  PrintDataBuffer();
  VerifySequenceNumbers(9, 8, 0);
  VerifyDataBuffer({});

  ReceiveTestPacketFromNetwork(10);
  trace << "After receiving packet 10:" << std::endl;
  PrintDataBuffer();
  VerifySequenceNumbers(9, 10, 0);
  VerifyDataBuffer({0, 0, 10, 10});
}
