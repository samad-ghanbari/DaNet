#include "splitsubnet1v4dialog.h"
#include "ui_splitsubnet1v4dialog.h"
#include "lib/database/danetdbman.h"
#include "lib/netTools/nettools.h"
#include <QMessageBox>

SplitSubnet1V4Dialog::SplitSubnet1V4Dialog(QWidget *parent, DanetDbMan *db, const int NetPoolId, const int SubnetPoolId, const QString Abbr, const QString Network, const int Prefix, const int Area,const int Dep, const QString Ap, const QString Desc) :
    QDialog(parent),
    ui(new Ui::SplitSubnet1V4Dialog),
    dbMan(db),
    netPoolId(NetPoolId),
    subnetPoolId(SubnetPoolId),
    area(Area),
    prefix(Prefix),
    dep(Dep),
    ap(Ap),
    desc(Desc)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowMinMaxButtonsHint);

    net = NetTools::netId(Network, prefix);
    mask = NetTools::prefixToSubnetMask(prefix);
    bc = NetTools::broadcast(net, prefix);
    host = NetTools::prefixHostCount(prefix);

    ui->okBtn->setEnabled(false);
    ui->checkBox->setChecked(false);

    ui->abbrLbl->setText(Abbr);
    ui->appLbl->setText(ap);

    ui->srcNetLbl->setText(net);
    ui->srcPrefixLbl->setText(QString::number(prefix));
    ui->srcMaskLbl->setText(mask);
    ui->srcBcLbl->setText(bc);
    ui->srcHostLbl->setText(QString::number(host));

    bool ipC = dbMan->getIpPoolCount(subnetPoolId, 1);
    if(ipC)
        ui->infoLbl->setText("Subnet cannot be splitted when it has IP pool.");

    int subC = dbMan->getSubnetPoolCount(2,subnetPoolId, 1);
    if(subC > 0)
        ui->infoLbl->setText("Subnet cannot be splitted when it has 2d-Subnet");

    if( (ipC > 0) || (subC > 0) )
    {
        ui->infoLbl->setVisible(true);
        ui->gb->setVisible(false);
        ui->checkBox->setChecked(false);
        ui->checkBox->setEnabled(false);
        ui->okBtn->setEnabled(false);
    }
    else // can be splitted
    {
        if(prefix < 32)
        {
            ui->infoLbl->setVisible(false);
            QMap<int, QString> map = NetTools::ipSplit(net, prefix, false);
            prefix1 = prefix+1;
            mask1 = NetTools::prefixToSubnetMask(prefix1);
            host1 = NetTools::prefixHostCount(prefix1);


            //net1
            sub1 = map.value(0);
            bc1 = NetTools::broadcast(sub1, prefix1);

            ui->netLbl1->setText(sub1);
            ui->prefixLbl1->setText(QString::number(prefix1));
            ui->maskLbl1->setText(mask1);
            ui->bcLbl1->setText(bc1);
            ui->hostLbl1->setText(QString::number(host1));

            //net2
            sub2 = map.value(1);
            bc2 = NetTools::broadcast(sub2, prefix1);

            ui->netLbl2->setText(sub2);
            ui->prefixLbl2->setText(QString::number(prefix1));
            ui->maskLbl2->setText(mask1);
            ui->bcLbl2->setText(bc2);
            ui->hostLbl2->setText(QString::number(host1));
        }
        else
        {
            ui->infoLbl->setText("Subnet cannot be splitted anymore.");
            ui->infoLbl->setVisible(true);
            ui->gb->setVisible(false);
            ui->checkBox->setChecked(false);
            ui->checkBox->setEnabled(false);
            ui->okBtn->setEnabled(false);
        }
    }

}

SplitSubnet1V4Dialog::~SplitSubnet1V4Dialog()
{
    delete ui;
}

void SplitSubnet1V4Dialog::on_checkBox_toggled(bool checked)
{
    if(checked)
        ui->okBtn->setEnabled(true);
    else
        ui->okBtn->setEnabled(false);
}

void SplitSubnet1V4Dialog::on_cancelBtn_clicked()
{
    this->close();
}

void SplitSubnet1V4Dialog::on_okBtn_clicked()
{
    if(dbMan->updateSubnetPool1(subnetPoolId,sub1,prefix1,mask1,bc1,host1))
    {
        if(dbMan->insertSubnetPool1(netPoolId, sub2,prefix1,mask1,bc2,host1, area,dep, ap, desc))
        {
            this->close();
        }
        else
        {
            dbMan->updateSubnetPool1(subnetPoolId,net,prefix,mask,bc,host);
            QMessageBox::warning(this, "ERROR", "Cannot update subnet 2.\n"+dbMan->lastError());
        }
    }
    else
        QMessageBox::warning(this, "ERROR", "Cannot update subnet 1.\n"+dbMan->lastError());

}
