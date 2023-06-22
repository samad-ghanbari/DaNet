#include "addvlanrefdialog.h"
#include "ui_addvlanrefdialog.h"
#include "lib/database/danetdbman.h"
#include <QSqlQuery>
#include <QMessageBox>

AddVlanRefDialog::AddVlanRefDialog(QWidget *parent, DanetDbMan *db) :
    QDialog(parent),
    ui(new Ui::AddVlanRefDialog),
    dbMan(db)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowMinMaxButtonsHint);

}

AddVlanRefDialog::~AddVlanRefDialog()
{
    delete ui;
}

void AddVlanRefDialog::checkEntry()
{
    bool d = ui->descTE->toPlainText().isEmpty();
    if(d)
        ui->okBtn->setEnabled(false);
    else
        ui->okBtn->setEnabled(true);
}

void AddVlanRefDialog::on_CancelBtn_clicked()
{
    this->close();
}

void AddVlanRefDialog::on_okBtn_clicked()
{
    int vlan = ui->vlanSB->value();
    QString desc = ui->descTE->toPlainText();

    QString username = dbMan->getLoggedinNameLastname();
    dbMan->insertLog(username, "Add VLAN Reference: "+QString::number(vlan) + " Desc: "+desc);

    if(dbMan->insertVlanRef(vlan, desc))
        this->close();
    else
        QMessageBox::information(this, "Error", "Can not Add VLAN.\n"+dbMan->lastError());
}

void AddVlanRefDialog::on_descTE_textChanged()
{
    checkEntry();
}
