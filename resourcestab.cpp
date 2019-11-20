#include "resourcestab.h"

ResourcesTab::ResourcesTab(QWidget *parent) : QWidget(parent)
{
    // Create a Qt Timer to periodically call updateGraphs()
    // every 1000 ms.
    timer = new QTimer(this);
    QObject::connect(timer, SIGNAL(timeout()), this, SLOT(updateGraphs()));
    timer->start(1000);
    timeCount = 0;

    /* SETUP CPU CHART */

    cpuSeries = new QLineSeries();
    cpuChart = new QChart();
    cpuChart->legend()->hide();
    cpuChart->addSeries(cpuSeries);
    cpuChart->createDefaultAxes();
    cpuChart->axisX()->hide();
    cpuChart->setTitle("CPU Usage");
    cpuChartView = new QChartView(cpuChart);
    cpuChartView->setRenderHint(QPainter::Antialiasing);
    cpuChart->axisY()->setRange(0, 100);

    /* SETUP MEMORY CHART */

    memorySeries = new QLineSeries;
    swapSeries = new QLineSeries;
    memoryChart = new QChart;
    memoryChart->legend()->hide();
    memoryChart->addSeries(memorySeries);
    memoryChart->addSeries(swapSeries);
    memoryChart->createDefaultAxes();
    memoryChart->axisX()->hide();
    // TODO: what if swap > ram?
    memoryChart->axisY()->setRange(0, get_total_memory());
    memoryChart->setTitle("Memory Usage");
    memoryChartView = new QChartView(memoryChart);
    memoryChartView->setRenderHint(QPainter::Antialiasing);

    /* SETUP NETWORK CHART */

    networkTransmitSeries = new QLineSeries;
    networkReceiveSeries = new QLineSeries;
    networkChart = new QChart;
    networkChart->legend()->hide();
    networkChart->addSeries(networkReceiveSeries);
    networkChart->addSeries(networkTransmitSeries);
    networkChart->createDefaultAxes();
    networkChart->axisX()->hide();
    // TODO: big enough? autoscale?
    networkChart->axisY()->setRange(0, 100000);
    networkChart->setTitle("Network Usage");
    networkChartView = new QChartView(networkChart);
    networkChartView->setRenderHint(QPainter::Antialiasing);

    /* SETUP PAGE LAYOUT */

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(cpuChartView);
    layout->addWidget(memoryChartView);
    layout->addWidget(networkChartView);
    setLayout(layout);

    lastIdleCount = 0;
    lastUsedCount = 0;
    lastReceiveCount = 0;
    lastTransmitCount = 0;

    // We call these once so the various lastCounts
    // are set properly.
    get_used_cpu();
    get_network_receive();
    get_network_transmit();
}

void ResourcesTab::updateGraphs() {
    cpuSeries->append(timeCount, get_used_cpu());
    cpuChart->axisX()->setRange(timeCount - CPU_GRAPH_RANGE, timeCount);
    cpuChartView->repaint();

    memorySeries->append(timeCount, get_used_memory());
    swapSeries->append(timeCount, get_used_swap());
    memoryChart->axisX()->setRange(timeCount - MEM_GRAPH_RANGE, timeCount);
    memoryChartView->repaint();

    networkTransmitSeries->append(timeCount, get_network_transmit());
    networkReceiveSeries->append(timeCount, get_network_receive());
    networkChart->axisX()->setRange(timeCount - NET_GRAPH_RANGE, timeCount);
    networkChartView->repaint();

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
    lastIdleCount = idleCount;
    lastUsedCount = usedCount;
    return usedPercent;
}

int ResourcesTab::get_used_memory() {
    return stoi(popen_string("free -m | grep 'Mem:' | awk '{print $3}'"));
}

int ResourcesTab::get_total_memory() {
    return stoi(popen_string("free -m | grep 'Mem:' | awk '{print $2}'"));
}

int ResourcesTab::get_used_swap() {
    return stoi(popen_string("free -m | grep 'Swap:' | awk '{print $3}'"));
}

int ResourcesTab::get_network_transmit() {
    int transBytes = stoi(popen_string("tail -n +3 /proc/net/dev | awk '{SUM += $10} END {print SUM}'"));
    int bytesDiff = transBytes - lastTransmitCount;
    lastTransmitCount = transBytes;
    return bytesDiff;
}

int ResourcesTab::get_network_receive() {
    int recvBytes = stoi(popen_string("tail -n +3 /proc/net/dev | awk '{SUM += $2} END {print SUM}'"));
    int bytesDiff = recvBytes - lastReceiveCount;
    lastReceiveCount = recvBytes;
    return bytesDiff;
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