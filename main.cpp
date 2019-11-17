#include <QApplication>
#include <QPushButton>
#include <QTabWidget>

#include "testtab.h"

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    QTabWidget *tabs = new QTabWidget;
    //tabs.addTab(button, "Test");
    tabs->addTab(new TestTab, "Test 1");
    tabs->addTab(new TestTab, "Test 2");
    tabs->show();
    return app.exec();
}
