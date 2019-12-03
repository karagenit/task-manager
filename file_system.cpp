#include "file_system.h"
#include <fstream>
#include <algorithm>
#include <sys/statvfs.h>
#include <sys/types.h>

#include "helper_functions.h"


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
  struct statvfs vfs;
  if (statvfs(directory_.c_str(), &vfs)) {
    throw exception();
  }
  total_ = vfs.f_blocks * vfs.f_bsize;
  free_ = vfs.f_bfree * vfs.f_bsize;
  available_ = vfs.f_bavail * vfs.f_bsize;
  used_ = total_ - free_;
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
  return humanize(total_);
}
std::string FileSystemEntry::GetFree() {
  return humanize(free_);
}
std::string FileSystemEntry::GetAvailable() {
  return humanize(available_);
}
std::string FileSystemEntry::GetUsed() {
  return humanize(used_);
}
unsigned long FileSystemEntry::UsedValue() {
  return used_;
}
unsigned long FileSystemEntry::TotalValue() {
  return total_;
}

vector<FileSystemEntry> read_file_system_entries() {
  vector<FileSystemEntry> out;

  vector<string> partitions;
  ifstream partitions_stream(PARTITIONS_FILE);
  string line;
  getline(partitions_stream, line);
  getline(partitions_stream, line);
  while (!partitions_stream.eof()) {
    string dev_name;
    partitions_stream >> dev_name >> dev_name >> dev_name >> dev_name;
    partitions.push_back("/dev/" + dev_name);
  }
  partitions_stream.close();

  ifstream mounts_stream(MOUNTS_FILE);
  while (!mounts_stream.eof()) {
    string device;
    string directory;
    string type;
    mounts_stream >> device >> directory >> type;
    auto it = find(partitions.begin(), partitions.end(), device);
    if (it != partitions.end()) {
      try {
        FileSystemEntry entry(device, directory, type);
        out.push_back(entry);
      }
      catch (exception &ex) {
        //cerr << ex.what() << endl;
      }
    }

    string temp;
    getline(mounts_stream, temp);
  }

  mounts_stream.close();

  return out;
}