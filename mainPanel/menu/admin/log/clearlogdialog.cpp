#include "clearlogdialog.h"
#include "ui_clearlogdialog.h"
#include "lib/database/danetdbman.h"
#include <QMessageBox>
#include <QDate>

ClearLogDialog::ClearLogDialog(QWidget *parent, DanetDbMan *db) :
    QDialog(parent),
    ui(new Ui::ClearLogDialog),
    dbMan(db)
{
    ui->setupUi(this);
    QList<QString> logInfo = dbMan->getLogInfos();// total, start date , end date
    ui->totalLbl->setText(logInfo.at(0));
    ui->startLbl->setText(logInfo.at(1));
    ui->endLbl->setText(logInfo.at(2));

    ui->okBtn->setEnabled(false);
    ui->checkBox->setChecked(false);

}

ClearLogDialog::~ClearLogDialog()
{
    delete ui;
}

void ClearLogDialog::on_cancelBtn_clicked()
{
    this->close();
}

void ClearLogDialog::on_okBtn_clicked()
{
    QDate date = QDate::currentDate();
    int y, m, d;
    y = date.year();
    m = date.month();
    d = date.day();

    bool ok = true;
    if(ui->yRB->isChecked())
    {
        // older than one year
        y--;
    }
    else if(ui->m6RB->isChecked())
    {
        //older than 6 month
        m = m - 6;
    }
    else if(ui->m3RB->isChecked())
    {
        //3 month
        m = m - 3;
    }
    else  if(ui->mRB->isChecked())
    {
        //older than one month
        m = m - 1;
    }
    else  if(ui->RB->isChecked())
    {
        //everything
        y = y + 1;
        m = m + 1;
    }



    QString Date = QString::number(y)+"-"+QString::number(m)+"-"+QString::number(d);

    ok = dbMan->clearOlderLogThan(Date);

    if(ok)
        this->close();
    else
        QMessageBox::warning(this, "ERROR", "Cannot delete records.\n"+dbMan->lastError());

}
void ClearLogDialog::on_checkBox_toggled(bool checked)
{
    if(checked)
        ui->okBtn->setEnabled(true);
    else
        ui->okBtn->setEnabled(false);
}
