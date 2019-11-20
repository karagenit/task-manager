#ifndef MENU_BAR_H_
#define MENU_BAR_H_
#include <QMenuBar>

class MenuBar : public QMenuBar {
  public:
    MenuBar();
  private:
    QMenu *moni_menu_;
    QMenu *edit_menu_;
    QMenu *view_menu_;
    QMenu *help_menu_;
};

#endif
