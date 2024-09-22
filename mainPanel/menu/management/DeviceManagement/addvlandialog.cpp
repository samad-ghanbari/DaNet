#include "addvlandialog.h"
#include "ui_addvlandialog.h"
#include "lib/database/danetdbman.h"
#include <QMessageBox>

AddVlanDialog::AddVlanDialog(QWidget *parent, DanetDbMan *db, int DevexId, QString Abbr, QString Saloon, QString Device) :
    QDialog(parent),
    ui(new Ui::AddVlanDialog),
    dbMan(db),
    devexId(DevexId)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowMinMaxButtonsHint);

    ui->abbrLbl->setText(Abbr);
    ui->saloonLbl->setText(Saloon);
    ui->deviceLbl->setText(Device);
    ui->okBtn->setEnabled(false);
}

AddVlanDialog::~AddVlanDialog()
{
    delete ui;
}

void AddVlanDialog::on_cancelBtn_clicked()
{
    this->close();
}

void AddVlanDialog::on_okBtn_clicked()
{
    int vlan = ui->vlanSB->value();
    QString desc = ui->descLE->text();
    if(dbMan->insertVlan(devexId, vlan, desc))
        this->close();
    else
        QMessageBox::warning(this,"Error", "Cannot insert VLAN. Duplicate VLAN entry is not permitted.\n"+dbMan->lastError());
}

void AddVlanDialog::on_descLE_textChanged(const QString &arg1)
{
    if(arg1.isEmpty())
        ui->okBtn->setEnabled(false);
    else
        ui->okBtn->setEnabled(true);
}
void AddVlanDialog::on_vlanSB_valueChanged(int arg1)
{
    arg1++;
    if(ui->descLE->text().isEmpty())
        ui->okBtn->setEnabled(false);
    else
        ui->okBtn->setEnabled(true);
}
