#include <cstddef>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <map>
#include <unordered_map>
#include <vector>

#include "hashmap_redis.h"
#include "helpers.h"

std::unordered_map<std::string, std::string> key_value_store;
std::unordered_map<std::string, std::unordered_map<std::string, std::string>> key_field_value_store;

// Find the next word in |str| beginning at |cur_index| and ending at the next
// space character.
std::string next_word(std::string str, size_t& cur_index) {
  size_t start_index = cur_index;
  while (str[cur_index] != ' ' && cur_index < str.size()) {
    cur_index++;
  }
  return str.substr(start_index, cur_index - start_index);
}

// Emulate a redis server.
// Takes a command string and parses it into an operation, key name, and
// arguments.
// Supported operations:
// - get key
//   If there is a value mapped under key return that value, otherwise "".
// - set key value
//   Maps key to value. Overwrites any existing mapping.
// - del key
//   Removes any mapping for key.
// - hget key field
//   If there is a value mapped under key.field, return that value, otherwise ""
// - hset key field value
//   Maps key.field to value. Overwrites any existing mapping.
// - hgetall key
//   Returns all field/value pairs mapped under key as a JSON string.
std::string redis(std::string cmd) {
  size_t cur_index = 0;
  const auto operation = next_word(cmd, cur_index);

  // TODO: Instead of all these if/else, convert the operation
  // into an enum and use a switch here. Handle case-insensitive
  // operation.
  if (operation == "get") {
    const auto key = next_word(cmd, ++cur_index);
    const auto& iter = key_value_store.find(key);
    if (iter == key_value_store.cend()) {
      return "";
    }
    return iter->second;
  } else if (operation == "set") {
    const auto key = next_word(cmd, ++cur_index);
    const auto value = cmd.substr(cur_index + 1, cmd.size() - 1 - cur_index);
    key_value_store[key] = value;
    return "OK";
  } else if (operation == "del") {
    const auto key = next_word(cmd, ++cur_index);
    const auto& iter = key_value_store.find(key);
    if (iter == key_value_store.cend()) {
      return "";
    }
    const auto value = iter->second;
    key_value_store.erase(iter);
    return value;
  } else if (operation == "hget") {
    const auto key = next_word(cmd, ++cur_index);
    const auto field = next_word(cmd, ++cur_index);
    const auto& iter = key_field_value_store.find(key);
    if (iter == key_field_value_store.cend()) {
      return "";
    }
    const auto& iter2 = iter->second.find(field);
    if (iter2 == iter->second.cend()) {
      return "";
    }
    return iter2->second;
  } else if (operation == "hset") {
    const auto key = next_word(cmd, ++cur_index);
    const auto field = next_word(cmd, ++cur_index);
    const auto value = cmd.substr(cur_index + 1, cmd.size() - 1 - cur_index);
    const auto& iter = key_field_value_store.find(key);
    if (iter == key_field_value_store.end()) {
      key_field_value_store[key][field] = value;
      return "OK";
    }
    iter->second[field] = value;
    return "OK";
  } else if (operation == "hgetall") {
    const auto key = next_word(cmd, ++cur_index);
    const auto& iter = key_field_value_store.find(key);
    if (iter == key_field_value_store.cend()) {
      return "";
    }
    std::string json = "{ ";
    for (const auto& f: iter->second) {
      json += "\"";
      json += f.first;
      json += "\": \"";
      json += f.second;
      json += "\", ";
    }
    return json += "}";
  }

  return "";
}

void test_redis(std::string cmd, std::string expected) {
  std::cout << std::endl << "Executing this command on the redis server: " << cmd << std::endl;

  const auto actual = redis(cmd);
  std::cout << "Found:    \"" << actual << "\"" << std::endl;
  std::cout << "Expected: \"" << expected << "\"" << std::endl;
}

int main_redis() {
  test_redis("set some_key some value", "OK");
  test_redis("get some_key", "some value");
  test_redis("get some_missing_key", "");
  test_redis("del some_key", "some value");
  test_redis("get some_key", "");
  test_redis("set some_key some other value", "OK");
  test_redis("set some_key another value", "OK");
  test_redis("get some_key", "another value");

  test_redis("hset h_key field1 some field value 1", "OK");
  test_redis("hset h_key field2 some field value 2", "OK");
  test_redis("hget h_key field1", "some field value 1");
  test_redis("hget h_key field2", "some field value 2");
  test_redis("hget h_key field3", "");
  test_redis("hget h_missing_key field", "");
  test_redis("hgetall h_key", "{ \"field1\": \"some field value 1\", \"field2\": \"some field value 2\", }");

  return 0;
}
