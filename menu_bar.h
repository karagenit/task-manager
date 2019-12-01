#ifndef MENU_BAR_H_
#define MENU_BAR_H_
#include <QMenuBar>

#include "processes_tab.h"

class MenuBar : public QMenuBar {
  Q_OBJECT

  public:
    MenuBar(ProcessesTab *);
  private:
    QMenu *moni_menu_;
    QMenu *edit_menu_;
    QMenu *view_menu_;
    QMenu *help_menu_;
    QAction *filter_action_;
    bool filtering_ = false;
    void set_filter_icon();
    ProcessesTab *procs_tab_;
  public slots:
    void handle_filter_clicked();
};

#endif
