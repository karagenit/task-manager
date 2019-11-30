#ifndef PROCESSES_TAB_H_
#define PROCESSES_TAB_H_

#include <QWidget>
#include <map>

#include "running_process.h"

class ProcessesTab : public QWidget {
  Q_OBJECT
  public:
    explicit ProcessesTab(QWidget *parent = nullptr);
  private:
    std::vector<RunningProcess *> get_root_processes();
    std::string expanded_name(RunningProcess *);
    QTreeWidget *tree_widget();
    QTreeWidget *tree_widget_;
    void prepare_menu(const QPoint &pos);
    int get_sender_pid();
    std::map<int, RunningProcess *> proc_map_;
    std::string current_user();

  signals:

  public slots:
    void handle_stop();
    void handle_continue();
   // void handle_end();
    void handle_kill();

    void handle_properties();
    void handle_fd_window();
    void handle_mmap_window();
    void refresh();
    void handle_user_filter();

};

#endif
