#include "depleteodfpindialog.h"
#include "ui_depleteodfpindialog.h"
#include "lib/database/danetdbman.h"
#include <QSqlQuery>
#include <QMessageBox>

DepleteOdfPinDialog::DepleteOdfPinDialog(QWidget *parent, DanetDbMan *db, const int PinId, const QString exch,
                                         const QString saloon, const QString odf, const QString pos, const QString pin,
                                         const QString LocalDevice,const QString LocalLabel,const QString Remote,const QString RemoteLabel) :
    QDialog(parent),
    ui(new Ui::DepleteOdfPinDialog),
    dbMan(db),
    pinId(PinId)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowMinMaxButtonsHint);

    ui->okBtn->setEnabled(false);
    ui->checkBox->setChecked(false);
    ui->abbrLbl->setText(exch);
    ui->saloonLbl->setText(saloon);
    ui->odfLbl->setText(odf);
    ui->posLbl->setText(pos);
    ui->pinLbl->setText(pin);

    ui->localDevLbl->setText(LocalDevice);
    ui->localLabelLbl->setText(LocalLabel);
    ui->remoteLbl->setText(Remote);
    ui->remoteLabelLbl->setText(RemoteLabel);

}

DepleteOdfPinDialog::~DepleteOdfPinDialog()
{
    delete ui;
}

void DepleteOdfPinDialog::on_checkBox_toggled(bool checked)
{
    if(checked)
        ui->okBtn->setEnabled(true);
    else
        ui->okBtn->setEnabled(false);
}

void DepleteOdfPinDialog::on_cancelBtn_clicked()
{
    this->close();
}

void DepleteOdfPinDialog::on_okBtn_clicked()
{
    if(dbMan->pinParamEditable(pinId))
    {
        if(dbMan->depletePinParam(pinId))
        {
            this->close();
        }
        else
            QMessageBox::warning(this,"ERROR", "Cannot Empty PIN.\n"+dbMan->lastError());

    }
    else
        QMessageBox::warning(this,"ERROR", "Cannot Empty PIN.\n"+dbMan->lastError());

}
