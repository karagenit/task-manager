#include <QApplication>
#include <QPushButton>
#include <QTabWidget>

#include "systemtab.h"
#include "resourcestab.h"
#include "file_system_tab.h"
#include "processes_tab.h"
#include "menu_bar.h"

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    QRect rec = app.desktop()->screenGeometry();
    QMainWindow *main_window = new QMainWindow;
    main_window->setMinimumSize(rec.width() * 0.5, rec.height() * 0.5);
    QTabWidget *tabs = new QTabWidget;
    tabs->addTab(new SystemTab, "System Info");
    tabs->addTab(new ResourcesTab, "Resources");
    tabs->addTab(new FileSystemTab, "File System");
    tabs->addTab(new ProcessesTab, "Processes");
    main_window->setCentralWidget(tabs);
    main_window->setMenuBar(new MenuBar);

    main_window->show();
    //tabs->show();
    return app.exec();
}
