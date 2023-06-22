#include "removevlanrefdialog.h"
#include "ui_removevlanrefdialog.h"
#include "lib/database/danetdbman.h"
#include <QMessageBox>

RemoveVlanRefDialog::RemoveVlanRefDialog(QWidget *parent, DanetDbMan *db, const int Id, const int vlan, const QString Desc) :
    QDialog(parent),
    ui(new Ui::RemoveVlanRefDialog),
    dbMan(db),
    id(Id),
    oldVlan(QString::number(vlan)),
    desc(Desc)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowMinMaxButtonsHint);

    ui->vlanLbl->setText(QString::number(vlan));
    ui->descTE->setText(Desc);

    ui->removeBtn->setEnabled(false);
    ui->checkBox->setChecked(false);
}

RemoveVlanRefDialog::~RemoveVlanRefDialog()
{
    delete ui;
}

void RemoveVlanRefDialog::on_cancelBtn_clicked()
{
    this->close();
}

void RemoveVlanRefDialog::on_removeBtn_clicked()
{
    QString username = dbMan->getLoggedinNameLastname();
    dbMan->insertLog(username, "Remove VLAN Reference: "+oldVlan +" Desc: "+desc);

    if(dbMan->deleteVlanRef(id))
        this->close();
    else
        QMessageBox::information(this, "Error", "Can not remove Vlan Reference.\n"+dbMan->lastError());
}

void RemoveVlanRefDialog::on_checkBox_toggled(bool checked)
{
    if(checked)
        ui->removeBtn->setEnabled(true);
    else
        ui->removeBtn->setEnabled(false);
}
