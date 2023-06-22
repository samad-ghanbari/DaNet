#include "reservetocommercialdialog.h"
#include "ui_reservetocommercialdialog.h"
#include "lib/database/danetdbman.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QtXlsx>
#include <QFileDialog>
#include <QFile>

ReserveToCommercialDialog::ReserveToCommercialDialog(QWidget *parent, DanetDbMan *db) :
    QDialog(parent),
    ui(new Ui::ReserveToCommercialDialog),
    dbMan(db)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowMinMaxButtonsHint);

    //area
    ui->areaCB->blockSignals(true);
    ui->areaCB->addItem("2", 2);
    ui->areaCB->addItem("3", 3);
    ui->areaCB->addItem("4", 4);
    ui->areaCB->addItem("5", 5);
    ui->areaCB->addItem("6", 6);
    ui->areaCB->addItem("7", 7);
    ui->areaCB->addItem("8", 8);
    ui->areaCB->addItem("Karaj", 9);
    ui->areaCB->setCurrentIndex(-1);
    ui->areaCB->blockSignals(false);

    //type
    ui->typeCB->blockSignals(true);
    ui->typeCB->addItem("Exchange", 2);
    ui->typeCB->addItem("Site", 3);
    ui->typeCB->setCurrentIndex(0);
    ui->typeCB->blockSignals(false);

    int workingArea = dbMan->getLoggedInDefaultArea();
    ui->areaCB->setCurrentIndex(ui->areaCB->findData(workingArea));


    //fill map values
    equType.insert("CX600X16", 1);
    equType.insert("CX600X16B", 2);
    equType.insert("CX600X8", 3);
    equType.insert("CX600X8D", 4);
    equType.insert("NE40X8", 5);
    equType.insert("NE40X16E", 16);
    equType.insert("CX200", 17);
    equType.insert("S9306IXP", 20);
    equType.insert("S9306CO", 21);

    intTypeCode = dbMan->getIntTypeCode();

    ModuleType.insert("LX-10Km", 1);
    ModuleType.insert("LX-15Km", 2);
    ModuleType.insert("SX-500m", 3);
    ModuleType.insert("LX-40Km", 4);
    ModuleType.insert("ZX-40Km", 5);
    ModuleType.insert("ZX-70Km", 6);
    ModuleType.insert("ZX-80Km", 7);
    ModuleType.insert("ZX-100Km", 8);
    ModuleType.insert("E", 9);

    project.insert("Metro",1);
    project.insert("Ip-MPLS",2);

    networkType.insert("Metro", 1);
    networkType.insert("Elmi", 2);
    networkType.insert("HighSpeed", 3);
    networkType.insert("IP-MPLS", 4);

    portStatus = 1;
    ui->portChB->setChecked(false);
    ui->portChB->setEnabled(false);


    emptyForm();
}

ReserveToCommercialDialog::ReserveToCommercialDialog(QWidget *parent, DanetDbMan *db, const int AreaNo, const int ExchId, const int DevexId, const int IntId):
    QDialog(parent),
    ui(new Ui::ReserveToCommercialDialog),
    dbMan(db)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowMinMaxButtonsHint);


    //area
    ui->areaCB->blockSignals(true);
    ui->areaCB->addItem("2", 2);
    ui->areaCB->addItem("3", 3);
    ui->areaCB->addItem("4", 4);
    ui->areaCB->addItem("5", 5);
    ui->areaCB->addItem("6", 6);
    ui->areaCB->addItem("7", 7);
    ui->areaCB->addItem("8", 8);
    ui->areaCB->addItem("Karaj", 9);
    ui->areaCB->setCurrentIndex(-1);
    ui->areaCB->blockSignals(false);

    //type
    ui->typeCB->blockSignals(true);
    ui->typeCB->addItem("Exchange", 2);
    ui->typeCB->addItem("Site", 3);
    ui->typeCB->setCurrentIndex(0);
    ui->typeCB->blockSignals(false);


    //fill map values
    equType.insert("CX600X16", 1);
    equType.insert("CX600X16B", 2);
    equType.insert("CX600X8", 3);
    equType.insert("CX600X8D", 4);
    equType.insert("NE40X8", 5);
    equType.insert("NE40X16E", 16);
    equType.insert("CX200", 17);
    equType.insert("S9306IXP", 20);
    equType.insert("S9306CO", 21);

    intTypeCode = dbMan->getIntTypeCode();

    ModuleType.insert("LX-10Km", 1);
    ModuleType.insert("LX-15Km", 2);
    ModuleType.insert("SX-500m", 3);
    ModuleType.insert("LX-40Km", 4);
    ModuleType.insert("ZX-40Km", 5);
    ModuleType.insert("ZX-70Km", 6);
    ModuleType.insert("ZX-80Km", 7);
    ModuleType.insert("ZX-100Km", 8);
    ModuleType.insert("E", 9);

    project.insert("Metro",1);
    project.insert("Ip-MPLS",2);

    networkType.insert("Metro", 1);
    networkType.insert("Elmi", 2);
    networkType.insert("HighSpeed", 3);
    networkType.insert("IP-MPLS", 4);

    portStatus = 1;
    ui->portChB->setChecked(false);
    ui->portChB->setEnabled(false);


    emptyForm();

    ui->areaCB->setCurrentIndex(ui->areaCB->findData(AreaNo));
    int motherId = dbMan->getSiteMotherId(ExchId);
    if(motherId == ExchId)
    {
        //exch
        ui->abbrCB->setCurrentIndex(ui->abbrCB->findData(ExchId));
        ui->typeCB->setCurrentIndex(ui->typeCB->findData(2));
    }
    else
    {
        //site
        ui->abbrCB->setCurrentIndex(ui->abbrCB->findData(motherId));
        ui->typeCB->setCurrentIndex(ui->typeCB->findData(3));
        ui->siteCB->setCurrentIndex(ui->siteCB->findData(ExchId));
    }

    ui->deviceCB->setCurrentIndex(ui->deviceCB->findData(DevexId));

    // find shelf, slot,subslot, port
    QList<int> SSSP = dbMan->getInterfaceShelfSlotSubslotPort(IntId);
    if(SSSP[0] > -1)
    {
        //shelf
        ui->shelfCB->setVisible(true);
        ui->noShelfLbl->setVisible(false);
        ui->shelfCB->setCurrentIndex(ui->shelfCB->findData(SSSP[0]));
    }

    if(SSSP[1] > -1)
    {
        //slot
        ui->slotCB->setCurrentIndex(ui->slotCB->findData(SSSP[1]));
    }

    if(SSSP[2] > -1)
    {
        //subslot
        ui->subSlotCB->setVisible(true);
        ui->noSubSlotLbl->setVisible(false);
        ui->subSlotCB->setCurrentIndex(ui->subSlotCB->findData(SSSP[2]));
    }

    if(SSSP[3] > -1)
    {
        //port
        ui->portChB->setChecked(true);
        ui->startPortLbl->setVisible(true);
        ui->startPortCB->setVisible(true);
        ui->endPortLbl->setVisible(true);
        ui->endPortCB->setVisible(true);
        ui->allPortLbl->setVisible(false);

        ui->startPortCB->setCurrentIndex(ui->startPortCB->findData(SSSP[3]));
        ui->endPortCB->setCurrentIndex(ui->endPortCB->findData(SSSP[3]));

        ui->okBtn->setEnabled(true);
    }

}

ReserveToCommercialDialog::~ReserveToCommercialDialog()
{
    delete ui;
}

void ReserveToCommercialDialog::emptyForm()
{
    ui->shelfCB->setVisible(false);
    ui->noShelfLbl->setVisible(true);

    ui->subSlotCB->setVisible(false);
    ui->noSubSlotLbl->setVisible(true);

    ui->portChB->setChecked(false);
    ui->startPortLbl->setVisible(false);
    ui->startPortCB->setVisible(false);
    ui->endPortLbl->setVisible(false);
    ui->endPortCB->setVisible(false);
    ui->allPortLbl->setVisible(true);


    ui->shelfCB->clear();
    ui->slotCB->clear();
    ui->subSlotCB->clear();
    ui->startPortCB->clear();
    ui->endPortCB->clear();
    ui->intTypeCB->clear();

    ui->infoLbl->setVisible(false);
    ui->okBtn->setEnabled(false);

}

void ReserveToCommercialDialog::checkEntry()
{
    int start = ui->startPortCB->currentData().toInt();
    int end = ui->endPortCB->currentData().toInt();
    bool c1 = (end >= start)? true : false;
    bool s = (ui->slotCB->currentText().isEmpty())? false : true;
    bool sub = (subSlotSupport && !ui->subSlotCB->currentText().isEmpty()) || !subSlotSupport || (ui->subSlotCB->currentData()==-2);
    bool sh = (shelfSupport && !ui->shelfCB->currentText().isEmpty()) || !shelfSupport;
    bool p = ui->portChB->isChecked();
    p = (p && c1 && (!ui->startPortCB->currentText().isEmpty()) && (!ui->endPortCB->currentText().isEmpty()) ) || !p;

    if(s && sub && sh && p)
        ui->okBtn->setEnabled(true);
    else
        ui->okBtn->setEnabled(false);
}

QList<int> ReserveToCommercialDialog::getInterfacesId()
{
    int devexId = ui->deviceCB->currentData().toInt();
    QString cond;
    cond = " WHERE slots_id IN(SELECT `id` FROM `slots` WHERE devex_id="+QString::number(devexId);
    if(shelfSupport)
    {
        int shelfNo = ui->shelfCB->currentData().toInt();
        cond += " AND shelf="+QString::number(shelfNo);
    }
    int slotNo = ui->slotCB->currentData().toInt();
    if(slotNo > -1)
        cond += " AND slot="+QString::number(slotNo);
    if(subSlotSupport)
    {
        int subSlot = ui->subSlotCB->currentData().toInt();
        if(subSlot > -1)
            cond += " AND sub_slot="+QString::number(subSlot);
    }

    QString t = ui->intTypeCB->currentText();
    if(t.compare("All", Qt::CaseInsensitive) != 0)
        cond += " AND interface_type='"+t+"' ";
    if(ui->portChB->isChecked())
    {
        cond += " ) AND ";
        int start = ui->startPortCB->currentData().toInt();
        int end = ui->endPortCB->currentData().toInt();

        cond += " port BETWEEN "+QString::number(start)+" AND "+ QString::number(end)+" ORDER BY port;";
    }
    else
        cond += ")  ORDER BY port;";

    QList<int> intIds = dbMan->getInerfacesIds(cond);

    return intIds;

}

bool ReserveToCommercialDialog::interfaceReservability(QList<int> intIds)
{
    bool sep = false;
    QString cond;
    foreach (int i, intIds)
    {
        if(sep)
        {
            cond += ",";
        }
        cond += QString::number(i);
        sep = true;
    }

    if(dbMan->isInterfacesEmpty(cond))
        return true;
    else
        return false;
}

void ReserveToCommercialDialog::on_areaCB_currentIndexChanged(int index)
{
    int area = index + 2;

    ui->abbrCB->blockSignals(true);
    ui->abbrCB->clear();
    ui->siteCB->clear();
    ui->deviceCB->clear();
    emptyForm();

    QString abbr;
    int id;
    QSqlQuery *query = dbMan->selectExchanges(area);
    //`id`,`abbr`

    while(query->next())
    {
        id = query->value(0).toInt();
        abbr = query->value(1).toString();
        ui->abbrCB->addItem(abbr, id);
    }

    ui->abbrCB->setCurrentIndex(-1);
    ui->abbrCB->blockSignals(false);

    ui->typeCB->setCurrentIndex(0);
    ui->siteFrame->setVisible(false);

}

void ReserveToCommercialDialog::on_abbrCB_currentIndexChanged(int index)
{
    index++;
    emptyForm();
    ui->typeCB->blockSignals(true);
    ui->typeCB->setCurrentIndex(0);
    ui->typeCB->blockSignals(false);

    ui->siteCB->blockSignals(true);
    ui->siteCB->clear();
    ui->siteCB->blockSignals(false);
    ui->siteFrame->setVisible(false);

    ui->deviceCB->blockSignals(true);
    ui->deviceCB->clear();

    int exchId = ui->abbrCB->currentData().toInt();

    QSqlQuery *query = dbMan->selectExchangeSrDevices(exchId); // switch router of exchange
    //id,device
    int id;
    QString device;
    while(query->next())
    {
        id = query->value(0).toInt();
        device = query->value(1).toString();
        ui->deviceCB->addItem(device,id);
    }

    ui->deviceCB->setCurrentIndex(-1);
    ui->deviceCB->blockSignals(false);
}

void ReserveToCommercialDialog::on_typeCB_currentIndexChanged(int index)
{
    index++;
    emptyForm();
    ui->siteCB->blockSignals(true);
    ui->siteCB->clear();

    int type = ui->typeCB->currentData().toInt();

    if(type == 2) //exch
    {
        ui->siteFrame->setVisible(false);

        ui->siteCB->blockSignals(false);
        ui->siteCB->clear();

        ui->deviceCB->blockSignals(true);
        ui->deviceCB->clear();

        int exchId = ui->abbrCB->currentData().toInt();

        QSqlQuery *query = dbMan->selectExchangeSrDevices(exchId);
        //id,device
        int id;
        QString device;
        while(query->next())
        {
            id = query->value(0).toInt();
            device = query->value(1).toString();
            ui->deviceCB->addItem(device,id);
        }

        ui->deviceCB->setCurrentIndex(-1);
        ui->deviceCB->blockSignals(false);

    }
    else //site
    {
        ui->siteFrame->setVisible(true);
        ui->deviceCB->blockSignals(true);
        ui->deviceCB->clear();
        ui->deviceCB->blockSignals(false);

        QString abbr;
        int id;
        int area = ui->areaCB->currentData().toInt();
        QString mother = ui->abbrCB->currentText();

        QSqlQuery *query = dbMan->selectSites(area,mother);
        //`id`,`abbr`

        while(query->next())
        {
            id = query->value(0).toInt();
            abbr = query->value(1).toString();
            ui->siteCB->addItem(abbr, id);
        }

        ui->siteCB->setCurrentIndex(-1);
        ui->siteCB->blockSignals(false);

    }
}

void ReserveToCommercialDialog::on_siteCB_currentIndexChanged(int index)
{
    index++;
    emptyForm();
    ui->deviceCB->blockSignals(true);
    ui->deviceCB->clear();

    if(ui->typeCB->currentData().toInt() == 3) //site
    {
        int exchId = ui->siteCB->currentData().toInt();

        QSqlQuery *query = dbMan->selectExchangeSrDevices(exchId);
        //id,device
        int id;
        QString device;
        while(query->next())
        {
            id = query->value(0).toInt();
            device = query->value(1).toString();
            ui->deviceCB->addItem(device,id);
        }
    }

    ui->deviceCB->setCurrentIndex(-1);
    ui->deviceCB->blockSignals(false);
}

void ReserveToCommercialDialog::on_deviceCB_currentIndexChanged(int index)
{
    index++;
    emptyForm();

    int devexId = ui->deviceCB->currentData().toInt();

    QList<int> shelfs = dbMan->getDevexDistinctShelfs(devexId);
    if(shelfs.count() > 0)
    {
        shelfSupport = true;
        ui->shelfCB->setVisible(true);
        ui->shelfCB->setEnabled(true);
        ui->shelfCB->clear();
        ui->noShelfLbl->setVisible(false);

        ui->shelfCB->blockSignals(true);
        foreach (int i, shelfs)
        {
            ui->shelfCB->addItem(QString::number(i), i);
        }
        ui->shelfCB->setCurrentIndex(-1);
        ui->shelfCB->blockSignals(false);
    }
    else
    {
        //no shelf
        shelfSupport = false;
        ui->shelfCB->setVisible(false);
        ui->shelfCB->setEnabled(false);
        ui->noShelfLbl->setVisible(true);

        //fill slot CB

        QList<int> slotsList = dbMan->getDevexDistinctSlots(devexId,-1);
        if(slotsList.count() > 0)
        {
            ui->slotCB->setVisible(true);
            ui->slotCB->setEnabled(true);
            ui->slotCB->clear();

            ui->slotCB->blockSignals(true);
            foreach (int i, slotsList)
            {
                ui->slotCB->addItem(QString::number(i), i);
            }
            ui->slotCB->setCurrentIndex(-1);
            ui->slotCB->blockSignals(false);
        }
        else
        {
            //no slot
            ui->slotCB->clear();
            ui->slotCB->setEnabled(false);

            QList<int> subslotsList = dbMan->getDevexDistinctSubslots(devexId,-1, -1);
            if(subslotsList.count() > 0)
            {
                ui->subSlotCB->setEnabled(true);
                ui->subSlotCB->setVisible(true);
                ui->subSlotCB->clear();
                ui->noSubSlotLbl->setVisible(false);

                subSlotSupport = true;
                ui->subSlotCB->blockSignals(true);
                foreach (int i, subslotsList)
                {
                    ui->subSlotCB->addItem(QString::number(i), i);
                }
                ui->subSlotCB->setCurrentIndex(-1);
                ui->subSlotCB->blockSignals(false);
            }
            else
            {
                //no sub slot

                ui->subSlotCB->setEnabled(false);
                ui->subSlotCB->setVisible(false);
                ui->subSlotCB->clear();
                ui->noSubSlotLbl->setVisible(true);


                subSlotSupport = false;
                //fill ports
                QString intType = ui->intTypeCB->currentText();
                QList<int> portList;
                if(intType.compare("All") == 0)
                    portList = dbMan->getDevexDistinctPorts(devexId, -1,-1,-1);
                else
                    portList = dbMan->getDevexDistinctPorts(devexId, -1,-1,-1, intType);

                if(portList.count() > 0)
                {
                    foreach (int i, portList)
                    {
                        ui->startPortCB->addItem(QString::number(i), i);
                        ui->endPortCB->addItem(QString::number(i), i);
                    }
                    ui->startPortCB->setCurrentIndex(-1);
                    ui->endPortCB->setCurrentIndex(-1);
                }
            }
        }

    }

    QList<QString> typeList = dbMan->getDevexInterfaceTypes(devexId);
    ui->intTypeCB->addItem("All");
    foreach (QString t, typeList)
        ui->intTypeCB->addItem(t);
}

void ReserveToCommercialDialog::on_shelfCB_currentIndexChanged(int index)
{
    index++;
    int devexId = ui->deviceCB->currentData().toInt();
    ui->slotCB->clear();
    ui->subSlotCB->clear();
    ui->intTypeCB->clear();
    ui->subSlotCB->setVisible(false);
    ui->noSubSlotLbl->setVisible(true);
    ui->startPortCB->clear();
    ui->endPortCB->clear();
    ui->portChB->setChecked(false);
    ui->infoLbl->setVisible(false);

    //fill slot CB of that shelf
    int shelfNo = ui->shelfCB->currentData().toInt();
    QList<int> slotList = dbMan->getDevexDistinctSlots(devexId, shelfNo);
    if(slotList.count() > 0)
    {
        ui->slotCB->setEnabled(true);
        ui->slotCB->clear();

        ui->slotCB->blockSignals(true);
        foreach (int i, slotList)
        {
            ui->slotCB->addItem(QString::number(i), i);
        }
        ui->slotCB->setCurrentIndex(-1);
        ui->slotCB->blockSignals(false);
    }

    QList<QString> typeList = dbMan->getDevexInterfaceTypes(devexId,shelfNo,-1,-1);
    ui->intTypeCB->addItem("All");
    foreach (QString t, typeList)
        ui->intTypeCB->addItem(t);

    ui->okBtn->setEnabled(false);
}

void ReserveToCommercialDialog::on_slotCB_currentIndexChanged(int index)
{
    index++;
    ui->subSlotCB->clear();
    ui->startPortCB->clear();
    ui->endPortCB->clear();
    ui->intTypeCB->clear();
    ui->infoLbl->setVisible(false);
    ui->portChB->setChecked(false);
    ui->portChB->setEnabled(false);

    int devexId = ui->deviceCB->currentData().toInt();
    int slotNo = ui->slotCB->currentData().toInt();

    int shelf = -1;
    if(shelfSupport)
        shelf = ui->shelfCB->currentData().toInt();

    QList<int> subslotsList = dbMan->getDevexDistinctSubslots(devexId,shelf,slotNo);
    if(subslotsList.count() > 0)
    {
        subSlotSupport = true;
        ui->subSlotCB->setEnabled(true);
        ui->subSlotCB->setVisible(true);
        ui->subSlotCB->clear();
        ui->noSubSlotLbl->setVisible(false);

        ui->subSlotCB->blockSignals(true);
        foreach (int i, subslotsList)
        {
            ui->subSlotCB->addItem(QString::number(i), i);
        }
        ui->subSlotCB->addItem("All", -2);
        ui->subSlotCB->setCurrentIndex(-1);
        ui->subSlotCB->blockSignals(false);
    }
    else
    {
        //no sub slot
        subSlotSupport = false;
        ui->subSlotCB->setEnabled(false);
        ui->subSlotCB->setVisible(false);
        ui->subSlotCB->clear();
        ui->noSubSlotLbl->setVisible(true);

        //fill ports
        QString intType = ui->intTypeCB->currentText();
        QList<int> portList;
        if(intType.compare("All") == 0)
            portList = dbMan->getDevexDistinctPorts(devexId, shelf,slotNo,-1);
        else
            portList = dbMan->getDevexDistinctPorts(devexId, shelf,slotNo,-1, intType);

        if(portList.count() > 0)
        {
            foreach (int i, portList)
            {
                ui->startPortCB->addItem(QString::number(i), i);
                ui->endPortCB->addItem(QString::number(i), i);
            }
            ui->startPortCB->setCurrentIndex(-1);
            ui->endPortCB->setCurrentIndex(-1);
        }
    }

    QList<QString> typeList = dbMan->getDevexInterfaceTypes(devexId,shelf,slotNo,-1);
    ui->intTypeCB->addItem("All");
    foreach (QString t, typeList)
        ui->intTypeCB->addItem(t);

    ui->okBtn->setEnabled(false);

}

void ReserveToCommercialDialog::on_subSlotCB_currentIndexChanged(int index)
{
    index++;
    ui->startPortCB->clear();
    ui->endPortCB->clear();
    ui->intTypeCB->clear();
    if(ui->subSlotCB->currentIndex() == -1)
    {
        ui->portChB->setEnabled(false);
        return;
    }
    ui->portChB->setChecked(false);
    ui->infoLbl->setVisible(false);

    int devexId = ui->deviceCB->currentData().toInt();
    int slotNo = ui->slotCB->currentData().toInt();
    int subSlotNo = ui->subSlotCB->currentData().toInt();
    int shelf =-1;
    if(shelfSupport)
        shelf = ui->shelfCB->currentData().toInt();

    if(ui->subSlotCB->currentData().toInt() < 0)
    {
        //all ports
        ui->portChB->setChecked(false);
        ui->portChB->setEnabled(false);
    }
    else
    {
        //fill ports
        ui->portChB->setEnabled(true);
        QString intType = ui->intTypeCB->currentText();
        QList<int> portList;
        if(intType.compare("All") == 0)
            portList = dbMan->getDevexDistinctPorts(devexId, shelf,slotNo,subSlotNo);
        else
            portList = dbMan->getDevexDistinctPorts(devexId, shelf,slotNo,subSlotNo, intType);
        if(portList.count() > 0)
        {
            foreach (int i, portList)
            {
                ui->startPortCB->addItem(QString::number(i), i);
                ui->endPortCB->addItem(QString::number(i), i);
            }
            ui->startPortCB->setCurrentIndex(-1);
            ui->endPortCB->setCurrentIndex(-1);
        }
    }


    QList<QString> typeList = dbMan->getDevexInterfaceTypes(devexId,shelf,slotNo,subSlotNo);
    ui->intTypeCB->addItem("All");
    foreach (QString t, typeList)
        ui->intTypeCB->addItem(t);

    checkEntry();
}

void ReserveToCommercialDialog::on_portChB_toggled(bool checked)
{
    ui->infoLbl->setVisible(false);

    if(checked)
    {
        ui->startPortCB->setVisible(true);
        ui->startPortLbl->setVisible(true);
        ui->endPortCB->setVisible(true);
        ui->endPortLbl->setVisible(true);
        ui->allPortLbl->setVisible(false);
    }
    else
    {
        ui->startPortCB->setVisible(false);
        ui->startPortLbl->setVisible(false);
        ui->endPortCB->setVisible(false);
        ui->endPortLbl->setVisible(false);
        ui->allPortLbl->setVisible(true);
    }

    checkEntry();
}

void ReserveToCommercialDialog::on_startPortCB_currentIndexChanged(int index)
{
    ui->infoLbl->setVisible(false);
    index++;
    checkEntry();
}

void ReserveToCommercialDialog::on_endPortCB_currentIndexChanged(int index)
{
    ui->infoLbl->setVisible(false);
    index++;
    checkEntry();
}

void ReserveToCommercialDialog::on_cancelBtn_clicked()
{
    this->close();
}

void ReserveToCommercialDialog::on_okBtn_clicked()
{
    QString Module = "LX-10Km";
    QString Exchange = "Reserved To Commercial Dep.";

    QString Device = "Customer Device";
    QString PeerLable = "Reserved To Commercial Dep.";

    QString user = dbMan->getLoggedinNameLastname();

    QList<int> IntIds = getInterfacesId();
    int exchId = ui->abbrCB->currentData().toInt();
    if(ui->typeCB->currentIndex() == 1)
        exchId = ui->siteCB->currentData().toInt();

    QString ports=ui->deviceCB->currentText()+"*";
    if(shelfSupport) ports += QString::number(ui->shelfCB->currentData().toInt())+"/";
    ports += QString::number(ui->slotCB->currentData().toInt())+"/";
    if(subSlotSupport) ports += QString::number(ui->subSlotCB->currentData().toInt())+"/";
    if(ui->portChB->isChecked())
        ports += QString::number(ui->startPortCB->currentData().toInt()) +"-"+QString::number(ui->endPortCB->currentData().toInt());
    else
        ports += "X";


    if(interfaceReservability(IntIds) && (IntIds.count() > 0) )
    {
        bool ok = true;
        QMap<int,QString> intIdType = dbMan->getIntIdType(IntIds);
        foreach (int i, intIdType.keys())
            intIdType[i] = dbMan->getInterfaceTypeSymble(intIdType[i]);


        //update interfaces
        foreach (int id, IntIds)
        {
            if(!dbMan->updateInterface(id,Module,Exchange,Device,intIdType.value(id),PeerLable,-1,1))
            {
                ok = false;
                break;
            }
        }

        if(ok)
        {
            //log
            dbMan->insertLog(exchId,user,"Ports ["+ports+"] were reserved to Commercial Dep.");

            //report
            QString path = QFileDialog::getSaveFileName(this, "Save DaNet Commercial Ports Report As", "","xlsx (*.xlsx)");

            if(!path.isEmpty())
            {

                if(!path.endsWith(".xlsx", Qt::CaseInsensitive)) path += ".xlsx";

                QString cond = " id IN ( ";
                bool sep = false;
                foreach (int i, IntIds)
                {
                    if(sep)
                    {
                        cond += ",";
                    }
                    cond += QString::number(i);
                    sep = true;
                }

                cond += " )";

                QSqlQuery *query = dbMan->selectViewCommercial(cond);
                //`id`, `area`, `exchange`, `device`, `local Label`, `interface_type`, `interface`, `module`, `odf`, `pos`, `pin`, `abbr`
                //############################### excel file
                //xsl file : fist create then save
                QXlsx::Document xlsx;
                xlsx.addSheet("DaNet Report");
                xlsx.deleteSheet("sheet1");

                xlsx.setDocumentProperty("title", "Interfaces Reserved To Commercial Department");
                xlsx.setDocumentProperty("subject", "Reservation");
                xlsx.setDocumentProperty("creator", "DaNet Software");
                xlsx.setDocumentProperty("company", "DaNet");
                xlsx.setDocumentProperty("category", "Report");
                xlsx.setDocumentProperty("keywords", "Data Network");
                xlsx.setDocumentProperty("description", "Developer Samad Ghanbari");



                QXlsx::Format format;
                format.setHorizontalAlignment(QXlsx::Format::AlignHCenter);
                format.setVerticalAlignment(QXlsx::Format::AlignVCenter);
                format.setBorderColor(Qt::darkBlue);
                format.setFontBold(true);
                format.setFontSize(14);
                format.setTextWarp(true);
                format.setFontColor(Qt::blue);
                format.setPatternBackgroundColor(QColor(200, 200, 230));
                format.setBorderStyle(QXlsx::Format::BorderDouble);

                xlsx.setRowHeight(1, 40);
                xlsx.mergeCells("A1:R1", format);
                xlsx.write("A1", "DaNet Report");

                format.setFontColor(Qt::darkBlue);
                format.setPatternBackgroundColor(QColor(220, 220, 230));

                xlsx.setRowHeight(2, 40);
                xlsx.mergeCells("A2:R2", format);
                xlsx.write("A2", "Interfaces Reserved To Commercial Department");

                //headers
                format.setFontSize(10);
                format.setFontColor(Qt::darkRed);
                format.setPatternBackgroundColor(QColor(200, 220, 230));
                format.setBorderStyle(QXlsx::Format::BorderHair);
                xlsx.setRowHeight(3, 30);
                xlsx.setColumnWidth(1, 5);
                xlsx.write("A3", "Area", format);
                xlsx.setColumnWidth(2, 15);
                xlsx.write("B3", "Exch", format);
                xlsx.setColumnWidth(3, 10);
                xlsx.write("C3", "EquType", format);
                xlsx.setColumnWidth(4, 20);
                xlsx.write("D3", "Local Label", format);
                xlsx.setColumnWidth(5, 15);
                xlsx.write("E3", "Interface", format);
                xlsx.setColumnWidth(6, 10);
                xlsx.write("F3", "Interface Type", format);
                xlsx.setColumnWidth(7, 7);
                xlsx.write("G3", "Module", format);
                xlsx.setColumnWidth(8, 5);
                xlsx.write("H3", "ODF", format);
                xlsx.setColumnWidth(9, 5);
                xlsx.write("I3", "POS", format);
                xlsx.setColumnWidth(10, 5);
                xlsx.write("J3", "PIN", format);
                xlsx.setColumnWidth(11, 10);
                xlsx.write("K3", "ABBR", format);
                xlsx.setColumnWidth(12, 7);
                xlsx.write("L3", "Project", format);
                xlsx.setColumnWidth(13, 10);
                xlsx.write("M3", "Network Type", format);
                xlsx.setColumnWidth(14, 10);
                xlsx.write("N3", "Remote Interface", format);
                xlsx.setColumnWidth(15, 10);
                xlsx.write("O3", "Port Status", format);
                xlsx.setColumnWidth(16, 15);
                xlsx.write("P3", "Equipment", format);
                xlsx.setColumnWidth(17, 10);
                xlsx.write("Q3", "Interface Type", format);
                xlsx.setColumnWidth(18, 10);
                xlsx.write("R3", "Module", format);

                //records
                format.setFontBold(false);
                format.setFontColor(Qt::black);
                format.setPatternBackgroundColor(QColor(255, 255, 255));
                format.setBorderStyle(QXlsx::Format::BorderHair);

                QString Area, Abbr,Exch,LocalLabel, Interface, InterfaceType,Module,Odf,Pos,Pin;
                int EquType,IntType, ModType, Project,NetType;

                int row = 3;
                while(query->next())
                {
                    row++;
                    xlsx.setRowHeight(row,25);
                    //0`id`, 1`area`, 2`exchange`, 3`device`, 4`local Label`, 5`interface_type`, 6`interface`, 7`module`, 8`odf`, 9`pos`, 10`pin`, 11`abbr`
                    Area = query->value(1).toString();
                    Exch = query->value(2).toString();
                    Device = query->value(3).toString();
                    LocalLabel = query->value(4).toString();
                    InterfaceType = query->value(5).toString();
                    Interface = query->value(6).toString();
                    Interface = dbMan->getInterfaceTypeSymble(InterfaceType)+"-"+Interface;
                    Module = query->value(7).toString();
                    Odf = query->value(8).toString();
                    Pos = query->value(9).toString();
                    Pin = query->value(10).toString();
                    Abbr = query->value(11).toString();

                    EquType = equType.value(Device);
                    IntType = intTypeCode.value(InterfaceType);
                    ModType = ModuleType.value(Module);
                    if(ModType < 1) ModType = 1;

                    Project = 1;
                    NetType = 1;

                    xlsx.write("A"+QString::number(row), Area, format);
                    xlsx.write("B"+QString::number(row), Exch, format);
                    xlsx.write("C"+QString::number(row), EquType, format);
                    xlsx.write("D"+QString::number(row), LocalLabel, format);
                    xlsx.write("E"+QString::number(row), Interface, format);
                    xlsx.write("F"+QString::number(row), IntType, format);
                    xlsx.write("G"+QString::number(row), ModType, format);
                    xlsx.write("H"+QString::number(row), Odf, format);
                    xlsx.write("I"+QString::number(row), Pos, format);
                    xlsx.write("J"+QString::number(row), Pin, format);
                    xlsx.write("K"+QString::number(row), Abbr, format);
                    xlsx.write("L"+QString::number(row), Project, format);
                    xlsx.write("M"+QString::number(row), NetType, format);
                    xlsx.write("N"+QString::number(row), "", format);
                    xlsx.write("O"+QString::number(row), portStatus, format);
                    xlsx.write("P"+QString::number(row), Device, format);
                    xlsx.write("Q"+QString::number(row), InterfaceType, format);
                    xlsx.write("R"+QString::number(row), Module, format);


                }

                xlsx.saveAs(path);
                int d = ui->deviceCB->currentIndex();
                ui->deviceCB->setCurrentIndex(-1);
                ui->deviceCB->setCurrentIndex(d);
            }

            this->close();

        }
        else
            QMessageBox::warning(this, "ERROR", "Cannot reserve interfaces totally.\n"+dbMan->lastError());
    }
    else
    {
        ui->okBtn->setEnabled(false);
        ui->infoLbl->setVisible(true);
    }

}

void ReserveToCommercialDialog::on_intTypeCB_currentIndexChanged(int index)
{
    index++;
    int devexId = ui->deviceCB->currentData().toInt();
    int shelf = -1;
    if(shelfSupport) shelf = ui->shelfCB->currentData().toInt();
    int slot = ui->slotCB->currentData().toInt();
    int subslot = -1;
    if(subSlotSupport) subslot = ui->subSlotCB->currentData().toInt();
    QString intType = ui->intTypeCB->currentText();

    //fill ports
    ui->startPortCB->clear();
    ui->endPortCB->clear();

    QList<int> portList;
    if((intType.compare("All") == 0) || (intType.isEmpty()) )
        portList = dbMan->getDevexDistinctPorts(devexId, shelf,slot,subslot);
    else
        portList = dbMan->getDevexDistinctPorts(devexId, shelf,slot,subslot, intType);

    if(portList.count() > 0)
    {
        foreach (int i, portList)
        {
            ui->startPortCB->addItem(QString::number(i), i);
            ui->endPortCB->addItem(QString::number(i), i);
        }
        ui->startPortCB->setCurrentIndex(-1);
        ui->endPortCB->setCurrentIndex(-1);
    }
}
