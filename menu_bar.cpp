#include "menu_bar.h"

#include <iostream>

MenuBar::MenuBar() {
  moni_menu_ = this->addMenu("Monitor");
  edit_menu_ = this->addMenu("Edit");
  view_menu_ = this->addMenu("View");
  filter_action_ = new QAction(tr("Show only processes owned by you"), view_menu_);
  set_filter_icon();
  connect(filter_action_, SIGNAL(triggered()), this, SLOT(handle_filter_clicked()));
  view_menu_->addAction(filter_action_);

  help_menu_ = this->addMenu("Help");
}

void MenuBar::set_filter_icon() {
  const char *icon = filtering_ ? "icons/check.svg" : "icons/empty_check.svg";
  filter_action_->setIcon(QIcon(icon));
  
}

void MenuBar::handle_filter_clicked() {
  filtering_ = !filtering_;
  set_filter_icon();
}
