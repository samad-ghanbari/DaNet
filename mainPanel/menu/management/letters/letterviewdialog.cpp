#include "letterviewdialog.h"
#include "ui_letterviewdialog.h"

LetterViewDialog::LetterViewDialog(const QString letter_id, const QString letter_no, QString from, const QString to, const QString desc, const QString ts,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LetterViewDialog)
{
    ui->setupUi(this);

    ui->letterIdLbl->setText(letter_id);
    ui->letterNoLbl->setText(letter_no);
    ui->fromLbl->setText(from);
    ui->toLbl->setText(to);
    ui->descTE->setText(desc);
    ui->tsLbl->setText(ts);
}

LetterViewDialog::~LetterViewDialog()
{
    delete ui;
}

void LetterViewDialog::on_okBtn_clicked()
{
    this->close();
}
