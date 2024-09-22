#include "editodfpindialog.h"
#include "ui_editodfpindialog.h"
#include "lib/database/danetdbman.h"
#include <QSqlQuery>
#include <QMessageBox>


EditOdfPinDialog::EditOdfPinDialog(QWidget *parent, DanetDbMan *db, const int PinId, const QString exch,
                                   const QString saloon, const QString odf, const QString pos, const QString pin,
                                   const QString LocalDevice,const QString LocalLabel,const QString Remote,const QString RemoteLabel):
    QDialog(parent),
    ui(new Ui::EditOdfPinDialog),
    dbMan(db),
    pinId(PinId)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowMinMaxButtonsHint);

    ui->okBtn->setEnabled(false);
    ui->abbrLbl->setText(exch);
    ui->saloonLbl->setText(saloon);
    ui->odfLbl->setText(odf);
    ui->posLbl->setText(pos);
    ui->pinLbl->setText(pin);

    ui->localDevLE->setText(LocalDevice);
    ui->localLblLE->setText(LocalLabel);
    ui->remoteLE->setText(Remote);
    ui->remoteLblLE->setText(RemoteLabel);

}

EditOdfPinDialog::~EditOdfPinDialog()
{
    delete ui;
}


bool EditOdfPinDialog::checkFields()
{
    bool c1 = ui->localDevLE->text().isEmpty() || ui->localLblLE->text().isEmpty();
    bool c2 = ui->remoteLE->text().isEmpty() || ui->remoteLblLE->text().isEmpty();

    if(c1 && c2)
        return false;
    else
        return true;
}

void EditOdfPinDialog::on_localDevLE_textChanged(const QString &arg1)
{
    if(arg1.isEmpty())
    {
        ui->okBtn->setEnabled(false);
        return;
    }

    if(checkFields())
        ui->okBtn->setEnabled(true);
    else
        ui->okBtn->setEnabled(false);
}

void EditOdfPinDialog::on_localLblLE_textChanged(const QString &arg1)
{
    if(arg1.isEmpty())
    {
        ui->okBtn->setEnabled(false);
        return;
    }

    if(checkFields())
        ui->okBtn->setEnabled(true);
    else
        ui->okBtn->setEnabled(false);
}

void EditOdfPinDialog::on_remoteLblLE_textChanged(const QString &arg1)
{
    if(arg1.isEmpty())
    {
        ui->okBtn->setEnabled(false);
        return;
    }

    if(checkFields())
        ui->okBtn->setEnabled(true);
    else
        ui->okBtn->setEnabled(false);
}

void EditOdfPinDialog::on_cancelBtn_clicked()
{
    this->close();
}

void EditOdfPinDialog::on_okBtn_clicked()
{
    QString localDev = ui->localDevLE->text().trimmed();
    QString localLbl = ui->localLblLE->text().trimmed();
    QString remote = ui->remoteLE->text().trimmed();
    QString remoteLbl = ui->remoteLblLE->text().trimmed();

    if(dbMan->updatePinParam(pinId,localDev,localLbl, remote, remoteLbl))
    {
        this->close();
    }
    else
    {
        QMessageBox::warning(this,"ERROR", "Cannot modify PIN parameter.\n"+dbMan->lastError());
    }

}


void EditOdfPinDialog::on_remoteLE_textChanged(const QString &arg1)
{
    if(arg1.isEmpty())
    {
        ui->okBtn->setEnabled(false);
        return;
    }

    if(checkFields())
        ui->okBtn->setEnabled(true);
    else
        ui->okBtn->setEnabled(false);
}
