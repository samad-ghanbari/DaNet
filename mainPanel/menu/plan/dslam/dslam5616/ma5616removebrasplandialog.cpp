#include "ma5616removebrasplandialog.h"
#include "ui_ma5616removebrasplandialog.h"
#include "lib/database/danetdbman.h"
#include <QSqlQuery>
#include <QMessageBox>

MA5616RemoveBrasPlanDialog::MA5616RemoveBrasPlanDialog(QWidget *parent, DanetDbMan *Db, const int DevexId, QString DslamName) :
    QDialog(parent),
    ui(new Ui::MA5616RemoveBrasPlanDialog),
    dbMan(Db),
    dslamName(DslamName),
    devexId(DevexId)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowMinMaxButtonsHint);

    ui->checkBox->setChecked(false);

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

MA5616RemoveBrasPlanDialog::~MA5616RemoveBrasPlanDialog()
{
    delete ui;
}

void MA5616RemoveBrasPlanDialog::on_checkBox_toggled(bool checked)
{
    if(checked)
        ui->okBtn->setEnabled(true);
    else
        ui->okBtn->setEnabled(false);
}

void MA5616RemoveBrasPlanDialog::on_cancelBtn_clicked()
{
    this->close();
}

void MA5616RemoveBrasPlanDialog::on_okBtn_clicked()
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
