#include "adddevicecarddialog.h"
#include "ui_adddevicecarddialog.h"
#include "lib/database/danetdbman.h"
#include "mainPanel/menu/installation/card/odfpinrequestdialog.h"
#include "mainPanel/menu/installation/card/portbyportodfrequestdialog.h"
#include <QMessageBox>
#include <QSqlQuery>


AddDeviceCardDialog::AddDeviceCardDialog(QWidget *parent, DanetDbMan *db, const int ExchId, const QString Abbr, const int DevexId, const QString Device, const bool AllSlots) :
    QDialog(parent),
    ui(new Ui::AddDeviceCardDialog),
    dbMan(db),
    devexId(DevexId),
    exchId(ExchId),
    device(Device),
    allSlots(AllSlots)
{
    ui->setupUi(this);

    ui->tabWidget->tabBar()->hide();
    ui->tabWidget->setCurrentIndex(0);

    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowMinMaxButtonsHint);

    ui->abbrLbl->setText(Abbr);
    ui->deviceLbl->setText(device);
    saloon = dbMan->getDevexSaloon(devexId);
    deviceId = dbMan->getDevexDeviceId(devexId);
    SR = dbMan->getDeviceSR(deviceId); //SR:true > port inserted on ODF too
    ui->eGB->setVisible(false);
    ui->gGB->setVisible(false);
    ui->g10GB->setVisible(false);
    ui->g40GB->setVisible(false);
    ui->g100GB->setVisible(false);

    ui->shelfChB->setChecked(false);
    ui->shelfSB->setVisible(false);
    ui->noshelfLbl->setVisible(true);

    ui->slotChB->setChecked(true);
    ui->slotCB->setVisible(true);
    ui->noSlotLbl->setVisible(false);

    ui->subSlotChB->setChecked(true);
    ui->subslotSB->setVisible(true);
    ui->noSubslotLbl->setVisible(false);


    ui->saloonLbl->setText(dbMan->getSaloonName(saloon));

    ui->portInfoSA->setVisible(false);
    ui->interfaceNameGB->setVisible(false);
    ui->odfGB->setVisible(false);

    bool DSLAM = dbMan->isDevexDslam(devexId);

    QSqlQuery *query = dbMan->selectPurchases();
    QString pur, purAbbr;
    while(query->next())
    {
        purAbbr = query->value(1).toString();
        pur = query->value(2).toString();
        ui->purCB->addItem(pur+"("+purAbbr+")", purAbbr);
    }
    ui->purCB->setCurrentIndex(2);

    //slot Count and initial value
    //in case no slot >> disable all
    QList<int> slotCountStarts = dbMan->getDeviceSlotCountStarts(deviceId);//count-shelf-slot-subslot
    QList<int> usedSlots;
    if(DSLAM)
        usedSlots = dbMan->getDevexUsedSlotList(devexId);
    if(slotCountStarts.count() == 4)
    {
        slotCount = slotCountStarts.at(0);
        startShelf = slotCountStarts.at(1);
        startSlot = slotCountStarts.at(2);
        startSubslot = slotCountStarts.at(3);

        for(int i = startSlot; i < (slotCount+startSlot); i++)
        {
            if(DSLAM)
            {
                if(!usedSlots.contains(i))
                    ui->slotCB->addItem(QString::number(i), i);
            }
            else
                ui->slotCB->addItem(QString::number(i), i);
        }

        if(startShelf == -1)
        {
            ui->shelfChB->setChecked(false);
            ui->shelfChB->setEnabled(false);
            ui->shelfSB->setVisible(false);
            ui->noshelfLbl->setVisible(true);
        }
        else
        {
            ui->shelfChB->setChecked(true);
            ui->shelfChB->setEnabled(true);
            ui->shelfSB->setVisible(true);
            ui->noshelfLbl->setVisible(false);
        }

        if(startSubslot == -1)
        {
            ui->subSlotChB->setChecked(false);
            ui->subSlotChB->setEnabled(false);
            ui->subslotSB->setVisible(false);
            ui->noSubslotLbl->setVisible(true);
        }

    }
    else
    {
        ui->cardIntGB->setEnabled(false);
        ui->interfaceNameGB->setEnabled(false);
        ui->portInfoSA->setEnabled(false);
        ui->odfGB->setEnabled(false);
        QMessageBox::warning(this,"Error","Cannot fetch device Information. Please try again later.");
        ui->okBtn->setEnabled(false);
        ui->fillShelfBtn->setEnabled(false);
    }

    if(DSLAM)
    {
        ui->okBtn->hide();
        ui->posNextBtn->hide();
        ui->posOkBtn->show();
    }
    else
    {
        ui->okBtn->show();
        ui->posNextBtn->show();
        ui->posOkBtn->hide();
        ui->fillShelfBtn->hide();
    }


    if(allSlots)
    {

        SR = false;
        ui->exchNextBtn->hide();
        ui->fillShelfBtn->show();

        ui->odfGB->setEnabled(false);
        ui->odfGB->setVisible(false);

        ui->pbpChB->setEnabled(false);
        ui->pbpChB->setVisible(false);
        pbpOdfAssign = false;

        ui->portInfoSA->setEnabled(false);
        ui->portInfoSA->setVisible(false);

        ui->interfaceNameGB->setEnabled(false);
        ui->interfaceNameGB->setVisible(false);

        if(dbMan->isDevexDslam(devexId))
        {
            this->setWindowTitle("Fill DSLAM");
            ui->fillShelfBtn->setEnabled(false);
            ui->fillShelfBtn->setVisible(true);

            ui->okBtn->setEnabled(false);

            cardMap = dbMan->getDeviceCards(deviceId);
            ui->cardCB->blockSignals(true);
            foreach (int key, cardMap.keys())
                ui->cardCB->addItem(cardMap.value(key),key);
            ui->cardCB->setCurrentIndex(-1);
            ui->cardCB->blockSignals(false);

        }
        else
        {
            ui->fillShelfBtn->setEnabled(false);
            ui->okBtn->setEnabled(false);
        }
    }
    else
    {
        if(SR)
        {
            ui->portInfoSA->setVisible(true);
            ui->interfaceNameGB->setVisible(true);
            ui->odfGB->setVisible(true);
            ui->eGB->setEnabled(false);
            ui->gGB->setEnabled(false);
            ui->g10GB->setEnabled(false);
            ui->g40GB->setEnabled(false);
            ui->g100GB->setEnabled(false);

            ui->pbpChB->setVisible(true);
            ui->odfGB->setEnabled(false);

            cardMap = dbMan->getDeviceCards(deviceId);
            ui->cardCB->blockSignals(true);
            foreach (int key, cardMap.keys())
                ui->cardCB->addItem(cardMap.value(key),key);
            ui->cardCB->setCurrentIndex(-1);
            ui->cardCB->blockSignals(false);

            ui->odfSaloonCB->blockSignals(true);
            ui->odfSaloonCB->clear();
            QList<QString> list = dbMan->getExistOdfSaloonNo(exchId);
            QSqlQuery *query = dbMan->selectSaloons(list);
            int saloonId;
            QString saloonStr;
            while(query->next())
            {
                saloonId = query->value(0).toInt();
                saloonStr = query->value(1).toString();
                ui->odfSaloonCB->addItem(saloonStr, saloonId);
            }

            ui->odfSaloonCB->setCurrentIndex(ui->odfSaloonCB->findData(saloon));
            ui->odfSaloonCB->blockSignals(false);


            if(SR && !allSlots)
            {
                odfMap = dbMan->getExchangeOdfMap(exchId, saloon);
                ui->odfCB->blockSignals(true);
                foreach (int key, odfMap.keys())
                    ui->odfCB->addItem(QString::number(key), odfMap.value(key));
                ui->odfCB->setCurrentIndex(-1);
                ui->odfCB->blockSignals(false);
            }
            else
            {
                ui->odfSaloonCB->blockSignals(true);
                ui->odfSaloonCB->clear();
                ui->odfSaloonCB->setCurrentIndex(-1);
                ui->odfSaloonCB->blockSignals(false);
                ui->odfSaloonCB->setEnabled(false);

                ui->odfGB->setEnabled(false);
                ui->odfGB->setVisible(false);

                ui->portInfoSA->setEnabled(false);
                ui->portInfoSA->setVisible(false);

            }

            ui->fillShelfBtn->setEnabled(false);
            ui->fillShelfBtn->setVisible(false);

            ui->okBtn->setEnabled(false);
            ui->okBtn->setVisible(true);

        }
        else
        {
            ui->odfGB->setEnabled(false);
            ui->odfGB->setVisible(false);
            ui->pbpChB->setEnabled(false);
            ui->pbpChB->setVisible(false);

            ui->portInfoSA->setEnabled(false);
            ui->portInfoSA->setVisible(false);

            ui->interfaceNameGB->setEnabled(true);
            ui->interfaceNameGB->setVisible(true);

            if(dbMan->isDevexDslam(devexId))
            {
                this->setWindowTitle("Add DSLAM New Card");
                ui->fillShelfBtn->setEnabled(false);
                ui->fillShelfBtn->setVisible(false);

                ui->okBtn->setEnabled(false);
                ui->okBtn->setVisible(true);

                ui->shelfChB->setChecked(true);
                ui->shelfSB->setVisible(true);
                ui->noshelfLbl->setVisible(false);

                ui->slotChB->setChecked(true);
                ui->slotCB->setVisible(true);
                ui->noSlotLbl->setVisible(false);

                ui->subSlotChB->setChecked(false);
                ui->subslotSB->setVisible(false);
                ui->noSubslotLbl->setVisible(true);

                cardMap = dbMan->getDeviceCards(deviceId);
                ui->cardCB->blockSignals(true);
                foreach (int key, cardMap.keys())
                    ui->cardCB->addItem(cardMap.value(key),key);
                ui->cardCB->setCurrentIndex(-1);
                ui->cardCB->blockSignals(false);

                QList<int> cardIdSlot = dbMan->getDslamLastCardSlotInfo(devexId);
                if(cardIdSlot.at(0) > -1)
                    ui->cardCB->setCurrentIndex(ui->cardCB->findData(cardIdSlot.at(0)));
                if( (cardIdSlot.at(1) > -1) && (ui->slotCB->findData(cardIdSlot.at(1)+1) > -1) )
                    ui->slotCB->setCurrentIndex(ui->slotCB->findData(cardIdSlot.at(1)+1));

            }
        }
    }

    odfNeeded = false;
    pbpOdfAssign = false;
    ui->pbpChB->setChecked(false);
    ui->pbpChB->setEnabled(false);
}

AddDeviceCardDialog::~AddDeviceCardDialog()
{
    delete ui;
}

void AddDeviceCardDialog::resetPortInfos()
{
    ui->eCountLbl->setText("");
    ui->eEndLbl->setText("");
    ui->eStartSB->setValue(0);

    ui->gCountLbl->setText("");
    ui->gEndLbl->setText("");
    ui->gStartSB->setValue(0);

    ui->g10CountLbl->setText("");
    ui->g10EndLbl->setText("");
    ui->g10StartSB->setValue(0);

    ui->g40CountLbl->setText("");
    ui->g40EndLbl->setText("");
    ui->g40StartSB->setValue(0);

    ui->g100CountLbl->setText("");
    ui->g100EndLbl->setText("");
    ui->g100StartSB->setValue(0);


}

bool AddDeviceCardDialog::insertPort(int Type, int PortStart, int PortEnd, QList<int> pinIdList, int &index)
{
    QString interfaceType,intType, label,interface;
    bool odfFlag = true;

    switch (Type)
    {
    case 0:
        interfaceType = "E";
        intType = ui->ethIntTypeCB->currentText();
        if(ui->eODFChB->isChecked())
            odfFlag = true;
        else
            odfFlag = false;
        break;
    case 1:
        interfaceType = "1G";
        intType = ui->gIntTypeCB->currentText();
        if(ui->g1ODFChB->isChecked())
            odfFlag = true;
        else
            odfFlag = false;
        break;
    case 2:
        interfaceType = "10G";
        intType = ui->g10IntTypeCB->currentText();
        if(ui->g10ODFChB->isChecked())
            odfFlag = true;
        else
            odfFlag = false;
        break;
    case 3:
        interfaceType = "40G";
        intType = ui->g40IntTypeCB->currentText();
        odfFlag = true;
        break;
    case 4:
        interfaceType = "100G";
        intType = ui->g100IntTypeCB->currentText();
        odfFlag = true;
        break;
    }

    while(PortStart <= PortEnd)
    {
        label = dbMan->createInterfaceLabel(device,interfaceType,shelf,slot,subSlot,PortStart);
        interface = dbMan->createInterface(shelf,slot,subSlot,PortStart);

        if(!odfFlag)//electrical
        {
            if(!dbMan->insertInterface(slotsId,intType, PortStart,-1,interface, label))
            {
                //                QMessageBox::warning(this, "ERROR","Can not insert port.\n"+dbMan->lastError());
                return false;
            }
        }
        else
        {
            if(!dbMan->insertInterface(slotsId,intType, PortStart, pinIdList[index],interface, label))
            {
                //QMessageBox::warning(this, "ERROR","Can not insert port.\n"+dbMan->lastError());
                return false;
            }
            index++;
        }

        PortStart++;
    }

    return true;
}

bool AddDeviceCardDialog::insertPort(int Type, int PortStart, int PortEnd, bool NoODF)
{
    if(NoODF == false) return false;

    QString interfaceType,intType, label,interface;
    switch (Type)
    {
    case 0:
        interfaceType = "E";
        intType = ui->ethIntTypeCB->currentText();
        break;
    case 1:
        interfaceType = "1G";
        intType = ui->gIntTypeCB->currentText();
        break;
    case 2:
        interfaceType = "10G";
        intType = ui->g10IntTypeCB->currentText();
        break;
    case 3:
        interfaceType = "40G";
        intType = ui->g40IntTypeCB->currentText();
        break;
    case 4:
        interfaceType = "100G";
        intType = ui->g100IntTypeCB->currentText();
        break;
    }

    while(PortStart <= PortEnd)
    {
        label = dbMan->createInterfaceLabel(device,interfaceType,shelf,slot,subSlot,PortStart);
        interface = dbMan->createInterface(shelf,slot,subSlot,PortStart);

        if(!dbMan->insertInterface(slotsId,intType, PortStart,-1,interface, label))
        {
            return false;
        }
        PortStart++;
    }

    return true;
}

bool AddDeviceCardDialog::insertPort(int Type, int port, int pinId)
{
    QString interfaceType,intType, label,interface;
    switch (Type)
    {
    case 0:
        interfaceType = "E";
        intType = ui->ethIntTypeCB->currentText();
        break;
    case 1:
        interfaceType = "1G";
        intType = ui->gIntTypeCB->currentText();
        break;
    case 2:
        interfaceType = "10G";
        intType = ui->g10IntTypeCB->currentText();
        break;
    case 3:
        interfaceType = "40G";
        intType = ui->g40IntTypeCB->currentText();
        break;
    case 4:
        interfaceType = "100G";
        intType = ui->g100IntTypeCB->currentText();
        break;
    }

    label = dbMan->createInterfaceLabel(device,interfaceType,shelf,slot,subSlot,port);
    interface = dbMan->createInterface(shelf,slot,subSlot,port);

    if(pinId < 0)//electrical
    {
        if(!dbMan->insertInterface(slotsId,intType, port,-1,interface, label))
            return false;
    }
    else
    {
        if(!dbMan->insertInterface(slotsId,intType, port, pinId,interface, label))
            return false;
    }

    return true;
}

void AddDeviceCardDialog::setPortNamingLable()
{
    bool Shelf = ui->shelfChB->isChecked();
    bool Slot = ui->slotChB->isChecked();
    bool Subslot = ui->subSlotChB->isChecked();

    int sh = ui->shelfSB->value();
    int sl = ui->slotCB->currentData().toInt();
    int su = ui->subslotSB->value();

    QString name="", hint = "Interface Sample(";//(Shelf / Slot / Subslot / Port)
    if(Shelf)
    {name = QString::number(sh) + " / ";hint += "Shelf / ";}
    if(Slot)
    {name += QString::number(sl) + " / ";hint += "Slot / ";}
    if(Subslot)
    {name += QString::number(su) + " / ";hint += "Subslot / ";}
    name += "Port";
    hint += "Port):";

    ui->wxyzLbl->setText(name);
    ui->wxyzHintLbl->setText(hint);
}

bool AddDeviceCardDialog::portTypeSkipedOdf(const int portType)
{
    bool skiped = false;

    switch (portType)
    {
    case 0:
        // "E";
        if( (!ui->eODFChB->isChecked()) || (ui->eODFChB->isChecked() && (ui->eSkipODFChB->isChecked())) )
            skiped = true;
        else
            skiped = false;
        break;
    case 1:
        // "1G";
        if( (!ui->g1ODFChB->isChecked()) || (ui->g1ODFChB->isChecked() && (ui->g1SkipODFChB->isChecked())) )
            skiped = true;
        else
            skiped = false;
        break;
    case 2:
        // "10G";
        if( (!ui->g10ODFChB->isChecked()) || (ui->g10ODFChB->isChecked() && (ui->g10SkipODFChB->isChecked())) )
            skiped = true;
        else
            skiped = false;
        break;
    case 3:
        // "40G";
        if( ui->g40SkipODFChB->isChecked() )
            skiped = true;
        else
            skiped = false;
        break;
    case 4:
        // "100G";
        if(ui->g100SkipODFChB->isChecked() )
            skiped = true;
        else
            skiped = false;
        break;
    }


    return skiped;
}

void AddDeviceCardDialog::on_cardCB_currentIndexChanged(int index)
{
    NoPort = false;
    ui->ethIntTypeCB->clear();
    ui->gIntTypeCB->clear();
    ui->g10IntTypeCB->clear();
    ui->g40IntTypeCB->clear();
    ui->g100IntTypeCB->clear();

    if(SR)
    {
        index++;
        ui->odfCB->blockSignals(true);
        ui->posCB->blockSignals(true);
        ui->pinCB->blockSignals(true);
        ui->posCB->setCurrentIndex(-1);
        ui->pinCB->setCurrentIndex(-1);
        ui->odfCB->setCurrentIndex(-1);
        ui->odfCB->blockSignals(false);
        ui->posCB->blockSignals(false);
        ui->pinCB->blockSignals(false);

        resetPortInfos();
        int val;
        int cardId = ui->cardCB->currentData().toInt();
        portCount.clear();
        portCount = dbMan->getCardPortInfo(cardId); // 0:E, 1:G, 2:G10, 3:G40, 4:G100
        int E, G, G10, G40, G100;
        E = portCount.value(0);
        G = portCount.value(1);
        G10 = portCount.value(2);
        G40 = portCount.value(3);
        G100 = portCount.value(4);
        //ethernet
        if(E > 0)
        {
            val = portCount.value(0);
            ui->eCountLbl->setText(QString::number(val));
            ui->eEndLbl->setText(QString::number(val-1));
            ui->eGB->setEnabled(true);
            ui->eGB->setVisible(true);
            //int type
            QMap<int, QString> typeMap;
            if(ui->eODFChB->isChecked())
                typeMap = dbMan->getInterfaceTypes(0.9,"FIBER", false);
            else
                typeMap = dbMan->getInterfaceTypes(0.9,"ELECTRICAL", false);
            foreach (int id, typeMap.keys())
            {
                ui->ethIntTypeCB->addItem(typeMap.value(id), id);
            }

        }
        else
        {
            ui->eGB->setEnabled(false);
            ui->eGB->setVisible(false);
        }
        //g
        if(G > 0)
        {
            val = portCount.value(1);
            ui->gCountLbl->setText(QString::number(portCount.value(1)));
            ui->gEndLbl->setText(QString::number(val-1));
            ui->gGB->setEnabled(true);
            ui->gGB->setVisible(true);
            //int type
            QMap<int, QString> typeMap;
            if(ui->g1ODFChB->isChecked())
                typeMap = dbMan->getInterfaceTypes(1,"FIBER", true);
            else
                typeMap = dbMan->getInterfaceTypes(1,"ELECTRICAL", true);

            foreach (int id, typeMap.keys())
            {
                ui->gIntTypeCB->addItem(typeMap.value(id), id);
            }
        }
        else
        {
            ui->gGB->setEnabled(false);
            ui->gGB->setVisible(false);
        }
        //g10
        if(G10 > 0)
        {
            val = portCount.value(2);
            ui->g10CountLbl->setText(QString::number(portCount.value(2)));
            ui->g10EndLbl->setText(QString::number(val-1));
            ui->g10GB->setEnabled(true);
            ui->g10GB->setVisible(true);
            //int type
            QMap<int, QString> typeMap;
            if(ui->g10ODFChB->isChecked())
                typeMap = dbMan->getInterfaceTypes(10,"FIBER", true);
            else
                typeMap = dbMan->getInterfaceTypes(10,"ELECTRICAL", true);

            foreach (int id, typeMap.keys())
            {
                ui->g10IntTypeCB->addItem(typeMap.value(id), id);
            }
        }
        else
        {
            ui->g10GB->setEnabled(false);
            ui->g10GB->setVisible(false);
        }
        //g40
        if(G40 > 0)
        {
            val = portCount.value(3);
            ui->g40CountLbl->setText(QString::number(portCount.value(3)));
            ui->g40EndLbl->setText(QString::number(val-1));
            ui->g40GB->setEnabled(true);
            ui->g40GB->setVisible(true);
            //int type
            QMap<int, QString> typeMap = dbMan->getInterfaceTypes(40,"FIBER", true);
            foreach (int id, typeMap.keys())
            {
                ui->g40IntTypeCB->addItem(typeMap.value(id), id);
            }
        }
        else
        {
            ui->g40GB->setEnabled(false);
            ui->g40GB->setVisible(false);
        }
        //g100
        if(G100 > 0)
        {
            val = portCount.value(4);
            ui->g100CountLbl->setText(QString::number(portCount.value(4)));
            ui->g100EndLbl->setText(QString::number(val-1));
            ui->g100GB->setEnabled(true);
            ui->g100GB->setVisible(true);
            //int type
            QMap<int, QString> typeMap = dbMan->getInterfaceTypes(100,"FIBER", true);
            foreach (int id, typeMap.keys())
            {
                ui->g100IntTypeCB->addItem(typeMap.value(id), id);
            }
        }
        else
        {
            ui->g100GB->setEnabled(false);
            ui->g100GB->setVisible(false);
        }

        ///////////////////////////
        int totalFiberPort = fiberPortCount();
        int totalElectricalPort = electricalPortCount();

        if((totalFiberPort == 0) && (totalElectricalPort == 0))
            NoPort = true;
        else
            NoPort = false;

        if( totalFiberPort > 0)
        {
            odfNeeded = true;
            ui->pbpChB->setEnabled(true);
            ui->pbpChB->setVisible(true);
            ui->odfGB->setEnabled(true);
            ui->odfGB->setVisible(true);
            ui->portInfoSA->setVisible(true);

            ui->posCB->setEnabled(false);
            ui->pinCB->setEnabled(false);
            ui->okBtn->setEnabled(false);
            //this->resize(800, 700);
        }
        else if(totalElectricalPort > 0)
        {
            odfNeeded = false;
            ui->pbpChB->setEnabled(false);
            ui->pbpChB->setVisible(false);
            ui->odfGB->setEnabled(false);
            ui->odfGB->setVisible(false);

            ui->portInfoSA->setVisible(true);
            ui->okBtn->setEnabled(true);
            //this->resize(800, 700);
        }
        else if( NoPort)
        {
            //no port, like control card
            odfNeeded = false;
            ui->pbpChB->setEnabled(false);
            ui->pbpChB->setVisible(false);
            ui->odfGB->setEnabled(false);
            ui->odfGB->setVisible(false);
            ui->portInfoSA->setVisible(false);
            ui->okBtn->setEnabled(true);
            //this->resize(800, 500);
        }

        if(NoPort)
        {
            ui->posOkBtn->show();
            ui->posNextBtn->hide();
        }
        else
        {
            ui->posOkBtn->hide();
            ui->posNextBtn->show();
        }

        if(odfNeeded)
        {
            ui->portNextBtn->show();
            ui->portOkBtn->hide();
        }
        else
        {
            ui->portOkBtn->show();
            ui->portNextBtn->hide();
        }

    }
    else
    {
        ui->pbpChB->setEnabled(false);
        ui->pbpChB->setVisible(false);

        if(allSlots)
        {
            ui->okBtn->setEnabled(false);
            ui->fillShelfBtn->setEnabled(true);
        }
        else
        {
            ui->okBtn->setEnabled(true);
            ui->fillShelfBtn->setEnabled(false);
        }

    }

    if(device.contains("S5300", Qt::CaseInsensitive))
        ui->gStartSB->setValue(1);
    else
        ui->gStartSB->setValue(0);

}

void AddDeviceCardDialog::on_odfCB_currentIndexChanged(int index)
{
    index++;
    ui->posCB->setEnabled(true);
    ui->pinCB->setEnabled(false);
    ui->okBtn->setEnabled(false);
    int odfId = ui->odfCB->currentData().toInt();
    posMap.clear();
    posMap = dbMan->getOdfPosMap(odfId);
    ui->posCB->blockSignals(true);
    ui->posCB->clear();
    ui->pinCB->clear();
    foreach (int key, posMap.keys())
        ui->posCB->addItem(QString::number(key), posMap.value(key));
    ui->posCB->setCurrentIndex(-1);
    ui->posCB->blockSignals(false);
}

void AddDeviceCardDialog::on_posCB_currentIndexChanged(int index)
{
    index++;
    int posId = ui->posCB->currentData().toInt();
    ui->pinCB->clear();
    ui->pinCB->setEnabled(true);
    ui->okBtn->setEnabled(false);
    QSqlQuery *query = dbMan->selectEmptyPin(posId);
    //`id`,`pin_no`
    QString pin;
    int pinId;
    ui->pinCB->blockSignals(true);
    while(query->next())
    {
        pinId = query->value(0).toInt();
        pin = query->value(1).toString();
        ui->pinCB->addItem(pin, pinId);
    }
    ui->pinCB->setCurrentIndex(-1);
    ui->pinCB->blockSignals(false);

}

void AddDeviceCardDialog::on_pinCB_currentIndexChanged(int index)
{
    index++;
    if(!ui->cardCB->currentText().isEmpty())
        ui->okBtn->setEnabled(true);
    else
        ui->okBtn->setEnabled(false);
}

void AddDeviceCardDialog::confirmBtn()
{
    bool done = false;

    if(ui->purCB->currentText().isEmpty())
    {
        QMessageBox::warning(this, "ERROR", "No purchase parameter is provided.");
        return;
    }
    int E, G, G10, G40, G100;
    E = portCount.value(0);
    G = portCount.value(1);
    G10 = portCount.value(2);
    G40 = portCount.value(3);
    G100 = portCount.value(4);
    if((E>0) && ui->ethIntTypeCB->currentText().isEmpty())
    {
        QMessageBox::warning(this, "Parameter Error", "No 'Interface Type' of Ethernet port is provided.");
        return;
    }

    if((G>0) && ui->gIntTypeCB->currentText().isEmpty())
    {
        QMessageBox::warning(this, "Parameter Error", "No 'Interface Type' of 1G port is provided.");
        return;
    }

    if((G10>0) && ui->g10IntTypeCB->currentText().isEmpty())
    {
        QMessageBox::warning(this, "ERROR", "No 'Interface Type' of 10G port is provided.");
        return;
    }

    if((G40>0) && ui->g40IntTypeCB->currentText().isEmpty())
    {
        QMessageBox::warning(this, "ERROR", "No 'Interface Type' of 40G port is provided.");
        return;
    }

    if((G100>0) && ui->g100IntTypeCB->currentText().isEmpty())
    {
        QMessageBox::warning(this, "ERROR", "No 'Interface Type' of G100 port is provided.");
        return;
    }


    if((fiberPortCount() > 0) && (!ui->pbpChB->isChecked()) )
    {
        if(ui->pinCB->currentText().isEmpty())
        {
            QMessageBox::warning(this, "Parameter Error", "ODF prameters is not provided.");
            return;
        }
    }

    //check if shelf/slot/subslot is empty or not
    QString tmp;
    if(ui->shelfChB->isChecked())
        shelf = ui->shelfSB->value();
    else
        shelf = -1;

    if(ui->slotChB->isChecked())
        slot = ui->slotCB->currentData().toInt();
    else
        slot = -1;

    if(ui->subSlotChB->isChecked())
        subSlot = ui->subslotSB->value();
    else
        subSlot = -1;

    bool EXIST_CARD = dbMan->slotExistance(devexId, shelf, slot, subSlot);

    if(EXIST_CARD)
    {
        if(dbMan->isDummySlot(devexId, shelf, slot, subSlot)) // EMPTY card
        {
            int sid = dbMan->getDevexSlotId(devexId, shelf, slot, subSlot);
            if(dbMan->deleteDeviceCard(sid))
                EXIST_CARD = false;
        }
    }

    if(EXIST_CARD)
    {
        if(shelf > -1)
            tmp = "Shelf: "+QString::number(shelf)+ " And";
        if(slot > -1)
            tmp += " Slot: "+QString::number(slot);
        if(subSlot > -1)
            tmp += " And Sub-Slot: "+QString::number(subSlot);
        QMessageBox::warning(this, "SLOT ERROR","Card exists in position: "+tmp);
    }
    else
    {
        QString purchase = ui->purCB->currentData().toString();
        QString desc = ui->descLE->text();

        QString username = dbMan->getLoggedinNameLastname();
        QString card = ui->cardCB->currentText();
        QString Position;
        if(shelf > -1)
            Position = QString::number(shelf)+"/";
        Position += QString::number(slot)+"/";
        if(subSlot > -1)
            Position += QString::number(subSlot);

        int cardId = ui->cardCB->currentData().toInt();

        slotsId = dbMan->getMaxSlotId();

        if(slotsId > -1)
        {
            if(dbMan->insertSlot(slotsId, devexId,shelf,slot,subSlot, cardId, purchase, desc))
            {
                //log
                dbMan->insertLog(exchId, username,"New Card: "+card+" To Device: "+device + " In Position: "+Position);

                done = true;
            }
            else
            {
                done = false;
                QMessageBox::warning(this, "SLOT ERROR", "There is error in slot insertion.\n"+dbMan->lastError());
            }
        }
        else
        {
            done = false;
            QMessageBox::warning(this, "SLOT ERROR", "Try again later.");
        }

        if(done && !SR)
        {
            this->close();
        }
        else if(SR && done)
        {
            if(NoPort)
            {
                this->close();
            }
            else
            {
                QMap<int, int> typeStart; //qmap is always sorted by key / same key > overwrite
                int val;
                if(G100 > 0)
                {
                    val = ui->g100StartSB->value();
                    typeStart.insert(4,val);
                }
                if(G40 > 0)
                {
                    val = ui->g40StartSB->value();
                    typeStart.insert(3,val);
                }
                if(G10 > 0)
                {
                    val = ui->g10StartSB->value();
                    typeStart.insert(2,val);
                }
                if(G > 0)
                {
                    val = ui->gStartSB->value();
                    typeStart.insert(1,val);
                }
                if(E > 0)
                {
                    val = ui->eStartSB->value();
                    typeStart.insert(0,val);
                }

                int totalFiberPort = fiberPortCount();
                //int totalElectricalPort = electricalPortCount();

                // pbp or consequently insertion
                if(pbpOdfAssign )
                {
                    QList<int> G100PinList, G40PinList,G10PinList,GPinList,EPinList, totalPinIdList; // port, pinId map
                    int startPort, endPort, portType, cnt,pId;
                    bool ok = true;
                    bool END_PORT;
                    QString itf, itfTemp;

                    if((G100 > 0) && (!ui->g100SkipODFChB->isChecked()))
                    {

                        portType = 4;
                        startPort = typeStart.value(portType);
                        cnt = portCount.value(portType);
                        endPort = cnt + startPort - 1;
                        itf = "100G ";
                        if(shelf > -1)
                            itf += QString::number(shelf)+"/";
                        itf += QString::number(slot)+"/";
                        if(subSlot > -1)
                            itf += QString::number(subSlot);

                        for(int i=startPort; i <= endPort;i++)
                        {
                            if(!ok) break;

                            itfTemp = itf +"/"+QString::number(i);
                            if(i == endPort)
                                END_PORT = true;
                            else
                                END_PORT = false;
                            PortByPortOdfRequestDialog portByPortOdfRequestDialog(this, dbMan,exchId, ui->abbrLbl->text(), saloon,deviceId, device,itfTemp, END_PORT,totalPinIdList);
                            portByPortOdfRequestDialog.exec();
                            pId = portByPortOdfRequestDialog.getPinId();
                            if(pId == -1)
                                ok = false;
                            else
                            {
                                G100PinList << pId;
                                totalPinIdList << pId;
                            }
                        }
                    }

                    if((G40 > 0) && (!ui->g40SkipODFChB->isChecked()))
                    {
                        portType = 3;
                        startPort = typeStart.value(portType);
                        cnt = portCount.value(portType);
                        endPort = cnt + startPort - 1;
                        itf = "40G ";
                        if(shelf > -1)
                            itf += QString::number(shelf)+"/";
                        itf += QString::number(slot)+"/";
                        if(subSlot > -1)
                            itf += QString::number(subSlot);

                        for(int i=startPort; i <= endPort;i++)
                        {
                            if(!ok) break;

                            itfTemp = itf + "/"+QString::number(i);
                            if(i == endPort)
                                END_PORT = true;
                            else
                                END_PORT = false;
                            PortByPortOdfRequestDialog portByPortOdfRequestDialog(this, dbMan,exchId, ui->abbrLbl->text(), saloon,deviceId, device,itfTemp, END_PORT,totalPinIdList);
                            portByPortOdfRequestDialog.exec();
                            pId = portByPortOdfRequestDialog.getPinId();
                            if(pId == -1)
                                ok = false;
                            else
                            {
                                G40PinList << pId;
                                totalPinIdList << pId;
                            }
                        }

                    }

                    if( (ui->g10ODFChB->isChecked()) && (!ui->g10SkipODFChB->isChecked()) && (G10 > 0) )
                    {
                        portType = 2;
                        startPort = typeStart.value(portType);
                        cnt = portCount.value(portType);
                        endPort = cnt + startPort - 1;
                        itf = "10G ";
                        if(shelf > -1)
                            itf += QString::number(shelf)+"/";
                        itf += QString::number(slot)+"/";
                        if(subSlot > -1)
                            itf += QString::number(subSlot);

                        for(int i=startPort; i <= endPort;i++)
                        {
                            if(!ok) break;

                            itfTemp = itf + "/"+QString::number(i);
                            if(i == endPort)
                                END_PORT = true;
                            else
                                END_PORT = false;
                            PortByPortOdfRequestDialog portByPortOdfRequestDialog(this, dbMan,exchId, ui->abbrLbl->text(), saloon,deviceId, device,itfTemp, END_PORT,totalPinIdList);
                            portByPortOdfRequestDialog.exec();
                            pId = portByPortOdfRequestDialog.getPinId();
                            if(pId == -1)
                                ok = false;
                            else
                            {
                                G10PinList << pId;
                                totalPinIdList << pId;
                            }
                        }
                    }

                    if( (ui->g1ODFChB->isChecked()) && (!ui->g1SkipODFChB->isChecked()) && (G > 0) )
                    {
                        portType = 1;
                        startPort = typeStart.value(portType);
                        cnt = portCount.value(portType);
                        endPort = cnt + startPort - 1;
                        itf = "1G ";
                        if(shelf > -1)
                            itf += QString::number(shelf)+"/";
                        itf += QString::number(slot)+"/";
                        if(subSlot > -1)
                            itf += QString::number(subSlot);

                        for(int i=startPort; i <= endPort;i++)
                        {
                            if(!ok) break;

                            itfTemp = itf + "/"+QString::number(i);
                            if(i == endPort)
                                END_PORT = true;
                            else
                                END_PORT = false;
                            PortByPortOdfRequestDialog portByPortOdfRequestDialog(this, dbMan,exchId, ui->abbrLbl->text(), saloon,deviceId, device,itfTemp, END_PORT,totalPinIdList);
                            portByPortOdfRequestDialog.exec();
                            pId = portByPortOdfRequestDialog.getPinId();
                            if(pId == -1)
                                ok = false;
                            else
                            {
                                GPinList << pId;
                                totalPinIdList << pId;
                            }
                        }
                    }

                    if( (ui->eODFChB->isChecked()) && (!ui->eSkipODFChB->isChecked()) && (E > 0) )
                    {
                        portType = 0;
                        startPort = typeStart.value(portType);
                        cnt = portCount.value(portType);
                        endPort = cnt + startPort - 1;
                        itf = "E ";
                        if(shelf > -1)
                            itf += QString::number(shelf)+"/";
                        itf += QString::number(slot)+"/";
                        if(subSlot > -1)
                            itf += QString::number(subSlot);

                        for(int i=startPort; i <= endPort;i++)
                        {
                            if(!ok) break;

                            itfTemp = itf + "/"+QString::number(i);
                            if(i == endPort)
                                END_PORT = true;
                            else
                                END_PORT = false;
                            PortByPortOdfRequestDialog portByPortOdfRequestDialog(this, dbMan,exchId, ui->abbrLbl->text(), saloon,deviceId, device,itfTemp, END_PORT,totalPinIdList);
                            portByPortOdfRequestDialog.exec();
                            pId = portByPortOdfRequestDialog.getPinId();
                            if(pId == -1)
                                ok = false;
                            else
                            {
                                EPinList << pId;
                                totalPinIdList << pId;
                            }
                        }
                    }
                    //port insertion
                    //100G
                    if((G100 > 0) && ok)
                    {
                        portType = 4;
                        startPort = typeStart.value(portType);
                        cnt = portCount.value(portType);
                        endPort = cnt + startPort - 1;
                        index = 0;

                        if(!ui->g100SkipODFChB->isChecked())
                        {
                            //check counts
                            if(G100PinList.count() != cnt )
                                ok = false;
                            else
                            {
                                for(int i=startPort; i <= endPort;i++)
                                {
                                    if(!ok) break;
                                    if(! insertPort(portType,i, G100PinList[index]))
                                    {
                                        ok = false;
                                        break;
                                    }
                                    index++;
                                }
                            }
                        }
                        else
                        {
                            for(int i=startPort; i <= endPort;i++)
                            {
                                if(!ok) break;
                                if(! insertPort(portType,i, -1))
                                {
                                    ok = false;
                                    break;
                                }
                                index++;
                            }
                        }


                    }

                    if((G40 > 0) && ok)
                    {
                        //40G
                        portType = 3;
                        startPort = typeStart.value(portType);
                        cnt = portCount.value(portType);
                        endPort = cnt + startPort - 1;
                        index = 0;
                        if(!ui->g40SkipODFChB->isChecked())
                        {
                            //check counts
                            if(G40PinList.count() != cnt )
                                ok = false;
                            else
                            {
                               for(int i=startPort; i <= endPort;i++)
                                {
                                    if(!ok) break;
                                    if(! insertPort(portType,i, G40PinList[index]))
                                    {
                                        ok = false;
                                        break;
                                    }
                                    index++;
                                }
                            }
                        }
                        else
                        {
                            for(int i=startPort; i <= endPort;i++)
                             {
                                 if(!ok) break;
                                 if(! insertPort(portType,i, -1))
                                 {
                                     ok = false;
                                     break;
                                 }
                                 index++;
                             }
                        }
                    }

                    if( (G10 > 0) && ok)
                    {
                        //10G
                        portType = 2;
                        startPort = typeStart.value(portType);
                        cnt = portCount.value(portType);

                        endPort = cnt + startPort - 1;
                        index = 0;
                        if(ui->g10ODFChB->isChecked() && (!ui->g10SkipODFChB->isChecked()))
                        {
                            //check counts
                            if(G10PinList.count() != cnt )
                                ok = false;
                            else
                                for(int i=startPort; i <= endPort;i++)
                            {
                                if(!ok) break;
                                if(! insertPort(portType,i, G10PinList[index]))
                                {
                                    ok = false;
                                    break;
                                }
                                index++;
                            }
                        }
                        else
                        {
                            for(int i=startPort; i <= endPort;i++)
                            {
                                if(!ok) break;
                                if(! insertPort(portType,i, -1))
                                {
                                    ok = false;
                                    break;
                                }
                            }
                        }

                    }

                    if( (G > 0) && ok )
                    {
                        //1G
                        portType = 1;
                        startPort = typeStart.value(portType);
                        cnt = portCount.value(portType);


                        endPort = cnt + startPort - 1;
                        index = 0;

                        if((ui->g1ODFChB->isChecked()) && (!ui->g1SkipODFChB->isChecked()))
                        {
                            //check counts
                            if(GPinList.count() != cnt )
                                ok = false;
                            else
                                for(int i=startPort; i <= endPort;i++)
                                {
                                    if(!ok) break;
                                    if(! insertPort(portType,i, GPinList[index]))
                                    {
                                        ok = false;
                                        break;
                                    }
                                    index++;
                                }
                        }
                        else
                        {
                            for(int i=startPort; i <= endPort;i++)
                            {
                                if(!ok) break;
                                if(! insertPort(portType,i, -1))
                                {
                                    ok = false;
                                    break;
                                }
                            }
                        }

                    }

                    if( (E > 0) && ok)
                    {
                        //E
                        portType = 0;
                        startPort = typeStart.value(portType);
                        cnt = portCount.value(portType);
                        endPort = cnt + startPort - 1;
                        //..
                        index = 0;

                        if((ui->eODFChB->isChecked()) && (!ui->eSkipODFChB->isChecked()))
                        {
                            //check counts
                            if(EPinList.count() != cnt )
                                ok = false;
                            else
                                for(int i=startPort; i <= endPort;i++)
                            {
                                if(!ok) break;
                                if(! insertPort(portType,i, EPinList[index]))
                                {
                                    ok = false;
                                    break;
                                }
                                index++;
                            }
                        }
                        else
                        {
                            for(int i=startPort; i <= endPort;i++)
                            {
                                if(!ok) break;
                                if(! insertPort(portType,i, -1))
                                {
                                    ok = false;
                                    break;
                                }
                            }
                        }
                    }

                    if(ok)
                        this->close();
                    else
                    {
                        //delete interfaces and slot
                        dbMan->deleteDeviceCard(slotsId);
                        QMessageBox::warning(this, "SLOT ERROR", "There is error in interface insertion.\n"+dbMan->lastError());
                        return;
                    }
                }
                else
                {
                    if(totalFiberPort > 0)
                    {
                        saloon = ui->odfSaloonCB->currentData().toInt();
                        int startOdfNo, startPosNo,ppp;
                        QList<int> pinIdList;
                        startOdfNo = ui->odfCB->currentText().toInt();
                        startPosNo = ui->posCB->currentText().toInt();
                        int pinId = ui->pinCB->currentData().toInt();

                        ppp = dbMan->getOdfPinPerPos(ui->odfCB->currentData().toInt());
                        if(ppp == -1 && ui->odfCB->isEnabled())
                        {
                            QMessageBox::warning(this, "ODF ERROR", "Check Odf No.");
                            return;
                        }

                        done = true;


                        //pinIdList = dbMan->getInsertPinId(exchId, saloon, startOdfNo,ppp,startPosNo, totalFiberPort);
                        pinIdList = dbMan->getInsertPinId(exchId, saloon, startOdfNo,ppp,startPosNo,pinId, totalFiberPort);

                        int newOdfNo,newPpp,newPosNo,newPinId;
                        while((pinIdList[0] == -1) || (pinIdList.count() != totalFiberPort) ) // not enough space in odf
                        {
                            OdfPinRequestDialog req(this,dbMan,exchId,pinIdList, ui->abbrLbl->text(), saloon);
                            req.exec();
                            newOdfNo = req.getOdfNo();
                            newPpp = req.getPpp();
                            newPosNo = req.getPosNo();
                            newPinId = req.getPinId();
                            if((newPinId == -1) || (newOdfNo == -1) || (newPpp == -1) || (newPosNo == -1) )
                            {
                                QMessageBox::warning(this, "ODF ERROR", "There is not enough space in ODF.");
                                dbMan->deleteSlot(slotsId);
                                done = false;
                                break;
                            }
                            //add next pinIds
                            pinIdList = dbMan->getInsertPinId(exchId,saloon,newOdfNo,newPpp,newPosNo,newPinId,totalFiberPort,pinIdList);
                        }

                        if(!done)
                            return;



                        QMapIterator<int, int> i(typeStart);
                        int startPort, endPort, portType, cnt;
                        index = 0;
                        while(done && (i.hasNext()) )
                        {
                            i.next();
                            portType = i.key();
                            startPort = i.value();
                            cnt = portCount.value(portType);
                            endPort = cnt + startPort - 1;

                            //port type fiber to odf count
                            //port type skiped
                            if(portTypeSkipedOdf(portType))
                            {
                                // no pin id
                                if(! insertPort(portType,startPort,endPort,true))
                                {
                                    done = false;
                                    break;
                                }
                            }
                            else
                            {
                                //link to odf also
                                if(! insertPort(portType,startPort,endPort, pinIdList,index))
                                {
                                    done = false;
                                    break;
                                }
                            }
                        }

                    }
                    else
                    {
                        QMapIterator<int, int> i(typeStart);
                        int startPort, endPort, portType, cnt;
                        while(done && (i.hasNext()) )
                        {
                            i.next();
                            portType = i.key();
                            startPort = i.value();
                            cnt = portCount.value(portType);
                            endPort = cnt + startPort - 1;
                            if(! insertPort(portType,startPort,endPort,true))
                            {
                                done = false;
                                break;
                            }
                        }
                    }


                    if(done)
                        this->close();
                    else
                    {
                        //delete interfaces and slot
                        dbMan->deleteDeviceCard(slotsId);
                        QMessageBox::warning(this, "SLOT ERROR", "There is error in interface insertion.\n"+dbMan->lastError());
                    }

                }
            }
        }
    }
}

void AddDeviceCardDialog::on_okBtn_clicked()
{
    confirmBtn();
}

void AddDeviceCardDialog::on_eStartSB_valueChanged(int arg1)
{
    ui->eEndLbl->setText(QString::number(arg1+portCount.value(0)-1));
}

void AddDeviceCardDialog::on_gStartSB_valueChanged(int arg1)
{
    ui->gEndLbl->setText(QString::number(arg1+portCount.value(1)-1));

}

void AddDeviceCardDialog::on_g10StartSB_valueChanged(int arg1)
{
    ui->g10EndLbl->setText(QString::number(arg1+portCount.value(2)-1));

}

void AddDeviceCardDialog::on_g40StartSB_valueChanged(int arg1)
{
    ui->g40EndLbl->setText(QString::number(arg1+portCount.value(3)-1));

}

void AddDeviceCardDialog::on_g100StartSB_valueChanged(int arg1)
{
    ui->g100EndLbl->setText(QString::number(arg1+portCount.value(4)-1));

}

void AddDeviceCardDialog::on_fillShelfBtn_clicked()
{
    QString purchase = ui->purCB->currentData().toString();
    QString desc = ui->descLE->text();
    int cardId = ui->cardCB->currentData().toInt();

    QString username = dbMan->getLoggedinNameLastname();
    QString card = ui->cardCB->currentText();

    shelf = 0;

    if(dbMan->fillDslamSlots(devexId,shelf, slotCount, startSlot, cardId, purchase, desc))
    {
        //log
        dbMan->insertLog(exchId,username, "Fill DSLAM: "+device+" With Card: "+card);

        this->close();
    }
    else
        QMessageBox::warning(this, "SLOT ERROR", "There is error in slot update.\n"+dbMan->lastError());

}

void AddDeviceCardDialog::on_odfSaloonCB_currentIndexChanged(int index)
{
    index++;
    int s = ui->odfSaloonCB->currentData().toInt();

    odfMap = dbMan->getExchangeOdfMap(exchId, s);
    ui->okBtn->setEnabled(false);
    ui->odfCB->blockSignals(true);
    ui->odfCB->clear();
    ui->posCB->clear();
    ui->pinCB->clear();
    foreach (int key, odfMap.keys())
        ui->odfCB->addItem(QString::number(key), odfMap.value(key));
    ui->odfCB->setCurrentIndex(-1);
    ui->odfCB->blockSignals(false);
}

void AddDeviceCardDialog::on_shelfChB_toggled(bool checked)
{
    if(checked)
    {
        ui->shelfSB->setEnabled(true);
        ui->shelfSB->setVisible(true);
        ui->noshelfLbl->setVisible(false);
    }
    else
    {
        ui->shelfSB->setEnabled(false);
        ui->shelfSB->setVisible(false);
        ui->noshelfLbl->setVisible(true);
    }

    setPortNamingLable();
}

void AddDeviceCardDialog::on_slotChB_toggled(bool checked)
{
    int c = ui->slotCB->count();
    if(c > 1)
    {
        ui->slotChB->setChecked(true);
        ui->slotCB->setEnabled(true);
        ui->slotCB->setVisible(true);
        ui->noSlotLbl->setVisible(false);
    }
    else
    {
        if(checked)
        {
            ui->slotCB->setEnabled(true);
            ui->slotCB->setVisible(true);
            ui->noSlotLbl->setVisible(false);
        }
        else
        {
            ui->slotCB->setEnabled(false);
            ui->slotCB->setVisible(false);
            ui->noSlotLbl->setVisible(true);
        }
    }


    setPortNamingLable();
}

void AddDeviceCardDialog::on_subSlotChB_toggled(bool checked)
{
    if(checked)
    {
        ui->subslotSB->setEnabled(true);
        ui->subslotSB->setVisible(true);
        ui->noSubslotLbl->setVisible(false);
    }
    else
    {
        ui->subslotSB->setEnabled(false);
        ui->subslotSB->setVisible(false);
        ui->noSubslotLbl->setVisible(true);
    }

    setPortNamingLable();
}

void AddDeviceCardDialog::on_shelfSB_valueChanged(int arg1)
{
    arg1++;
    setPortNamingLable();
}

void AddDeviceCardDialog::on_slotCB_currentIndexChanged(int index)
{
    index++;
    setPortNamingLable();
}

void AddDeviceCardDialog::on_subslotSB_valueChanged(int arg1)
{
    arg1++;
    setPortNamingLable();
}

int AddDeviceCardDialog::fiberPortCount()
{
    // port count 0:E, 1:G, 2:G10, 3:G40, 4:G100
    int E, G, G10, G40, G100;
    E = portCount.value(0);
    G = portCount.value(1);
    G10 = portCount.value(2);
    G40 = portCount.value(3);
    G100 = portCount.value(4);

    // ports needed to be insert in odf
    int totalFiberPort =  0;
    if(ui->eODFChB->isChecked())
    {
        if(!ui->eSkipODFChB->isChecked())
            totalFiberPort += E;
    }
    if(ui->g1ODFChB->isChecked())
    {
        if(!ui->g1SkipODFChB->isChecked())
            totalFiberPort += G;
    }
    if(ui->g10ODFChB->isChecked())
    {
        if(!ui->g10SkipODFChB->isChecked())
            totalFiberPort += G10;
    }

    if(!ui->g40SkipODFChB->isChecked())
        totalFiberPort += G40;
    if(!ui->g100SkipODFChB->isChecked())
        totalFiberPort += G100;


    return totalFiberPort;
}

int AddDeviceCardDialog::electricalPortCount()
{
    // port count 0:E, 1:G, 2:G10
    int E, G, G10;
    E = portCount.value(0);
    G = portCount.value(1);
    G10 = portCount.value(2);

    int totalElecPort = 0;
    if(!ui->eODFChB->isChecked())
        totalElecPort = E;
    if(!ui->g1ODFChB->isChecked())
        totalElecPort += G;
    if(!ui->g10ODFChB->isChecked())
        totalElecPort += G10;

    return totalElecPort;
}

void AddDeviceCardDialog::on_pbpChB_toggled(bool checked)
{
    if(checked)
    {
        pbpOdfAssign = true;
        ui->odfGB->setEnabled(false);
        ui->odfGB->setVisible(false);

        if(ui->cardCB->currentText().isEmpty())
            ui->okBtn->setEnabled(false);
        else
            ui->okBtn->setEnabled(true);
    }
    else
    {
        pbpOdfAssign = false;
        ui->odfGB->setEnabled(true);
        ui->odfGB->setVisible(true);
        //ui->scrollArea->verticalScrollBar()->setValue(ui->scrollArea->verticalScrollBar()->maximum());
        if(odfNeeded)
        {
            ui->odfGB->setEnabled(true);
            ui->odfGB->setVisible(true);
        }
        else
        {
            ui->odfGB->setEnabled(false);
        }

        if(ui->pinCB->currentText().isEmpty())
            ui->okBtn->setEnabled(false);
        else
            ui->okBtn->setEnabled(true);
    }

}

void AddDeviceCardDialog::fiberElectricalCheck()
{
    int totalFiberPort = fiberPortCount();
    int totalElectricalPort = electricalPortCount();
    ui->pinCB->setCurrentIndex(-1);
    ui->posCB->setCurrentIndex(-1);
    ui->odfCB->setCurrentIndex(-1);
    odfNeeded = false;

    if( totalFiberPort > 0)
    {
        odfNeeded = true;
        ui->pbpChB->setEnabled(true);
        ui->pbpChB->setVisible(true);
        ui->odfGB->setEnabled(true);
        ui->odfGB->setVisible(true);
        ui->portInfoSA->setVisible(true);

        ui->posCB->setEnabled(false);
        ui->pinCB->setEnabled(false);
        ui->okBtn->setEnabled(false);
        //this->resize(800, 700);
    }
    else if(totalElectricalPort > 0)
    {
        odfNeeded = false;
        ui->pbpChB->setChecked(false);
        ui->pbpChB->setEnabled(false);
        ui->pbpChB->setVisible(false);
        ui->odfGB->setEnabled(false);
        ui->odfGB->setVisible(false);

        ui->portInfoSA->setVisible(true);
        ui->okBtn->setEnabled(true);
        //this->resize(800, 700);
    }

    if(odfNeeded)
    {
        ui->portNextBtn->show();
        ui->portOkBtn->hide();
    }
    else
    {
        ui->portOkBtn->show();
        ui->portNextBtn->hide();
    }

}

void AddDeviceCardDialog::on_eODFChB_toggled(bool checked)
{
    ui->ethIntTypeCB->clear();
    if(checked)
    {
        ui->eSkipODFChB->setVisible(true);
        ui->eSkipODFChB->setChecked(false);
        //int type
        QMap<int, QString> typeMap = dbMan->getInterfaceTypes(0.9,"FIBER", false);
        QList<int> list = typeMap.keys();
        foreach (int id, list)
        {
            ui->ethIntTypeCB->addItem(typeMap.value(id), id);
        }
    }
    else
    {
        //int type
        ui->eSkipODFChB->setVisible(false);
        ui->eSkipODFChB->setChecked(false);

        QMap<int, QString> typeMap = dbMan->getInterfaceTypes(0.9,"ELECTRICAL", false);
        QList<int> list = typeMap.keys();
        foreach (int id, list)
        {
            ui->ethIntTypeCB->addItem(typeMap.value(id), id);
        }
    }

    fiberElectricalCheck();
}

void AddDeviceCardDialog::on_g1ODFChB_toggled(bool checked)
{
    ui->gIntTypeCB->clear();

    if(checked)
    {
        ui->g1SkipODFChB->setVisible(true);
        ui->g1SkipODFChB->setChecked(false);
        //int type
        QMap<int, QString> typeMap = dbMan->getInterfaceTypes(1,"FIBER", true);
        QList<int> list = typeMap.keys();
        foreach (int id, list)
        {
            ui->gIntTypeCB->addItem(typeMap.value(id), id);
        }
    }
    else
    {
        ui->g1SkipODFChB->setVisible(false);
        ui->g1SkipODFChB->setChecked(false);
        //int type
        QMap<int, QString> typeMap = dbMan->getInterfaceTypes(1,"ELECTRICAL", true);

        foreach (int id, typeMap.keys())
        {
            ui->gIntTypeCB->addItem(typeMap.value(id), id);
        }
    }

    fiberElectricalCheck();
}

void AddDeviceCardDialog::on_g10ODFChB_toggled(bool checked)
{
    ui->g10IntTypeCB->clear();

    if(checked)
    {
        ui->g10SkipODFChB->setVisible(true);
        ui->g10SkipODFChB->setChecked(false);
        //int type
        QMap<int, QString> typeMap = dbMan->getInterfaceTypes(10,"FIBER", true);
        QList<int> list = typeMap.keys();
        foreach (int id, list)
        {
            ui->g10IntTypeCB->addItem(typeMap.value(id), id);
        }
    }
    else
    {
        ui->g10SkipODFChB->setVisible(false);
        ui->g10SkipODFChB->setChecked(false);
        //int type
        QMap<int, QString> typeMap = dbMan->getInterfaceTypes(10,"ELECTRICAL", true);
        QList<int> list = typeMap.keys();
        foreach (int id, list)
        {
            ui->g10IntTypeCB->addItem(typeMap.value(id), id);
        }
    }
    fiberElectricalCheck();
}

void AddDeviceCardDialog::on_exchNextBtn_clicked()
{
    ui->tabWidget->setCurrentIndex(1);
}

void AddDeviceCardDialog::on_posBackBtn_clicked()
{
    ui->tabWidget->setCurrentIndex(0);
}

void AddDeviceCardDialog::on_posNextBtn_clicked()
{
    ui->tabWidget->setCurrentIndex(2);
}

void AddDeviceCardDialog::on_portBackBtn_clicked()
{
    ui->tabWidget->setCurrentIndex(1);
}

void AddDeviceCardDialog::on_portNextBtn_clicked()
{
    ui->tabWidget->setCurrentIndex(3);
}

void AddDeviceCardDialog::on_odfBackBtn_clicked()
{
    ui->tabWidget->setCurrentIndex(2);
}

void AddDeviceCardDialog::on_posOkBtn_clicked()
{
    confirmBtn();
}

void AddDeviceCardDialog::on_portOkBtn_clicked()
{
    confirmBtn();
}

void AddDeviceCardDialog::on_g40SkipODFChB_toggled(bool checked)
{
    if(checked)
    {
        fiberElectricalCheck();
    }
    else
    {
        fiberElectricalCheck();
    }

}

void AddDeviceCardDialog::on_g100SkipODFChB_toggled(bool checked)
{
    if(checked)
    {
        fiberElectricalCheck();
    }
    else
    {
        fiberElectricalCheck();
    }
}

void AddDeviceCardDialog::on_eSkipODFChB_toggled(bool checked)
{
    if(checked)
    {
        fiberElectricalCheck();
    }
    else
    {
        fiberElectricalCheck();
    }
}

void AddDeviceCardDialog::on_g1SkipODFChB_toggled(bool checked)
{
    if(checked)
    {
        fiberElectricalCheck();
    }
    else
    {
        fiberElectricalCheck();
    }
}

void AddDeviceCardDialog::on_g10SkipODFChB_toggled(bool checked)
{
    if(checked)
    {
        fiberElectricalCheck();
    }
    else
    {
        fiberElectricalCheck();
    }
}
