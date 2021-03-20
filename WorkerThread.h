#ifndef WORKERTHREAD_H
#define WORKERTHREAD_H

#include <QObject>
#include <QCloseEvent>
#include <QDebug>

class WorkerThread : public QObject
{

public:
    WorkerThread();

signals:
    void closeEvent(QCloseEvent *event);
};

#endif // WORKERTHREAD_H
