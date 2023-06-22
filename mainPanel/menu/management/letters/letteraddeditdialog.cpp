#include "letteraddeditdialog.h"
#include "ui_letteraddeditdialog.h"
#include "lib/database/danetdbman.h"
#include <QMessageBox>

LetterAddEditDialog::LetterAddEditDialog(DanetDbMan *db, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LetterAddEditDialog),
      dbMan(db)
{
    ui->setupUi(this);
    id = -1;
    ui->fromCB->setCurrentIndex(6);
    ui->toCB->setCurrentIndex(7);
}

LetterAddEditDialog::~LetterAddEditDialog()
{
    delete ui;
}

void LetterAddEditDialog::setValues(int Id, QString letter_id, QString letter_no, QString from_dep, QString to_dep, QString desc)
{
    id = Id;
    ui->label->setText("Letter Info Modification");
    ui->letterIdLE->setText(letter_id);
    ui->letterNoLE->setText(letter_no);
    ui->fromCB->setCurrentText(from_dep);
    ui->toCB->setCurrentText(to_dep);
    ui->descTE->setText(desc);
}

void LetterAddEditDialog::on_okBtn_clicked()
{
    QString letter_id = ui->letterIdLE->text().trimmed();
    QString letter_no = ui->letterNoLE->text().trimmed();
    QString from = ui->fromCB->currentText().trimmed();
    QString to = ui->toCB->currentText().trimmed();
    QString desc = ui->descTE->toPlainText().trimmed();

    if(letter_id.isEmpty() || letter_no.isEmpty())
    {
        QMessageBox::warning(this, "Error", "Letter ID or Letter Number cannot be both empty!");
        return;
    }
    if(from.compare(to, Qt::CaseInsensitive) == 0)
    {
        QMessageBox::warning(this, "Error", "From-Dep. and To-Dep. cannot be the same! ");
        return;
    }
    if(id > 0)
    {
        //edit
        dbMan->updateLetter(id,letter_id,letter_no, from,to, desc);
    }
    else
    {
        //new
        dbMan->insertLetter(letter_id,letter_no, from,to, desc);
    }

    this->close();
}
