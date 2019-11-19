#ifndef RESOURCESTAB_H
#define RESOURCESTAB_H

#include <QWidget>
#include <QTimer>
#include <QtCharts>

#include <unistd.h>

using namespace QtCharts;

class ResourcesTab : public QWidget
{
    Q_OBJECT
public:
    explicit ResourcesTab(QWidget *parent = nullptr);
    double get_idle_cpu();
    std::string popen_string(std::string cmd);

signals:

public slots:
    void updateGraphs();

private:
    QTimer *timer;
    QChart *chart;
    QChartView *chartView;
    QLineSeries *series;
    int timeCount;
    long timeScale;
    int lastIdleCount;
};

#endif // RESOURCESTAB_H
