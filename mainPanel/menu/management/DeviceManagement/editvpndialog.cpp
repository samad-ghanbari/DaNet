#include "editvpndialog.h"
#include "ui_editvpndialog.h"
#include "lib/database/danetdbman.h"
#include <QMessageBox>

EditVPNDialog::EditVPNDialog(QWidget *parent, DanetDbMan *db, const QString abbr, const QString saloon, const QString device, const int VpnId, const QString Vpn, const QString RD, const QString Desc, const QString exp, const QString imp) :
    QDialog(parent),
    ui(new Ui::EditVPNDialog),
    dbMan(db),
    vpnId(VpnId),
    rdReg("^\\d+:\\d+$"),
    imexReg("^\\d+:\\d+(\\s\\d+:\\d+)*"),
    rdValidator(new QRegExpValidator(rdReg, this)),
    imexValidator(new QRegExpValidator(imexReg, this))
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowMinMaxButtonsHint);

    ui->abbrLbl->setText(abbr);
    ui->saloonLbl->setText(saloon);
    ui->deviceLbl->setText(device);

    ui->vpnLE->setText(Vpn);
    ui->rdLE->setText(RD);
    ui->descLE->setText(Desc);

    ui->expLE->setText(exp);
    ui->impLE->setText(imp);

    ui->rdLE->setValidator(rdValidator);
    ui->impLE->setValidator(imexValidator);
    ui->expLE->setValidator(imexValidator);

    ui->okBtn->setEnabled(false);
}

EditVPNDialog::~EditVPNDialog()
{
    delete ui;
}

void EditVPNDialog::enableOkBtn()
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

void EditVPNDialog::on_cancelBtn_clicked()
{
    this->close();
}

void EditVPNDialog::on_okBtn_clicked()
{
    QString vpn = ui->vpnLE->text();
    QString rd = ui->rdLE->text();
    QString desc = ui->descLE->text();
    QString exp = ui->expLE->text();
    QString imp = ui->impLE->text();

    if(dbMan->updateVpn(vpnId, vpn,rd,desc, exp, imp))
    {
        this->close();
    }
    else
        QMessageBox::warning(this, "ERROR", "Cannot Modify VPN. Check your inputs.\n"+dbMan->lastError());

}

void EditVPNDialog::on_vpnLE_textChanged(const QString &arg1)
{
    if(arg1.isEmpty()) {ui->okBtn->setEnabled(false);return;}

    enableOkBtn();
}

void EditVPNDialog::on_rdLE_textChanged(const QString &arg1)
{
    if(arg1.isEmpty()) {ui->okBtn->setEnabled(false);return;}

    enableOkBtn();
}

void EditVPNDialog::on_descLE_textChanged(const QString &arg1)
{
    if(arg1.isEmpty()) {ui->okBtn->setEnabled(false);return;}

    enableOkBtn();
}

void EditVPNDialog::on_expLE_textChanged(const QString &arg1)
{
    if(arg1.isEmpty()) {ui->okBtn->setEnabled(false);return;}

    enableOkBtn();
}

void EditVPNDialog::on_impLE_textChanged(const QString &arg1)
{
    if(arg1.isEmpty()) {ui->okBtn->setEnabled(false);return;}

    enableOkBtn();
}
