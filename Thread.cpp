#include "Thread.h"

#include <QDebug>

Thread::Thread()
{

}

void Thread::run(){
    qDebug() << QThread::currentThreadId();
}
