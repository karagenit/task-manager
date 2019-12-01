#ifndef MENU_BAR_H_
#define MENU_BAR_H_
#include <QMenuBar>

class MenuBar : public QMenuBar {
  Q_OBJECT

  public:
    MenuBar();
  private:
    QMenu *moni_menu_;
    QMenu *edit_menu_;
    QMenu *view_menu_;
    QMenu *help_menu_;
    QAction *filter_action_;
    bool filtering_ = false;
    void set_filter_icon();
  public slots:
    void handle_filter_clicked();
};

#endif
