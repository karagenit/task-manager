#ifndef RUNNING_PROCESS_H_
#define RUNNING_PROCESS_H_

#include <QTreeWidget>
#include <string>
#include <vector>

typedef struct file_info {
  int fd;
  std::string type;
  std::string object;
} file_info;

class RunningProcess {
 public:
    explicit RunningProcess(int pid);
    ~RunningProcess();
    bool hide;
    int pid;
    std::string get_status();
    std::string get_cpu_percent();
    std::string get_user();
    std::string get_memory();
    std::string get_virtual_memory();
    std::string get_shared_memory();
    std::string get_resident_memory();

    std::string get_cpu_time();
    std::string get_start_datetime();
    std::string get_name();
    void stop();
    void continue_proc();
    void end();
    void kill();
    std::vector<file_info> get_files();
    std::vector<RunningProcess *> get_children();
    RunningProcess *get_parent();
    int get_parent_pid();
    void add_child(RunningProcess *);
    void remove_child(RunningProcess *);
    QTreeWidgetItem *get_qtree_item();
    void update_qtree_item();
    QTreeWidget *get_detailed_view();
    QTreeWidget *get_mmap_tree();

 private:
    int parent_pid_;
    int get_uid(bool real = true);
    std::string name_;
    std::string get_status_field(std::string);
    std::vector<RunningProcess *> children_;
    unsigned long int get_boot_time();
    long int get_cpu_time_seconds();
    QList<QTreeWidgetItem *> get_map_items();
    std::string calc_vm_size(std::string, std::string);

 protected:
    RunningProcess *parent_;
    QTreeWidgetItem *tree_item_;
};

#endif  // RUNNING_PROCESS_H_
