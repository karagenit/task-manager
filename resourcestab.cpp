#include "resourcestab.h"

ResourcesTab::ResourcesTab(QWidget *parent) : QWidget(parent)
{
    // Create a Qt Timer to periodically call updateGraphs()
    // every 1000 ms.
    timer = new QTimer(this);
    QObject::connect(timer, SIGNAL(timeout()), this, SLOT(updateGraphs()));
    timer->start(1000);

    timeCount = 0;
    series = new QLineSeries();

    chart = new QChart();
    chart->legend()->hide();
    chart->addSeries(series);
    chart->createDefaultAxes();
    chart->setTitle("CPU Usage");

    chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(chartView);
    setLayout(layout);

    // For some reason, it segfaults if we try to set this
    // when we set up the other chart elements...
    chart->axisY()->setRange(0, 100);

    lastIdleCount = 0;
    lastUsedCount = 0;
    // call once to properly set lastIdleCount
    get_used_cpu();
}

void ResourcesTab::updateGraphs() {
    series->append(timeCount, get_used_cpu());
    chart->axisX()->setRange(0, timeCount);
    chartView->repaint();
    timeCount++;
}

double ResourcesTab::get_used_cpu() {
    std::string idle = popen_string("cat /proc/stat | head -n 1 | awk '{print $5}'");
    std::string used = popen_string("cat /proc/stat | head -n 1 | awk '{print ($2+$4)}'");
    int idleCount = stoi(idle, nullptr);
    int usedCount = stoi(used, nullptr);
    int idleTicks = idleCount - lastIdleCount;
    int usedTicks = usedCount - lastUsedCount;
    double usedPercent = 100.0 * usedTicks / (usedTicks + idleTicks);
    qDebug() << usedPercent;
    lastIdleCount = idleCount;
    lastUsedCount = usedCount;
    return usedPercent;
}

std::string ResourcesTab::popen_string(std::string cmd) {
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
