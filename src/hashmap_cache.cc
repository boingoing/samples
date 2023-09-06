#include <chrono>
#include <cstddef>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <map>
#include <unordered_map>

#include "hashmap_cache.h"
#include "helpers.h"

template <typename KeyType, typename ValueType>
class SetAssociativeCache {
 public:
  class Slot {
   public:
    KeyType key;
    ValueType value;
    std::chrono::time_point<std::chrono::system_clock> access_time;
  };

  class SlotContainer : public std::vector<Slot> {
  };

 private:
  size_t num_sets_ = 4;
  size_t num_slots_ = 5;
  std::vector<SlotContainer> slots_;

 public:
  SetAssociativeCache() {
    Reallocate();
  }

  SetAssociativeCache(size_t num_sets, size_t num_slots) {
    num_sets_ = num_sets;
    num_slots_ = num_slots;
    Reallocate();
  }

  ~SetAssociativeCache() = default;

  void Set(KeyType key, ValueType value) {
    const auto set_index = HashToSet(key);
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

    const auto now = std::chrono::system_clock::now();
    Slot* lru_slot = &container[0];
    auto delta = now - lru_slot->access_time;
    for (auto& slot : container) {
      if (now - slot.access_time > delta) {
        lru_slot = &slot;
      }
    }

    std::cout << "  Evicting key \"" << lru_slot->key << "\" => \"" << lru_slot->value << "\" ";
    std::cout << "and setting key \"" << key << "\" => \"" << value << "\"" << std::endl;

    lru_slot->access_time = now;
    lru_slot->key = key;
    lru_slot->value = value;
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
  }

 private:
  void Reallocate() {
    slots_.resize(num_sets_);
  }

  size_t HashToSet(KeyType key) {
    return key % num_sets_;
  }
};

int main_cache() {
  std::cout << std::endl << "Testing SetAssociativeCache..." << std::endl;

  SetAssociativeCache<int, int> cache;
  cache.Set(1, 2);
  cache.Set(5, 3);
  cache.Set(9, 4);
  cache.Set(13, 5);
  cache.Set(17, 6);
  cache.Set(21, 7);
  cache.Set(9, 8);

  return 0;
}
