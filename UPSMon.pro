#-------------------------------------------------
#
# Project created by QtCreator 2013-06-09T17:39:46
#
#-------------------------------------------------
CONFIG  += serialport

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = UPSControl
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    upsmonthread.cpp \
    checkcurrport.cpp \
    offdialog.cpp

HEADERS  += widget.h \
    upsmonthread.h \
    checkcurrport.h \
    offdialog.h

FORMS    += widget.ui \
    offdialog.ui

RESOURCES += \
    res.qrc
