#ifndef OFFDIALOG_H
#define OFFDIALOG_H

#include <QDialog>

namespace Ui {
class OffDialog;
}

class OffDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit OffDialog(QWidget *parent = 0);
    void changeVal(int waitTime, int battVal);
    ~OffDialog();
    

signals:
    void newValBatAndTime(int waitTime, int battVal);
private slots:
    void on_buttonBox_accepted();

private:
    Ui::OffDialog *ui;
};

#endif // OFFDIALOG_H
