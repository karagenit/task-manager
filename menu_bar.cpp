#include "menu_bar.h"

MenuBar::MenuBar() {
  moni_menu_ = this->addMenu("Monitor");
  edit_menu_ = this->addMenu("Edit");
  view_menu_ = this->addMenu("View");
  help_menu_ = this->addMenu("Help");
}
