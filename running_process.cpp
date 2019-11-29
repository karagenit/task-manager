#include "running_process.h"

#include <sstream>
#include <fstream>
#include <cstring>
#include <iostream>
#include <dirent.h>
#include <unistd.h>

#include "helper_functions.h"

RunningProcess::RunningProcess(int pid) {
  // set pid
  this->parent_ = NULL;

  this->pid = pid;
  parent_pid_ = 0;

  tree_item_ = new QTreeWidgetItem();

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

  update_qtree_item();


  in.close();
}

RunningProcess::~RunningProcess() {
  if (tree_item_) {
    QTreeWidgetItem *parent = tree_item_->parent();
    if (parent) {
      parent->removeChild(tree_item_);
    }
  }
}

void RunningProcess::remove_child(RunningProcess *child) {
  if (child == NULL) {
    return;
  }
  auto i = children_.begin();
  while (i != children_.end()) {
    RunningProcess *p = *i;
    if (p == child) {
      children_.erase(i);
      tree_item_->removeChild(child->tree_item_);

      child->parent_ = NULL;
      return;
    }
    i++;
  }
}

std::string RunningProcess::get_user() {
  
  //get the process owner's uid from /proc/<pid>/uid_map
  std::ifstream in("/proc/" + std::to_string(this->pid) + "/uid_map");
  if (!in) {
    return "---";
  }
  int uid;
  in >> uid >> uid >> uid;
  in.close();
  //TODO- get the name of the user instead of just their uid
  return std::to_string(uid);
}

std::string RunningProcess::get_status() {
  std::ifstream in("/proc/" + std::to_string(this->pid) + "/status");
  if (!in) {
    return "-";
  }
  std::string line;
  while (std::getline(in, line)) {
    std::istringstream stream(line);
    std::string label;
    stream >> label;
    if (label.compare("State:") == 0) {
      std::string state;
      stream >> state >> state;
      return state.substr(1, state.length() - 2);
    }
    
  }
  return "-";
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
    if (is_number(ent->d_name)) {
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

  tree_item_->addChild(child->tree_item_);
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

void RunningProcess::update_qtree_item() {
  tree_item_->setText(0, QString(name_.c_str()));
  tree_item_->setText(1, QString(get_status().c_str()));
  tree_item_->setText(2, QString(get_cpu_percent().c_str()));
  tree_item_->setText(3, QString(std::to_string(pid).c_str()));
  tree_item_->setText(4, get_memory().c_str());
}

QTreeWidgetItem *RunningProcess::get_qtree_item() {
  update_qtree_item();
  return tree_item_;
}

QTreeWidget *RunningProcess::get_detailed_view() {
  QTreeWidget *tree = new QTreeWidget();
  tree->setColumnCount(2);

  QStringList header_labels;
  header_labels.push_back(QString("Property"));
  header_labels.push_back(QString("Value"));
  tree->setHeaderLabels(header_labels);
  

  std::vector<std::string> keys;
  std::vector<std::string> vals;

  keys.push_back("Name");
  vals.push_back(name_);

  keys.push_back("User");
  vals.push_back(get_user());

  keys.push_back("State");
  vals.push_back(get_status());

  keys.push_back("Memory");
  vals.push_back(get_memory());

  keys.push_back("Virtual Memory");
  vals.push_back(get_virtual_memory());

  keys.push_back("Resident Memory");
  vals.push_back(get_resident_memory());

  keys.push_back("Shared Memory");
  vals.push_back(get_shared_memory());

  keys.push_back("CPU Time");
  vals.push_back(get_cpu_time());

  keys.push_back("Time Started");
  vals.push_back(get_start_datetime());

  for (unsigned int i = 0; i < keys.size(); i++) {
    QStringList fields;
    fields.push_back(QString(keys.at(i).c_str()));
    fields.push_back(QString(vals.at(i).c_str()));
    QTreeWidgetItem *item = new QTreeWidgetItem((QTreeWidget *)0, fields);
    tree->addTopLevelItem(item);
  }


  //name, user, state, memory, virtual memory, resident memory,
  //shared memory, CPU time, date/time started

  
  return tree;
}

std::string RunningProcess::get_start_datetime() {
  return "---";
}

std::string RunningProcess::get_shared_memory() {
  return "---";
}

std::string RunningProcess::get_resident_memory() {
  return "---";
}

std::string RunningProcess::get_virtual_memory() {
  return "---";
}

std::string RunningProcess::get_cpu_time() {
  return "---";
}