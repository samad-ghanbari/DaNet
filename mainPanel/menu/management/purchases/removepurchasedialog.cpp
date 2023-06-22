#include "removepurchasedialog.h"
#include "ui_removepurchasedialog.h"
#include "lib/database/danetdbman.h"
#include <QMessageBox>

RemovePurchaseDialog::RemovePurchaseDialog(QWidget *parent, DanetDbMan *db, const int Id, const QString abbr, const QString pur) :
    QDialog(parent),
    ui(new Ui::RemovePurchaseDialog),
    dbMan(db),
    id(Id)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowMinMaxButtonsHint);

    ui->abbrLbl->setText(abbr);
    ui->purLbl->setText(pur);
    ui->checkBox->setChecked(false);
    ui->okBtn->setEnabled(false);

}

RemovePurchaseDialog::~RemovePurchaseDialog()
{
    delete ui;
}

void RemovePurchaseDialog::on_CancelBtn_clicked()
{
    this->close();
}

void RemovePurchaseDialog::on_okBtn_clicked()
{
    if(dbMan->deletePurchase(id))
    {
        this->close();
    }
    else
        QMessageBox::warning(this, "ERROR", "Cannot delete purchase.\n"+dbMan->lastError());
}

void RemovePurchaseDialog::on_checkBox_toggled(bool checked)
{
    if(checked)
        ui->okBtn->setEnabled(true);
    else
        ui->okBtn->setEnabled(false);
}
