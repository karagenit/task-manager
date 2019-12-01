#include "menu_bar.h"

#include <iostream>

MenuBar::MenuBar(ProcessesTab *procs_tab) {
  procs_tab_ = procs_tab;
  moni_menu_ = this->addMenu("Monitor");
  edit_menu_ = this->addMenu("Edit");
  view_menu_ = this->addMenu("View");
  my_procs_action_ = new QAction(tr("My processes"), view_menu_);
  connect(my_procs_action_, SIGNAL(triggered()), this, SLOT(my_procs_clicked()));

  all_procs_action_ = new QAction(tr("All processes"), view_menu_);
  connect(all_procs_action_, SIGNAL(triggered()), this, SLOT(all_procs_clicked()));

  set_filter_icon();
  view_menu_->addAction(my_procs_action_);
  view_menu_->addAction(all_procs_action_);

  help_menu_ = this->addMenu("Help");
}

void MenuBar::set_filter_icon() {
  const char *my_procs_icon = filtering_ ? "icons/check.svg" : "icons/empty_check.svg";
  const char *all_procs_icon = !filtering_ ? "icons/check.svg" : "icons/empty_check.svg";

  my_procs_action_->setIcon(QIcon(my_procs_icon));
  all_procs_action_->setIcon(QIcon(all_procs_icon));

  QMetaObject::invokeMethod(procs_tab_, "set_filtering", Qt::QueuedConnection,
    Q_ARG(bool, filtering_));
}

void MenuBar::my_procs_clicked() {
  if (!filtering_) {
    filtering_ = true;
    set_filter_icon();
  }
}

void MenuBar::all_procs_clicked() {
  if (filtering_) {
    filtering_ = false;
    set_filter_icon();
  }
}