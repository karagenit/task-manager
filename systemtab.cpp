#include "systemtab.h"
#include <QLabel>
#include <QVBoxLayout>

SystemTab::SystemTab(QWidget *parent) : QWidget(parent)
{
    QLabel *os_version = new QLabel(QString::fromStdString(get_os_version()));
    QLabel *kernel_version = new QLabel(QString::fromStdString(get_kernel_version()));
    QLabel *memory = new QLabel(QString::fromStdString(get_memory()));
    QLabel *cpu = new QLabel(QString::fromStdString(get_cpu_version()));
    QLabel *disk = new QLabel(QString::fromStdString(get_free_disk()));

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(os_version);
    layout->addWidget(kernel_version);
    layout->addWidget(memory);
    layout->addWidget(cpu);
    layout->addWidget(disk);
    setLayout(layout);
}

std::string SystemTab::get_os_version() {
    return "OS Version: " + popen_string("hostnamectl | grep \"Operating\" | awk -F ':' '{ print $2 }' | awk '{$1=$1;print}' | tr -d '\n'");
}

std::string SystemTab::get_kernel_version() {
    return "Kernel Version: " + popen_string("uname -r | tr -d '\n'");
}

std::string SystemTab::get_memory() {
    return "Total Memory: " + popen_string("awk '/MemTotal/{print $2 \" \" $3}' /proc/meminfo | tr -d '\n'");
}

std::string SystemTab::get_cpu_version() {
    return "Processor: " + popen_string("grep 'name' /proc/cpuinfo | uniq | awk -F ':' '{print $2}' | awk '{$1=$1;print}' | tr -d '\n'");
}

std::string SystemTab::get_free_disk() {
    return "Free Disk Space: " + popen_string("df -h --output=avail --total | awk 'END {print $1}' | tr -d '\n'");
}

std::string SystemTab::popen_string(std::string cmd) {
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
