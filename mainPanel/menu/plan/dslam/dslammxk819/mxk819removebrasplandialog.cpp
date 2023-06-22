#include "mxk819removebrasplandialog.h"
#include "ui_mxk819removebrasplandialog.h"
#include "lib/database/danetdbman.h"
#include <QSqlQuery>
#include <QMessageBox>

Mxk819RemoveBrasPlanDialog::Mxk819RemoveBrasPlanDialog(QWidget *parent, DanetDbMan *Db, const int DevexId, QString DslamName) :
    QDialog(parent),
    ui(new Ui::Mxk819RemoveBrasPlanDialog),
    dbMan(Db),
    dslamName(DslamName),
    devexId(DevexId)
{
    ui->setupUi(this);
    ui->checkBox->setChecked(false);
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowMinMaxButtonsHint);

    QSqlQuery *query = dbMan->selectDslam(devexId);
    //`type`, `purchase`, `sn`
    if(query->next())
    {
        QString temp = query->value(0).toString();
        DslamName += "-"+temp;
        temp = query->value(1).toString();//pur
        DslamName += "-"+temp;
        int i = query->value(2).toInt();
        if(i < 10 )
            DslamName += "-0"+QString::number(i);
        else
            DslamName += "-"+QString::number(i);
    }
    ui->dslamLbl->setText(DslamName);
}

Mxk819RemoveBrasPlanDialog::~Mxk819RemoveBrasPlanDialog()
{
    delete ui;
}

void Mxk819RemoveBrasPlanDialog::on_cancelBtn_clicked()
{
    this->close();
}

void Mxk819RemoveBrasPlanDialog::on_okBtn_clicked()
{
    QString username = dbMan->getLoggedinNameLastname();
    int exchId = dbMan->getDevexExchangeId(devexId);
    //log
    dbMan->insertLog(exchId, username, "Removed BRAS Plan "+dslamName );

    //bras
    if(!dbMan->deleteDslamPppoe(devexId))
        QMessageBox::warning(this, "ERROR", "Cannot remove BRAS parameters.\n"+dbMan->lastError());

    this->close();
}

void Mxk819RemoveBrasPlanDialog::on_checkBox_toggled(bool checked)
{
    if(checked)
        ui->okBtn->setEnabled(true);
    else
        ui->okBtn->setEnabled(false);
}
