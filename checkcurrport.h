#ifndef CHECKCURRPORT_H
#define CHECKCURRPORT_H

#include <QThread>
#include <QtAddOnSerialPort/serialport.h>
#include <QtAddOnSerialPort/serialport-global.h>

QT_BEGIN_NAMESPACE_SERIALPORT
class SerialPort;
QT_END_NAMESPACE_SERIALPORT

QT_USE_NAMESPACE_SERIALPORT

class CheckCurrPort : public QThread
{
    Q_OBJECT
public:
    explicit CheckCurrPort(QObject *parent = 0);
    void zapuskalka(QString currPort);
signals:
    void portDisconnected();
    void terminateNow();
    
public slots:
    
private slots:
    void run();

private:
    QString currPort;
};

#endif // CHECKCURRPORT_H
