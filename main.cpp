#include <QApplication>
#include <QPushButton>
#include <QTabWidget>

#include "systemtab.h"
#include "resourcestab.h"

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    QTabWidget *tabs = new QTabWidget;
    tabs->addTab(new SystemTab, "System Info");
    tabs->addTab(new ResourcesTab, "Resources");
    tabs->show();
    return app.exec();
}
