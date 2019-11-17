#include "helper_functions.h"

#include <ctype.h>

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
  auto index = 0;
  if (str.size() == 0) {
    return string(str);
  }

  while ((index < str.size()) && (isspace(str[index]))) {
    index--;
  }

  return str.substr(index - 1, str.size());
}

string trim(const string str) {
  return ltrim(rtrim(str));
}
