#ifndef TRANSMISSIONPROCESS_H
#define TRANSMISSIONPROCESS_H

#include<QProcess>

class TransmissionProcess
{
public:
    TransmissionProcess();

    QProcess transmissionProcess;

    bool start;

    //extern"C" launch();

    void reload();

signals:

    void waitForFinish();

    void torrentFinish();

};

#endif // TRANSMISSIONPROCESS_H
