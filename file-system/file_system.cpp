#include "file_system.h"

#include <filesystem>

namespace fs = std::filesystem;

FileSystemEntry::FileSystemEntry(string) {
  device_ = "";
  directory_ = "";
  type_ = "";
  total_ = "";
  free_ = "";
  available_ = "";
  used_ = "";
}
