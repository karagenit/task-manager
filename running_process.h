#include <string>
#include <vector>

typedef struct file_info {
  int fd;
  std::string type;
  std::string object;
} file_info;

class RunningProcess {
  public:
    RunningProcess(int pid); //specify parent/children in constructor?
    int pid;
    std::string get_status();
    std::string get_cpu_percent();
    std::string get_user();
    std::string get_memory();
    std::string get_virtual_memory();
    std::string get_cpu_time();
    std::string get_start_datetime();
    void stop();
    void resume();
    void kill();
    std::vector<file_info> get_files();
    std::vector<RunningProcess *> get_children();
    RunningProcess *get_parent();
    int get_parent_pid();
    void add_child(RunningProcess *);
    
  private:
    int parent_pid_;
    int owner_uid_;
    std::string name_;
    std::string popen_string(std::string);
    std::vector<RunningProcess *> children_;
  protected:
    RunningProcess *parent_;
    
};