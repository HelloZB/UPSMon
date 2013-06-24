#include "checkcurrport.h"
#include <QtAddOnSerialPort/serialportinfo.h>

//#include <QTime>
//#include <QDebug>

CheckCurrPort::CheckCurrPort(QObject *parent) :
    QThread(parent)
{
}

void CheckCurrPort::zapuskalka(QString currPort)
{
    this->currPort=currPort;   
    this->start();
}

void CheckCurrPort::run()
{
//    qDebug()<<QTime::currentTime().toString()<<"startt";
    quint8 counter=1;
    while(true){
        msleep(1500/counter);
        bool found=0;
        foreach (const SerialPortInfo &info, SerialPortInfo::availablePorts()){  //SerialPortInfo &info, SerialPortInfo::availablePorts())

            if(info.portName()==currPort){
                found=1;
                SerialPort testConn;//(currPort);
                testConn.setPort(currPort);
                if(testConn.open(QIODevice::ReadWrite)){
                   //???? testConn.close();
                    emit portDisconnected();
                    counter++;
                    if(counter>2){
                        counter=2;
                        emit terminateNow();
                    }
                    //closeSerialPort(1);
//                    qDebug()<<QTime::currentTime().toString()<<"opened!";
                    //return;
                } /*else {
                    qDebug()<<QTime::currentTime().toString()<<"can not open";
                }*/
                break;
            }
       }
        if(!found){
//            closeSerialPort(1);            
//             qDebug()<<QTime::currentTime().toString()<<" not found!";
             emit portDisconnected();
            counter++;
            if(counter>2){
                counter=2;
//                qDebug()<<QTime::currentTime().toString()<<" not found!";
                emit terminateNow();
            }
             //return;
        }
    }
//    qDebug()<<QTime::currentTime().toString()<<"exitttt";
}
