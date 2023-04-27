#include <cstdint>
#include <queue>
#include <array>
#include <mutex>
#include <iostream>
#include <chrono>
#include <thread>

#define THREAD_COUNT 8

using namespace std::chrono_literals;

enum class ThreadCommunicationDirection : uint8_t {
  Send = 0,
  Recv = 1,
};

struct ThreadAction {
  ThreadCommunicationDirection direction;
  int node_id_source;
  int node_id_dest;
  int send_value;
  bool was_consumed;
};

std::array<std::queue<ThreadAction>, THREAD_COUNT> thread_queues;
std::mutex thread_queues_mutex;

void Send(int node_id_dest, int value, int node_id_source) {
  // Enqueue the Send action into our queue.
  {
    std::lock_guard guard(thread_queues_mutex);
    thread_queues[node_id_source].push(
      { ThreadCommunicationDirection::Send, node_id_source, node_id_dest, value });
  }
  // Block until the dest node has a matching Recv call from us.
  while (1) {
    {
      std::lock_guard guard(thread_queues_mutex);
      // Recv() handles marking our pending Send action as consumed, we should just
      // remove it here.
      if (!thread_queues[node_id_source].empty()) {
        const auto& action = thread_queues[node_id_source].front();
        if (action.direction == ThreadCommunicationDirection::Send
            && action.node_id_source == node_id_source
            && action.was_consumed) {
          thread_queues[node_id_source].pop();
          return;
        }
      }
    }
    // Just quick hack to avoid signals
    std::this_thread::sleep_for(20ms);
  }
}

int Recv(int node_id_source, int node_id_dest) {
  // Enqueue the Recv action into our queue.
  {
    std::lock_guard guard(thread_queues_mutex);
    thread_queues[node_id_dest].push(
      { ThreadCommunicationDirection::Recv, node_id_source, node_id_dest });
  }
  // Block until the source node has a matching Send call to us.
  while (1) {
    {
      std::lock_guard guard(thread_queues_mutex);
      // If the source node has a pending Send call to our dest node,
      // we should complete the Send() and Recv() pair.
      if (!thread_queues[node_id_source].empty()) {
        auto& action = thread_queues[node_id_source].front();
        if (action.direction == ThreadCommunicationDirection::Send
            && action.node_id_dest == node_id_dest
            && !action.was_consumed) {
          // Discard the Send() and Recv() pair here as we need the value.
          thread_queues[node_id_dest].pop();
          // We don't remove the Send() action from the source node queue.
          action.was_consumed = true;
          return action.send_value;
        }
      }
    }
    // Just quick hack to avoid signals
    std::this_thread::sleep_for(20ms);
  }
}

int GetSum(int node_count, int my_node_id, int my_value) {
  int sum = my_value;
  // Start at 2^0
  int power_of_2 = 1;
  while (1) {
    int next_power_of_2 = power_of_2 * 2;
    // If this node is on the left split in the current level of the binary
    // tree, it sends partial sum to the right.
    bool is_on_left = (my_node_id % next_power_of_2) < power_of_2;
    // Sign of the power of 2 offset we should add to our node index.
    int sign = is_on_left ? 1 : -1;
    // Choose the partner node we will send to and receive from.
    int partner = my_node_id + sign * power_of_2;
    // Stop if calculated partner is outside the range of nodes which exist.
    if (partner >= node_count || partner < 0) {
      break;
    }
    // Synchronize our partial sum with the partner node which has the
    // corresponding partial sum from the other side of the binary tree at the
    // current level.
    if (is_on_left) {
      // If our node is the partner on the left, we first send to the partner
      // and then receive from the partner.
      Send(partner, sum, my_node_id);
      sum += Recv(partner, my_node_id);
    }
    else {
      // If our node is the partner on the right, we first receive from and then
      // send to the partner.
      int temp_sum = Recv(partner, my_node_id);
      Send(partner, sum, my_node_id);
      // Delay adding the partial sum we received from our partner until we've
      // sent our own partial sum to the partner.
      sum += temp_sum;
    }
    // Move on to the next level in the binary tree.
    power_of_2 = next_power_of_2;
  }

  std::cout << "Node " << my_node_id << " sum: " << sum << std::endl;
  return sum;
}

int main() {
  std::vector<std::thread> threads;
  threads.emplace_back(GetSum, 8, 0, 1);
  threads.emplace_back(GetSum, 8, 1, 1);
  threads.emplace_back(GetSum, 8, 2, 1);
  threads.emplace_back(GetSum, 8, 3, 1);
  threads.emplace_back(GetSum, 8, 4, 1);
  threads.emplace_back(GetSum, 8, 5, 1);
  threads.emplace_back(GetSum, 8, 6, 1);
  threads.emplace_back(GetSum, 8, 7, 1);

  for (auto& thread : threads) {
    thread.join();
  }
}
