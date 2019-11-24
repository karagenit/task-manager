#include "running_process.h"

#include <sstream>
#include <fstream>
#include <cstring>
#include <iostream>
#include <dirent.h>
#include <unistd.h>

RunningProcess::RunningProcess(int pid) {
  // set pid
  this->pid = pid;

  // parse /proc/<pid>/stat for process name and parent pid
  std::ifstream in("/proc/" + std::to_string(this->pid) + "/stat");
  if (!in) {
    return;
  }
  std::string name;
  in >> name >> name;
  // discard parentheses from process name
  this->name_ = name.substr(1, name.length() - 2);

  std::string parent_pid_string;
  in >> parent_pid_string >> parent_pid_string;
  this->parent_pid_ = std::stoi(parent_pid_string);
  //std::cout << "parent_pid_: " << parent_pid_ << "\n";

  in.close();
}

std::string RunningProcess::get_user() {
  
  //get the process owner's uid from /proc/<pid>/uid_map
  std::ifstream in("/proc/" + std::to_string(this->pid) + "/uid_map");
  if (!in) {
    return NULL;
  }
  int uid;
  in >> uid >> uid >> uid;
  in.close();
  //TODO- get the name of the user instead of just their uid
  return std::to_string(uid);
}

std::string RunningProcess::get_status() {
  std::ifstream in("/proc/" + std::to_string(this->pid) + "/stat");
  if (!in) {
    return NULL;
  }
  std::string answer;
  for (int i = 0; i < 3; i++) {
    in >> answer;
  }
  in.close();
  //TODO- interpret the status as a human readable string
  return answer;
}


std::vector<file_info> RunningProcess::get_files() {
  std::vector<file_info> answer;
  std::string path = "/proc/" + std::to_string(this->pid) + "/fd";
  DIR *dir = opendir(path.c_str());
  if (dir == NULL) {
    //TODO- better handle the case where we don't have permission
    //to open /proc/<pid>/fd
    return answer; //return an empty list?
  }
  struct dirent *ent;
  while ((ent = readdir(dir)) != NULL) {
    std::string fd_path = path + "/" + ent->d_name;
    std::string buffer(128, '\0');
    int bytes_read = readlink(fd_path.c_str(), &buffer[0], buffer.capacity() - 1);
    if (bytes_read > 0) {
      buffer[bytes_read] = '\0';
    }
    file_info info;
    info.fd = std::stoi(ent->d_name);
    info.object = buffer;
    info.type = "file";
    const char *socket = std::strstr(buffer.c_str(), "socket");
    if (socket) {
      info.type = "socket";
    }
    else {
      const char *pipe = std::strstr(buffer.c_str(), "pipe");
      if (pipe) {
        info.type = "pipe";
      }
    }
    answer.push_back(info);
  }
  closedir(dir);

  return answer;
}

// Returns the Proportional Set Size from /proc/<pid>/smaps_rollup
std::string RunningProcess::get_memory() {
  std::ifstream in("/proc/" + std::to_string(this->pid) + "/smaps_rollup");
  if (!in) {
    return "---";
  }
  std::string line;
  while (std::getline(in, line)) {
    std::istringstream stream(line);
    std::string type;
    stream >> type;
    if (type.compare("Pss:") == 0) {
      std::string amount;
      std::string unit;
      stream >> amount >> unit;
      in.close();
      return amount + " " + unit;
    }
  }
  in.close();
  return "---";
}

void RunningProcess::add_child(RunningProcess *child) {
  if (child == NULL) {
    return;
  }
  children_.push_back(child);
  child->parent_ = this;
}

RunningProcess *RunningProcess::get_parent() {
  return parent_;
}

std::vector<RunningProcess *> RunningProcess::get_children() {
  return children_;
}

int RunningProcess::get_parent_pid() {
  return parent_pid_;
}

std::string RunningProcess::get_name() {
  return name_;
}

std::string RunningProcess::get_cpu_percent() {
  return std::string("XX");
}

QTreeWidgetItem *RunningProcess::get_qtree_item() {
  QStringList fields;
  fields.push_back(QString(name_.c_str())); //name
  fields.push_back(QString(get_status().c_str())); //status
  fields.push_back(QString(get_cpu_percent().c_str())); //% CPU
  std::string pid_string = std::to_string(pid);
  fields.push_back(QString(pid_string.c_str())); // PID
  fields.push_back(QString(get_memory().c_str())); // Memory
  QTreeWidgetItem *answer = new QTreeWidgetItem((QTreeWidget *)0, fields);
  
  return answer;
}