#include "editnetv4dialog.h"
#include "ui_editnetv4dialog.h"
#include "lib/database/danetdbman.h"
#include "lib/netTools/nettools.h"
#include <QSqlQuery>
#include <QRegExpValidator>
#include <QIntValidator>
#include <QMessageBox>

EditNetV4Dialog::EditNetV4Dialog(QWidget *parent, DanetDbMan *db, const int NetPoolId, const QString net, const int prefix) :
    QDialog(parent),
    ui(new Ui::EditNetV4Dialog),
    dbMan(db),
    netPoolId(NetPoolId),
    ipRange("(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])"),
    ipRegex("^" + ipRange+ "\\." + ipRange+ "\\." + ipRange+"\\." + ipRange + "$"),
    intValidator(new QIntValidator(4,32, this)),
    ipValidator(new QRegExpValidator(ipRegex, this))
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowMinMaxButtonsHint);

    ui->netLE->setValidator(ipValidator);
    ui->maskLE->setValidator(ipValidator);
    ui->prefixLE->setValidator(intValidator);

    ui->netLE->setText(net);
    ui->prefixLE->setText(QString::number(prefix));
    oldNet = net+"/"+QString::number(prefix);

    bool assigned = dbMan->isSubnetPoolAssigned(1,netPoolId,0) || dbMan->isSubnetPoolAssigned(2,netPoolId,0) || dbMan->isSubnetPoolAssigned(3,netPoolId,0);
    if(assigned)
        ui->infoLbl->setText("Network cannot be modified when it has subnet assigned.");

    bool hasPool = dbMan->getIpPoolCount(netPoolId, 0);
    if(hasPool)
        ui->infoLbl->setText("Remove related IP-Pools first.");

    int sub2Count = dbMan->getSubnetPoolCount(2,netPoolId,0);
    int sub3Count = dbMan->getSubnetPoolCount(3,netPoolId,0);

    if(sub2Count > 0)
        ui->infoLbl->setText("Remove related all 2d-Subnets first.");
    if(sub3Count > 0)
        ui->infoLbl->setText("Remove related all 3rd-Subnets first.");

    if(hasPool || assigned || (sub2Count > 0) || (sub3Count > 0) )
    {
        //has ip pool
        ui->infoLbl->setVisible(true);
        ui->netGB->setEnabled(false);
    }
    else
    {
        //enable
        ui->infoLbl->setVisible(false);
        ui->netGB->setEnabled(true);
    }

    ui->okBtn->setEnabled(false);
}

EditNetV4Dialog::~EditNetV4Dialog()
{
    delete ui;
}

void EditNetV4Dialog::entryCheck()
{
    bool p = ui->prefixLE->hasAcceptableInput();
    bool n = ui->netLE->hasAcceptableInput();
    bool m = ui->maskLE->hasAcceptableInput();

    if(p && n && m)
        ui->okBtn->setEnabled(true);
    else
        ui->okBtn->setEnabled(false);
}

void EditNetV4Dialog::on_cancelBtn_clicked()
{
    this->close();
}

void EditNetV4Dialog::on_okBtn_clicked()
{

    QString net = ui->netLE->text();
    QString mask = ui->maskLE->text();
    int prefix = ui->prefixLE->text().toInt();
    QString bc = ui->bcLbl->text();
    int host = ui->hostLbl->text().toInt();

    QString user = dbMan->getLoggedinNameLastname();

    //remove existing
    if(dbMan->deleteIpPool(netPoolId, 0))
    {
        if(dbMan->deleteSubnetPool(1, netPoolId, 0))
        {

            if(dbMan->updateNetPool(netPoolId, net, prefix, mask, bc, host))
            {

                if(dbMan->insertSubnetPool1(netPoolId, net, prefix, mask, bc, host, ""))
                {
                    dbMan->insertLog(user,"Edit Network: "+oldNet+" to Network: "+net+"/"+QString::number(prefix));
                    this->close();
                }
                else
                {
                    dbMan->deleteNetPool(netPoolId);
                    QMessageBox::warning(this, "ERROR", "check Subnet.\n"+dbMan->lastError());
                }
            }
            else
                QMessageBox::warning(this, "ERROR", "check Network.\n"+dbMan->lastError());


        }
        else
            QMessageBox::warning(this, "ERROR", "Cannot Remove Subnet-Pools.\n"+dbMan->lastError());
    }
    else
        QMessageBox::warning(this, "ERROR", "Cannot Remove IP-Pools.\n"+dbMan->lastError());
}

void EditNetV4Dialog::on_netLE_textChanged(const QString &arg1)
{
    ui->prefixLE->clear();

    if(arg1.isEmpty())
        ui->okBtn->setEnabled(false);
    else
        ui->okBtn->setEnabled(false);
}

void EditNetV4Dialog::on_prefixLE_textChanged(const QString &arg1)
{
    if(ui->prefixLE->hasAcceptableInput())
    {
        QString mask = NetTools::prefixToSubnetMask(arg1.toInt());
        ui->maskLE->blockSignals(true);
        ui->maskLE->setText(mask);
        ui->maskLE->blockSignals(false);
        int host = NetTools::prefixHostCount(arg1.toInt());
        ui->hostLbl->setText(QString::number(host));

        ui->netLE->blockSignals(true);
        if(ui->netLE->hasAcceptableInput())
        {
            QString ip = ui->netLE->text();
            ip = NetTools::netId(ip, arg1.toInt());
            ui->netLE->setText(ip);
            QString bc = NetTools::broadcast(ip,arg1.toInt());
            ui->bcLbl->setText(bc);
        }
        else
        {
            ui->netLE->clear();
            ui->bcLbl->clear();
        }
        ui->netLE->blockSignals(false);
    }
    else
    {
        ui->bcLbl->clear();
        ui->hostLbl->clear();
        ui->maskLE->clear();
    }

    if(arg1.isEmpty())
        ui->okBtn->setEnabled(false);
    else
        entryCheck();
}

void EditNetV4Dialog::on_maskLE_textChanged(const QString &arg1)
{
    ui->prefixLE->blockSignals(true);
    ui->prefixLE->clear();
    ui->prefixLE->blockSignals(false);
    ui->bcLbl->clear();
    ui->hostLbl->clear();

    if(ui->maskLE->hasAcceptableInput())
    {
        if(NetTools::subnetMaskValidate(arg1))
        {
            int p = NetTools::subnetMaskToPrefix(arg1);
            ui->prefixLE->setText(QString::number(p));
        }
        else
            ui->okBtn->setEnabled(false);
    }
    else
        ui->okBtn->setEnabled(false);
}
