#include "editpurchasedialog.h"
#include "ui_editpurchasedialog.h"
#include "lib/database/danetdbman.h"
#include <QMessageBox>

EditPurchaseDialog::EditPurchaseDialog(QWidget *parent, DanetDbMan *db, const int Id, const QString Abbr, QString Pur) :
    QDialog(parent),
    ui(new Ui::EditPurchaseDialog),
    dbMan(db),
    id(Id)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowMinMaxButtonsHint);

    ui->abbrLE->setText(Abbr);
    ui->purLE->setText(Pur);

    ui->okBtn->setEnabled(false);
}

EditPurchaseDialog::~EditPurchaseDialog()
{
    delete ui;
}

void EditPurchaseDialog::on_CancelBtn_clicked()
{
    this->close();
}

void EditPurchaseDialog::on_okBtn_clicked()
{
    QString abbr = ui->abbrLE->text();
    QString pur = ui->purLE->text();

    if(dbMan->updatePurchase(id,abbr,pur))
    {
        this->close();
    }
    else
        QMessageBox::warning(this, "ERROR", "Cannot update purchase.\n"+dbMan->lastError());
}

void EditPurchaseDialog::on_abbrLE_textChanged(const QString &arg1)
{
    if(arg1.isEmpty() || ui->purLE->text().isEmpty())
        ui->okBtn->setEnabled(false);
    else
        ui->okBtn->setEnabled(true);
}


void EditPurchaseDialog::on_purLE_textChanged(const QString &arg1)
{
    if(arg1.isEmpty() || ui->abbrLE->text().isEmpty())
        ui->okBtn->setEnabled(false);
    else
        ui->okBtn->setEnabled(true);
}
