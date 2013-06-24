#include "upsmonthread.h"
#include <QtAddOnSerialPort/serialportinfo.h>
//#include <QDebug>
#include <QProcess>

//=================================================================================================================
UPSMonThread::UPSMonThread(QObject *parent) :
    QThread(parent)
{
    stopAll=1;
    connect(&myPorts, SIGNAL(portDisconnected()), this, SLOT(stopSlot()));
    connect(&myPorts, SIGNAL(terminateNow()), this, SLOT(terminate()));
    connect(this, SIGNAL(terminated()), SLOT(closePort()));
    waitTime=3;
    battVal=80;
    offTimer.setSingleShot(1);
    connect(&offTimer, SIGNAL(timeout()), SLOT(signalToOff()));
    offSystemRun=0;

}
//=================================================================================================================

void UPSMonThread::run()
{
    mySerial=new SerialPort();
    int bat=0;
    int Vin=0;
    int Hin=0;
    int LL=0;
    int Vout=0;
    int Hout=0;
    int counter=0;
    while(1){
        if(stopAll){
            if(offTimer.isActive())
                offTimer.stop();

            if(! openUPSPort())
                sleep(3);

        }
        else{
            if(!stopAll){
                mySerial->write(QByteArray::fromHex("1"));
                //qDebug("01");
            }
            if(!stopAll)
                mySerial->waitForBytesWritten(50);
            if(!stopAll)
                mySerial->waitForReadyRead(1000);
            QByteArray readArr;
            if(!stopAll)
                readArr =mySerial->readAll();

            for(int i=0; i<999 && !stopAll && readArr.size()<16; i++ ){
                if(mySerial->waitForReadyRead(50) && !stopAll)
                    readArr.append(mySerial->readAll());
                else
                    break;
            }
           // qDebug()<< readArr.toHex();
            if(readArr.size()>=16){

                bat+=readArr.toHex().mid(2,2).toInt(0,16);
                Vin+=readArr.toHex().mid(4,2).toInt(0,16)*2;
                Hin+=readArr.toHex().mid(8,2).toInt(0,16);
                LL+=readArr.toHex().mid(0,2).toInt(0,16);
                Vout+=readArr.toHex().mid(6,2).toInt(0,16)*2;
                Hout+=readArr.toHex().mid(12,2).toInt(0,16);
                counter++;
                if(counter>9){

                    QStringList list;
                    list.append(QString::number(bat/10));//bat
                    list.append(QString::number(Vin/10));//V in
                    list.append(QString::number(Hin/10));//H in
                    list.append(QString::number(LL/10));//Load level

                    list.append(QString::number(Vout/10 ));//V out
                    list.append(QString::number(Hout/10 ));//H out

                    if(Vin==0){
                        emit vyvidBattStatus(tr("<b>Живлення від акумулятора.</b><br>Вимкнення через %1 хв, <br>або по досягненню заряда у %2         ").arg(waitTime).arg(battVal));
                        if(!offTimer.isActive())
                            offTimer.start(waitTime);
                        if((bat/10)<battVal)
                            signalToOff();

                    } else {
                        emit vyvidBattStatus(tr("Живлення від мережі"));
                        if(offTimer.isActive())
                            offTimer.stop();
                    }

                    counter=0;
                    bat=0;
                    Vin=0;
                    Hin=0;
                    LL=0;
                    Vout=0;
                    Hout=0;

                    emit vyvidDanyh(list);
                }
            }
            msleep(150);

        }

    }
}

//=================================================================================================================
void UPSMonThread::stopSlot()
{
    stopAll=1;
}
//=================================================================================================================
bool UPSMonThread::openUPSPort()
{
    if(myPorts.isRunning())
        myPorts.terminate();
    if(mySerial->isOpen())
        mySerial->close();


    emit vyvidMess(tr("<font color=red>Не з’єднано</font>"));
    QStringList newList;
    QStringList newListInfo;
    QString portName;
    foreach (const SerialPortInfo &info, SerialPortInfo::availablePorts()) {

        newList<<info.portName();
        newListInfo<< info.manufacturer();
    }

    for(int i=0; i<newList.size(); i++){
        if(newListInfo.at(i).left(8).toUpper()=="POWERCOM"){
            portName=newList.at(i);
            break;
        }
    }

    if(!portName.isEmpty()){
        mySerial->setPort(portName);
        if(mySerial->open(QIODevice::ReadWrite)){
            if(mySerial->setRate(1200) && mySerial->setParity(SerialPort::NoParity) && mySerial->setDataBits(SerialPort::Data8) && mySerial->setStopBits(SerialPort::OneStop) && mySerial->setFlowControl(SerialPort::NoFlowControl)){
                myPorts.zapuskalka(portName);
                stopAll=0;

                if(!stopAll)
                    mySerial->waitForBytesWritten(500);
                if(!stopAll){
                    mySerial->write(QByteArray::fromHex("0172"));
                   // qDebug("0172");
                    if(!stopAll)
                        mySerial->waitForBytesWritten(500);
                }
                if(!stopAll){
                    if(mySerial->waitForReadyRead(1000) && !stopAll)
                        /*qDebug()<<*/ mySerial->readAll().toHex();
                    while(mySerial->waitForReadyRead(50))
                        /*qDebug()<< */ mySerial->readAll().toHex();
                }
                emit vyvidMess(tr("З’єднано"));

                return 1;
            }

        }
    }
    return 0;

}
//=================================================================================================================
void UPSMonThread::closePort()
{
    if(myPorts.isRunning())
        myPorts.terminate();
    if(mySerial->isOpen())
        mySerial->close();
}
//=================================================================================================================

void UPSMonThread::changeBatAndWaitTime(int waitTime, int battVal)
{

    this->waitTime=waitTime;
    this->battVal=battVal;
    offTimer.setInterval(waitTime);
}
//=================================================================================================================
void UPSMonThread::signalToOff()
{
//    qDebug("off comp");
//    QProcess *logout = new QProcess(this);
//    QStringList arguments;
//    arguments.append("dcop");
//    arguments.append("ksmserver");
//    arguments.append("ksmserver");
//    arguments.append("logout");
//    arguments.append("0");
//    arguments.append("0");
    //    arguments.append("0");
//    if(!offSystemRun){


         if(!stopAll)
            mySerial->write(QByteArray::fromHex("B9BC0100"));
          if(!stopAll)
              mySerial->waitForBytesWritten(500);

         QProcess::startDetached("qdbus org.kde.ksmserver /KSMServer org.kde.KSMServerInterface.logout 0 2 2");
          sleep(10);
        emit offcomp();
//    }
//    offSystemRun=1;

}
//=================================================================================================================
