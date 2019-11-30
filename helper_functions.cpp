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