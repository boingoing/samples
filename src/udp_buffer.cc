#include <cstddef>
#include <cstdint>
#include <iostream>
#include <unordered_set>
#include <vector>

// Implement packet-level buffer for unreliable network

#define packet_byte_size 2

struct Packet {
  uint32_t sequence_number;
  std::byte data[packet_byte_size];
};

uint32_t window_start_sequence_number = 0;
uint32_t latest_received_sequence_number = 0;
uint32_t contiguous_sequence_number_count = 0;
std::vector<std::byte> data_buffer;
std::unordered_set<uint32_t> received_sequence_numbers;

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

void SendToListener(std::vector<std::byte>& data) {
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
    p.data[i] = std::byte(sequence_number);
  }
  ReceiveFromNetwork(p);
}

void PrintDataBuffer() {
  std::cout << "Window starting sequence number: " << window_start_sequence_number << std::endl;
  std::cout << "Latest received sequence number: " << latest_received_sequence_number << std::endl;
  std::cout << "Available contiguous packets: " << contiguous_sequence_number_count << std::endl;
  std::cout << "Data: " << std::endl;
  std::cout << "{ ";
  if (latest_received_sequence_number >= window_start_sequence_number && received_sequence_numbers.find(latest_received_sequence_number) != received_sequence_numbers.cend()) {
    for (auto iter = data_buffer.cbegin(); iter != data_buffer.cbegin() + (((latest_received_sequence_number - window_start_sequence_number) + 1) * packet_byte_size); iter++) {
      std::cout << std::to_integer<int>(*iter) << ", ";
    }
  }
  std::cout << "}" << std::endl;
}

int main() {
  InitBuffer();

  std::cout << "Initial buffer state:" << std::endl;
  PrintDataBuffer();
  ReceiveTestPacketFromNetwork(0);
  ReceiveTestPacketFromNetwork(1);
  ReceiveTestPacketFromNetwork(2);
  ReceiveTestPacketFromNetwork(3);
  std::cout << "After receiving packets 0-3:" << std::endl;
  PrintDataBuffer();

  std::vector<std::byte> out;
  SendToListener(out);
  std::cout << "After delivering packets up to 3:" << std::endl;
  PrintDataBuffer();

  ReceiveTestPacketFromNetwork(4);
  std::cout << "After receiving packet 4:" << std::endl;
  PrintDataBuffer();

  ReceiveTestPacketFromNetwork(6);
  std::cout << "After receiving packet 6:" << std::endl;
  PrintDataBuffer();

  ReceiveTestPacketFromNetwork(8);
  std::cout << "After receiving packet 8:" << std::endl;
  PrintDataBuffer();

  ReceiveTestPacketFromNetwork(7);
  std::cout << "After receiving packet 7:" << std::endl;
  PrintDataBuffer();

  ReceiveTestPacketFromNetwork(5);
  std::cout << "After receiving packet 5:" << std::endl;
  PrintDataBuffer();

  SendToListener(out);
  std::cout << "After delivering packets up to 8:" << std::endl;
  PrintDataBuffer();

  ReceiveTestPacketFromNetwork(10);
  std::cout << "After receiving packet 10:" << std::endl;
  PrintDataBuffer();
}
