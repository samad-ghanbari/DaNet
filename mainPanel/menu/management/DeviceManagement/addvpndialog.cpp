#include "addvpndialog.h"
#include "ui_addvpndialog.h"
#include "lib/database/danetdbman.h"
#include <QMessageBox>

AddVPNDialog::AddVPNDialog(QWidget *parent, DanetDbMan *db, const int DevexId, QString Abbr, QString Saloon, QString Device) :
    QDialog(parent),
    ui(new Ui::AddVPNDialog),
    dbMan(db),
    devexId(DevexId),
    rdReg("^\\d+:\\d+$"),
    imexReg("^\\d+:\\d+(\\s\\d+:\\d+)*"),
    rdValidator(new QRegExpValidator(rdReg, this)),
    imexValidator(new QRegExpValidator(imexReg, this))
{
    ui->setupUi(this);
    ui->okBtn->setEnabled(false);
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowMinMaxButtonsHint);

    ui->abbrLbl->setText(Abbr);
    ui->saloonLbl->setText(Saloon);
    ui->deviceLbl->setText(Device);

    ui->rdLE->setValidator(rdValidator);
    ui->impLE->setValidator(imexValidator);
    ui->expLE->setValidator(imexValidator);
}

AddVPNDialog::~AddVPNDialog()
{
    delete ui;
}

void AddVPNDialog::enableOkBtn()
{
    bool vpnLE = ui->vpnLE->text().isEmpty();
    bool rdLE = !ui->rdLE->hasAcceptableInput();
    bool descLE = ui->descLE->text().isEmpty();
    bool expLE = !ui->expLE->hasAcceptableInput();
    bool impLE = !ui->impLE->hasAcceptableInput();

    if(vpnLE || rdLE || descLE || expLE || impLE)
        ui->okBtn->setEnabled(false);
    else
        ui->okBtn->setEnabled(true);
}

void AddVPNDialog::on_cancelBtn_clicked()
{
    this->close();
}

void AddVPNDialog::on_okBtn_clicked()
{
    QString vpn = ui->vpnLE->text();
    QString rd = ui->rdLE->text();
    QString desc = ui->descLE->text();

    QString expRT = ui->expLE->text();
    QString impRT = ui->impLE->text();

    int newVpnId = dbMan->getLastVpnId();
    if(newVpnId > -1)
    {
        newVpnId++;
        if(dbMan->insertVpn(newVpnId, devexId, vpn, rd, desc))
        {
            if(dbMan->insertVpnRTs(newVpnId, expRT, impRT))
            {
                this->close();
            }
            else
            {
                dbMan->deleteVpn(newVpnId);
                QMessageBox::warning(this,"ERROR", "Cannot insert VPN.check your Inputs.\n"+dbMan->lastError());
            }
        }
        else
        {
            QMessageBox::warning(this,"ERROR", "Cannot insert VPN. check your Inputs.\n"+dbMan->lastError());
        }
    }
    else
        QMessageBox::warning(this,"ERROR", "Please try again, there is no response from Database.");

}

void AddVPNDialog::on_vpnLE_textChanged(const QString &arg1)
{
    if(arg1.isEmpty()) {ui->okBtn->setEnabled(false);return;}

    enableOkBtn();
}

void AddVPNDialog::on_rdLE_textChanged(const QString &arg1)
{
    if(arg1.isEmpty()) {ui->okBtn->setEnabled(false);return;}

    enableOkBtn();
}

void AddVPNDialog::on_descLE_textChanged(const QString &arg1)
{
    if(arg1.isEmpty()) {ui->okBtn->setEnabled(false);return;}

    enableOkBtn();
}

void AddVPNDialog::on_expLE_textChanged(const QString &arg1)
{
    if(arg1.isEmpty()) {ui->okBtn->setEnabled(false);return;}

    enableOkBtn();
}

void AddVPNDialog::on_impLE_textChanged(const QString &arg1)
{
    if(arg1.isEmpty()) {ui->okBtn->setEnabled(false);return;}

    enableOkBtn();
}
