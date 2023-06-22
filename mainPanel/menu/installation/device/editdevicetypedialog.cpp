#include "editdevicetypedialog.h"
#include "ui_editdevicetypedialog.h"
#include "lib/database/danetdbman.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QDebug>

EditDeviceTypeDialog::EditDeviceTypeDialog(QWidget *parent, DanetDbMan *db, const int ExchId, const QString abbr, const int saloon, const int Row, const int Rack, const int Shelf,
                                           const int DevexId, const QString purchase, const int DeviceId, const QString DeviceName) :
    QDialog(parent),
    ui(new Ui::EditDeviceTypeDialog),
    dbMan(db),
    exchId(ExchId),
    devexId(DevexId),
    row(Row),
    rack(Rack),
    shelf(Shelf),
    deviceName(DeviceName),
    deviceId(DeviceId)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowMinMaxButtonsHint);

    ui->abbrLbl->setText(abbr);
    ui->saloonLbl->setText(dbMan->getSaloonName(saloon));

    QString rrs = QString::number(row)+'*'+QString::number(rack)+"*"+QString::number(shelf);
    ui->rrsLbl->setText(rrs);
    ui->deviceLbl->setText(deviceName);
    QSqlQuery *query = dbMan->selectPurchases();
    QString pur, purAbbr,PURCHASE;
    ui->purCB->blockSignals(true);
    while(query->next())
    {
        purAbbr = query->value(1).toString();
        pur = query->value(2).toString();
        if(purchase.compare(purAbbr, Qt::CaseInsensitive) == 0)
            PURCHASE = pur + "*"+purAbbr;

        ui->purCB->addItem(pur, purAbbr);
    }
    ui->purCB->setCurrentIndex(-1);
    ui->purCB->blockSignals(false);
    ui->purchaseLbl->setText(PURCHASE);

    int filledSlot = dbMan->getDeviceMaxFilledSlot(devexId);

    oldStartSlot = -1;
    QList<int> count_Stars = dbMan->getDeviceSlotCountStarts(deviceId);//slotCount/startShelf/startSlot/startSubslot
    if(count_Stars.count() == 4)
    {
        oldStartShelf = count_Stars.at(1);
        oldStartSlot = count_Stars.at(2);
        oldStartSubslot = count_Stars.at(3);
    }

    QString type = dbMan->getDeviceType(deviceId);

    if(deviceName.contains("S5300", Qt::CaseInsensitive))
        S5300Device = true;
    else
        S5300Device = false;

    ui->deviceCB->blockSignals(true);
    if(S5300Device)
        deviceMap = dbMan->selectDeviceMap(type, 1,-1, 1,0);
    else
        deviceMap = dbMan->selectDeviceMap(type, filledSlot,oldStartShelf, oldStartSlot,oldStartSubslot);

    foreach (int k, deviceMap.keys())
        if(k != deviceId)
            ui->deviceCB->addItem(deviceMap.value(k), k);
    ui->deviceCB->setCurrentIndex(-1);
    ui->deviceCB->blockSignals(false);

    if(dbMan->isDeviceDslam(deviceId))
    {
        int dslamType = dbMan->getDevexDslamType(devexId); // 4-> 5616
        if(dslamType == 4)
        {
            ui->errorLbl->setVisible(false);
        }
        else
        {
            ui->purCB->setEnabled(false);
            ui->deviceCB->setEnabled(false);
            ui->errorLbl->setVisible(true);
            ui->okBtn->setEnabled(false);
            ui->checkBox->setChecked(false);
            ui->checkBox->setEnabled(false);
        }
    }
    else
    {
        ui->purCB->setEnabled(true);
        ui->deviceCB->setEnabled(true);
        ui->errorLbl->setVisible(false);
        ui->okBtn->setEnabled(false);
        ui->checkBox->setChecked(false);
        ui->checkBox->setEnabled(true);
    }

    ui->checkBox->setChecked(false);
    ui->checkBox->setEnabled(false);
    ui->okBtn->setEnabled(false);

}

EditDeviceTypeDialog::~EditDeviceTypeDialog()
{
    delete ui;
}

void EditDeviceTypeDialog::on_purCB_currentTextChanged(const QString &arg1)
{
    ui->okBtn->setEnabled(false);
    if(arg1.isEmpty() || ui->deviceCB->currentText().isEmpty())
    {
        ui->checkBox->setChecked(false);
        ui->checkBox->setEnabled(false);
    }
    else
    {
        ui->checkBox->setChecked(false);
        ui->checkBox->setEnabled(true);
    }
}

void EditDeviceTypeDialog::on_deviceCB_currentIndexChanged(int index)
{
    index++;
    ui->okBtn->setEnabled(false);
    if(ui->purCB->currentText().isEmpty() || ui->deviceCB->currentText().isEmpty())
    {
        ui->checkBox->setChecked(false);
        ui->checkBox->setEnabled(false);
    }
    else
    {
        ui->checkBox->setChecked(false);
        ui->checkBox->setEnabled(true);
    }
}

void EditDeviceTypeDialog::on_cancelBtn_clicked()
{
    this->close();
}

void EditDeviceTypeDialog::on_okBtn_clicked()
{
    QString pur = ui->purCB->currentData().toString();
    int DeviceId = ui->deviceCB->currentData().toInt();
    QString oldDeviceName = dbMan->getDevexDeviceName(devexId);
    QString newDeviceName = dbMan->createDeviceName(exchId,row, rack,shelf,DeviceId);
    QString username = dbMan->getLoggedinNameLastname();

    int slotId = dbMan->getSlotId(devexId, 0);
    QString localDevice = dbMan->getDevice(DeviceId, true);

    if(S5300Device)
    {
        if(dbMan->updateDevex(devexId, DeviceId,pur, newDeviceName))
        {
            int cardId = dbMan->getcardId(DeviceId, "24*1G");

            if(!dbMan->updateSlots(slotId,-1, 1,0,cardId ))
            {
                QMessageBox::warning(this, "Error", "Can not update slot subslot number."+dbMan->lastError());
                return;
            }

            //decrease port one
            if(!dbMan->decreasePortOneUnit(slotId))
            {
                QMessageBox::warning(this, "Error", "Can not decrease interface port value one unit."+dbMan->lastError());
                return;
            }

            if(!dbMan->updateDevexInterfaces(devexId))
            {
                QMessageBox::warning(this, "Error", "Can not update slot interfaces"+dbMan->lastError());
                return;
            }

            if(!dbMan->updatePinLocalDevice(devexId,localDevice))
            {
                QMessageBox::warning(this, "Error", "Can not update ODF local device."+dbMan->lastError());
                return;
            }

            if(dbMan->updateLabel(oldDeviceName, newDeviceName))
            {
                //update DSLAM connections
                //Agg-Dslam
                //CX-DSLAM
                if(!dbMan->updateAggDslam(devexId))
                {
                    QMessageBox::warning(this, "Error", "Can not update Agg-DSLAM connections."+dbMan->lastError());
                    return;
                }

//                if(!dbMan->updateCxDslam(devexId))
//                {
//                    QMessageBox::warning(this, "Error", "Can not update CX-DSLAM connections."+dbMan->lastError());
//                    return;
//                }

                dbMan->insertLog(exchId, username, "Change Device: "+oldDeviceName+ " To Device: "+newDeviceName);
                this->close();
            }
            else
                QMessageBox::warning(this, "Error", "Can not update ports label"+dbMan->lastError());

        }
        else
            QMessageBox::warning(this, "Error", "Can not update device type"+dbMan->lastError());
    }
    else
    {
        QString oldPur = dbMan->getDevexPurchase(devexId);
        QString dslamType = dbMan->getDslamType(devexId);
        QString temp = dslamType+QString("-")+oldPur; //L-C
        if(dbMan->updateDevex(devexId, DeviceId,pur, newDeviceName))
        {
            if(dbMan->isDeviceDslam(DeviceId))
            {
                QString oldDevice = dbMan->getDevice(deviceId,false); //MA5616
                QString newDevice = dbMan->getDevice(DeviceId, false);//MA5616H
                QString temp2 = dslamType+QString("-")+pur;
                //empty cards
                //edit dslam agg
                QString desc = dbMan->getAggDslamDesc(devexId);
                desc = desc.replace(oldDevice, newDevice, Qt::CaseInsensitive);
                desc = desc.replace(temp, temp2, Qt::CaseInsensitive);
                if(!desc.isEmpty())
                    dbMan->updateAggDslamDesc(devexId,desc);

                //edit dslam metro //no connection
                //edit dslam ppoe
                dbMan->updateDslamPppoeDesc(devexId,oldDevice, newDevice, temp, temp2);
                //update interfaces

                QMessageBox::warning(this, "Info", "Don't forget to update DSLAM cards.");

                dbMan->insertLog(exchId, username, "Change Device: "+oldDeviceName+ " To Device: "+newDeviceName);
                this->close();
            }
            else
            {
                if(!dbMan->updatePinLocalDevice(devexId,localDevice))
                {
                    QMessageBox::warning(this, "Error", "Can not update ODF local device."+dbMan->lastError());
                    return;
                }

                if(dbMan->updateLabel(oldDeviceName, newDeviceName))
                {
                    //update DSLAM connections
                    //Agg-Dslam
                    //CX-DSLAM

                    if(!dbMan->updateAggDslam(devexId))
                    {
                        QMessageBox::warning(this, "Error", "Can not update Agg-DSLAM connections."+dbMan->lastError());
                        return;
                    }

                    if(!dbMan->updateCxDslam(devexId))
                    {
                        QMessageBox::warning(this, "Error", "Can not update CX-DSLAM connections."+dbMan->lastError());
                        return;
                    }

                    dbMan->insertLog(exchId, username, "Change Device: "+oldDeviceName+ " To Device: "+newDeviceName);
                    this->close();
                }
                else
                    QMessageBox::warning(this, "Error", "Can not update ports label"+dbMan->lastError());
            }

        }
        else
            QMessageBox::warning(this, "Error", "Can not update device type"+dbMan->lastError());
    }
}

void EditDeviceTypeDialog::on_checkBox_toggled(bool checked)
{
    if(checked)
        ui->okBtn->setEnabled(true);
    else
        ui->okBtn->setEnabled(false);
}

