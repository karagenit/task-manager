#ifndef RESOURCESTAB_H
#define RESOURCESTAB_H

#include <QWidget>
#include <QTimer>

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
};

#endif // RESOURCESTAB_H
