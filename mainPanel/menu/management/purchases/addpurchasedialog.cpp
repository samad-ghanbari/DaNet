#include "addpurchasedialog.h"
#include "ui_addpurchasedialog.h"
#include "lib/database/danetdbman.h"
#include <QMessageBox>

AddPurchaseDialog::AddPurchaseDialog(QWidget *parent, DanetDbMan *db) :
    QDialog(parent),
    ui(new Ui::AddPurchaseDialog),
    dbMan(db)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowMinMaxButtonsHint);

    ui->okBtn->setEnabled(false);
}

AddPurchaseDialog::~AddPurchaseDialog()
{
    delete ui;
}

void AddPurchaseDialog::on_abbrLE_textChanged(const QString &arg1)
{
    if(arg1.isEmpty() || ui->purLE->text().isEmpty())
        ui->okBtn->setEnabled(false);
    else
        ui->okBtn->setEnabled(true);
}

void AddPurchaseDialog::on_purLE_textChanged(const QString &arg1)
{
    if(arg1.isEmpty() || ui->abbrLE->text().isEmpty())
        ui->okBtn->setEnabled(false);
    else
        ui->okBtn->setEnabled(true);
}

void AddPurchaseDialog::on_CancelBtn_clicked()
{
    this->close();
}

void AddPurchaseDialog::on_okBtn_clicked()
{
    QString abbr = ui->abbrLE->text().trimmed();
    QString pur = ui->purLE->text().trimmed();

    if(dbMan->insertPurchase(abbr,pur))
    {
        this->close();
    }
    else
        QMessageBox::warning(this, "ERROR", "Cannot add purchase.\n"+dbMan->lastError());

}
