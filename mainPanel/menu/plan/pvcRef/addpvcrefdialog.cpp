#include "addpvcrefdialog.h"
#include "ui_addpvcrefdialog.h"
#include "lib/database/danetdbman.h"
#include <QSqlQuery>
#include <QMessageBox>

AddPvcRefDialog::AddPvcRefDialog(QWidget *parent, DanetDbMan *db) :
    QDialog(parent),
    ui(new Ui::AddPvcRefDialog),
    dbMan(db)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowMinMaxButtonsHint);

}

AddPvcRefDialog::~AddPvcRefDialog()
{
    delete ui;
}

void AddPvcRefDialog::checkEntry()
{
    bool d = ui->descTE->toPlainText().isEmpty();
    if(d)
        ui->okBtn->setEnabled(false);
    else
        ui->okBtn->setEnabled(true);
}

void AddPvcRefDialog::on_CancelBtn_clicked()
{
    this->close();
}

void AddPvcRefDialog::on_okBtn_clicked()
{
    int vpi = ui->vpiSB->value();
    int vci = ui->vciSB->value();
    QString desc = ui->descTE->toPlainText();
    QString pvc = QString::number(vpi)+"/"+QString::number(vci);
    QString username = dbMan->getLoggedinNameLastname();
    dbMan->insertLog(username, "Add PVC Reference: "+pvc + " Desc: "+desc);

    if(dbMan->insertPvcRef(vpi, vci, desc))
        this->close();
    else
        QMessageBox::information(this, "Error", "Can not Add PVC.\n"+dbMan->lastError());
}

void AddPvcRefDialog::on_vpiSB_valueChanged(int arg1)
{
    arg1++;
    checkEntry();
}

void AddPvcRefDialog::on_vciSB_valueChanged(int arg1)
{
    arg1++;
    checkEntry();
}

void AddPvcRefDialog::on_descTE_textChanged()
{
    checkEntry();
}
