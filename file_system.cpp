#include "file_system.h"
#include <experimental/filesystem>
#include <fstream>
#include <iostream>

namespace fs = std::experimental::filesystem;

#define MOUNTS_FILE ("/proc/mounts")
#define PARTITIONS_FILE ("/proc/partitions")

using namespace std;

FileSystemEntry::FileSystemEntry(string device, string directory, string type) : device_(device),
                                                                                 directory_(directory), type_(type) {
  total_ = 0;
  free_ = 0;
  available_ = 0;
  used_ = 0;

  UpdateFileSystemEntry();
}
void FileSystemEntry::UpdateFileSystemEntry() {
  fs::space_info space = fs::space(directory_);
  total_ = space.capacity;
  free_ = space.free;
  available_ = space.available;
  used_ = space.capacity - space.free;
}

FileSystemEntry::~FileSystemEntry() {

}
std::string FileSystemEntry::GetDevice() {
  return device_;
}
std::string FileSystemEntry::GetDirectory() {
  return directory_;
}
std::string FileSystemEntry::GetType() {
  return type_;
}
std::string FileSystemEntry::GetTotal() {
  return to_string(total_);
}
std::string FileSystemEntry::GetFree() {
  return to_string(free_);
}
std::string FileSystemEntry::GetAvailable() {
  return to_string(available_);
}
std::string FileSystemEntry::GetUsed() {
  return to_string(used_);
}

vector<FileSystemEntry> read_file_system_entries() {
  vector<FileSystemEntry> out;

  ifstream mounts_stream(MOUNTS_FILE);
  while (!mounts_stream.eof()) {
    string device;
    string directory;
    string type;
    mounts_stream >> device >> directory >> type;
    try {
      FileSystemEntry entry(device, directory, type);
      out.push_back(entry);
    }
    catch (exception &ex) {
      cerr << ex.what() << endl;
    }

    string temp;
    getline(mounts_stream, temp);
  }

  return out;
}