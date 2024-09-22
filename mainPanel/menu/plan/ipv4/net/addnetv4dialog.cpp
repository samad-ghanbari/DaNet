#include "addnetv4dialog.h"
#include "ui_addnetv4dialog.h"
#include <lib/database/danetdbman.h>
#include "lib/netTools/nettools.h"
#include <QSqlQuery>
#include <QRegExpValidator>
#include <QIntValidator>
#include <QMessageBox>

AddNetV4Dialog::AddNetV4Dialog(QWidget *parent, DanetDbMan *db) :
    QDialog(parent),
    ui(new Ui::AddNetV4Dialog),
    dbMan(db),
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
}

AddNetV4Dialog::~AddNetV4Dialog()
{
    delete ui;
}

void AddNetV4Dialog::entryCheck()
{
    bool p = ui->prefixLE->hasAcceptableInput();
    bool n = ui->netLE->hasAcceptableInput();
    bool m = ui->maskLE->hasAcceptableInput();
    bool des = !ui->descLE->text().trimmed().isEmpty();


    if(p && n && m && des)
        ui->okBtn->setEnabled(true);
    else
        ui->okBtn->setEnabled(false);

}

void AddNetV4Dialog::on_cancelBtn_clicked()
{
    this->close();
}

void AddNetV4Dialog::on_okBtn_clicked()
{

    QString net = ui->netLE->text();
    int prefix = ui->prefixLE->text().toInt();
    QString mask = ui->maskLE->text();    
    QString bc = ui->bcLbl->text();
    int host = ui->hostLbl->text().toInt();
    QString desc = ui->descLE->text();

    int netId = dbMan->getNewNetPoolId();
    QString user = dbMan->getLoggedinNameLastname();

    if(netId > -1)
    {
        if(dbMan->insertNetPool(netId, net, prefix, mask, bc, host, desc))
        {

            if(dbMan->insertSubnetPool1(netId, net, prefix, mask, bc, host, ""))
            {
                dbMan->insertLog(user,"Add New Network: "+net+"/"+QString::number(prefix));
                this->close();
            }
            else
            {
                dbMan->deleteNetPool(netId);
                QMessageBox::warning(this, "ERROR", "check Subnet.\n"+dbMan->lastError());
            }
        }
        else
            QMessageBox::warning(this, "ERROR", "check Network.\n"+dbMan->lastError());
    }
    else
        QMessageBox::warning(this, "ERROR", "Invalid Net Pool Id.\n"+dbMan->lastError());

}

void AddNetV4Dialog::on_netLE_textChanged(const QString &arg1)
{
    ui->prefixLE->clear();

    if(arg1.isEmpty())
        ui->okBtn->setEnabled(false);
    else
        ui->okBtn->setEnabled(false);
}

void AddNetV4Dialog::on_prefixLE_textChanged(const QString &arg1)
{
    if(ui->prefixLE->hasAcceptableInput())
    {
        QString mask = NetTools::prefixToSubnetMask(arg1.toInt());
        ui->maskLE->blockSignals(true);
        ui->maskLE->setText(mask);
        ui->maskLE->blockSignals(false);
        ui->maskLE->setStyleSheet("QLineEdit{color: rgb(85,0,127)}");
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

void AddNetV4Dialog::on_maskLE_textChanged(const QString &arg1)
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
            ui->maskLE->setStyleSheet("QLineEdit{color: rgb(85,0,127)}");
        }
        else
        {
            ui->okBtn->setEnabled(false);
            ui->maskLE->setStyleSheet("QLineEdit{color: darkred}");
        }

    }
    else
    {
        ui->okBtn->setEnabled(false);
        ui->maskLE->setStyleSheet("QLineEdit{color: darkred}");
    }
}

void AddNetV4Dialog::on_descLE_textChanged(const QString &arg1)
{
    if(arg1.isEmpty())
        ui->okBtn->setEnabled(false);
    else
        entryCheck();
}
