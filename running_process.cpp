#include "running_process.h"

#include <dirent.h>
#include <unistd.h>

#include <sstream>
#include <fstream>
#include <cstring>
#include <iostream>
#include <iomanip>

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
  while (*(name.end() - 1) != ')') {
    std::string temp;
    in >> temp;
    name = name + " " + temp;
  }
  // discard parentheses from process name
  this->name_ = name.substr(1, name.length() - 2);

  std::string parent_pid_string;
  in >> parent_pid_string >> parent_pid_string;
  this->parent_pid_ = std::stoi(parent_pid_string);

  update_qtree_item();


  in.close();
}

RunningProcess::~RunningProcess() {
  if (tree_item_) {
    QTreeWidgetItem *parent = tree_item_->parent();
    if (parent != NULL) {
      parent->removeChild(tree_item_);
    } else {
      tree_item_->setHidden(true);
    }
    delete tree_item_;
  }
}

void RunningProcess::remove_child(RunningProcess *child) {
  if (child == NULL) {
    return;
  }
  auto i = children_.cbegin();
  while (i != children_.cend()) {
    RunningProcess *p = *i;
    if (p == child) {
      children_.erase(i++);
      tree_item_->removeChild(child->tree_item_);
      child->parent_ = NULL;
      return;
    } else {
      i++;
    }
  }
}

std::string RunningProcess::get_status_field(std::string field) {
  std::ifstream in("/proc/" + std::to_string(pid) + "/status");
  std::string line;
  while (std::getline(in, line)) {
    if (line.find(field) == 0) {
      std::string answer = line.substr(field.length());
      in.close();
      return answer;
    }
  }
  in.close();
  return "---";
}

// get_uid() to get real uid
// get_uid(false) to get effective uid

int RunningProcess::get_uid(bool real) {
  std::string uid_fields = get_status_field("Uid:");
  std::istringstream ss(uid_fields);
  int uid;
  ss >> uid;
  if (!real) {  // effective uid instead of real
    ss >> uid;
  }
  if (ss.fail()) {
    return -1;
  }
  return uid;
}


std::string RunningProcess::get_user() {
  int uid = get_uid();
  return uid_to_name(uid);
}

std::string RunningProcess::get_status() {
  std::string state_line = get_status_field("State:");
  std::istringstream stream(state_line);
  std::string state;
  stream >> state >> state;
  if (stream.fail()) {
    return "---";
  }

  // remove parentheses from the state
  return state.substr(1, state.length() - 2);
}


std::vector<file_info> RunningProcess::get_files() {
  std::vector<file_info> answer;
  std::string path = "/proc/" + std::to_string(this->pid) + "/fd";
  DIR *dir = opendir(path.c_str());
  if (dir == NULL) {
    // TODO(sam): better handle the case where we don't have permission
    // to open /proc/<pid>/fd
    return answer;  // return an empty list?
  }
  struct dirent *ent;
  while ((ent = readdir(dir)) != NULL) {
    if (is_number(ent->d_name)) {
      std::string fd_path = path + "/" + ent->d_name;
      std::string buffer(128, '\0');
      int bytes_read = readlink(fd_path.c_str(), &buffer[0],
        buffer.capacity() - 1);
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
      } else {
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

std::string RunningProcess::get_memory() {
  std::string mem_line = get_status_field("RssAnon:");
  return trim(mem_line);
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
    QTreeWidgetItem *item = new QTreeWidgetItem(
      static_cast<QTreeWidget *>(0), fields);
    tree->addTopLevelItem(item);
  }

  return tree;
}

unsigned long int RunningProcess::get_boot_time() {
  std::ifstream in("/proc/stat");
  if (!in) {
    return -1;
  }
  std::string line;
  while (std::getline(in, line)) {
    std::istringstream ss(line);
    std::string label;
    ss >> label;
    if (label.compare("btime") == 0) {
      unsigned long int answer;
      ss >> answer;
      in.close();
      return answer;
    }
  }
  in.close();
  return -1;
}

std::string RunningProcess::get_start_datetime() {
  std::ifstream in("/proc/" + std::to_string(pid) + "/stat");
  if (!in) {
    return "---";
  }
  std::string junk;
  for (int i = 0; i < 21; i++) {
    in >> junk;
  }
  unsigned long long start_time;
  in >> start_time;
  if (in.fail()) {
    return "---";
  }
  in.close();

  // start_time represents the time after system boot that the process started
  unsigned long int boot_time = get_boot_time();
  // boot_time represents the seconds since the epoch when the system booted.
  time_t real_time = boot_time + start_time / sysconf(_SC_CLK_TCK);
  char buff[128];
  ctime_r(&real_time, buff);
  return trim(std::string(buff));
}

std::string RunningProcess::get_shared_memory() {
  std::string shared_line = get_status_field("RssShmem:");
  return trim(shared_line);
}

std::string RunningProcess::get_resident_memory() {
  std::string res_line = get_status_field("VmRSS:");
  return trim(res_line);
}

std::string RunningProcess::get_virtual_memory() {
  std::string virt_line = get_status_field("VmSize:");
  return trim(virt_line);
}

long int RunningProcess::get_cpu_time_seconds() {
  // sum of stime and utime from /proc/[pid]/stat
  std::ifstream in("/proc/" + std::to_string(pid) + "/stat");
  if (!in) {
    return -1;
  }
  std::string junk;
  for (int i = 0; i < 13; i++) {
    in >> junk;
  }
  long int buff;
  long int answer = 0;
  for (int i = 0; i < 2; i++) {
    in >> buff;
    answer += buff;
  }
  if (in.fail()) {
    answer = -1;
  }
  in.close();
  return answer / sysconf(_SC_CLK_TCK);
}
std::string RunningProcess::get_cpu_time() {
  long int seconds = get_cpu_time_seconds();
  if (seconds == -1) {
    return "---";
  }
  return std::to_string(seconds) + std::string(" seconds");
}

int num_cpus() {
  std::ifstream in("/proc/cpuinfo");
  if (!in) {
    return 1;
  }

  int answer = 0;
  std::string line;
  while (std::getline(in, line)) {
    if (line.find("processor") == 0) {
      answer += 1;
    }
  }
  in.close();
  return answer;
}

std::string RunningProcess::get_cpu_percent() {
  long int seconds_running = get_cpu_time_seconds();

  std::ifstream uptime_in("/proc/uptime");
  if (!uptime_in) {
    return "0%";
  }
  double uptime;
  uptime_in >> uptime;
  if (uptime_in.fail()) {
    return "0%";
    uptime_in.close();
  }
  uptime_in.close();
  uptime *= num_cpus();
  double percent = 100 * (static_cast<double>(seconds_running)) / uptime;

  percent *= 1000;
  percent = (static_cast<int>(percent));
  percent /= 1000;
  std::ostringstream oss;
  oss << std::setprecision(5) << percent << "%";
  return oss.str();
}

std::string RunningProcess::calc_vm_size(std::string vm_start,
  std::string vm_end) {
  unsigned long long start_num, end_num;
  std::stringstream converter;
  converter << std::hex << vm_start;
  converter >> start_num;
  converter.clear();
  converter << vm_end;
  converter >> end_num;

  double vm_size_num = end_num - start_num;

  const char *suffixes[] = {" B", " kiB", " MiB", " GiB", " TB"};
  unsigned int i = 0;
  while (i < sizeof(suffixes) / sizeof(char *) - 1 && vm_size_num > 1024) {
    vm_size_num /= 1024;
    i++;
  }
  std::string vm_size = std::to_string(static_cast<int>(vm_size_num)) +
    suffixes[i];
  return vm_size;
}

QList<QTreeWidgetItem *> RunningProcess::get_map_items() {
  QList<QTreeWidgetItem *> answer;
  std::ifstream in("/proc/" + std::to_string(pid) + "/smaps");
  if (!in) {
    return answer;
  }
  std::string line;
  while (getline(in, line)) {
    std::string ranges, flags, offset, device, inode, name;
    device = "---";
    name = "---";
    inode = "---";
    std::istringstream ss(line);
    ss >> ranges >> flags >> offset >> device >> inode >> name;

    if (ranges.find("-") == std::string::npos) {
      continue;
    }
    std::string vm_start = ranges.substr(0, ranges.find("-"));
    std::string vm_end = ranges.substr(ranges.find("-") + 1);
    std::string vm_size = calc_vm_size(vm_start, vm_end);
    QStringList fields;
    fields.push_back(QString(name.c_str()));
    fields.push_back(QString(vm_start.c_str()));
    fields.push_back(QString(vm_end.c_str()));
    fields.push_back(QString(vm_size.c_str()));
    fields.push_back(QString(flags.c_str()));
    fields.push_back(QString(offset.c_str()));

    std::string shared_clean, shared_dirty, priv_clean, priv_dirty = "---";
    std::string line1;
    int to_find = 4;
    while (to_find > 0 && std::getline(in, line1)) {
      std::istringstream stream(line1);
      std::string label;
      stream >> label;
      if (label.compare("Shared_Clean:") == 0) {
        shared_clean = trim(line1.substr(strlen("Shared_Clean:")));
        to_find--;
      } else if (label.compare("Shared_Dirty:") == 0) {
        shared_dirty = trim(line1.substr(strlen("Shared_Dirty:")));
        to_find--;
      } else if (label.compare("Private_Clean:") == 0) {
        priv_clean = trim(line1.substr(strlen("Private_Clean:")));
        to_find--;
      } else if (label.compare("Private_Dirty:") == 0) {
        priv_dirty = trim(line1.substr(strlen("Private_Dirty:")));
        to_find--;
      }
    }
    fields.push_back(QString(priv_clean.c_str()));
    fields.push_back(QString(priv_dirty.c_str()));
    fields.push_back(QString(shared_clean.c_str()));
    fields.push_back(QString(shared_dirty.c_str()));
    fields.push_back(QString(device.c_str()));
    fields.push_back(QString(inode.c_str()));


    QTreeWidgetItem *item = new QTreeWidgetItem(static_cast<QTreeWidget *>(0),
      fields);
    answer.push_back(item);
  }
  in.close();
  return answer;
}

QTreeWidget *RunningProcess::get_mmap_tree() {
  QTreeWidget *mmap_tree = new QTreeWidget;

  mmap_tree->setColumnCount(6);

  QStringList header_labels;
  header_labels.push_back(QString("Filename"));
  header_labels.push_back(QString("VM Start"));
  header_labels.push_back(QString("VM End"));
  header_labels.push_back(QString("VM Size"));
  header_labels.push_back(QString("Flags"));
  header_labels.push_back(QString("VM Offset"));
  header_labels.push_back(QString("Private clean"));
  header_labels.push_back(QString("Private dirty"));
  header_labels.push_back(QString("Shared clean"));
  header_labels.push_back(QString("Shared dirty"));
  header_labels.push_back(QString("Device"));
  header_labels.push_back(QString("Inode"));

  mmap_tree->setHeaderLabels(header_labels);

  QList<QTreeWidgetItem *> maps = get_map_items();
  mmap_tree->addTopLevelItems(maps);
  return mmap_tree;
}
