#ifndef PROCESSES_TAB_H_
#define PROCESSES_TAB_H_

#include <QWidget>

#include "running_process.h"

class ProcessesTab : public QWidget {
  Q_OBJECT
  public:
    explicit ProcessesTab(QWidget *parent = nullptr);
  private:
    static std::vector<RunningProcess *> get_root_processes();
    QTreeWidget *tree_widget();
    QTreeWidget *tree_widget_;
    void prepare_menu(const QPoint &pos);

  signals:

  public slots:
    void handle_stop();
};

#endif
