#ifndef HELPER_FUNCTIONS_H_
#define HELPER_FUNCTIONS_H_

#include <string>

std::string trim(const std::string str);
std::string ltrim(const std::string str);
std::string rtrim(const std::string str);

bool is_number(const std::string str);
std::string uid_to_name(int uid);
std::string humanize(unsigned long str);

#endif
