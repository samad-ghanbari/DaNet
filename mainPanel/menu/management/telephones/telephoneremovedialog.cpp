#include "telephoneremovedialog.h"
#include "ui_telephoneremovedialog.h"
#include "lib/database/danetdbman.h"
#include <QMessageBox>

TelephoneRemoveDialog::TelephoneRemoveDialog(DanetDbMan *db, const int ID, const QString name, const QString dep, const QString pos, const QString tel,  QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TelephoneRemoveDialog),
    dbMan(db),
    id(ID)

{
    ui->setupUi(this);
    ui->confirmChb->setChecked(false);
    ui->okBtn->setEnabled(false);

    ui->nameLbl->setText(name);
    ui->depLbl->setText(dep);
    ui->posLbl->setText(pos);
    ui->telLbl->setText(tel);
}

TelephoneRemoveDialog::~TelephoneRemoveDialog()
{
    delete ui;
}

void TelephoneRemoveDialog::on_confirmChb_toggled(bool checked)
{
    if(checked)
        ui->okBtn->setEnabled(true);
    else
        ui->okBtn->setEnabled(false);
}

void TelephoneRemoveDialog::on_okBtn_clicked()
{
    if(dbMan->deleteTelephone(id))
    {
        this->close();
    }
    else
    {
        QMessageBox::warning(this, "Error", "There was an error during the operation.");
    }
}
