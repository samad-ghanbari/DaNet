#include "editpvcrefdialog.h"
#include "ui_editpvcrefdialog.h"
#include "lib/database/danetdbman.h"
#include <QSqlQuery>
#include <QMessageBox>

EditPvcRefDialog::EditPvcRefDialog(QWidget *parent, DanetDbMan *db, const int Id, const int vpi, const int vci, const QString desc) :
    QDialog(parent),
    ui(new Ui::EditPvcRefDialog),
    dbMan(db),
    id(Id)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowMinMaxButtonsHint);

    ui->vpiSB->setValue(vpi);
    ui->vciSB->setValue(vci);
    ui->descTE->setText(desc);

    oldPvc = QString::number(vpi)+"/"+QString::number(vci);
    ui->okBtn->setEnabled(false);
}

EditPvcRefDialog::~EditPvcRefDialog()
{
    delete ui;
}

void EditPvcRefDialog::checkEntry()
{
    bool d = ui->descTE->toPlainText().isEmpty();
    if(d)
        ui->okBtn->setEnabled(false);
    else
        ui->okBtn->setEnabled(true);
}

void EditPvcRefDialog::on_CancelBtn_clicked()
{
    this->close();
}

void EditPvcRefDialog::on_okBtn_clicked()
{
    int vpi = ui->vpiSB->value();
    int vci = ui->vciSB->value();
    QString desc = ui->descTE->toPlainText();
    QString pvc = QString::number(vpi)+"/"+QString::number(vci);
    QString username = dbMan->getLoggedinNameLastname();
    dbMan->insertLog(username, "Edit PVC: "+oldPvc+" To PVC: "+pvc + " Desc: "+desc);

    if(dbMan->updatePvcRef(id,vpi, vci, desc))
        this->close();
    else
        QMessageBox::information(this, "Error", "Can not edit PVC.\n"+dbMan->lastError());
}

void EditPvcRefDialog::on_descTE_textChanged()
{
    checkEntry();
}

void EditPvcRefDialog::on_vpiSB_valueChanged(int arg1)
{
    arg1++;
    checkEntry();
}

void EditPvcRefDialog::on_vciSB_valueChanged(int arg1)
{
    arg1++;
    checkEntry();
}
