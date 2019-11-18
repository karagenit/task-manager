#include "../helper_functions.h"
#include <iostream>

#define GREEN "\033[32m"
#define RED "\033[31m"
#define RESET "\033[0m"

using namespace std;

int main() {
  string input[] = {
    " asdf ",
    "  asdf asdf ",
    "asdf  ",
    "   asdf",
    "asdf",
    "asdf asdf"
  };

  string expected[] =  {
    "asdf",
    "asdf asdf",
    "asdf",
    "asdf",
    "asdf",
    "asdf asdf"
  };

  for (auto i = 0; i < 6; i++) {
    if (trim(input[i]) != expected[i]) {
      cerr << RED << "Test Failed - input: " << input[i] << ", output: " << trim(input[i]) << ", expected: " << expected[i] << RESET << endl;
    }
    else {
      cout << GREEN << "Test Passed - input: " << input[i] << ", output: " << expected[i] << RESET << endl;
    }
  }
}
