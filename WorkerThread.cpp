#include "WorkerThread.h"

WorkerThread::WorkerThread()
{

}

void WorkerThread::closeEvent(QCloseEvent *event){
     qDebug() << "Close Worker";
}
