#include "./menu_bar.h"

#include <iostream>

MenuBar::MenuBar(ProcessesTab *procs_tab) {
  procs_tab_ = procs_tab;
  moni_menu_ = this->addMenu("Monitor");
  edit_menu_ = this->addMenu("Edit");
  view_menu_ = this->addMenu("View");
  my_procs_action_ = new QAction(tr("My processes"), view_menu_);
  connect(my_procs_action_, SIGNAL(triggered()), this,
    SLOT(my_procs_clicked()));

  all_procs_action_ = new QAction(tr("All processes"), view_menu_);
  connect(all_procs_action_, SIGNAL(triggered()), this,
    SLOT(all_procs_clicked()));

  set_filter_icon();
  view_menu_->addAction(my_procs_action_);
  view_menu_->addAction(all_procs_action_);

  view_menu_->addSeparator();

  dependencies_action_ = new QAction(tr("Show dependencies"), view_menu_);
  connect(dependencies_action_, SIGNAL(triggered()), this,
    SLOT(dependencies_clicked()));

  set_dependencies_icon();
  view_menu_->addAction(dependencies_action_);

  help_menu_ = this->addMenu("Help");
}

void MenuBar::set_filter_icon() {
  const char *my_procs_icon = filtering_ ? "icons/radio_button_checked.svg" :
    "icons/radio_button_unchecked.svg";
  const char *all_procs_icon = !filtering_ ? "icons/radio_button_checked.svg" :
    "icons/radio_button_unchecked.svg";

  my_procs_action_->setIcon(QIcon(my_procs_icon));
  all_procs_action_->setIcon(QIcon(all_procs_icon));
}

void MenuBar::set_dependencies_icon() {
  const char *icon = show_dependencies_ ? "icons/checkbox_checked.svg" :
    "icons/checkbox_unchecked.svg";
  dependencies_action_->setIcon(QIcon(icon));
}

void MenuBar::my_procs_clicked() {
  if (!filtering_) {
    filtering_ = true;
    set_filter_icon();
    QMetaObject::invokeMethod(procs_tab_, "set_filtering", Qt::QueuedConnection,
      Q_ARG(bool, filtering_));
  }
}

void MenuBar::all_procs_clicked() {
  if (filtering_) {
    filtering_ = false;
    set_filter_icon();
    QMetaObject::invokeMethod(procs_tab_, "set_filtering", Qt::QueuedConnection,
      Q_ARG(bool, filtering_));
  }
}

void MenuBar::dependencies_clicked() {
  show_dependencies_ = !show_dependencies_;
  set_dependencies_icon();
  QMetaObject::invokeMethod(procs_tab_, "set_show_dependencies",
    Qt::QueuedConnection, Q_ARG(bool, show_dependencies_));
}
