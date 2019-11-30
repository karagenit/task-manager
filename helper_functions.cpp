#include "helper_functions.h"

#include <ctype.h>
#include <string.h>

#include <fstream>

using namespace std;

string rtrim(const string str) {
  auto index = str.size();
  if (str.size() == 0) {
    return string(str);
  }
  while ((--index >= 0) && (isspace(str[index]))) { }

  return str.substr(0, index + 1);
}

string ltrim(const string str) {
  unsigned index = 0;
  if (str.size() == 0) {
    return string(str);
  }

  while ((index < str.size()) && (isspace(str[index]))) {
    index++;
  }

  return str.substr(index, str.size());
}

string trim(const string str) {
  return ltrim(rtrim(str));
}

bool is_number (const string str) {
  if (str.empty()) {
    return false;
  }
  string::const_iterator i = str.begin();
  while (i != str.end()) {
    if (!isdigit(*i)) {
      return false;
    }
    i++;
  }
  return true;
}

std::string uid_to_name(int uid) {
  std::ifstream in("/etc/passwd");
  std::string line;
  while (std::getline(in, line)) {
    unsigned int start_pos = line.find(":x:");
    if (start_pos == std::string::npos) {
      continue;
    }
    unsigned int end_pos = line.find(":", start_pos + strlen(":x:"));
    if (end_pos == std::string::npos) {
      continue;
    }
    std::string id = line.substr(start_pos + strlen(":x:"), end_pos - start_pos - strlen(":x:"));
    if (is_number(id)) {
      int num_id = std::stoi(id);
      if (num_id == uid) {
        return line.substr(0, start_pos);
      }
    }
  }
  return "---";
}