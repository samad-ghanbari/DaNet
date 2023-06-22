#include "telephoneaddeditdialog.h"
#include "ui_telephoneaddeditdialog.h"
#include "lib/database/danetdbman.h"
#include <QMessageBox>

TelephoneAddEditDialog::TelephoneAddEditDialog(DanetDbMan *db, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TelephoneAddEditDialog),
    dbMan(db)
{
    ui->setupUi(this);
    id = -1;
}

TelephoneAddEditDialog::~TelephoneAddEditDialog()
{
    delete ui;
}

void TelephoneAddEditDialog::setValues(int Id, QString name, QString dep, QString pos, QString tel)
{
    id = Id;
    ui->label->setText("Telephone Modification");
    ui->nameLE->setText(name);
    ui->depCB->setCurrentText(dep);
    ui->posCB->setCurrentText(pos);
    ui->telLE->setText(tel);
}

void TelephoneAddEditDialog::on_okBtn_clicked()
{
    QString name = ui->nameLE->text().trimmed();
    QString dep = ui->depCB->currentText().trimmed();
    QString pos = ui->posCB->currentText().trimmed();
    QString tel = ui->telLE->text().trimmed();
    if(name.isEmpty() || tel.isEmpty())
    {
        QMessageBox::warning(this, "Error", "Contact Name and telephone Number cannot be empty!");
        return;
    }
    if(id > 0)
    {
        //edit
        dbMan->updateTelephone(id,name, dep,pos,tel);
    }
    else
    {
        //new
        dbMan->insertTelephone(name, dep, pos, tel);
    }

    this->close();
}
