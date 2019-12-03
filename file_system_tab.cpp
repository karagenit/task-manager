#include "file_system_tab.h"
#include "helper_functions.h"
#include <QLabel>
#include <QVBoxLayout>
#include <vector>
#include <QTableWidget>
#include <QHeaderView>
#include <QProgressBar>

#include "file_system.h"

using namespace std;

vector<QWidget *> get_qlabel_entries(FileSystemEntry entry, QWidget *parent) {
  vector<QWidget *> widgets;

  QLabel *device = new QLabel(parent);
  QLabel *directory = new QLabel(parent);
  QLabel *type = new QLabel(parent);
  QLabel *total = new QLabel(parent);
  QLabel *free = new QLabel(parent);
  QLabel *available = new QLabel(parent);
  QWidget *used_widget = new QWidget(parent);
  QHBoxLayout *used_layout = new QHBoxLayout;
  QProgressBar *used = new QProgressBar;
  QLabel *used_label = new QLabel;

  used->setRange(0, entry.TotalValue());
  used->setValue(entry.UsedValue());

  used_label->setText(QString::fromStdString(entry.GetUsed() + "  "));
  used_layout->addWidget(used_label);
  used_layout->addWidget(used, Qt::AlignRight);

  used_widget->setLayout(used_layout);
  device->setText(QString::fromStdString(entry.GetDevice() + "  "));
  directory->setText(QString::fromStdString(entry.GetDirectory() + "  "));
  type->setText(QString::fromStdString(entry.GetType() + "  "));
  total->setText(QString::fromStdString(entry.GetTotal() + "  "));
  free->setText(QString::fromStdString(entry.GetFree() + "  "));
  available->setText(QString::fromStdString(entry.GetAvailable() + "  "));

  widgets.push_back(device);
  widgets.push_back(directory);
  widgets.push_back(type);
  widgets.push_back(total);
  widgets.push_back(free);
  widgets.push_back(available);
  widgets.push_back(used_widget);

  return widgets;
}

FileSystemTab::FileSystemTab(QWidget *parent) : QWidget(parent) {
  QVBoxLayout *layout = new QVBoxLayout;
  layout->setMargin(0);
  layout->setSpacing(0);

  QTableWidget *table = new QTableWidget;
  table->verticalHeader()->setVisible(false);
  table->setShowGrid(false);
  QStringList headers;
  headers << "Device" << "Directory" << "Type" << "Total" << "Free" << "Available" << "Used";
  vector<FileSystemEntry> entries = read_file_system_entries();
  table->setRowCount(entries.size() + 1);
  table->setColumnCount(7);
  table->setHorizontalHeaderLabels(headers);
  for (unsigned i = 0; i < entries.size(); i++) {
    auto widgets =  get_qlabel_entries(entries[i], table);
    for (unsigned j = 0; j < widgets.size(); j++) {
      table->setCellWidget(i, j, widgets[j]);
    }
  }
  table->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
  table->horizontalHeader()->setStretchLastSection(true);

  layout->addWidget(table);
  setLayout(layout);
}
