#include "telephoneviewdialog.h"
#include "ui_telephoneviewdialog.h"

TelephoneViewDialog::TelephoneViewDialog(const QString name,  const QString dep, const QString pos, const QString tel, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TelephoneViewDialog)
{
    ui->setupUi(this);

    ui->nameLbl->setText(name);
    ui->depLbl->setText(dep);
    ui->posLbl->setText(pos);
    ui->telLbl->setText(tel);

}

TelephoneViewDialog::~TelephoneViewDialog()
{
    delete ui;
}

void TelephoneViewDialog::on_okBtn_clicked()
{
    this->close();
}
