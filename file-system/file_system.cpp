#include "file_system.h"
#include <experimental/filesystem>

using namespace std;

// need to add the -lstdc++fs flag at the end of the compile step

FileSystemEntry::FileSystemEntry(string) {
  device_ = "";
  directory_ = "";
  type_ = "";
  total_ = "";
  free_ = "";
  available_ = "";
  used_ = "";
}
