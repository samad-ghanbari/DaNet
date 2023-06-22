#include "removesaloondialog.h"
#include "ui_removesaloondialog.h"
#include "lib/database/danetdbman.h"
#include <QMessageBox>

RemoveSaloonDialog::RemoveSaloonDialog(QWidget *parent, DanetDbMan *db, const int Id, const QString Sal):
    QDialog(parent),
    ui(new Ui::RemoveSaloonDialog),
    dbMan(db),
    id(Id)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowMinMaxButtonsHint);

    ui->saloonLbl->setText(Sal);
    ui->checkBox->setChecked(false);
    ui->okBtn->setEnabled(false);

}

RemoveSaloonDialog::~RemoveSaloonDialog()
{
    delete ui;
}

void RemoveSaloonDialog::on_CancelBtn_clicked()
{
    this->close();
}

void RemoveSaloonDialog::on_okBtn_clicked()
{
    if(dbMan->deleteSaloon(id))
        this->close();
    else
        QMessageBox::warning(this, "ERROR", "Cannot delete saloon.\n"+dbMan->lastError());
}

void RemoveSaloonDialog::on_checkBox_toggled(bool checked)
{
    if(checked)
        ui->okBtn->setEnabled(true);
    else
        ui->okBtn->setEnabled(false);
}
