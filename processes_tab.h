#ifndef PROCESSES_TAB_H_
#define PROCESSES_TAB_H_

#include <QWidget>
#include <QLabel>
#include <map>
#include <vector>
#include <string>

#include "./running_process.h"

class ProcessesTab : public QWidget {
  Q_OBJECT

 public:
    explicit ProcessesTab(QWidget *parent = nullptr);

 private:
    bool filtering_ = false;
    bool dependencies_ = true;
    std::vector<RunningProcess *> get_root_processes();
    std::string expanded_name(RunningProcess *);
    QTreeWidget *tree_widget();
    QTreeWidget *tree_widget_;
    QLabel *load_label_;
    void update_load_label();
    void prepare_menu(const QPoint &pos);
    int get_sender_pid();
    std::map<int, RunningProcess *> proc_map_;
    std::string current_user();
    void restore_non_filtering();
    void delete_proc(RunningProcess *);

 signals:

 public slots:
    void handle_stop();
    void handle_continue();
    void handle_kill();

    void handle_properties();
    void handle_fd_window();
    void handle_mmap_window();
    void refresh();
    void handle_user_filter();

    void set_filtering(bool);
    void set_show_dependencies(bool);
};

#endif  // PROCESSES_TAB_H_
