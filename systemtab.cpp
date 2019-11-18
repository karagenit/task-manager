#include "systemtab.h"
#include <QLabel>
#include <QVBoxLayout>

SystemTab::SystemTab(QWidget *parent) : QWidget(parent)
{
    QLabel *os_version = new QLabel("OS Version: Debian 10");
    QLabel *kernel_version = new QLabel("Kernel Version: 1.0.0");
    QLabel *memory = new QLabel("Memory: 16 GB");
    QLabel *cpu = new QLabel("Processor: Intel i5-3320M, 2 Cores @ 2.60 GHz");
    QLabel *disk = new QLabel("Free Disk Space: 182 GB");

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(os_version);
    layout->addWidget(kernel_version);
    layout->addWidget(memory);
    layout->addWidget(cpu);
    layout->addWidget(disk);
    setLayout(layout);
}
