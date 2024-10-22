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
    memorySeries->setName("RAM");
    swapSeries = new QLineSeries;
    swapSeries->setName("Swap");
    memoryChart = new QChart;
    memoryChart->legend()->setVisible(true);
    memoryChart->addSeries(memorySeries);
    memoryChart->addSeries(swapSeries);
    memoryChart->createDefaultAxes();
    memoryChart->axisX()->hide();
    memoryChart->axisY()->setRange(0, get_max_mem_swap());
    memoryChart->setTitle("Memory Usage");
    memoryChartView = new QChartView(memoryChart);
    memoryChartView->setRenderHint(QPainter::Antialiasing);

    /* SETUP NETWORK CHART */

    networkTransmitSeries = new QLineSeries;
    networkTransmitSeries->setName("Transmit");
    networkReceiveSeries = new QLineSeries;
    networkReceiveSeries->setName("Receive");
    networkChart = new QChart;
    networkChart->legend()->setVisible(true);
    networkChart->addSeries(networkReceiveSeries);
    networkChart->addSeries(networkTransmitSeries);
    networkChart->createDefaultAxes();
    networkChart->axisX()->hide();
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
    maxNetworkValue = 100000;

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

    int transmit = get_network_transmit();
    int receive = get_network_receive();
    networkTransmitSeries->append(timeCount, transmit);
    networkReceiveSeries->append(timeCount, receive);
    maxNetworkValue = std::max(maxNetworkValue, transmit);
    maxNetworkValue = std::max(maxNetworkValue, receive);
    networkChart->axisX()->setRange(timeCount - NET_GRAPH_RANGE, timeCount);
    networkChart->axisY()->setRange(0, maxNetworkValue);
    networkChartView->repaint();

    timeCount++;
}

double ResourcesTab::get_used_cpu() {
    std::ifstream proc_stat("/proc/stat");
    std::string cpu;
    int userCount, niceCount, systemCount, idleCount;
    proc_stat >> cpu >> userCount >> niceCount >> systemCount >> idleCount;
    proc_stat.close();
    int usedCount = userCount + systemCount;
    int idleTicks = idleCount - lastIdleCount;
    int usedTicks = usedCount - lastUsedCount;
    double usedPercent = 100.0 * usedTicks / (usedTicks + idleTicks);
    lastIdleCount = idleCount;
    lastUsedCount = usedCount;
    return usedPercent;
}

int ResourcesTab::get_used_memory() {
    return get_total_memory() - get_free_memory();
}

int ResourcesTab::get_free_memory() {
    std::ifstream meminfo("/proc/meminfo");
    std::string key, units;
    int value;
    meminfo >> key >> value >> units;
    meminfo >> key >> value >> units;
    meminfo >> key >> value >> units;
    return value;
}

int ResourcesTab::get_total_memory() {
    std::ifstream meminfo("/proc/meminfo");
    std::string key, units;
    int value;
    meminfo >> key >> value >> units;
    return value;
}

int ResourcesTab::get_total_swap() {
    std::ifstream meminfo("/proc/meminfo");
    std::string key, units;
    int value;
    for (int i = 0; i < 15; i++) {
        meminfo >> key >> value >> units;
    }
    return value;
}

int ResourcesTab::get_used_swap() {
    return get_total_swap() - get_free_swap();
}

int ResourcesTab::get_free_swap() {
    std::ifstream meminfo("/proc/meminfo");
    std::string key, units;
    int value;
    for (int i = 0; i < 16; i++) {
        meminfo >> key >> value >> units;
    }
    return value;
}

int ResourcesTab::get_max_mem_swap() {
    return std::max(get_total_memory(), get_total_swap());
}

int ResourcesTab::get_network_transmit() {
    std::ifstream net_dev("/proc/net/dev");
    std::string name;
    long transmit_sum = 0;
    long t;

    // Skip two header lines
    std::getline(net_dev, name);
    std::getline(net_dev, name);

    while (net_dev >> name >> t >> t >> t >> t >> t >> t >> t >> t >> t) {
        // TODO: only if matching eth* ?
        transmit_sum += t;
        std::getline(net_dev, name);
    }

    int diff = transmit_sum - lastTransmitCount;
    lastTransmitCount = transmit_sum;
    return diff;
}

int ResourcesTab::get_network_receive() {
    std::ifstream net_dev("/proc/net/dev");
    std::string name;
    long receive_sum = 0;
    long r;

    // Skip two header lines
    std::getline(net_dev, name);
    std::getline(net_dev, name);

    while (net_dev >> name >> r) {
        // TODO: only if matching eth* ?
        receive_sum += r;
        std::getline(net_dev, name);
    }

    int diff = receive_sum - lastReceiveCount;
    lastReceiveCount = receive_sum;
    return diff;
}
