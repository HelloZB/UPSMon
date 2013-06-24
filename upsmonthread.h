#ifndef UPSMONTHREAD_H
#define UPSMONTHREAD_H

#include <QThread>
#include "checkcurrport.h"
#include <QStringList>
#include <QtAddOnSerialPort/serialport.h>
#include <QtAddOnSerialPort/serialport-global.h>
#include <QTimer>

QT_BEGIN_NAMESPACE_SERIALPORT
class SerialPort;
QT_END_NAMESPACE_SERIALPORT

QT_USE_NAMESPACE_SERIALPORT

class UPSMonThread : public QThread
{
    Q_OBJECT
public:
    explicit UPSMonThread(QObject *parent = 0);
    void run();

public slots:
    void stopSlot();
    void closePort();
    void changeBatAndWaitTime(int waitTime, int battVal);

private slots:
    bool openUPSPort();
    void signalToOff();
signals:
    void vyvidDanyh(QStringList);
    void vyvidMess(QString);
    void vyvidBattStatus(QString);
    void offcomp();
    
public slots:

private:
    bool stopAll;
    CheckCurrPort myPorts;
    SerialPort *mySerial;

    QTimer offTimer;

    int waitTime;
    int battVal;

    bool offSystemRun;


};

#endif // UPSMONTHREAD_H
