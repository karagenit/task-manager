#ifndef RESOURCESTAB_H
#define RESOURCESTAB_H

#include <QWidget>
#include <QTimer>
#include <QtCharts>

#include <unistd.h>

#define CPU_GRAPH_RANGE (30)

using namespace QtCharts;

class ResourcesTab : public QWidget
{
    Q_OBJECT
public:
    explicit ResourcesTab(QWidget *parent = nullptr);
    double get_used_cpu();
    std::string popen_string(std::string cmd);

signals:

public slots:
    void updateGraphs();

private:
    QTimer *timer;
    QChart *cpuChart;
    QChartView *cpuChartView;
    QLineSeries *cpuSeries;
    QChart *memoryChart;
    QChartView *memoryChartView;
    QLineSeries *memorySeries;
    QChart *networkChart;
    QChartView *networkChartView;
    QLineSeries *networkSeries;
    int timeCount;
    int lastIdleCount;
    int lastUsedCount;
};

#endif // RESOURCESTAB_H
