#include "editcardpurchasedialog.h"
#include "ui_editcardpurchasedialog.h"
#include <QSqlQuery>
#include "lib/database/danetdbman.h"
#include <QMessageBox>

EditCardPurchaseDialog::EditCardPurchaseDialog(QWidget *parent, DanetDbMan *db, const QString abbr, const int DevexId, const QString device, const int SlotsId, const QString card, const int shelf, const int slot, const int subSlot, QString purchase) :
    QDialog(parent),
    ui(new Ui::EditCardPurchaseDialog),
    dbMan(db),
    slotsId(SlotsId)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowMinMaxButtonsHint);

    ui->abbrLbl->setText(abbr);

    int saloon = dbMan->getDevexSaloon(DevexId);
    ui->saloonLbl->setText(dbMan->getSaloonName(saloon));


    ui->deviceLbl->setText(device);
    ui->cardLbl->setText(card);

    QString position = "";

    if(shelf > -1)
        position = QString::number(shelf);

    if(slot > -1)
    {
        if(position.isEmpty())
            position = QString::number(slot);
        else
            position += "/"+ QString::number(slot);
    }

    if(subSlot > -1)
        position += "/"+QString::number(subSlot);

    ui->positionLbl->setText(position);

    QSqlQuery *query = dbMan->selectPurchases();
    //`id`,`abbr`,`purchase`
    QString pur, ab;
    while(query->next())
    {
        ab = query->value(1).toString();
        pur = query->value(2).toString();

        pur = ab+" ("+pur+")";
        ui->purchaseCB->addItem(pur, ab);
    }

    ui->purchaseCB->setCurrentIndex(ui->purchaseCB->findData(purchase));

    ui->okBtn->setEnabled(false);
}

EditCardPurchaseDialog::~EditCardPurchaseDialog()
{
    delete ui;
}

void EditCardPurchaseDialog::on_cancelBtn_clicked()
{
    this->close();
}

void EditCardPurchaseDialog::on_okBtn_clicked()
{
    QString pur = ui->purchaseCB->currentData().toString();

    if(dbMan->updateSlotPurchase(slotsId,pur))
        this->close();
    else
        QMessageBox::warning(this, "ERROR", "Cannot update purchase.\n"+dbMan->lastError());
}

void EditCardPurchaseDialog::on_purchaseCB_currentIndexChanged(int index)
{
    index++;
    ui->okBtn->setEnabled(true);
}
