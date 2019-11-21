#include "processes_tab.h"

#include <QLabel>
#include <QVBoxLayout>
#include <vector>
#include <string>
#include <map>
#include <dirent.h>

#include "running_process.h"
#include "helper_functions.h"

ProcessesTab::ProcessesTab(QWidget *parent) : QWidget(parent) {
  std::vector<RunningProcess *> all_processes = ProcessesTab::get_all_processes();

  QLabel *label = new QLabel(QString::fromStdString(std::string("coolio")));

  QVBoxLayout *layout = new QVBoxLayout;
  layout->addWidget(label);
  setLayout(layout);
  //TODO- actually render the UI from the list of all processes
}

std::vector<RunningProcess *> ProcessesTab::get_all_processes() {
  std::vector<RunningProcess *> answer;

  std::map<int, RunningProcess *> proc_map;
  DIR *dir = opendir("/proc/");
  if (dir != NULL) {
    struct dirent *ent;
    
    while ((ent = readdir(dir)) != NULL) {
      std::string proc_path = std::string("/proc/") + ent->d_name;
      if (is_number(proc_path)) {
        RunningProcess *proc = new RunningProcess(std::stoi(proc_path));
        answer.push_back(proc);
        proc_map[std::stoi(proc_path)] = proc;
      }
    }
    closedir(dir);
  }

  std::vector<RunningProcess *>::iterator i = answer.begin();
  while (i != answer.end()) {
    RunningProcess *proc = *i;
    int parent_pid = proc->get_parent_pid();
    if (proc_map.find(parent_pid) != proc_map.end()) {
      RunningProcess *parent = proc_map[parent_pid];
      parent->add_child(proc);
    }
    i++;
  }

  return answer;
}
