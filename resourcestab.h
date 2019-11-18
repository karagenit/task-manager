#ifndef RESOURCESTAB_H
#define RESOURCESTAB_H

#include <QWidget>
#include <QTimer>
#include <QtCharts>

using namespace QtCharts;

class ResourcesTab : public QWidget
{
    Q_OBJECT
public:
    explicit ResourcesTab(QWidget *parent = nullptr);

signals:

public slots:
    void updateGraphs();

private:
    QTimer *timer;
    QChart *chart;
    QChartView *chartView;
    QLineSeries *series;
};

#endif // RESOURCESTAB_H
