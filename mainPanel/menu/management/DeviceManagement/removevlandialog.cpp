#include "removevlandialog.h"
#include "ui_removevlandialog.h"
#include "lib/database/danetdbman.h"
#include <QMessageBox>

RemoveVlanDialog::RemoveVlanDialog(QWidget *parent, DanetDbMan *db, int VlanId, QString Abbr, QString Saloon, QString Device, int Vlan, QString Desc) :
    QDialog(parent),
    ui(new Ui::RemoveVlanDialog),
    dbMan(db),
    vlanId(VlanId)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowMinMaxButtonsHint);

    ui->okBtn->setEnabled(false);
    ui->checkBox->setChecked(false);

    ui->abbrLbl->setText(Abbr);
    ui->saloonLbl->setText(Saloon);
    ui->deviceLbl->setText(Device);
    ui->vlanLbl->setText(QString::number(Vlan));
    ui->descLbl->setText(Desc);
}

RemoveVlanDialog::~RemoveVlanDialog()
{
    delete ui;
}

void RemoveVlanDialog::on_cancelBtn_clicked()
{
    this->close();
}

void RemoveVlanDialog::on_okBtn_clicked()
{
    if(dbMan->deleteVlan(vlanId))
    {
        this->close();
    }
    else
        QMessageBox::warning(this,"ERROR", "Cannot Remove VLAN.\n"+dbMan->lastError());
}

void RemoveVlanDialog::on_checkBox_toggled(bool checked)
{
    if(checked)
        ui->okBtn->setEnabled(true);
    else
        ui->okBtn->setEnabled(false);
}



