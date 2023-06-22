#include "removedevicecarddialog.h"
#include "ui_removedevicecarddialog.h"
#include "lib/database/danetdbman.h"
#include <QMessageBox>

RemoveDeviceCardDialog::RemoveDeviceCardDialog(QWidget *parent, DanetDbMan *db, const QString abbr, const int DevexId, const QString device, const int SlotsId, const QString card, const int shelf, const int slot, const int subSlot) :
    QDialog(parent),
    ui(new Ui::RemoveDeviceCardDialog),
    dbMan(db),
    devexId(DevexId),
    slotsId(SlotsId),
    all(false)
{
    ui->setupUi(this);

    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowMinMaxButtonsHint);

    ui->abbrLbl->setText(abbr);

    ui->removeAllCardsBtn->setEnabled(false);
    ui->removeAllCardsBtn->setVisible(false);


    int saloon = dbMan->getDevexSaloon(devexId);
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

    if(dbMan->cardRemovable(slotsId))
    {
        ui->errorLbl->setVisible(false);
        ui->checkBox->setChecked(false);
        ui->checkBox->setEnabled(true);
    }
    else
    {
        ui->errorLbl->setVisible(true);
        ui->checkBox->setChecked(false);
        ui->checkBox->setEnabled(false);
    }

    ui->okBtn->setEnabled(false);
}

RemoveDeviceCardDialog::RemoveDeviceCardDialog(QWidget *parent, DanetDbMan *db, const QString abbr, const int DevexId, const QString device) :
    QDialog(parent),
    ui(new Ui::RemoveDeviceCardDialog),
    dbMan(db),
    devexId(DevexId),
    slotsId(-1),
    all(true)
{
    ui->setupUi(this);
    this->setWindowTitle("Remove All DSLAM Cards");
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowMinMaxButtonsHint);

    ui->abbrLbl->setText(abbr);

    ui->okBtn->setEnabled(false);
    ui->okBtn->setVisible(false);

    this->setMinimumHeight(180);
    this->setMaximumHeight(181);


    int saloon = dbMan->getDevexSaloon(devexId);
    switch (saloon)
    {
    case 1:
        ui->saloonLbl->setText("Data");
        break;
    case 2:
        ui->saloonLbl->setText("Switch-Data");
        break;
    case 3:
        ui->saloonLbl->setText("PCM-Data");
        break;
    case 4:
        ui->saloonLbl->setText("Site");
    }

    ui->deviceLbl->setText(device);
    ui->frame->setVisible(false);

    if(dbMan->isDevexDslam(devexId))
    {
        ui->removeAllCardsBtn->setEnabled(false);
        ui->checkBox->setChecked(false);
        ui->checkBox->setEnabled(true);
        ui->errorLbl->setVisible(false);
    }
    else
    {
        ui->removeAllCardsBtn->setEnabled(false);
        ui->checkBox->setChecked(false);
        ui->checkBox->setEnabled(false);
    }
}

RemoveDeviceCardDialog::~RemoveDeviceCardDialog()
{
    delete ui;
}

void RemoveDeviceCardDialog::on_cancelBtn_clicked()
{
    this->close();
}

void RemoveDeviceCardDialog::on_okBtn_clicked()
{
    QString card = ui->cardLbl->text();
    QString position= ui->positionLbl->text();
    QString device = ui->deviceLbl->text();

    QString username = dbMan->getLoggedinNameLastname();

    int exchId = dbMan->getDevexExchangeId(devexId);


    if(dbMan->deleteDeviceCard(slotsId))
    {
        dbMan->insertLog(exchId, username, "Remove Card: "+card+" From Device: "+device+" Position: "+position);
        this->close();
    }
    else
        QMessageBox::warning(this, "ERROR", "Error in removing ports.\n"+dbMan->lastError());
}

void RemoveDeviceCardDialog::on_checkBox_toggled(bool checked)
{
    if(checked)
    {
        if(all)
            ui->removeAllCardsBtn->setEnabled(true);
        else
            ui->okBtn->setEnabled(true);
    }
    else
    {
        ui->okBtn->setEnabled(false);
        ui->removeAllCardsBtn->setEnabled(false);
    }

}

void RemoveDeviceCardDialog::on_removeAllCardsBtn_clicked()
{
    QString username = dbMan->getLoggedinNameLastname();
    QString device = ui->deviceLbl->text();
    int exchId = dbMan->getDevexExchangeId(devexId);

    if(dbMan->depleteDslamShelfSlots(devexId))
    {
        dbMan->insertLog(exchId, username, "Remove all DSLAM "+device+" cards");
        this->close();
    }
    else
        QMessageBox::warning(this, "ERROR", "Error in removing cards.\n"+dbMan->lastError());
}
