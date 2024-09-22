#include "devicesstatdialog.h"
#include "ui_devicesstatdialog.h"
#include <QSqlQuery>
#include <QStandardItemModel>
#include "lib/database/danetdbman.h"
#include <QBrush>
#include <QDesktopServices>
#include <QFileDialog>
#include <QtXlsx>

DevicesStatDialog::DevicesStatDialog(QWidget *parent, DanetDbMan *db) :
    QDialog(parent),
    ui(new Ui::DevicesStatDialog),
    dbMan(db),
    model(new QStandardItemModel(this))
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowMinMaxButtonsHint);

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


}

DevicesStatDialog::~DevicesStatDialog()
{
    delete ui;
}

void DevicesStatDialog::on_CancelBtn_clicked()
{
    this->close();
}

void DevicesStatDialog::on_exportBtn_clicked()
{
    QString path = QFileDialog::getSaveFileName(this, "Save DaNet Devices Statistics As", "","xlsx (*.xlsx)");
    if(!path.isEmpty())
    {
        if(!path.endsWith(".xlsx"))
            path += ".xlsx";

        path = path.replace(" ", "_");

        //############################### excel file
        //xsl file : fist create then save
        QXlsx::Document xlsx;
        xlsx.addSheet("DaNet Statistics");
        xlsx.deleteSheet("sheet1");

        xlsx.setDocumentProperty("title", "DaNet Devices Status");
        xlsx.setDocumentProperty("subject", "Statistics");
        xlsx.setDocumentProperty("creator", "DaNet Software");
        xlsx.setDocumentProperty("company", "DaNet");
        xlsx.setDocumentProperty("category", "Report");
        xlsx.setDocumentProperty("keywords", "Data Network Statistics Status");
        xlsx.setDocumentProperty("description", "Developer Samad Ghanbari");

        QXlsx::Format format;
        format.setHorizontalAlignment(QXlsx::Format::AlignHCenter);
        format.setVerticalAlignment(QXlsx::Format::AlignVCenter);
        format.setBorderColor(Qt::darkBlue);
        format.setFontBold(true);
        format.setFontSize(14);
        format.setTextWarp(true);
        format.setFontColor(Qt::blue);
        format.setPatternBackgroundColor(QColor(210, 210, 240));
        format.setBorderStyle(QXlsx::Format::BorderDouble);

        xlsx.setRowHeight(1, 40);
        xlsx.mergeCells("A1:O1", format);
        xlsx.write("A1", "DaNet Report");

        format.setFontColor(Qt::darkBlue);
        format.setPatternBackgroundColor(QColor(220, 220, 230));

        xlsx.setRowHeight(2, 40);
        xlsx.mergeCells("A2:O2", format);
        xlsx.write("A2", "Area "+ui->areaCB->currentText()+" DaNet Devices Status");


        //headers
        format.setFontSize(12);
        format.setFontColor(Qt::darkRed);
        format.setPatternBackgroundColor(QColor(200, 220, 230));
        format.setBorderStyle(QXlsx::Format::BorderHair);
        xlsx.setRowHeight(3, 30);
        xlsx.setColumnWidth(1, 8);
        xlsx.write("A3", "Area", format);
        xlsx.setColumnWidth(2, 15);
        xlsx.write("B3", "Abbr", format);
        xlsx.setColumnWidth(3, 25);
        xlsx.write("C3", "Equipment", format);
        xlsx.setColumnWidth(4, 20);
        xlsx.write("D3", "Line Slots Count", format);
        xlsx.setColumnWidth(5, 20);
        xlsx.write("E3", "Empty Line Slots", format);
        xlsx.setColumnWidth(6, 15);
        xlsx.write("F3", "E Ports", format);
        xlsx.setColumnWidth(7, 15);
        xlsx.write("G3", "Free E", format);
        xlsx.setColumnWidth(8, 15);
        xlsx.write("H3", "1G Ports", format);
        xlsx.setColumnWidth(9, 15);
        xlsx.write("I3", "Free 1G", format);
        xlsx.setColumnWidth(10, 15);
        xlsx.write("J3", "10G Ports", format);
        xlsx.setColumnWidth(11, 15);
        xlsx.write("K3", "Free 10G", format);
        xlsx.setColumnWidth(12, 15);
        xlsx.write("L3", "40 Ports", format);
        xlsx.setColumnWidth(13, 15);
        xlsx.write("M3", "Free 40G", format);
        xlsx.setColumnWidth(14, 15);
        xlsx.write("N3", "100G Ports", format);
        xlsx.setColumnWidth(15, 15);
        xlsx.write("O3", "Free 100G", format);


        //records
        //format.setFontBold(false);
        //format.setFontSize(12);
        format.setFontColor(Qt::black);
        format.setPatternBackgroundColor(QColor(255, 255, 255));
        format.setBorderStyle(QXlsx::Format::BorderHair);

        QString area,abbr,equip,slotsCount, emptySlots,Eports,emptyE,Gports, emptyG,G10ports,empty10G,G40ports,empty40G,G100ports,empty100G;
        int row = 3;
        int i;
        QModelIndex index;
        for(i = 0; i < model->rowCount(); i++)
        {
            format.setFontColor(Qt::black);
            row++;
            xlsx.setRowHeight(row,25);

            index = model->index(i, 0);
            area = model->data(index).toString();
            index = index.sibling(i, 1);
            abbr = model->data(index).toString();
            index = index.sibling(i, 2);
            equip = model->data(index).toString();
            index = index.sibling(i, 3);
            slotsCount = model->data(index).toString();
            index = index.sibling(i, 4);
            emptySlots = model->data(index).toString();
            index = index.sibling(i, 5);
            Eports = model->data(index).toString();
            index = index.sibling(i, 6);
            emptyE = model->data(index).toString();
            index = index.sibling(i, 7);
            Gports = model->data(index).toString();
            index = index.sibling(i, 8);
            emptyG = model->data(index).toString();
            index = index.sibling(i, 9);
            G10ports = model->data(index).toString();
            index = index.sibling(i, 10);
            empty10G = model->data(index).toString();
            index = index.sibling(i, 11);
            G40ports = model->data(index).toString();
            index = index.sibling(i, 12);
            empty40G = model->data(index).toString();
            index = index.sibling(i, 13);
            G100ports = model->data(index).toString();
            index = index.sibling(i, 14);
            empty100G = model->data(index).toString();
            index = index.sibling(i, 15);

            xlsx.write("A"+QString::number(row), area, format);
            xlsx.write("B"+QString::number(row), abbr, format);
            xlsx.write("C"+QString::number(row), equip, format);
            xlsx.write("D"+QString::number(row), slotsCount , format);
            xlsx.write("E"+QString::number(row), emptySlots, format);
            format.setFontColor(Qt::darkBlue);
            xlsx.write("F"+QString::number(row), Eports, format);
            xlsx.write("G"+QString::number(row), emptyE, format);
            format.setFontColor(Qt::darkGreen);
            xlsx.write("H"+QString::number(row), Gports, format);
            xlsx.write("I"+QString::number(row), emptyG, format);
            format.setFontColor(Qt::darkRed);
            xlsx.write("J"+QString::number(row), G10ports, format);
            xlsx.write("K"+QString::number(row), empty10G, format);
            format.setFontColor(Qt::darkCyan);
            xlsx.write("L"+QString::number(row), G40ports, format);
            xlsx.write("M"+QString::number(row), empty40G, format);
            format.setFontColor(Qt::darkMagenta);
            xlsx.write("N"+QString::number(row), G100ports, format);
            xlsx.write("O"+QString::number(row), empty100G, format);
        }

        xlsx.saveAs(path);
        QDesktopServices::openUrl(path);
        accept();
    }
}

void DevicesStatDialog::on_areaCB_currentIndexChanged(int index)
{
    model->clear();

    if(index > -1)
    {
        ui->abbrCB->setEnabled(true);
        ui->abbrCB->clear();

        int area = ui->areaCB->currentData().toInt();
        if(area > 1)
        {
            QSqlQuery *query = dbMan->selectAreaAbbreviations(area, true);
            //`id`,`abbr`
            int id;
            QString abbr;
            ui->abbrCB->blockSignals(true);
            while(query->next())
            {
                id = query->value(0).toInt();
                abbr = query->value(1).toString();
                ui->abbrCB->addItem(abbr,id);

            }
            ui->abbrCB->addItem("All", 0);
            ui->abbrCB->setCurrentIndex(-1);
            ui->abbrCB->blockSignals(false);
        }

    }
    else
    {
        ui->abbrCB->setEnabled(false);
        ui->exportBtn->setEnabled(false);
    }
}

void DevicesStatDialog::on_abbrCB_currentIndexChanged(int index)
{
    model->clear();

    if(index > -1)
    {
        model->setColumnCount(15);
        model->setHorizontalHeaderLabels(QStringList()<<"Area"<<"Abbr"<<"Equipment"<<"Line\nSlots Count"<<"Empty\nLine Slolts"
                                         <<"E\nPorts"<<"Free E\nPorts"<<"1G\nPorts"<<"Free 1G\nPorts"<<"10G\nPorts"<<"Free 10G\nPorts"
                                         <<"40G\nPorts"<<"Free 40G\nPorts"<<"100G\nPorts" <<"Free 100G\nPorts");

        ui->TV->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        ui->TV->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
        ui->TV->verticalHeader()->hide();

        int area = ui->areaCB->currentData().toInt();
        int ExchId = ui->abbrCB->currentData().toInt(); // 0:all

        int devexId,exchId,slotCount, row=0;
        int usedSlots,emptySlots, Eports, G1ports, G10ports, G40ports, G100ports, emptyE, emptyG, emptyG10, emptyG40, emptyG100;
        QString deviceName,abbr;
        QModelIndex index;

        QBrush greenBrush, redBrush, blueBrush, cyanBrush,magnetaBrush;
        greenBrush.setColor(Qt::darkGreen);
        redBrush.setColor(Qt::darkRed);
        blueBrush.setColor(Qt::darkBlue);
        cyanBrush.setColor(Qt::darkCyan);
        magnetaBrush.setColor(Qt::darkMagenta);

        QSqlQuery query;

        if(ExchId == 0)
        {
            //all exchanges
            query = *dbMan->selectAreaDevex(area);
            //dx.`id`, e.area, dx.`exchange_id`,e.abbr, dx.`device_id`, dx.`device`,d.slot_count, d.sr

        }
        else
        {
            //selected exchange
            exchId = ui->abbrCB->currentData().toInt();
            query = *dbMan->selectExchangeDevex(exchId);
            //dx.`id`, e.area, dx.`exchange_id`,e.abbr, dx.`device_id`, dx.`device`,d.slot_count, d.sr
        }


        while(query.next())
        {
            row = model->rowCount();
            model->insertRow(row);

            devexId = query.value(0).toInt();
            exchId = query.value(2).toInt();
            abbr = query.value(3).toString();
            deviceName = query.value(5).toString();
            slotCount = query.value(6).toInt();

            index = model->index(row,0);
            model->setData(index,area);

            index = index.sibling(row,1);
            model->setData(index,abbr);

            index = index.sibling(row,2);
            model->setData(index,deviceName);

            index = index.sibling(row,3);
            model->setData(index,slotCount);

            usedSlots = dbMan->getDevexUsedLineSlots(devexId);
            emptySlots = slotCount - usedSlots;
            index = index.sibling(row,4);
            if(emptySlots < 0)
                model->setData(index,QString("-"));
            else
                model->setData(index,emptySlots);

            QList<int> list = dbMan->getDevexPortStat(devexId);
            // totalG100 << usedG100 <<(totalG100-usedG100) <<totalG40 << usedG40 <<(totalG40-usedG40) << totalG10 << usedG10 <<(totalG10-usedG10) << totalG << usedG <<(totalG-usedG) << totalE << usedE <<(totalE-usedE) << totalAdsl << totalVdsl << totalShdsl;
            G100ports = list.at(0);
            emptyG100 = list.at(2);
            G40ports = list.at(3);
            emptyG40 = list.at(5);
            G10ports = list.at(6);
            emptyG10 = list.at(8);
            G1ports = list.at(9);
            emptyG = list.at(11);
            Eports = list.at(12);
            emptyE = list.at(14);

            index = index.sibling(row,5);
            model->setData(index,Eports);
            model->setData(index,greenBrush, Qt::ForegroundRole);

            index = index.sibling(row,6);
            model->setData(index,emptyE);
            model->setData(index,greenBrush, Qt::ForegroundRole);

            index = index.sibling(row,7);
            model->setData(index,G1ports);
            model->setData(index,blueBrush, Qt::ForegroundRole);

            index = index.sibling(row,8);
            model->setData(index,emptyG);
            model->setData(index,blueBrush, Qt::ForegroundRole);

            index = index.sibling(row,9);
            model->setData(index,G10ports);
            model->setData(index,redBrush, Qt::ForegroundRole);

            index = index.sibling(row,10);
            model->setData(index,emptyG10);
            model->setData(index,redBrush, Qt::ForegroundRole);

            index = index.sibling(row,11);
            model->setData(index,G40ports);
            model->setData(index,cyanBrush, Qt::ForegroundRole);

            index = index.sibling(row,12);
            model->setData(index,emptyG40);
            model->setData(index,cyanBrush, Qt::ForegroundRole);

            index = index.sibling(row,13);
            model->setData(index,G100ports);
            model->setData(index,magnetaBrush, Qt::ForegroundRole);

            index = index.sibling(row,14);
            model->setData(index,emptyG100);
            model->setData(index,magnetaBrush, Qt::ForegroundRole);
        }

        ui->TV->setModel(model);
        ui->exportBtn->setEnabled(true);
    }
    else
    {
        ui->exportBtn->setEnabled(false);
    }
}

