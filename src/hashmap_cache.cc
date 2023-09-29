//-------------------------------------------------------------------------------------------------------
// Copyright (C) Taylor Woll. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------

#include <chrono>
#include <cstddef>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <list>
#include <map>
#include <mutex>
#include <unordered_map>

#include "test/TestCase.h"

#define VERBOSE 0

template <typename KeyType>
class ReplacementAlgorithmBase {
 public:
  virtual void NotifyUsed(KeyType key) = 0;
  virtual const KeyType& ChooseEvictionTarget() const = 0;
  virtual void RemoveKey(const KeyType& key) = 0;
};

template <typename KeyType>
class LeastRecentlyUsedReplacementAlgorithm : ReplacementAlgorithmBase<KeyType> {
 private:
  std::list<KeyType> usage_tracker_;
  std::unordered_map<KeyType, typename std::list<KeyType>::iterator> list_node_map_;

 public:
  void NotifyUsed(KeyType key) override {
    RemoveKey(key);
    usage_tracker_.push_front(key);
    list_node_map_[key] = usage_tracker_.begin();
  }

  const KeyType& ChooseEvictionTarget() const override {
    return usage_tracker_.back();
  }

  void RemoveKey(const KeyType& key) override {
    const auto& iter = list_node_map_.find(key);

    if (iter != list_node_map_.cend()) {
      usage_tracker_.erase(iter->second);
      list_node_map_.erase(iter);
    }
  }
};

template <typename KeyType, typename ValueType, typename ReplacementAlgorithm = LeastRecentlyUsedReplacementAlgorithm<KeyType>>
class CacheSet {
 private:
  size_t capacity_;
  std::unordered_map<KeyType, ValueType> map_store_;
  ReplacementAlgorithm replacement_algorithm_;
  std::mutex mutex_;

 public:
  CacheSet() = delete;
  CacheSet(const CacheSet&) = delete;
  CacheSet(CacheSet&&) noexcept {}

  CacheSet(size_t capacity) : capacity_(capacity) {}

  ~CacheSet() = default;

  ValueType Get(KeyType key) {
    std::lock_guard guard(mutex_);
    const auto& iter = map_store_.find(key);

    if (iter != map_store_.cend()) {
      replacement_algorithm_.NotifyUsed(key);
      return iter->second;
    }

    return {};
  }

  void Set(KeyType key, ValueType value) {
    std::lock_guard guard(mutex_);
    const auto& iter = map_store_.find(key);

    if (iter != map_store_.end()) {
#if VERBOSE
      std::cout << " Updating key \"" << key << "\" from \"" << iter->second << "\" to \"" << value << "\"" << std::endl;
#endif
      iter->second = value;
    } else {
      if (map_store_.size() == capacity_) {
#if VERBOSE
        std::cout << " Conflict miss. Determining which key to replace..." << std::endl;
#endif
        const auto& key_to_evict = replacement_algorithm_.ChooseEvictionTarget();

        const auto& iter = map_store_.find(key_to_evict);
#if VERBOSE
        std::cout << "  Evicting key \"" << iter->first << "\" => \"" << iter->second << "\" ";
        std::cout << "and setting key \"" << key << "\" => \"" << value << "\"" << std::endl;
#endif

        RemoveKey(key_to_evict);
      } else {
#if VERBOSE
        std::cout << " Setting new key \"" << key << "\" => \"" << value << "\"" << std::endl;
#endif
      }

      map_store_[key] = value;
    }

    replacement_algorithm_.NotifyUsed(key);
  }

  void Print() const {
    bool is_first = true;
    std::cout << "[ ";
    for (const auto& m : map_store_) {
      if (!is_first) {
        std::cout << "| ";
      }
      is_first = false;
      std::cout << m.first << " => " << m.second << " ";
    }
    std::cout << "]" << std::endl;
  }

 private:
  void RemoveKey(const KeyType& key) {
    const auto& iter = map_store_.find(key);

    if (iter != map_store_.cend()) {
      replacement_algorithm_.RemoveKey(key);
      map_store_.erase(iter);
    }
  }
};

template <typename KeyType, typename ValueType, typename ReplacementAlgorithm = LeastRecentlyUsedReplacementAlgorithm<KeyType>>
class SetAssociativeCache {
 private:
  size_t num_sets_;
  size_t num_slots_;
  std::vector<CacheSet<KeyType, ValueType, ReplacementAlgorithm>> slots_;

 public:
  SetAssociativeCache() = delete;
  SetAssociativeCache(const SetAssociativeCache&) = delete;

  SetAssociativeCache(size_t num_sets, size_t num_slots) : num_sets_(num_sets), num_slots_(num_slots) {
    Reallocate();
  }

  ~SetAssociativeCache() = default;

  void Set(KeyType key, ValueType value) {
    const auto set_index = HashToSet(key);
#if VERBOSE
    std::cout << "Attempting to set key \"" << key << "\" => \"" << value << "\" in bucket " << set_index << "..." << std::endl;
#endif
    auto& container = slots_[set_index];
    return container.Set(key, value);
  }

  ValueType Get(KeyType key) {
    const auto set_index = HashToSet(key);
    auto& container = slots_[set_index];
    return container.Get(key);
  }

  void Print() const {
    size_t i = 0;
    for (const auto& c : slots_) {
      std::cout << "Set " << i++ << " = ";
      c.Print();
    }
  }

 private:
  void Reallocate() {
    slots_.clear();
    slots_.reserve(num_sets_);
    for (size_t i = 0; i < num_sets_; i++) {
      slots_.emplace_back(num_slots_);
    }
  }

  size_t HashToSet(KeyType key) {
    return key % num_sets_;
  }
};

class SetAssociativeCacheTest : public TestCase {};

TEST_CASE(SetAssociativeCacheTest, tests) {
  trace << std::endl << "Testing SetAssociativeCache..." << std::endl;

  SetAssociativeCache<int, int> cache(4, 5);
  cache.Set(1, 2);
  cache.Set(5, 3);
  cache.Set(9, 4);
  cache.Set(13, 5);
  cache.Set(17, 6);
  cache.Set(21, 7);
  cache.Set(9, 8);
  cache.Get(5);
  cache.Set(25, 9);

#if VERBOSE
  cache.Print();
#endif

  // TODO: Verify cache miss evicts expected key.
}
