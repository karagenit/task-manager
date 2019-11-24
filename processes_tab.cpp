#include "processes_tab.h"

#include <QLabel>
#include <QVBoxLayout>
#include <vector>
#include <string>
#include <map>
#include <iostream>
#include <dirent.h>
#include <QTreeWidget>

#include "running_process.h"
#include "helper_functions.h"

ProcessesTab::ProcessesTab(QWidget *parent) : QWidget(parent) {
  std::vector<RunningProcess *> all_processes = ProcessesTab::get_all_processes();

  QVBoxLayout *layout = new QVBoxLayout;
  layout->setMargin(0);
  layout->setSpacing(0);

  QTreeWidget *tree = new QTreeWidget();
  tree->setColumnCount(2);
  QStringList header_labels;
  header_labels.push_back(QString("PID"));
  header_labels.push_back(QString("Name"));
  tree->setHeaderLabels(header_labels);
  QList<QTreeWidgetItem *> items;
  for (int i = 0; i < 50; i++) {
    QTreeWidgetItem *item = new QTreeWidgetItem((QTreeWidget *)0, QStringList(QString("item: %1").arg(i)));
    QTreeWidgetItem *child = new QTreeWidgetItem((QTreeWidget *)0, QStringList(QString("item: %1").arg(i)));
    item->addChild(child);
    items.append(item);

  }
  tree->insertTopLevelItems(0, items);
  
  layout->addWidget(tree);
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
