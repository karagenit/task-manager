#ifndef FILE_SYSTEM_H_
#define FILE_SYSTEM_H_

#include <string>

class FileSystemEntry {
  public:
    FileSystemEntry(std::string);
    ~FileSystemEntry();
  private:
    std::string device_;
    std::string directory_;
    std::string type_;
    std::string total_;
    std::string free_;
    std::string available_;
    std::string used_;
};

vector<FileSystemEntry> read_file_system_entries();

#endif
