#include "removenetv4dialog.h"
#include "ui_removenetv4dialog.h"
#include "lib/database/danetdbman.h"
#include <QMessageBox>

RemoveNetV4Dialog::RemoveNetV4Dialog(QWidget *parent, DanetDbMan *db, const int NetPoolId, const QString net, const int prefix, const QString mask, const QString bc, const int host) :
    QDialog(parent),
    ui(new Ui::RemoveNetV4Dialog),
    dbMan(db),
    netPoolId(NetPoolId)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowMinMaxButtonsHint);

    ui->netLbl->setText(net);
    ui->prefixLbl->setText(QString::number(prefix));
    Network = net+"/"+QString::number(prefix);
    ui->maskLbl->setText(mask);
    ui->bcLbl->setText(bc);
    ui->hostLbl->setText(QString::number(host));

    bool assigned = dbMan->isSubnetPoolAssigned(1,netPoolId,0);
    if(assigned)
        ui->infoLbl->setText("Network cannot be modified when it has subnet assigned.");

    bool hasPool = dbMan->getIpPoolCount(netPoolId,0);
    if(hasPool)
        ui->infoLbl->setText("Remove related IP-Pools first.");

    int sub2Count = dbMan->getSubnetPoolCount(2,netPoolId,0);
    int sub3Count = dbMan->getSubnetPoolCount(3,netPoolId,0);
    if(sub2Count > 0)
        ui->infoLbl->setText("Remove related all 2d-Subnets first.");
    if(sub3Count > 0)
        ui->infoLbl->setText("Remove related all 3rd-Subnets first.");

    if( hasPool || assigned || (sub2Count > 0) || (sub3Count > 0) )
    {
        ui->infoLbl->setVisible(true);
        ui->checkBox->setChecked(false);
        ui->checkBox->setEnabled(false);
        ui->okBtn->setEnabled(false);
    }
    else
    {
        ui->infoLbl->setVisible(false);
        ui->checkBox->setChecked(false);
        ui->checkBox->setEnabled(true);
        ui->okBtn->setEnabled(false);
    }
}

RemoveNetV4Dialog::~RemoveNetV4Dialog()
{
    delete ui;
}

void RemoveNetV4Dialog::on_cancelBtn_clicked()
{
    this->close();
}

void RemoveNetV4Dialog::on_okBtn_clicked()
{
    QString user = dbMan->getLoggedinNameLastname();

    if(dbMan->deleteSubnetPool(1,netPoolId,0))
    {
        if(dbMan->deleteNetPool(netPoolId))
        {
            dbMan->insertLog(user,"Remove Network: "+Network);
            this->close();
        }
        else
            QMessageBox::warning(this, "ERROR", "Cannot Remove network.\n"+dbMan->lastError());
    }
    else
        QMessageBox::warning(this, "ERROR", "Cannot Remove Subnet-Pools.\n"+dbMan->lastError());
}

void RemoveNetV4Dialog::on_checkBox_toggled(bool checked)
{
    if(checked)
        ui->okBtn->setEnabled(true);
    else
        ui->okBtn->setEnabled(false);
}
