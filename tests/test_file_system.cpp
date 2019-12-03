#include "../file_system.h"

#include <iostream>
//
// Created by hahnw on 12/2/19.
//

using namespace std;
int main() {
  vector<FileSystemEntry> entries = read_file_system_entries();
  for (auto &entry : entries) {
    cout << entry.GetDevice() << '\t' << entry.GetDirectory() << '\t' << entry.GetType() << '\t' << entry.GetAvailable() << '\t' << entry.GetFree() << '\t' << entry.GetTotal() << '\t' << entry.GetUsed() << endl;
  }
}