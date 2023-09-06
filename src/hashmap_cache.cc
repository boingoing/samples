#include <chrono>
#include <cstddef>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <map>
#include <mutex>
#include <unordered_map>

#include "hashmap_cache.h"
#include "helpers.h"

template <typename KeyType, typename ValueType>
class Slot {
 public:
  KeyType key;
  ValueType value;
  std::chrono::time_point<std::chrono::system_clock> access_time;
};

template <typename KeyType, typename ValueType>
class SlotContainer : public std::vector<Slot<KeyType, ValueType>> {
};

template <typename SlotContainer>
struct LeastRecentlyUsedReplacementAlgorithm {
  size_t operator()(const SlotContainer& container) const noexcept {
    const auto now = std::chrono::system_clock::now();
    auto delta = now - container[0].access_time;
    size_t index = 0;
    for (size_t i = 0; i < container.size(); i++) {
      if (now - container[i].access_time > delta) {
        index = i;
      }
    }
    return index;
  }
};

template <typename KeyType, typename ValueType, typename ReplacementAlgorithm = LeastRecentlyUsedReplacementAlgorithm<SlotContainer<KeyType, ValueType>>>
class SetAssociativeCache {
 private:
  using SlotContainer = SlotContainer<KeyType, ValueType>;
  using Slot = Slot<KeyType, ValueType>;

  size_t num_sets_;
  size_t num_slots_;
  std::vector<SlotContainer> slots_;
  ReplacementAlgorithm replacement_algorithm_;
  std::mutex slot_mutex_;

 public:
  SetAssociativeCache() = delete;

  SetAssociativeCache(size_t num_sets, size_t num_slots) : num_sets_(num_sets), num_slots_(num_slots) {
    Reallocate();
  }

  ~SetAssociativeCache() = default;

  void Set(KeyType key, ValueType value) {
    const auto set_index = HashToSet(key);
    std::lock_guard guard(slot_mutex_);
    auto& container = slots_[set_index];

    std::cout << "Attempting to set key \"" << key << "\" => \"" << value << "\" in bucket " << set_index << "..." << std::endl;

    // Check to see if key is already mapped.
    for (auto& slot : container) {
      if (slot.key == key) {
        std::cout << " Updating key \"" << key << "\" from \"" << slot.value << "\" to \"" << value << "\"" << std::endl;

        slot.value = value;
        slot.access_time = std::chrono::system_clock::now();
        return;
      }
    }

    // If we haven't filled the container, add a new slot for this key at the back.
    if (container.size() < num_slots_) {
      std::cout << " Setting new key \"" << key << "\" => \"" << value << "\"" << std::endl;

      container.push_back({key, value, std::chrono::system_clock::now()});
      return;
    }

    // The container is full. Evict something.
    std::cout << " Conflict miss. Determining which key to replace..." << std::endl;

    const auto index = replacement_algorithm_(container);
    auto& evicted_slot = container[index];

    std::cout << "  Evicting key \"" << evicted_slot.key << "\" => \"" << evicted_slot.value << "\" ";
    std::cout << "and setting key \"" << key << "\" => \"" << value << "\"" << std::endl;

    evicted_slot.access_time = std::chrono::system_clock::now();
    evicted_slot.key = key;
    evicted_slot.value = value;
  }

  ValueType Get(KeyType key) {
    const auto set_index = HashToSet(key);
    auto& container = slots_[set_index];

    for (auto& slot : container) {
      if (slot.key == key) {
        slot.access_time = std::chrono::system_clock::now();
        return slot.value;
      }
    }

    return {};
  }

  void Print() {
    for (size_t i = 0; i < slots_.size(); i++) {
      std::cout << "Set " << i << " = [ ";
      const auto& container = slots_[i];
      for (size_t j = 0; j < container.size(); j++) {
        const auto& slot = container[j];
        if (j != 0) {
          std::cout << "| ";
        }
        std::cout << slot.key << " => " << slot.value << " ";
      }
      std::cout << "]" << std::endl;
    }
  }

 private:
  void Reallocate() {
    std::lock_guard guard(slot_mutex_);
    slots_.resize(num_sets_);
  }

  size_t HashToSet(KeyType key) {
    return key % num_sets_;
  }
};

int main_cache() {
  std::cout << std::endl << "Testing SetAssociativeCache..." << std::endl;

  SetAssociativeCache<int, int> cache(4,5);
  cache.Set(1, 2);
  cache.Set(5, 3);
  cache.Set(9, 4);
  cache.Set(13, 5);
  cache.Set(17, 6);
  cache.Set(21, 7);
  cache.Set(9, 8);
  cache.Print();

  return 0;
}
