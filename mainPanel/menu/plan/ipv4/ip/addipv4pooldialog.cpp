#include "addipv4pooldialog.h"
#include "ui_addipv4pooldialog.h"
#include "lib/database/danetdbman.h"
#include "lib/netTools/nettools.h"
#include <QMessageBox>

AddIpV4PoolDialog::AddIpV4PoolDialog(QWidget *parent, DanetDbMan *db, const int SubnetNo, const int SubnetId, const QString Subnet, const int Prefix, const QString Mask, const QString Bc, const int Host, const QString Abbr, const QString App, const QString Desc) :
    QDialog(parent),
    ui(new Ui::AddIpV4PoolDialog),
    dbMan(db),
    subnetId(SubnetId),
    prefix(Prefix),
    subnet(Subnet),
    bc(Bc),
    subnetNo(SubnetNo)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowMinMaxButtonsHint);

    ui->subnetLbl->setText(subnet);
    ui->prefixLbl->setText(QString::number(Prefix));
    ui->maskLbl->setText(Mask);
    ui->bcLbl->setText(bc);
    ui->hostLbl->setText(QString::number(Host));

    ui->abbrLbl->setText(Abbr);
    ui->appLbl->setText(App);
    ui->descLbl->setText(Desc);

    int ipC = -1;

    if(subnetNo == 1)
        ipC = dbMan->getIpPoolCount(subnetId, 1);
    else if(subnetNo == 2)
        ipC = dbMan->getIpPoolCount(subnetId, 2);
    else if(subnetNo == 3)
        ipC = dbMan->getIpPoolCount(subnetId, 3);

    if(ipC == 0)
    {
        ui->infoLbl->setVisible(false);
        ui->checkBox->setChecked(false);
        ui->checkBox->setEnabled(true);
    }
    else
    {
        ui->infoLbl->setVisible(true);
        ui->checkBox->setChecked(false);
        ui->checkBox->setEnabled(false);
    }

    ui->addBtn->setEnabled(false);
}

AddIpV4PoolDialog::~AddIpV4PoolDialog()
{
    delete ui;
}

void AddIpV4PoolDialog::on_checkBox_toggled(bool checked)
{
    if(checked)
        ui->addBtn->setEnabled(true);
    else
        ui->addBtn->setEnabled(false);
}

void AddIpV4PoolDialog::on_addBtn_clicked()
{
    QString net0 = NetTools::netId(subnet,prefix);
    QString bc0 = NetTools::broadcast(subnet,prefix);
    QString mask0 = NetTools::prefixToSubnetMask(prefix);
    int host0 = NetTools::prefixHostCount(prefix);
    if(bc.compare(bc0,Qt::CaseInsensitive) != 0)
    {
        //bc is not correct
        //modify it.
        if(subnetNo == 1)
        {
            if(dbMan->updateSubnetPool1(subnetId,net0,prefix,mask0,bc0,host0))
                QMessageBox::warning(this, "INFO", "Subnet record is updated.");
            else
            {
                QMessageBox::warning(this, "ERROR", "Subnet update is neccessary.");
                return;
            }
        }
        else if(subnetNo == 2)
        {
            if(dbMan->updateSubnetPool2(subnetId,net0,prefix,mask0,bc0,host0))
                QMessageBox::warning(this, "INFO", "Subnet record is updated.");
            else
            {
                QMessageBox::warning(this, "ERROR", "Subnet update is neccessary.");
                return;
            }
        }
        else if(subnetNo == 3)
        {
            if(dbMan->updateSubnetPool3(subnetId,net0,prefix,mask0,bc0,host0))
                QMessageBox::warning(this, "INFO", "Subnet record is updated.");
            else
            {
                QMessageBox::warning(this, "ERROR", "Subnet update is neccessary.");
                return;
            }
        }
        else return;
    }

    QMap<int, int> netOctets = NetTools::ipMaskOctets(net0);
    QMap<int, int> bcOctets = NetTools::ipMaskOctets(bc0);
    int n0, n1, n2, n3, b0, b1, b2, b3;

    b0 = bcOctets.value(0);
    b1 = bcOctets.value(1);
    b2 = bcOctets.value(2);
    b3 = bcOctets.value(3);
    QString ip;
    bool start = true;

    n0 = netOctets.value(0);
    while(n0 <= b0)
    {
        n1 = netOctets.value(1);
        while(n1 <= b1)
        {
            n2 = netOctets.value(2);
            while(n2 <= b2)
            {
                n3 = netOctets.value(3);
                if(prefix < 31 && start)
                {
                    n3++; // first IP
                    start = false;
                }
                while(n3 <= b3)
                {
                    ip = NetTools::octetsToIp(n0,n1, n2, n3);
                    if((ip.compare(bc0,Qt::CaseInsensitive) != 0) || (prefix > 30))
                    {
                        if(!dbMan->insertIpPool(subnetNo,subnetId,ip))
                        {
                            QMessageBox::warning(this, "ERROR", "cannot insert IP.\n"+dbMan->lastError());
                            if(dbMan->deleteIpPool(subnetId, subnetNo))
                                QMessageBox::warning(this, "INFO", "IP pool is removed.");
                            else
                                QMessageBox::warning(this, "ERROR", "cannot remove IP Pool.\n"+dbMan->lastError());

                            return;
                        }
                    }

                    n3++;
                }
                n2++;
            }
            n1++;
        }
        n0++;
    }

    this->close();
}

void AddIpV4PoolDialog::on_cancelBtn_clicked()
{
    this->close();
}
