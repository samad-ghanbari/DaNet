#include "editvlanrefdialog.h"
#include "ui_editvlanrefdialog.h"
#include "lib/database/danetdbman.h"
#include <QSqlQuery>
#include <QMessageBox>

EditVlanRefDialog::EditVlanRefDialog(QWidget *parent, DanetDbMan *db, const int Id, const int vlan, const QString desc) :
    QDialog(parent),
    ui(new Ui::EditVlanRefDialog),
    dbMan(db),
    id(Id),
    oldVlan(QString::number(vlan))
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowMinMaxButtonsHint);

    ui->vlanSB->setValue(vlan);
    ui->descTE->setText(desc);
    ui->okBtn->setEnabled(false);
}

EditVlanRefDialog::~EditVlanRefDialog()
{
    delete ui;
}

void EditVlanRefDialog::checkEntry()
{
    bool d =ui->descTE->toPlainText().isEmpty();

    if(d)
        ui->okBtn->setEnabled(false);
    else
        ui->okBtn->setEnabled(true);
}

void EditVlanRefDialog::on_CancelBtn_clicked()
{
    this->close();
}

void EditVlanRefDialog::on_okBtn_clicked()
{
    int vlan = ui->vlanSB->value();
    QString desc = ui->descTE->toPlainText();
    QString username = dbMan->getLoggedinNameLastname();
    dbMan->insertLog(username, "Edit VLAN Reference: "+oldVlan+" To VLAN: "+QString::number(vlan) + " Desc: "+desc);

    if(dbMan->updateVlanRef(id,vlan, desc))
        this->close();
    else
        QMessageBox::information(this, "Error", "Can not edit VLAN.\n"+dbMan->lastError());
}

void EditVlanRefDialog::on_vlanSB_valueChanged(int arg1)
{
    arg1++;
    checkEntry();
}

void EditVlanRefDialog::on_descTE_textChanged()
{
    checkEntry();
}
