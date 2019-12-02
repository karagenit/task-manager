#ifndef FILE_SYSTEM_H_
#define FILE_SYSTEM_H_

#include <string>
#include <vector>

class FileSystemEntry {
  public:
    FileSystemEntry(std::string, std::string, std::string);
    ~FileSystemEntry();
    std::string GetDevice();
    std::string GetDirectory();
    std::string GetType();
    std::string GetTotal();
    std::string GetFree();
    std::string GetAvailable();
    std::string GetUsed();
    void UpdateFileSystemEntry();
  private:
  std::string device_;
  std::string directory_;
  std::string type_;
  unsigned int total_;
  unsigned int free_;
  unsigned int available_;
  unsigned int used_;
};

std::vector<FileSystemEntry> read_file_system_entries();

#endif
