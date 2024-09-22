#include "changedslamcarddialog.h"
#include "ui_changedslamcarddialog.h"
#include "lib/database/danetdbman.h"
#include <QMessageBox>

ChangeDslamCardDialog::ChangeDslamCardDialog(QWidget *parent, DanetDbMan *db, const QString abbr, const int DevexId, const QString device, const int SlotsId, const int cardId, const int shelf, const int slot, const int subSlot) :
    QDialog(parent),
    ui(new Ui::ChangeDslamCardDialog),
    dbMan(db),
    devexId(DevexId),
    slotsId(SlotsId)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowMinMaxButtonsHint);


    ui->abbrLbl->setText(abbr);


    int saloon = dbMan->getDevexSaloon(devexId);
    ui->saloonLbl->setText(dbMan->getSaloonName(saloon));


    ui->deviceLbl->setText(device);

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



    if(dbMan->isDevexDslam(DevexId))
    {
        ui->errorLbl->setVisible(false);
        ui->cardCB->setEnabled(true);
        int devId = dbMan->getDevexDeviceId(DevexId);
        QMap<int, QString> map = dbMan->getDeviceCards(devId);
        foreach (int i, map.keys())
        {
            ui->cardCB->addItem(map.value(i), i);
        }

        ui->cardCB->setCurrentIndex(ui->cardCB->findData(cardId));
    }
    else
    {
        ui->errorLbl->setVisible(true);
        ui->cardCB->clear();
        ui->cardCB->setEnabled(false);
    }

    ui->okBtn->setEnabled(false);
}

ChangeDslamCardDialog::~ChangeDslamCardDialog()
{
    delete ui;
}

void ChangeDslamCardDialog::on_cancelBtn_clicked()
{
    this->close();
}

void ChangeDslamCardDialog::on_okBtn_clicked()
{
    int cardId = ui->cardCB->currentData().toInt();
    QString card = ui->cardCB->currentText();
    QString pos = ui->positionLbl->text();
    QString device = ui->deviceLbl->text();

    QString username = dbMan->getLoggedinNameLastname();

    int exchId = dbMan->getDevexExchangeId(devexId);


    if(dbMan->updateSlots(slotsId,cardId))
    {
        dbMan->insertLog(exchId, username, "Changed Card to "+card+" in Device: "+device+" at position: "+pos);
        this->close();
    }
    else
        QMessageBox::warning(this, "ERROR", "Cannot change card.\n"+dbMan->lastError());
}

void ChangeDslamCardDialog::on_cardCB_currentIndexChanged(int index)
{
    index++;
    ui->okBtn->setEnabled(true);
}
