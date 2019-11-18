#include "systemtab.h"
#include <QLabel>
#include <QVBoxLayout>

SystemTab::SystemTab(QWidget *parent) : QWidget(parent)
{
    QLabel *label = new QLabel(tr("Tab Contents"));
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(label);
    setLayout(layout);
}
