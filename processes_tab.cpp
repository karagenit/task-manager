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

void expand_children(RunningProcess *proc, QTreeWidgetItem *tree) {
  if (proc == NULL || tree == NULL) {
    return;
  }
  QList<QTreeWidgetItem *> list;
  std::vector<RunningProcess *> children = proc->get_children();
  /*if (children.size() == 0) {
    return;
  }*/
  auto i = children.begin();
  while (i != children.end()) {
    RunningProcess *child = *i;
    QTreeWidgetItem *child_tree_widget = child->get_qtree_item();
    expand_children(child, child_tree_widget);
    list.append(child_tree_widget);
    i++;
  }
  tree->addChildren(list);
}

QList<QTreeWidgetItem *> get_root_items(std::vector<RunningProcess *> procs) {
  QList<QTreeWidgetItem *> answer;
  auto i = procs.begin();
  while (i != procs.end()) {
    RunningProcess *proc = *i;
    QTreeWidgetItem *root = proc->get_qtree_item();
    expand_children(proc, root);
    answer.append(root);
    i++;
  }
  return answer;
}

ProcessesTab::ProcessesTab(QWidget *parent) : QWidget(parent) {
  std::vector<RunningProcess *> root_procs = ProcessesTab::get_root_processes();

  QVBoxLayout *layout = new QVBoxLayout;
  layout->setMargin(0);
  layout->setSpacing(0);

  QTreeWidget *tree = tree_widget();
  QList<QTreeWidgetItem *> items;

  QList<QTreeWidgetItem *> root_items = get_root_items(root_procs);
  tree->insertTopLevelItems(0, root_items);
  layout->addWidget(tree);
  setLayout(layout);
}

std::vector<RunningProcess *> ProcessesTab::get_root_processes() {
  std::vector<RunningProcess *> answer;

  std::map<int, RunningProcess *> proc_map;
  DIR *dir = opendir("/proc/");
  if (dir != NULL) {
    struct dirent *ent;
    
    while ((ent = readdir(dir)) != NULL) {
      std::string proc_path = std::string("/proc/") + ent->d_name;
      if (is_number(ent->d_name)) {
        RunningProcess *proc = new RunningProcess(std::stoi(ent->d_name));
        answer.push_back(proc);
        proc_map[std::stoi(ent->d_name)] = proc;
      }
    }
    closedir(dir);
  }

  std::vector<RunningProcess *> tree_roots;

  std::vector<RunningProcess *>::iterator i = answer.begin();

  while (i != answer.end()) {
    RunningProcess *proc = *i;
    int parent_pid = proc->get_parent_pid();
    if (proc_map.find(parent_pid) != proc_map.end()) {
      RunningProcess *parent = proc_map[parent_pid];
      parent->add_child(proc);
    }
    else {
      tree_roots.push_back(proc);
    }
    i++;
  }

  return tree_roots;
}

QTreeWidget *ProcessesTab::tree_widget() {
  QTreeWidget *tree = new QTreeWidget();
  tree->setColumnCount(2);
  QStringList header_labels;
  header_labels.push_back(QString("Process Name"));
  header_labels.push_back(QString("Status"));
  header_labels.push_back(QString("% CPU"));
  header_labels.push_back(QString("PID"));
  header_labels.push_back(QString("Memory"));
  tree->setHeaderLabels(header_labels);
  return tree;
}
