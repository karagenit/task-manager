#ifndef MENU_BAR_H_
#define MENU_BAR_H_
#include <QMenuBar>

#include "./processes_tab.h"

class MenuBar : public QMenuBar {
  Q_OBJECT

 public:
    MenuBar(ProcessesTab *);

 private:
    QMenu *moni_menu_;
    QMenu *edit_menu_;
    QMenu *view_menu_;
    QMenu *help_menu_;
    QAction *all_procs_action_;
    QAction *my_procs_action_;
    QAction *dependencies_action_;
    bool filtering_ = false;
    bool show_dependencies_ = true;
    void set_filter_icon();
    void set_dependencies_icon();
    ProcessesTab *procs_tab_;
 public slots:
    void my_procs_clicked();
    void all_procs_clicked();
    void dependencies_clicked();
};

#endif  // MENU_BAR_H_
