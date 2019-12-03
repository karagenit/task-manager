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
    unsigned long UsedValue();
    unsigned long TotalValue();
    void UpdateFileSystemEntry();
  private:
  std::string device_;
  std::string directory_;
  std::string type_;
  unsigned long total_;
  unsigned long free_;
  unsigned long available_;
  unsigned long used_;
};

std::vector<FileSystemEntry> read_file_system_entries();

#endif
