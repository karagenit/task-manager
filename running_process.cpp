#include "running_process.h"

#include <sstream>

RunningProcess::RunningProcess(int pid) {
  // set pid
  this->pid = pid;

  // set parent pid
  std::string parent_cmd = "cat /proc/" + std::to_string(this->pid) +
    "/stat" + " | cut -d' ' -f4";
  std::string parent_pid = popen_string(parent_cmd);
  this->parent_pid_ = std::stoi(parent_pid);

  // set name
  std::string name_cmd = "cat /proc/" + std::to_string(this->pid) +
    "/stat" + " | cut -d' ' -f2";
  this->name_ = popen_string(parent_cmd);
}

std::string RunningProcess::get_user() {
  std::string uid_cmd = "cat /proc/" + std::to_string(this->pid) + 
    "/uid_map";
  std::string uid_map = popen_string(uid_cmd);
  std::stringstream ss;
  ss << uid_map;
  //get the third int from uid_map (the process owner's uid)
  int uid;
  for (int i = 0; i < 3; i++) {
    ss >> uid;
  }
  return std::to_string(uid);
}

std::string RunningProcess::get_status() {
  std::string stat_cmd = "cat /proc/" + std::to_string(this->pid) +
    "/stat" + " | cut -d' ' -f3";
  return popen_string(stat_cmd);
}

/* inspired by 
https://www.jeremymorgan.com/tutorials/c-programming/how-to-capture-the-output-of-a-linux-command-in-c/ 
*/

std::string RunningProcess::popen_string(std::string cmd) {
  std::string answer;
  FILE *fp = popen(cmd.c_str(), "r");
  char buffer[1024];
  while (!feof(fp)) {
    if (fgets(buffer, 1024, fp)) {
      answer.append(buffer);
    }
  }
  pclose(fp);
  return answer;
}

std::vector<file_info> RunningProcess::get_files() {
  //TODO- handle the case where we don't have permission to view the fds
  std::vector<file_info> answer;
  std::string ls_fds = "ls /proc/" + std::to_string(this->pid) + "/fd";
  std::string all_fds = popen_string(ls_fds);
}