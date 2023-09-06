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

class ReplacementAlgorithmBase {
 public:
  virtual void NotifyUsed(size_t slot_index) = 0;
  virtual size_t ChooseEvictionTarget() = 0;
};

class LeastRecentlyUsedReplacementAlgorithm : ReplacementAlgorithmBase {
 private:
  size_t age_counter_ = 0;
  std::vector<size_t> age_bits_;

 public:
  LeastRecentlyUsedReplacementAlgorithm() = delete;

  LeastRecentlyUsedReplacementAlgorithm(size_t num_slots) : age_bits_(num_slots) {
  }

  ~LeastRecentlyUsedReplacementAlgorithm() = default;

  void NotifyUsed(size_t slot_index) override {
    age_bits_[slot_index] = age_counter_++;
  }

  size_t ChooseEvictionTarget() override {
    auto index_age = age_bits_[0];
    size_t index = 0;
    for (size_t i = 1; i < age_bits_.size(); i++) {
      if (age_bits_[i] < index_age) {
        index = i;
        index_age = age_bits_[i];
      }
    }
    return index;
  }
};

template <typename KeyType, typename ValueType, typename ReplacementAlgorithm = LeastRecentlyUsedReplacementAlgorithm>
class SetAssociativeCache {
 private:
  struct Slot {
    KeyType key;
    ValueType value;
  };

  struct SlotContainer : public std::vector<Slot> {
    ReplacementAlgorithm replacement_algorithm_;

    SlotContainer() = delete;
    SlotContainer(size_t num_slots) : replacement_algorithm_(num_slots) {}
    ~SlotContainer() = default;
  };

  size_t num_sets_;
  size_t num_slots_;
  std::vector<SlotContainer> slots_;
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
    for (size_t i = 0; i < container.size(); i++) {
      auto& slot = container[i];
      if (slot.key == key) {
        std::cout << " Updating key \"" << key << "\" from \"" << slot.value << "\" to \"" << value << "\"" << std::endl;

        slot.value = value;
        container.replacement_algorithm_.NotifyUsed(i);
        return;
      }
    }

    // If we haven't filled the container, add a new slot for this key at the back.
    if (container.size() < num_slots_) {
      std::cout << " Setting new key \"" << key << "\" => \"" << value << "\"" << std::endl;

      container.replacement_algorithm_.NotifyUsed(container.size());
      container.push_back({key, value});
      return;
    }

    // The container is full. Evict something.
    std::cout << " Conflict miss. Determining which key to replace..." << std::endl;

    const auto index = container.replacement_algorithm_.ChooseEvictionTarget();
    container.replacement_algorithm_.NotifyUsed(index);
    auto& evicted_slot = container[index];

    std::cout << "  Evicting key \"" << evicted_slot.key << "\" => \"" << evicted_slot.value << "\" ";
    std::cout << "and setting key \"" << key << "\" => \"" << value << "\"" << std::endl;

    evicted_slot.key = key;
    evicted_slot.value = value;
  }

  ValueType Get(KeyType key) {
    const auto set_index = HashToSet(key);
    auto& container = slots_[set_index];

    for (size_t i = 0; i < container.size(); i++) {
      auto& slot = container[i];
      if (slot.key == key) {
        container.replacement_algorithm_.NotifyUsed(i);
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
    slots_.clear();
    slots_.reserve(num_sets_);
    for (size_t i = 0; i < num_sets_; i++) {
      slots_.emplace_back(SlotContainer(num_slots_));
    }
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
  cache.Get(5);
  cache.Set(25, 9);
  cache.Print();

  return 0;
}
