#include "removevpndialog.h"
#include "ui_removevpndialog.h"
#include "lib/database/danetdbman.h"
#include <QMessageBox>

RemoveVPNDialog::RemoveVPNDialog(QWidget *parent, DanetDbMan *db, const QString abbr, const QString saloon, const QString device, const int VpnId, const QString Vpn, const QString RD, const QString Desc, const QString exp, const QString imp) :
    QDialog(parent),
    ui(new Ui::RemoveVPNDialog),
    dbMan(db),
    vpnId(VpnId)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowMinMaxButtonsHint);

    ui->abbrLbl->setText(abbr);
    ui->saloonLbl->setText(saloon);
    ui->deviceLbl->setText(device);

    ui->vpnLbl->setText(Vpn);
    ui->rdLbl->setText(RD);
    ui->descLbl->setText(Desc);
    ui->expLbl->setText(exp);
    ui->impLbl->setText(imp);
    ui->okBtn->setEnabled(false);
}

RemoveVPNDialog::~RemoveVPNDialog()
{
    delete ui;
}

void RemoveVPNDialog::on_checkBox_toggled(bool checked)
{
    if(checked)
        ui->okBtn->setEnabled(true);
    else
        ui->okBtn->setEnabled(false);
}

void RemoveVPNDialog::on_cancelBtn_clicked()
{
    this->close();
}

void RemoveVPNDialog::on_okBtn_clicked()
{
    if(dbMan->deleteVpn(vpnId))
    {
        this->close();
    }
    else
        QMessageBox::warning(this,"ERROR", "Cannot Delete VPN.\n"+dbMan->lastError());
}
