#include "testtab.h"
#include <QLabel>
#include <QVBoxLayout>

TestTab::TestTab(QWidget *parent) : QWidget(parent)
{
    QLabel *label = new QLabel(tr("Tab Contents"));
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(label);
    setLayout(layout);
}
