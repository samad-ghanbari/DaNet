#include "editvlandialog.h"
#include "ui_editvlandialog.h"
#include "lib/database/danetdbman.h"
#include <QMessageBox>


EditVlanDialog::EditVlanDialog(QWidget *parent, DanetDbMan *db, int ShelfId, QString Abbr, QString Saloon, QString Device, int Vlan, QString Desc) :
    QDialog(parent),
    ui(new Ui::EditVlanDialog),
    dbMan(db),
    vlan(Vlan),
    vlanId(ShelfId),
    desc(Desc)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowMinMaxButtonsHint);

    ui->abbrLbl->setText(Abbr);
    ui->saloonLbl->setText(Saloon);
    ui->deviceLbl->setText(Device);

    ui->vlanSB->setValue(vlan);
    ui->descLE->setText(desc);
}

EditVlanDialog::~EditVlanDialog()
{
    delete ui;
}

void EditVlanDialog::on_cancelBtn_clicked()
{
    this->close();
}

void EditVlanDialog::on_okBtn_clicked()
{
    int V = ui->vlanSB->value();
    QString D = ui->descLE->text();

    if(dbMan->updateVlan(vlanId, V, D))
    {
        this->close();
    }
    else
        QMessageBox::warning(this, "Error", "Cannot modify VLAN.\n"+dbMan->lastError());

}

void EditVlanDialog::on_vlanSB_valueChanged(int arg1)
{
    if( ( (arg1 != vlan) || (ui->descLE->text().trimmed().compare(desc,Qt::CaseSensitive) != 0) ) && !ui->descLE->text().isEmpty())
        ui->okBtn->setEnabled(true);
    else
        ui->okBtn->setEnabled(false);
}

void EditVlanDialog::on_descLE_textChanged(const QString &arg1)
{
    if( ( (ui->vlanSB->value() != vlan) || (arg1.trimmed().compare(desc,Qt::CaseSensitive) != 0) ) && !ui->descLE->text().isEmpty())
        ui->okBtn->setEnabled(true);
    else
        ui->okBtn->setEnabled(false);
}
