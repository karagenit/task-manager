#ifndef SYSTEMTAB_H
#define SYSTEMTAB_H

#include <QWidget>

class SystemTab : public QWidget
{
    Q_OBJECT
public:
    explicit SystemTab(QWidget *parent = nullptr);
    //TODO: implement this as part of some utils class? Doesn't really belong here,
    //      and is duplicated in RunningProcess
    std::string popen_string(std::string);
    std::string get_os_version();
    std::string get_kernel_version();
    std::string get_memory();
    std::string get_cpu_version();
    std::string get_free_disk();

signals:

public slots:
};

#endif // SYSTEMTAB_H
