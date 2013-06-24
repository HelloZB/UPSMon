#include "offdialog.h"
#include "ui_offdialog.h"

OffDialog::OffDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OffDialog)
{
    ui->setupUi(this);
}

OffDialog::~OffDialog()
{
    delete ui;
}

void OffDialog::changeVal(int waitTime, int battVal)
{
    ui->waitTime->setValue(waitTime);
    ui->battVal->setValue(battVal);
}

void OffDialog::on_buttonBox_accepted()
{
    emit newValBatAndTime(ui->waitTime->value(), ui->battVal->value());
}
