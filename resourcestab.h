#ifndef RESOURCESTAB_H
#define RESOURCESTAB_H

#include <QWidget>
#include <QTimer>
#include <QtCharts>

#include <unistd.h>

#define CPU_GRAPH_RANGE (30)
#define MEM_GRAPH_RANGE (30)
#define NET_GRAPH_RANGE (30)

using namespace QtCharts;

class ResourcesTab : public QWidget
{
    Q_OBJECT
public:
    explicit ResourcesTab(QWidget *parent = nullptr);
    double get_used_cpu();
    int get_used_memory();
    int get_total_memory();
    int get_used_swap();
    int get_network_transmit();
    int get_network_receive();
    std::string popen_string(std::string cmd);

signals:

public slots:
    void updateGraphs();

private:
    QTimer *timer;
    int timeCount;

    QChart *cpuChart;
    QChartView *cpuChartView;
    QLineSeries *cpuSeries;
    int lastIdleCount;
    int lastUsedCount;

    QChart *memoryChart;
    QChartView *memoryChartView;
    QLineSeries *memorySeries;
    QLineSeries *swapSeries;

    QChart *networkChart;
    QChartView *networkChartView;
    QLineSeries *networkTransmitSeries;
    QLineSeries *networkReceiveSeries;
    int lastTransmitCount;
    int lastReceiveCount;
};

#endif // RESOURCESTAB_H