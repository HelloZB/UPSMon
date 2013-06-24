#include "widget.h"
#include "ui_widget.h"
#include <QProcess>
#include <QTextCodec> //qt4
#include <QMessageBox>
#include <QMenu>
#include <QCloseEvent>
#include <QSettings>
#include "offdialog.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    QTextCodec *utfcodec = QTextCodec::codecForName("UTF-8"); //qt4
    QTextCodec::setCodecForTr(utfcodec);
    QTextCodec::setCodecForCStrings(utfcodec);
dozvilNaVyhid=0;

    m_tray_icon = new QSystemTrayIcon(QIcon(":/new/emulator.png"), this);


     QAction *another_action = new QAction( "Показати/Сховати", m_tray_icon );
     another_action->setIcon(QIcon(":/new/emulator.png"));
     connect( another_action, SIGNAL(triggered()), this, SLOT(showHide()) );

     QAction *set_action = new QAction( "Налаштування", m_tray_icon );
     set_action->setIcon(QIcon(":/new/sett.png"));
     connect( set_action, SIGNAL(triggered()), this, SLOT(settigsDialog()) );


     QAction *quit_action = new QAction( "Вихід", m_tray_icon );
     quit_action->setIcon(QIcon(":/new/exit.png"));
     connect( quit_action, SIGNAL(triggered()), this, SLOT(dozvilNaclose()) );

     QMenu *tray_icon_menu = new QMenu;
     tray_icon_menu->addAction( another_action );
     tray_icon_menu->addAction( set_action );
     tray_icon_menu->addAction( quit_action );

     m_tray_icon->setContextMenu( tray_icon_menu );

     connect( m_tray_icon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(trayIconClicked(QSystemTrayIcon::ActivationReason)));

     m_tray_icon->show();

     myThreadTimer.setSingleShot(1);
     myThreadTimer.setInterval(3000);


     myUPS=new UPSMonThread(this);

    connect(myUPS, SIGNAL(vyvidDanyh(QStringList)), this, SLOT(vyvidDanyh(QStringList)));
    connect(myUPS, SIGNAL(vyvidMess(QString)), this, SLOT(vyvidPovidomlen(QString)));
    connect(myUPS, SIGNAL(vyvidBattStatus(QString)), this, SLOT(vyvidBattStatus(QString)));
    connect(myUPS, SIGNAL(terminated()), &myThreadTimer, SLOT(start()));
    connect(myUPS, SIGNAL(offcomp()), this, SLOT(dozvilNaclose()));
    connect(&myThreadTimer, SIGNAL(timeout()), this, SLOT(onTimerOut()));

    loadSett();
    myUPS->changeBatAndWaitTime(waitTime, battVal);
    myThreadTimer.start();
//    this->hide();
    QTimer::singleShot(0, this, SLOT(showHide()));

//    connect(this, SIGNAL())

}

Widget::~Widget()
{
    myUPS->terminate();

    delete ui;
}


void Widget::vyvidDanyh(QStringList list)
{
    if(list.size()>5){
        ui->lcdBat->display(list.at(0));
        ui->lcdIn->display(list.at(1));
        ui->lcdInH->display(list.at(2));
        ui->lcdLL->display(list.at(3));
        ui->lcdOut->display(list.at(4));
        ui->lcdOutH->display(list.at(5));
    }
}

void Widget::vyvidPovidomlen(QString str)
{
    ui->lblMess->setText(str);
}

void Widget::vyvidBattStatus(QString str)
{
    ui->label_7->setText(str);
}

void Widget::changeEvent(QEvent *event)
{
   // QMainWindow::changeEvent(event);
    if(event->type() == QEvent::WindowStateChange) {
        if(isMinimized() )
            this->hide();
    }
}

void Widget::closeEvent(QCloseEvent *event)
{
    if(!dozvilNaVyhid){
        if (m_tray_icon->isVisible()) {
            this->hide();
            event->ignore();
        }
    }
}

void Widget::showHide()
{
    if(this->isHidden())
        this->setVisible(1);
    else
        this->hide();
}

void Widget::trayIconClicked(QSystemTrayIcon::ActivationReason reason)
{
    if( reason==QSystemTrayIcon::Trigger)
        showHide();
}

void Widget::settigsDialog()
{
    if(this->isHidden())
    this->setHidden(0);

    OffDialog *myDialog=new OffDialog(this);
    connect(myDialog, SIGNAL(newValBatAndTime(int,int)), this, SLOT(changeBatAndWaitTime(int,int)));
    connect(myDialog, SIGNAL(newValBatAndTime(int,int)), myUPS, SLOT(changeBatAndWaitTime(int,int)));
    myDialog->changeVal(waitTime, battVal);
    myDialog->exec();
    delete myDialog;
}


void Widget::changeBatAndWaitTime(int waitTime, int battVal)
{
    this->waitTime=waitTime;
    this->battVal=battVal;
      saveSett();
    //signal to thread
}

void Widget::saveSett()
{
    QSettings settings("Hello_ZB","MyUPSMon");
    settings.beginGroup("main");
    settings.setValue("waitTime",waitTime);
    settings.setValue("battVal", battVal);
    settings.endGroup();
}

void Widget::loadSett()
{
    QSettings settings("Hello_ZB","MyUPSMon");
    settings.beginGroup("main");
    waitTime=settings.value("waitTime",3).toInt();
    battVal=settings.value("battVal", 80).toInt();
    settings.endGroup();
}

void Widget::onTimerOut()
{
    myUPS->start();
}


void Widget::dozvilNaclose()
{
    disconnect(myUPS, SIGNAL(terminated()), &myThreadTimer, SLOT(start()));
    dozvilNaVyhid=1;
    myUPS->terminate();
    close();
}

//void Widget::on_pushButton_clicked()
//{
//    QProcess::startDetached("shutdown -s -f -t 00");
//}
