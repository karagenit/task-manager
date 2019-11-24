#include "processes_tab.h"

#include <QLabel>
#include <QVBoxLayout>
#include <vector>
#include <string>
#include <map>
#include <iostream>
#include <dirent.h>
#include <QTreeWidget>
#include <QAction>
#include <QMenu>
#include <QDialog>

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


  DIR *dir = opendir("/proc/");
  if (dir != NULL) {
    struct dirent *ent;
    
    while ((ent = readdir(dir)) != NULL) {
      std::string proc_path = std::string("/proc/") + ent->d_name;
      if (is_number(ent->d_name)) {
        RunningProcess *proc = new RunningProcess(std::stoi(ent->d_name));
        answer.push_back(proc);
        proc_map_[std::stoi(ent->d_name)] = proc;
      }
    }
    closedir(dir);
  }

  std::vector<RunningProcess *> tree_roots;

  std::vector<RunningProcess *>::iterator i = answer.begin();

  while (i != answer.end()) {
    RunningProcess *proc = *i;
    int parent_pid = proc->get_parent_pid();
    if (proc_map_.find(parent_pid) != proc_map_.end()) {
      RunningProcess *parent = proc_map_[parent_pid];
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
  tree->setColumnCount(5);
  QStringList header_labels;
  header_labels.push_back(QString("Process Name"));
  header_labels.push_back(QString("Status"));
  header_labels.push_back(QString("% CPU"));
  header_labels.push_back(QString("PID"));
  header_labels.push_back(QString("Memory"));
  tree->setHeaderLabels(header_labels);

  tree->setContextMenuPolicy(Qt::CustomContextMenu);
  connect(tree, &QTreeWidget::customContextMenuRequested, this,
    &ProcessesTab::prepare_menu);
  tree_widget_ = tree;
  return tree;
}

void ProcessesTab::prepare_menu(const QPoint &pos) {
  QTreeWidgetItem *item = tree_widget_->itemAt(pos);


  QAction *stop_act = new QAction(tr("Stop Process"), this);
  
  int pid = item->text(3).toInt();
  QVariant qv_pid = qVariantFromValue(pid);
  stop_act->setData(qv_pid);
  connect(stop_act, SIGNAL(triggered()), this, SLOT(handle_stop()));
  QMenu menu(this);
  menu.addAction(stop_act);

  QAction *continue_act = new QAction(tr("Continue Process"), this);
  menu.addAction(continue_act);

  menu.addSeparator();

  QAction *end_act = new QAction(tr("End Process"), this);
  menu.addAction(end_act);

  QAction *kill_act = new QAction(tr("Kill Process"), this);
  menu.addAction(kill_act);

  menu.addSeparator();

  QAction *mmap_act = new QAction(tr("Memory Maps"), this);
  mmap_act->setData(qv_pid);
  connect(mmap_act, SIGNAL(triggered()), this, SLOT(handle_mmap_window()));
  menu.addAction(mmap_act);

  QAction *fd_act = new QAction(tr("Open Files"), this);
  fd_act->setData(qv_pid);
  connect(fd_act, SIGNAL(triggered()), this, SLOT(handle_fd_window()));
  menu.addAction(fd_act);

  menu.addSeparator();
  QAction *properties_act = new QAction(tr("Properties"), this);
  properties_act->setData(qv_pid);
  connect(properties_act, SIGNAL(triggered()), this, SLOT(handle_properties()));
  menu.addAction(properties_act);

  QPoint pt(pos);
  menu.exec(tree_widget_->mapToGlobal(pos));
}

int ProcessesTab::get_sender_pid() {
  QAction *act = qobject_cast<QAction *>(sender());
  QVariant v = act->data();
  int pid = v.value<int>();
  return pid;
}

void ProcessesTab::handle_stop() {
  int pid = get_sender_pid();
  std::cout << "stopping process " << pid << "\n";
}

void ProcessesTab::handle_properties() {
  int pid = get_sender_pid();

  QDialog popup(this);
  QVBoxLayout layout;
  std::string title = "Properties of process " + std::to_string(pid);
  QLabel title_label(tr(title.c_str()));
  layout.addWidget(&title_label);
  popup.setLayout(&layout);
  popup.exec();
}

void ProcessesTab::handle_fd_window() {
  int pid = get_sender_pid();
  RunningProcess *proc = proc_map_[pid];

  QDialog popup(this);
  QVBoxLayout layout;
  std::string title = "Files opened by " + expanded_name(proc) + ":";
  QLabel title_label(tr(title.c_str()));
  layout.addWidget(&title_label);

  QTreeWidget *tree = new QTreeWidget();
  tree->setColumnCount(3);
  QStringList header_labels;
  header_labels.push_back(QString("FD"));
  header_labels.push_back(QString("Type"));
  header_labels.push_back(QString("Object"));
  tree->setHeaderLabels(header_labels);

  std::vector<file_info> files = proc->get_files();

  auto i = files.begin();
  while (i != files.end()) {
    file_info info = *i;

    QStringList fields;
    fields.push_back(QString(std::to_string(info.fd).c_str())); //fd
    fields.push_back(QString(info.type.c_str())); //type
    fields.push_back(QString(info.object.c_str())); // object
    QTreeWidgetItem *item = new QTreeWidgetItem((QTreeWidget *)0, fields);
    tree->addTopLevelItem(item);

    i++;
  }

  layout.addWidget(tree);


  popup.setLayout(&layout);
  popup.exec();
}

void ProcessesTab::handle_mmap_window() {
  int pid = get_sender_pid();

  QDialog popup(this);
  QVBoxLayout layout;
  std::string title = "Memory maps for process " + std::to_string(pid);
  QLabel title_label(tr(title.c_str()));
  layout.addWidget(&title_label);
  popup.setLayout(&layout);
  popup.exec();
}

std::string ProcessesTab::expanded_name(RunningProcess *proc) {
  if (proc == NULL) {
    return "NULL";
  }
  return "process \"" + proc->get_name() + "\" (PID " + std::to_string(proc->pid) + ")";
}