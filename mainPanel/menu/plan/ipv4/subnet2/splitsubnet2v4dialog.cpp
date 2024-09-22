#include "splitsubnet2v4dialog.h"
#include "ui_splitsubnet2v4dialog.h"
#include "lib/database/danetdbman.h"
#include "lib/netTools/nettools.h"
#include <QMessageBox>

SplitSubnet2V4Dialog::SplitSubnet2V4Dialog(QWidget *parent, DanetDbMan *db, const int SubnetPool1Id, const int SubnetPool2Id, const QString Abbr, const QString Network, const int Prefix, const int Area, const QString Ap, const QString Desc) :
    QDialog(parent),
    ui(new Ui::SplitSubnet2V4Dialog),
    dbMan(db),
    subnetPool1Id(SubnetPool1Id),
    subnetPool2Id(SubnetPool2Id),
    area(Area),
    prefix(Prefix),
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

    int ipC = dbMan->getIpPoolCount(subnetPool2Id, 2);
    if(ipC > 0)
        ui->infoLbl->setText("Subnet cannot be splitted when it has IP pool.");

    int subC = dbMan->getSubnetPoolCount(3,subnetPool2Id, 2);
    if(subC > 0)
        ui->infoLbl->setText("Subnet cannot be splitted when it has 3rd-Subnet");

    if( (subC > 0) || (ipC > 0))
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

SplitSubnet2V4Dialog::~SplitSubnet2V4Dialog()
{
    delete ui;
}

void SplitSubnet2V4Dialog::on_checkBox_toggled(bool checked)
{
    if(checked)
        ui->okBtn->setEnabled(true);
    else
        ui->okBtn->setEnabled(false);
}

void SplitSubnet2V4Dialog::on_cancelBtn_clicked()
{
    this->close();
}

void SplitSubnet2V4Dialog::on_okBtn_clicked()
{
    if(dbMan->updateSubnetPool2(subnetPool2Id,sub1,prefix1,mask1,bc1,host1))
    {
        if(dbMan->insertSubnetPool2(subnetPool1Id, sub2,prefix1,mask1,bc2,host1, area, ap, desc))
        {
            this->close();
        }
        else
        {
            dbMan->updateSubnetPool2(subnetPool2Id,net,prefix,mask,bc,host);
            QMessageBox::warning(this, "ERROR", "Cannot update subnetwork 2.\n"+dbMan->lastError());
        }
    }
    else
        QMessageBox::warning(this, "ERROR", "Cannot update subnetwork 1.\n"+dbMan->lastError());

}
