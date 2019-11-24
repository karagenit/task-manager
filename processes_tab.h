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
    static QTreeWidget *tree_widget();

signals:
public slots:
};

#endif
