#include "letterremovedialog.h"
#include "ui_letterremovedialog.h"
#include "lib/database/danetdbman.h"
#include <QMessageBox>

LetterRemoveDialog::LetterRemoveDialog(DanetDbMan *db, const int ID, const QString letter_id, const QString letter_no, QString from, const QString to, const QString desc, const QString ts, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LetterRemoveDialog),
    dbMan(db),
    id(ID)
{
    ui->setupUi(this);
    ui->okBtn->setEnabled(false);
    ui->confirmChb->setChecked(false);

    ui->letterIdLbl->setText(letter_id);
    ui->letterNoLbl->setText(letter_no);
    ui->fromLbl->setText(from);
    ui->toLbl->setText(to);
    ui->descTE->setText(desc);
    ui->tsLbl->setText(ts);

}

LetterRemoveDialog::~LetterRemoveDialog()
{
    delete ui;
}

void LetterRemoveDialog::on_confirmChb_toggled(bool checked)
{
    if(checked)
        ui->okBtn->setEnabled(true);
    else
        ui->okBtn->setEnabled(false);
}

void LetterRemoveDialog::on_okBtn_clicked()
{
    if(dbMan->deleteLetter(id))
        this->close();
    else
        QMessageBox::warning(this, "Error", "There was an error during the operation!");

}
