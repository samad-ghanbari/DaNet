#include "reportreservetocommercialdialog.h"
#include "ui_reportreservetocommercialdialog.h"
#include "lib/database/danetdbman.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QtXlsx>
#include <QFileDialog>
#include <QFile>

ReportReserveToCommercialDialog::ReportReserveToCommercialDialog(QWidget *parent, DanetDbMan *db) :
    QDialog(parent),
    ui(new Ui::ReportReserveToCommercialDialog),
    dbMan(db)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowMinMaxButtonsHint);

    slotCount = 0;
    startShelf = -1;
    startSlot = -1;
    startSubslot = -1;

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
    equType.insert("CX600X8-D", 4);
    equType.insert("NE40X8E", 5);
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


    emptyForm();
}

ReportReserveToCommercialDialog::~ReportReserveToCommercialDialog()
{
    delete ui;
}

void ReportReserveToCommercialDialog::emptyForm()
{
    ui->shelfCB->setVisible(false);
    ui->noShelfLbl->setVisible(true);

    ui->subSlotCB->setVisible(false);
    ui->noSubSlotLbl->setVisible(true);

    ui->shelfCB->clear();
    ui->slotCB->clear();
    ui->subSlotCB->clear();

    ui->okBtn->setEnabled(false);

}

void ReportReserveToCommercialDialog::checkEntry()
{
    bool d = !ui->deviceCB->currentText().isEmpty();
    bool s = !ui->slotCB->currentText().isEmpty();
    bool sub = ( (startSubslot > -1) && !ui->subSlotCB->currentText().isEmpty()) || (startSubslot == -1) || (ui->slotCB->currentData() == -2) ;
    bool sh = ((startShelf > -1) && !ui->shelfCB->currentText().isEmpty()) || (startShelf == -1);

    if(d && s && sub && sh )
        ui->okBtn->setEnabled(true);
    else
        ui->okBtn->setEnabled(false);
}

QList<int> ReportReserveToCommercialDialog::getInterfacesId()
{
    int devexId = ui->deviceCB->currentData().toInt();
    QString cond;
    cond = " WHERE slots_id IN(SELECT `id` FROM `slots` WHERE devex_id="+QString::number(devexId);
    if(startShelf > -1)
    {
        int shelfNo = ui->shelfCB->currentData().toInt();
        cond += " AND shelf="+QString::number(shelfNo);
    }
    int slotNo = ui->slotCB->currentData().toInt();
    if(slotNo > -1)
    {
        cond += " AND slot="+QString::number(slotNo);
        if(startSubslot > -1)
        {
            int subSlot = ui->subSlotCB->currentData().toInt();
            if(subSlot > -1)
                cond += " AND sub_slot="+QString::number(subSlot);
        }
    }

    cond += ") AND `commercial`=1 ORDER BY port;";

    QList<int> intIds = dbMan->getInerfacesIds(cond);

    return intIds;
}

void ReportReserveToCommercialDialog::on_areaCB_currentIndexChanged(int index)
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

void ReportReserveToCommercialDialog::on_abbrCB_currentIndexChanged(int index)
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

void ReportReserveToCommercialDialog::on_typeCB_currentIndexChanged(int index)
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


        int area = ui->areaCB->currentData().toInt();
        QString mother = ui->abbrCB->currentText();

        QSqlQuery *query = dbMan->selectSites(area,mother);
        //`id`,`abbr`

        int i;
        QString a, s,c,n;
        while(query->next())
        {
            i = query->value(0).toInt();
            a = query->value(1).toString();
            s = query->value(2).toString();
            c = query->value(3).toString();
            n = query->value(4).toString();
            ui->siteCB->addItem(c+"-"+n+"*"+a+"*"+s, i);
        }

        ui->siteCB->setCurrentIndex(-1);
        ui->siteCB->blockSignals(false);

    }
}

void ReportReserveToCommercialDialog::on_siteCB_currentIndexChanged(int index)
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

void ReportReserveToCommercialDialog::on_deviceCB_currentIndexChanged(int index)
{
    index++;
    emptyForm();

    int devexId = ui->deviceCB->currentData().toInt();
    slotCount = 0;
    startShelf = -1;
    startSlot = -1;
    startSubslot = -1;
    //
    QList<int> slotCountStarts = dbMan->getDevexSlotCountStarts(devexId); //count-shelf-slot-subslot
    if(slotCountStarts.count() == 4)
    {
        slotCount = slotCountStarts.at(0);
        startShelf = slotCountStarts.at(1);
        startSlot = slotCountStarts.at(2);
        startSubslot = slotCountStarts.at(3);
    }
    else
    {
        QMessageBox::warning(this,"Error","Cannot fetch device Information. Please try again later.");
        emptyForm();
    }

    if(startShelf == -1)
    {
        ui->shelfCB->setVisible(false);
        ui->shelfCB->setEnabled(false);
        ui->noShelfLbl->setVisible(true);

        if(slotCount > 0)
        {
            ui->slotCB->blockSignals(true);
            QList<int> list = dbMan->getDevexDistinctSlots(devexId,-1);
            foreach(int i,list)
            {
                ui->slotCB->addItem(QString::number(i), i);
            }
            ui->slotCB->addItem("All", -2);
            ui->slotCB->setCurrentIndex(-1);
            ui->slotCB->blockSignals(false);
        }
    }
    else
    {
        ui->shelfCB->setVisible(true);
        ui->shelfCB->setEnabled(true);
        ui->noShelfLbl->setVisible(false);
        ui->slotCB->clear();
        QList<int> list = dbMan->getDevexDistinctShelfs(devexId);
        ui->shelfCB->blockSignals(true);
        foreach (int i, list)
        {
            ui->shelfCB->addItem(QString::number(i), i);
        }
        ui->shelfCB->setCurrentIndex(-1);
        ui->shelfCB->blockSignals(false);
    }

    if(startSubslot == -1)
    {
        ui->subSlotCB->setVisible(false);
        ui->subSlotCB->setEnabled(false);
        ui->noSubSlotLbl->setVisible(true);
    }
}

void ReportReserveToCommercialDialog::on_shelfCB_currentIndexChanged(int index)
{
    index++;
    int devexId = ui->deviceCB->currentData().toInt();
    ui->slotCB->clear();
    ui->subSlotCB->clear();
    ui->subSlotCB->setVisible(false);
    ui->noSubSlotLbl->setVisible(true);

    int shelfNo = ui->shelfCB->currentData().toInt();
    QList<int> list = dbMan->getDevexDistinctSlots(devexId,shelfNo);
    ui->slotCB->blockSignals(true);
    foreach (int i, list)
    {
        ui->slotCB->addItem(QString::number(i), i);
    }
    ui->slotCB->addItem("All", -2);
    ui->slotCB->setCurrentIndex(-1);
    ui->slotCB->blockSignals(false);
}

void ReportReserveToCommercialDialog::on_slotCB_currentIndexChanged(int index)
{
    index++;
    ui->subSlotCB->clear();
    ui->subSlotCB->setVisible(false);
    ui->subSlotCB->setEnabled(false);
    ui->noSubSlotLbl->setVisible(true);
    ui->okBtn->setEnabled(false);

    int devexId = ui->deviceCB->currentData().toInt();
    int slotNo = ui->slotCB->currentData().toInt();
    int shelf = -1;
    if(startShelf > -1)
        shelf = ui->shelfCB->currentData().toInt();
    if(ui->slotCB->currentData() == -2)
    {
        checkEntry();
        return;
    }

    QList<int> list = dbMan->getDevexDistinctSubslots(devexId,shelf,slotNo);
    if(list.count() > 0)
    {
        ui->subSlotCB->setVisible(true);
        ui->subSlotCB->setEnabled(true);
        ui->noSubSlotLbl->setVisible(false);
        ui->subSlotCB->blockSignals(true);
        foreach (int i, list)
        {
            ui->subSlotCB->addItem(QString::number(i), i);
        }
        ui->subSlotCB->addItem("All", -2);
        ui->subSlotCB->setCurrentIndex(-1);
        ui->subSlotCB->blockSignals(false);
    }
}

void ReportReserveToCommercialDialog::on_subSlotCB_currentIndexChanged(int index)
{
    index++;
    checkEntry();
}

void ReportReserveToCommercialDialog::on_cancelBtn_clicked()
{
    this->close();
}

void ReportReserveToCommercialDialog::on_okBtn_clicked()
{
    QList<int> IntIds = getInterfacesId();
    if( IntIds.count() > 0 )
    {
        //report
        QString path = QFileDialog::getSaveFileName(this, "Save DaNet Commercial Ports Report As", "","xlsx (*.xlsx)");

        if(!path.isEmpty())
        {
            if(!path.endsWith(".xlsx")) path += ".xlsx";


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

            cond += " );";

            QSqlQuery *query = dbMan->selectViewCommercial(cond);

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

            QString Area, Abbr,Exch,Device,LocalLabel, Interface, InterfaceType,Module,Odf,Pos,Pin;
            int EquType,IntType, ModType, Project,NetType;

            //0`id`,1 `area`,2 `exchange`,3`device`, 4`local Label`, 5`interface_type`, 6`interface`,
            //7`module`, 8`odf`, 9`pos`, 10`pin`, 11`abbr`
            int row = 3;
            while(query->next())
            {
                row++;
                xlsx.setRowHeight(row,25);

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

            this->close();
        }
    }
    else
    {
        ui->okBtn->setEnabled(false);
        QMessageBox::warning(this, "ERROR", "No interfaces were assigned to commercial department.");
    }

}

