#include "helper_functions.h"

#include <ctype.h>
#include <string.h>
#include <pwd.h>
#include <sys/types.h>

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

  struct passwd *info = getpwuid(uid); // NOT thread safe
  if (info == NULL) {
    return "---";
  }
  return info->pw_name;

}

string truncate(string str) {
  for (unsigned i = 0; i < str.size(); i++) {
    if (str[i] == '.') {
      if (i + 1 < str.size()) {
        return str.substr(0, i + 2);
      }
    }
  }

  return str;
}

string humanize(unsigned long old_value) {
  double value = old_value;
  if (value < 1024) {
    return truncate(to_string(value)) + " B";
  }

  value /= 1024;

  if (value < 1024) {
    return truncate(to_string(value)) + " KB";
  }

  value /= 1024;

  if (value < 1024) {
    return truncate(to_string(value)) + "MB";
  }

  value /= 1024;

  if (value < 1024) {
    return truncate(to_string(value)) + " GB";
  }

  value /= 1024;

  return truncate(to_string(value / 1024)) + " TB";
}