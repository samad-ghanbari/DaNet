#include "editdevicepurchasedialog.h"
#include "ui_editdevicepurchasedialog.h"
#include <QSqlQuery>
#include "lib/database/danetdbman.h"
#include <QMessageBox>

EditDevicePurchaseDialog::EditDevicePurchaseDialog(QWidget *parent, DanetDbMan *db, const QString abbr, const QString saloon,  const int DevexId,
                                                   const int Row, const int Rack, const int Shelf, const QString DeviceName, const QString purchase) :
    QDialog(parent),
    ui(new Ui::EditDevicePurchaseDialog),
    dbMan(db),
    devexId(DevexId)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowMinMaxButtonsHint);

    ui->abbrLbl->setText(abbr);
    ui->saloonLbl->setText(saloon);
    ui->deviceLbl->setText(DeviceName);

    ui->RRSLbl->setText(QString::number(Row)+'*'+QString::number(Rack)+'*'+QString::number(Shelf));

    QSqlQuery *query = dbMan->selectPurchases();
    //`id`,`abbr`,`purchase`
    QString a, p;
    while(query->next())
    {
        a = query->value(1).toString();
        p = query->value(2).toString();
        p = a+"-"+p;
        ui->purchaseCB->addItem(p, a);
    }
    ui->purchaseCB->setCurrentIndex(ui->purchaseCB->findData(purchase));


    ui->okBtn->setEnabled(false);
    ui->checkBox->setChecked(false);
    ui->checkBox->setEnabled(false);
}

EditDevicePurchaseDialog::~EditDevicePurchaseDialog()
{
    delete ui;
}

void EditDevicePurchaseDialog::on_cancelBtn_clicked()
{
    this->close();
}

void EditDevicePurchaseDialog::on_okBtn_clicked()
{
    QString pur = ui->purchaseCB->currentData().toString();
    bool ok = true;

    if(dbMan->isDevexDslam(devexId))
    {
        if(dbMan->dslamAggExistance(devexId))
            ok = false;

        if(dbMan->dslamPppoeExistance(devexId))
            ok = false;

        if(dbMan->cxDslamExistance(devexId))
            ok = false;
    }

    if(ok)
    {
        if(dbMan->updateDevexPurchase(devexId, pur))
        {
            this->close();
        }
        else
            QMessageBox::warning(this, "Error", "Cannot update device purchase.\n"+dbMan->lastError());
    }
    else
        QMessageBox::warning(this, "Error", "Cannot change palnned device.\n");

}

void EditDevicePurchaseDialog::on_checkBox_toggled(bool checked)
{
    if(checked)
        ui->okBtn->setEnabled(true);
    else
        ui->okBtn->setEnabled(false);
}

void EditDevicePurchaseDialog::on_purchaseCB_currentIndexChanged(int index)
{
    index++;
    ui->checkBox->setEnabled(true);
}
