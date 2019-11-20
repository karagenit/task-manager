#include <QApplication>
#include <QPushButton>
#include <QTabWidget>

#include "systemtab.h"
#include "resourcestab.h"
#include "file_system_tab.h"
#include "processes_tab.h"

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    QTabWidget *tabs = new QTabWidget;
    tabs->addTab(new SystemTab, "System Info");
    tabs->addTab(new ResourcesTab, "Resources");
    tabs->addTab(new FileSystemTab, "File System");
    tabs->addTab(new ProcessesTab, "Processes");
    tabs->show();
    return app.exec();
}
