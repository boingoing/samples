#include <cstddef>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <map>
#include <unordered_map>
#include <vector>

#include "test/TestCase.h"

std::unordered_map<std::string, std::string> key_value_store;
std::unordered_map<std::string, std::map<std::string, std::string>> key_field_value_store;

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

struct RedisTestData : TestCaseDataWithExpectedResult<std::string> {
  std::string command;
};

std::vector<RedisTestData> redis_tests = {
  {"OK", "set some_key some value"},
  {"some value", "get some_key"},
  {"", "get some_missing_key"},
  {"some value", "del some_key"},
  {"", "get some_key"},
  {"OK", "set some_key some other value"},
  {"OK", "set some_key another value"},
  {"another value", "get some_key"},

  {"OK", "hset h_key field1 some field value 1"},
  {"OK", "hset h_key field2 some field value 2"},
  {"some field value 1", "hget h_key field1"},
  {"some field value 2", "hget h_key field2"},
  {"", "hget h_key field3"},
  {"", "hget h_missing_key field"},
  {"{ \"field1\": \"some field value 1\", \"field2\": \"some field value 2\", }", "hgetall h_key"},
};

class RedisTest : public TestCase {};

TEST_CASE_WITH_DATA(RedisTest, tests, RedisTestData, redis_tests) {
  trace << std::endl << "Executing this command on the redis server: " << data.command << std::endl;

  const auto actual = redis(data.command);
  assert.equal(actual, data.expected);
}
