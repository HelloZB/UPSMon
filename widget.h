#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "upsmonthread.h"
#include <QSystemTrayIcon>
#include <QTimer>


namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT
    
public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

public slots:
    void vyvidDanyh(QStringList list);
    void vyvidPovidomlen(QString str);
    void vyvidBattStatus(QString str);
    void changeBatAndWaitTime(int waitTime, int battVal);
    
private slots:
    void showHide();
    void trayIconClicked(QSystemTrayIcon::ActivationReason);
    void settigsDialog();
    void saveSett();
    void loadSett();
    void onTimerOut();
    void dozvilNaclose();
//    void on_pushButton_clicked();

private:

    void closeEvent(QCloseEvent *);
    void changeEvent(QEvent *);
    bool dozvilNaVyhid;

    Ui::Widget *ui;
    UPSMonThread *myUPS;
    QSystemTrayIcon *m_tray_icon;
    QTimer myThreadTimer;

    int waitTime;
    int battVal;
};

#endif // WIDGET_H
