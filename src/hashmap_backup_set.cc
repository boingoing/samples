//-------------------------------------------------------------------------------------------------------
// Copyright (C) Taylor Woll. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------

#include <cstddef>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <map>
#include <stack>
#include <unordered_set>
#include <vector>

#include "test/TestCase.h"
#include "test/TestCaseData.h"

struct FileDescriptor {
  std::string sha1hash;
  std::string filename;

  friend std::ostream& operator<<(std::ostream& os, const FileDescriptor& fd) {
    return os << "[" << fd.sha1hash << " " << fd.filename << "]";
  }
};

// Hold the details of a set of backup files.
// Each file consists of a full filesystem path and the sha1 hash of
// the file contents.
// File identity is determined by the sha1 hash regardless of the filename.
class BackupSet {
 private:
  std::map<std::string, std::string> hash_to_filename_map_;

  friend class BackupSetWriter;

 public:
  // Add a mapping from |sha1| => |filename| into the backup set.
  void addFile(const std::string& sha1, const std::string& filename) {
    // Assume no collision.
    hash_to_filename_map_[sha1] = filename;
  }

  // Return the set of filenames which are found in |rhs| but not found in this.
  std::vector<std::string> getMissingFiles(const BackupSet& rhs) {
    std::vector<std::string> missing;
    for (const auto& hash_filename_pair : rhs.hash_to_filename_map_) {
      const auto iter = hash_to_filename_map_.find(hash_filename_pair.first);
      if (iter == hash_to_filename_map_.cend()) {
        missing.push_back(hash_filename_pair.second);
      }
    }
    return missing;
  }
};

// A BackupSet may be serialized into a series of lines where each line
// begins with a 40-character sha1hash followed by a single space character
// followed by the filename and line terminator.
// This utility class can read a BackupSet from a buffer containing a serialized
// BackupSet as defined above.
class BackupSetReader {
 private:
  BackupSet& backup_set_;

 public:
  BackupSetReader() = delete;
  explicit BackupSetReader(BackupSet& backup_set) : backup_set_(backup_set) {}
  ~BackupSetReader() = default;

  void read(std::istream& is) {
    std::string line;
    std::string sha1hash;
    std::string filename;

    // Process one line at a time.
    while (std::getline(is, line)) {
      std::istringstream line_stream(line);

      // Read the sha1 hash and ' ' delimiter.
      if (line_stream >> sha1hash >> std::ws) {

        // Fetch the rest of the line as the filename.
        if (std::getline(line_stream, filename)) {
          // If that all parsed correctly, add the file to the set.
          backup_set_.addFile(sha1hash, filename);
        }
      }
    }
  }
};

// A BackupSet may be serialized into a series of lines where each line
// begins with a 40-character sha1hash followed by a single space character
// followed by the filename and line terminator.
// This utility class can serialize a BackupSet into a buffer in the format
// as defined above.
class BackupSetWriter {
 private:
  const BackupSet& backup_set_;

 public:
  BackupSetWriter() = delete;
  explicit BackupSetWriter(const BackupSet& backup_set) : backup_set_(backup_set) {}
  ~BackupSetWriter() = default;

  void write(std::ostream& os) {
    for (const auto& sha1_filename_pair : backup_set_.hash_to_filename_map_) {
       os << sha1_filename_pair.first << " " << sha1_filename_pair.second << std::endl;
    }
  }
};

struct BackupSetTestData : TestCaseData {
  std::vector<FileDescriptor> old_set;
  std::vector<FileDescriptor> new_set;
  std::vector<std::string> old_not_in_new;
  std::vector<std::string> new_not_in_old;

  BackupSetTestData(std::vector<FileDescriptor> old_set,
      std::vector<FileDescriptor> new_set,
      std::vector<std::string> old_not_in_new,
      std::vector<std::string> new_not_in_old) :
      old_set(old_set), new_set(new_set),
      old_not_in_new(old_not_in_new), new_not_in_old(new_not_in_old) {}
};

std::vector<BackupSetTestData> backup_set_tests = {
  {{{"11111", "c:\\temp\\something.pdf"},{"22222", "c:\\temp\\something else.pdf"}},
  {{"33333", "c:\\temp\\another thing.pdf"},{"11111", "c:\\temp\\something.pdf"}},
  {"c:\\temp\\something else.pdf"},
  {"c:\\temp\\another thing.pdf"}},
};

class BackupSetTest : public TestCase {};

TEST_CASE_WITH_DATA(BackupSetTest, tests, BackupSetTestData, backup_set_tests) {
  trace << std::endl << "Finding the missing files between old and new backup sets." << std::endl;
  trace << "Old backup set:" << std::endl;
  trace.vector(data.old_set);
  trace << "New backup set:" << std::endl;
  trace.vector(data.new_set);

  BackupSet old_set;
  for (const auto& fd : data.old_set) {
    old_set.addFile(fd.sha1hash, fd.filename);
  }

  BackupSet new_set;
  for (const auto& fd : data.new_set) {
    new_set.addFile(fd.sha1hash, fd.filename);
  }

  const auto new_not_in_old = old_set.getMissingFiles(new_set);
  trace << "Files found in new but not present in old (NewNotInOld):" << std::endl;
  trace.vector(new_not_in_old);
  assert.equal(new_not_in_old, data.new_not_in_old);

  const auto old_not_in_new = new_set.getMissingFiles(old_set);
  trace << "Files found in old but not present in new (OldNotInNew):" << std::endl;
  trace.vector(old_not_in_new);
  assert.equal(old_not_in_new, data.old_not_in_new);
}

struct BackupSetWriterTestData : TestCaseDataWithExpectedResult<std::string> {
  std::vector<FileDescriptor> backup_set;
};

std::vector<BackupSetWriterTestData> backup_set_writer_tests = {
  {"11111 c:\\file 1.txt\n"
    "22222 c:\\file 2.txt\n"
    "33333 c:\\file 3.txt\n"
    "44444 c:\\file 4.txt\n", {
      {"11111", "c:\\file 1.txt"},
      {"22222", "c:\\file 2.txt"},
      {"33333", "c:\\file 3.txt"},
      {"44444", "c:\\file 4.txt"}}},
};

TEST_CASE_WITH_DATA(BackupSetTest, writer_buffer, BackupSetWriterTestData, backup_set_writer_tests) {
  trace << std::endl << "Attempting to write this BackupSet:" << std::endl;
  trace.vector(data.backup_set);

  BackupSet backup_set;
  for (const auto& fd : data.backup_set) {
    backup_set.addFile(fd.sha1hash, fd.filename);
  }
  
  BackupSetWriter writer(backup_set);
  std::stringstream str;
  writer.write(str);
  trace << "Found: " << std::endl << str.str() << std::endl;
  trace << "Expected: " << std::endl << data.expected << std::endl;
  assert.equal(str.str(), data.expected);
}

struct BackupSetReaderTestData : TestCaseDataWithExpectedResult<std::vector<FileDescriptor>> {
  std::string str;
};

std::vector<BackupSetReaderTestData> backup_set_reader_tests = {
  {std::vector<FileDescriptor>({
      {"1111111111111111111111111111111111111111", "c:\\file 1.txt"},
      {"2222222222222222222222222222222222222222", "c:\\file 2.txt"},
      {"3333333333333333333333333333333333333333", "c:\\file 3.txt"},
      {"4444444444444444444444444444444444444444", "c:\\file 4.txt"}}),
      "1111111111111111111111111111111111111111 c:\\file 1.txt\n"
      "2222222222222222222222222222222222222222 c:\\file 2.txt\n"
      "3333333333333333333333333333333333333333 c:\\file 3.txt\n"
      "4444444444444444444444444444444444444444 c:\\file 4.txt\n"},
};

TEST_CASE_WITH_DATA(BackupSetTest, reader_buffer, BackupSetReaderTestData, backup_set_reader_tests) {
  trace << std::endl << "Attempting to read a BackupSet from this buffer:" << std::endl;
  trace << data.str << std::endl;

  BackupSet backup_set;
  BackupSetReader reader(backup_set);
  std::istringstream str(data.str);
  reader.read(str);

  BackupSet expected;
  for (const auto& fd : data.expected) {
    expected.addFile(fd.sha1hash, fd.filename);
  }

  trace << "Expected BackupSet:" << std::endl;
  trace.vector(data.expected);

  const auto expected_not_found = backup_set.getMissingFiles(expected);
  trace << "Missing files expected:" << std::endl;
  trace.vector(expected_not_found);
  assert.equal(expected_not_found, {});

  const auto unexpected_found = expected.getMissingFiles(backup_set);
  trace << "Unexpected files found:" << std::endl;
  trace.vector(unexpected_found);
  assert.equal(unexpected_found, {});
}

struct BackupSetRoundtripTestData : TestCaseData {
  std::vector<FileDescriptor> backup_set;
  std::string str;

  BackupSetRoundtripTestData(std::vector<FileDescriptor> backup_set, std::string str) : backup_set(backup_set), str(str) {}
};

std::vector<BackupSetRoundtripTestData> backup_set_roundtrip_tests = {
  {std::vector<FileDescriptor>({
      {"1111111111111111111111111111111111111111", "c:\\file 1.txt"},
      {"2222222222222222222222222222222222222222", "c:\\file 2.txt"},
      {"3333333333333333333333333333333333333333", "c:\\file 3.txt"},
      {"4444444444444444444444444444444444444444", "c:\\file 4.txt"}}),
      "1111111111111111111111111111111111111111 c:\\file 1.txt\n"
      "2222222222222222222222222222222222222222 c:\\file 2.txt\n"
      "3333333333333333333333333333333333333333 c:\\file 3.txt\n"
      "4444444444444444444444444444444444444444 c:\\file 4.txt\n"},
};

TEST_CASE_WITH_DATA(BackupSetTest, roundtrip_buffer, BackupSetRoundtripTestData, backup_set_roundtrip_tests) {
  trace << std::endl << "Attempting to roundtrip a BackupSet from this buffer:" << std::endl;
  trace << data.str << std::endl;

  BackupSet read_set;
  BackupSetReader reader(read_set);
  std::istringstream istr(data.str);
  reader.read(istr);

  BackupSetWriter writer(read_set);
  std::stringstream ostr;
  writer.write(ostr);

  trace << "Found: " << std::endl << ostr.str() << std::endl;
  trace << "Expected: " << std::endl << data.str << std::endl;
  assert.equal(ostr.str(), data.str);
}
