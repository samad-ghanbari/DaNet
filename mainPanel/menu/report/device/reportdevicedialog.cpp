#include "reportdevicedialog.h"
#include "ui_reportdevicedialog.h"
#include "lib/database/danetdbman.h"
#include "lib/report/device/ReportDevice.h"
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QStandardItemModel>
#include <QPainter>
#include <QDesktopServices>
#include <QMessageBox>
#include <QFile>
#include <QDir>
#include <QFileDialog>

/*
`slots_id`, `devex_id`, `card_id`, `shelf`, `slot`, `sub_slot`, `card`,
`adsl`, `vdsl`, `shdsl`, `e`, `g`, `g10`, `g40`, `g100`, `purchase`, `description`
*/

#define SLOTS_ID 0
#define DEVEX_ID 1
#define CARD_ID 2
#define SHELF 3
#define SLOT_NO 4
#define SUB_SLOT 5
#define CARD 6
#define ADSL 7
#define VDSL 8
#define SHDSL 9
#define E 10
#define G 11
#define G10 12
#define G40 13
#define G100 14
#define PURCHASE 15
#define DESCRIPTION 16

ReportDeviceDialog::ReportDeviceDialog(QWidget *parent, DanetDbMan *db) :
    QDialog(parent),
    ui(new Ui::ReportDeviceDialog),
    dbMan(db),
    model(new QSqlQueryModel(this)),
    statModel(new QStandardItemModel(this))
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowMinMaxButtonsHint);

    ui->toolBox->setCurrentIndex(0);

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
    ui->typeCB->setCurrentIndex(-1);
    ui->typeCB->blockSignals(false);

    ui->abbrCB->setEnabled(false);
    ui->typeCB->setEnabled(false);
    ui->siteCB->setEnabled(false);
    ui->siteFrame->setEnabled(false);
    ui->siteFrame->setVisible(false);
    ui->deviceCB->setEnabled(false);

    //slot
    ui->reportTV->setModel(model);
    ui->reportTV->verticalHeader()->hide();
    ui->reportTV->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->reportTV->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    //stat
    ui->statTV->setModel(statModel);
    ui->statTV->verticalHeader()->hide();
    ui->statTV->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->statTV->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    int workingArea = dbMan->getLoggedInDefaultArea();
    ui->areaCB->setCurrentIndex(ui->areaCB->findData(workingArea));
    ui->nmsLbl->setVisible(false);
    ui->pppoeLbl->setVisible(false);

    ui->paperCB->addItem("A0", 0);
    ui->paperCB->addItem("A1", 1);
    ui->paperCB->addItem("A2", 2);
    ui->paperCB->addItem("A3", 3);
    ui->paperCB->addItem("A4", 4);
    ui->paperCB->addItem("A5", 5);
    ui->paperCB->setCurrentIndex(ui->paperCB->findData(3));

    ui->orientationCB->addItem("Landscape", 0);
    ui->orientationCB->addItem("Portrait", 1);
    ui->orientationCB->setCurrentIndex(ui->orientationCB->findData(1));

    ui->fontStyleCB->addItem("Normal", 0);
    ui->fontStyleCB->addItem("Bold", 1);
    ui->fontStyleCB->setCurrentText("Bold");
}

ReportDeviceDialog::~ReportDeviceDialog()
{
    delete ui;
}

void ReportDeviceDialog::fillReportTV(const int devexId)
{
    model->clear();
    QSqlQuery *query = dbMan->selectViewSlots(devexId);


    bool SHELF_flag = false, SUBSLOT_flag=false,ADSL_flag=false,VDSL_flag=false, SHDSL_flag=false,E_flag=false,G_flag=false,G10_flag=false,G40_flag=false,G100_flag=false;

    while(query->next())
    {
        SHELF_flag = SHELF_flag || (!query->value(SHELF).isNull());
        SUBSLOT_flag = SUBSLOT_flag || (!query->value(SUB_SLOT).isNull());
        ADSL_flag = ADSL_flag || (query->value(ADSL).toInt() > 0);
        VDSL_flag = VDSL_flag || (query->value(VDSL).toInt() > 0);
        SHDSL_flag = SHDSL_flag || (query->value(SHDSL).toInt() > 0);
        E_flag = E_flag || (query->value(E).toInt() > 0);
        G_flag = G_flag || (query->value(G).toInt() > 0);
        G10_flag = G10_flag || (query->value(G10).toInt() > 0);
        G40_flag = G40_flag || (query->value(G40).toInt() > 0);
        G100_flag = G100_flag || (query->value(G100).toInt() > 0);

    }


    model->setQuery(*query);


    model->setHeaderData(SHELF,Qt::Horizontal,"Shelf\nNo");
    model->setHeaderData(SLOT_NO,Qt::Horizontal,"Slot\nNo");
    model->setHeaderData(SUB_SLOT,Qt::Horizontal,"Sub-Slot\nNo");
    model->setHeaderData(CARD,Qt::Horizontal,"Card");
    model->setHeaderData(ADSL,Qt::Horizontal,"ADSL\nPort");
    model->setHeaderData(VDSL,Qt::Horizontal,"VDSL\nPort");
    model->setHeaderData(SHDSL,Qt::Horizontal,"SHDSL\nPort");
    model->setHeaderData(E,Qt::Horizontal,"E\nPort");
    model->setHeaderData(G,Qt::Horizontal,"1G\nPort");
    model->setHeaderData(G10,Qt::Horizontal,"10G\nPort");
    model->setHeaderData(G40,Qt::Horizontal,"40G\nPort");
    model->setHeaderData(G100,Qt::Horizontal,"100G\nPort");
    model->setHeaderData(PURCHASE,Qt::Horizontal,"Purchase");
    model->setHeaderData(DESCRIPTION,Qt::Horizontal,"Card\nDescription");

    ui->reportTV->hideColumn(SLOTS_ID);
    ui->reportTV->hideColumn(DEVEX_ID);
    ui->reportTV->hideColumn(CARD_ID);

    if(SHELF_flag)
        ui->reportTV->showColumn(SHELF);
    else
        ui->reportTV->hideColumn(SHELF);

    if(SUBSLOT_flag)
        ui->reportTV->showColumn(SUB_SLOT);
    else
        ui->reportTV->hideColumn(SUB_SLOT);

    if(ADSL_flag)
        ui->reportTV->showColumn(ADSL);
    else
        ui->reportTV->hideColumn(ADSL);

    if(VDSL_flag)
        ui->reportTV->showColumn(VDSL);
    else
        ui->reportTV->hideColumn(VDSL);

    if(SHDSL_flag)
        ui->reportTV->showColumn(SHDSL);
    else
        ui->reportTV->hideColumn(SHDSL);

    if(E_flag)
        ui->reportTV->showColumn(E);
    else
        ui->reportTV->hideColumn(E);

    if(G_flag)
        ui->reportTV->showColumn(G);
    else
        ui->reportTV->hideColumn(G);

    if(G10_flag)
        ui->reportTV->showColumn(G10);
    else
        ui->reportTV->hideColumn(G10);

    if(G40_flag)
        ui->reportTV->showColumn(G40);
    else
        ui->reportTV->hideColumn(G40);

    if(G100_flag)
        ui->reportTV->showColumn(G100);
    else
        ui->reportTV->hideColumn(G100);


    fillStatTV(devexId);
}

void ReportDeviceDialog::fillStatTV(const int devexId)
{
    statModel->clear();
    statModel->setColumnCount(19);
    QList<int> portStat;
    portStat = dbMan->getDevexPortStat(devexId);
    // totalG100 << usedG100 <<(totalG100-usedG100) <<totalG40 << usedG40 <<(totalG40-usedG40) << totalG10 << usedG10 <<(totalG10-usedG10) << totalG << usedG <<(totalG-usedG) << totalE << usedE <<(totalE-usedE) << totalAdsl << totalVdsl << totalShdsl;

    QStandardItem *G100Item = new QStandardItem("100 G");
    G100Item->setBackground(QColor(210, 210, 255));
    QStandardItem *G40Item = new QStandardItem("40 G");
    G40Item->setBackground(QColor(150, 180, 255));
    QStandardItem *G10Item = new QStandardItem("10 G");
    G10Item->setBackground(QColor(160, 210, 255));
    QStandardItem *GItem = new QStandardItem("1 G");
    GItem->setBackground(QColor(200, 180, 255));
    QStandardItem *eItem = new QStandardItem("E");
    eItem->setBackground(QColor(170, 190, 240));
    QStandardItem *adslItem = new QStandardItem("ADSL");
    adslItem->setBackground(QColor(200, 255, 200));
    QStandardItem *vdslItem = new QStandardItem("VDSL");
    vdslItem->setBackground(QColor(220, 255, 255));
    QStandardItem *shdslItem = new QStandardItem("SHDSL");
    shdslItem->setBackground(QColor(230, 255, 230));

    //100G
    statModel->insertRow(0);
    statModel->setItem(0,0, G100Item);
    QModelIndex index = statModel->index(0,1);
    statModel->setData(index, portStat.at(0));
    index = index.sibling(0, 2);
    statModel->setData(index, portStat.at(1));
    index = index.sibling(0, 3);
    statModel->setData(index, portStat.at(2));
    //40G
    statModel->insertRow(1);
    statModel->setItem(1,0, G40Item);
    index = index.sibling(1, 4);
    statModel->setData(index, portStat.at(3));
    index = index.sibling(1, 5);
    statModel->setData(index, portStat.at(4));
    index = index.sibling(1, 6);
    statModel->setData(index, portStat.at(5));
    // 10G
    statModel->insertRow(2);
    statModel->setItem(2,0, G10Item);
    index = index.sibling(2, 7);
    statModel->setData(index, portStat.at(6));
    index = index.sibling(2, 8);
    statModel->setData(index, portStat.at(7));
    index = index.sibling(2, 9);
    statModel->setData(index, portStat.at(8));
    //1G
    statModel->insertRow(3);
    statModel->setItem(3,0, GItem);
    index = index.sibling(3, 10);
    statModel->setData(index, portStat.at(9));
    index = index.sibling(3, 11);
    statModel->setData(index, portStat.at(10));
    index = index.sibling(3, 12);
    statModel->setData(index, portStat.at(11));
    // E
    statModel->insertRow(4);
    statModel->setItem(4,0, eItem);
    index = index.sibling(4, 13);
    statModel->setData(index, portStat.at(12));
    index = index.sibling(4, 14);
    statModel->setData(index, portStat.at(13));
    index = index.sibling(4, 15);
    statModel->setData(index, portStat.at(14));
    // ADSL
    statModel->insertRow(5);
    statModel->setItem(5,0, adslItem);
    index = index.sibling(5, 16);
    statModel->setData(index, portStat.at(15));
    //VDSL
    statModel->insertRow(6);
    statModel->setItem(6,0, vdslItem);
    index = index.sibling(6, 17);
    statModel->setData(index, portStat.at(16));
    //SHDSL
    statModel->insertRow(7);
    statModel->setItem(7,0, shdslItem);
    index = index.sibling(7, 18);
    statModel->setData(index, portStat.at(17));

    statModel->setHeaderData(0, Qt::Horizontal, "port\nType");

    statModel->setHeaderData(1, Qt::Horizontal, "Total 100G\nPorts");
    statModel->setHeaderData(2, Qt::Horizontal, "Used 100G\nPorts");
    statModel->setHeaderData(3, Qt::Horizontal, "Free 100G\nPorts");

    statModel->setHeaderData(4, Qt::Horizontal, "Total 40G\nPorts");
    statModel->setHeaderData(5, Qt::Horizontal, "Used 40G\nPorts");
    statModel->setHeaderData(6, Qt::Horizontal, "Free 40G\nPorts");

    statModel->setHeaderData(7, Qt::Horizontal, "Total 10G\nPorts");
    statModel->setHeaderData(8, Qt::Horizontal, "Used 10G\nPorts");
    statModel->setHeaderData(9, Qt::Horizontal, "Free 10G\nPorts");

    statModel->setHeaderData(10, Qt::Horizontal, "Total 1G\nPorts");
    statModel->setHeaderData(11, Qt::Horizontal, "Used 1G\nPorts");
    statModel->setHeaderData(12, Qt::Horizontal, "Free 1G\nPorts");

    statModel->setHeaderData(13, Qt::Horizontal, "Total E\nPorts");
    statModel->setHeaderData(14, Qt::Horizontal, "Used E\nPorts");
    statModel->setHeaderData(15, Qt::Horizontal, "Free E\nPorts");

    statModel->setHeaderData(16, Qt::Horizontal, "Total ADSL\nPorts");
    statModel->setHeaderData(17, Qt::Horizontal, "Total VDSL\nPorts");
    statModel->setHeaderData(18, Qt::Horizontal, "Total SHDSL\nPorts");

    if(portStat.at(0) == 0)
    {
        ui->statTV->hideColumn(1);
        ui->statTV->hideColumn(2);
        ui->statTV->hideColumn(3);
        ui->statTV->hideRow(0);
    }

    if(portStat.at(3) == 0)
    {
        ui->statTV->hideColumn(4);
        ui->statTV->hideColumn(5);
        ui->statTV->hideColumn(6);
        ui->statTV->hideRow(1);
    }

    if(portStat.at(6) == 0)
    {
        ui->statTV->hideColumn(7);
        ui->statTV->hideColumn(8);
        ui->statTV->hideColumn(9);
        ui->statTV->hideRow(2);
    }

    if(portStat.at(9) == 0)
    {
        ui->statTV->hideColumn(10);
        ui->statTV->hideColumn(11);
        ui->statTV->hideColumn(12);
        ui->statTV->hideRow(3);
    }

    if(portStat.at(12) == 0)
    {
        ui->statTV->hideColumn(13);
        ui->statTV->hideColumn(14);
        ui->statTV->hideColumn(15);
        ui->statTV->hideRow(4);
    }

    if(portStat.at(15) == 0)
    {
        ui->statTV->hideColumn(16);
        ui->statTV->hideRow(5);
    }

    if(portStat.at(16) == 0)
    {
        ui->statTV->hideColumn(17);
        ui->statTV->hideRow(6);
    }

    if(portStat.at(17) == 0)
    {
        ui->statTV->hideColumn(18);
        ui->statTV->hideRow(7);
    }

}

void ReportDeviceDialog::on_cancelBtn_clicked()
{
    this->close();
}

void ReportDeviceDialog::on_okBtn_clicked()
{
    //

    int exchId = ui->abbrCB->currentData().toInt();
    if(ui->typeCB->currentData().toInt() == 3)
        exchId = ui->siteCB->currentData().toInt();

    QString customINfo = ui->customInfoLE->text().trimmed();

    QList<QString> exchInfo = dbMan->getExchangeInfo(exchId);//abbr-exch-add

    int devexId = ui->deviceCB->currentData().toInt();
    QString device = ui->deviceLbl->text();
    QString devexName = ui->deviceCB->currentText();
    QString saloon = ui->saloonLbl->text();

    int ps = ui->paperCB->currentData().toInt();
    int orient = ui->orientationCB->currentData().toInt();
    QFont font;
    font.setBold(ui->fontStyleCB->currentData().toInt());
    font.setPixelSize(ui->fontSizeSB->value());
    font.setFamily("Tahoma");

    QString path = QFileDialog::getSaveFileName(this, "Save DaNet Device Report As", "","pdf (*.pdf)");
    if(!path.isEmpty())
    {

        if(!path.endsWith(".pdf")) path += ".pdf";


        QList<int> slotCountStart = dbMan->getDevexSlotCountStarts(devexId);
        int slotCount, startSlot;
        if(slotCountStart.count() == 4)
        {
            slotCount = slotCountStart.at(0);
            startSlot = slotCountStart.at(2);
        }
        else
        {
            slotCount = -1;
            startSlot = -1;
        }
        //in case of dslam > get racks nms ip and vlans
        bool dslamDevice = dbMan->isDevexDslam(devexId);
        if(dslamDevice)
        {//dslam
            QMap<int, int> dslamIds = dbMan->getDslamRackDevexId(devexId);
            QList<QString> dslamNames = dbMan->getDslamRackName(dslamIds);
            QList<QString> dslamNms = dbMan->getDslamRackMIp(dslamIds);
            QList<QString> dslampppoe = dbMan->getDslamRackPppoe(dslamIds);

            QSqlQueryModel *allModel = new QSqlQueryModel(this);

            QSqlQuery *query = dbMan->selectViewSlots(dslamIds);
            allModel->setQuery(*query);

            if(customINfo.isEmpty())
            {
                ReportDevice reportdevice(this, allModel,device,slotCount,startSlot,dslamIds, dslamNames, dslamNms,dslampppoe, exchInfo,saloon, path,ps, orient,font);
                if(reportdevice.print())
                {
                    QDesktopServices::openUrl(path);
                    ui->deviceCB->setCurrentIndex(-1);
                }
                else
                {
                    QMessageBox::warning(this, "Error", reportdevice.lastError());
                }
            }
            else
            {
                ReportDevice reportdevice(this, allModel,device,slotCount,startSlot,dslamIds, dslamNames, dslamNms,dslampppoe, exchInfo,saloon, path,ps, orient,font, customINfo);
                if(reportdevice.print())
                {
                    QDesktopServices::openUrl(path);
                    ui->deviceCB->setCurrentIndex(-1);
                }
                else
                {
                    QMessageBox::warning(this, "Error", reportdevice.lastError());
                }
            }

        }
        else
        {//switch/router

            if(customINfo.isEmpty())
            {
                ReportDevice reportdevice(this, model,device,slotCount,startSlot,devexId, devexName, exchInfo,saloon,path,ps, orient,font);
                if(reportdevice.print())
                {
                    QDesktopServices::openUrl(path);
                    ui->deviceCB->setCurrentIndex(-1);
                }
                else
                {
                    QMessageBox::warning(this, "Error", reportdevice.lastError());
                }
            }
            else
            {
                ReportDevice reportdevice(this, model,device,slotCount,startSlot,devexId, devexName, exchInfo,saloon,path,ps, orient,font, customINfo);
                if(reportdevice.print())
                {
                    QDesktopServices::openUrl(path);
//                    ui->deviceCB->setCurrentIndex(-1);
//                    printFrameFlag = true;
//                    ui->printFrame->setVisible(false);
                }
                else
                {
                    QMessageBox::warning(this, "Error", reportdevice.lastError());
                }
            }


        }

        this->close();
    }

}

void ReportDeviceDialog::on_areaCB_currentIndexChanged(int index)
{
    model->clear();
    ui->nmsLbl->setVisible(false);
    ui->pppoeLbl->setVisible(false);
    statModel->clear();
    ui->abbrCB->blockSignals(true);
    ui->abbrCB->clear();
    ui->saloonLbl->clear();
    ui->deviceLbl->clear();
    ui->toolBox->setCurrentIndex(0);

    ui->typeCB->blockSignals(true);
    ui->typeCB->setCurrentIndex(-1);
    ui->typeCB->setEnabled(false);
    ui->typeCB->blockSignals(false);

    ui->siteCB->blockSignals(true);
    ui->siteCB->clear();
    ui->siteCB->blockSignals(false);
    ui->siteFrame->setEnabled(false);
    ui->siteFrame->setVisible(false);

    ui->deviceCB->clear();
    ui->deviceCB->setEnabled(false);

    ui->okBtn->setEnabled(false);

    int area = index + 2;
    QSqlQuery *query = dbMan->selectExchanges(area);
    int id;
    QString abbr;
    while(query->next())
    {
        id = query->value(0).toInt();
        abbr = query->value(1).toString();
        ui->abbrCB->addItem(abbr, id);
    }

    ui->abbrCB->setCurrentIndex(-1);
    if(ui->abbrCB->count() > 0)
        ui->abbrCB->setEnabled(true);

    ui->abbrCB->blockSignals(false);


}

void ReportDeviceDialog::on_abbrCB_currentIndexChanged(int index)
{
    index++;
    ui->nmsLbl->setVisible(false);
    ui->pppoeLbl->setVisible(false);
    ui->saloonLbl->clear();
    ui->deviceLbl->clear();
    ui->typeCB->setCurrentIndex(-1);
    ui->typeCB->setEnabled(true);
    model->clear();
    statModel->clear();
    ui->toolBox->setCurrentIndex(0);

    ui->siteCB->blockSignals(true);
    ui->siteCB->clear();
    ui->siteCB->blockSignals(false);
    ui->siteFrame->setEnabled(false);
    ui->siteFrame->setVisible(false);

    ui->deviceCB->clear();
    ui->deviceCB->setEnabled(false);

    ui->okBtn->setEnabled(false);

    ui->typeCB->setCurrentIndex(0);
}

void ReportDeviceDialog::on_typeCB_currentIndexChanged(int index)
{
    ui->saloonLbl->clear();
    ui->deviceLbl->clear();
    ui->nmsLbl->setVisible(false);
    ui->pppoeLbl->setVisible(false);
    model->clear();
    statModel->clear();
    ui->okBtn->setEnabled(false);
    ui->toolBox->setCurrentIndex(0);

    if(index == 1) // site
    {
        ui->siteFrame->setEnabled(true);
        ui->siteFrame->setVisible(true);
        ui->siteCB->setEnabled(true);
        ui->siteCB->blockSignals(true);
        ui->siteCB->clear();

        ui->deviceCB->clear();
        ui->deviceCB->setEnabled(true);

        QString mother = ui->abbrCB->currentText();
        int area = ui->areaCB->currentData().toInt();
        QSqlQuery *query = dbMan->selectSites(area, mother);
        //`id`,`abbr`, `exchange`, `site_cascade`, `site_node`
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
    else // exch
    {
        ui->siteCB->blockSignals(true);
        ui->siteCB->clear();
        ui->siteCB->blockSignals(false);
        ui->siteFrame->setEnabled(false);
        ui->siteFrame->setVisible(false);

        ui->deviceCB->setEnabled(true);
        ui->deviceCB->blockSignals(true);
        ui->deviceCB->clear();
        devexIdDeviceId.clear();
        devexIdSaloonNo.clear();

        if(ui->typeCB->currentData().toInt() == 2) //exch
        {
            int exchId = ui->abbrCB->currentData().toInt();

            QSqlQuery *query = dbMan->selectDevex(exchId);
            // `id`, `exchange_id`, `saloon`, `device_id`, `rack_row`, `rack_no`, `shelf_no`, `device_name`, type,purchase,sn

            int DEVEXID,DEVICEID,SALOON;
            QString DEVICENAME, pur, t,sn,dvType0, dvType1;
            while(query->next())
            {
                DEVEXID = query->value(0).toInt();
                SALOON = query->value(2).toInt();
                DEVICEID = query->value(3).toInt();
                DEVICENAME = query->value(7).toString();
                pur = query->value(9).toString();
                dvType1 = query->value(11).toString();
                if(dvType1.contains("AGG", Qt::CaseInsensitive))
                    dvType1 = "AGG";
                if(dvType1.contains("BRAS", Qt::CaseInsensitive))
                    dvType1 = "BRAS";
                if(dvType0.isEmpty())
                    dvType0 = dvType1;
                if((dvType0 != dvType1))
                {
                    ui->deviceCB->insertSeparator(ui->deviceCB->count());
                    dvType0 = dvType1;
                }

                if(query->value(8).isNull() || query->value(10).isNull())
                {
                    ui->deviceCB->addItem(DEVICENAME,DEVEXID);
                }
                else
                {
                    t = query->value(8).toString();
                    sn = query->value(10).toString();
                    DEVICENAME += "-"+t+"-"+pur+"-"+sn;
                    ui->deviceCB->addItem(DEVICENAME,DEVEXID);
                }

                devexIdDeviceId.insert(DEVEXID,DEVICEID);
                devexIdSaloonNo.insert(DEVEXID, SALOON);
            }


            ui->deviceCB->setCurrentIndex(-1);
            ui->deviceCB->blockSignals(false);
        }

    }
}

void ReportDeviceDialog::on_siteCB_currentIndexChanged(int index)
{
    index++;
    ui->saloonLbl->clear();
    ui->deviceLbl->clear();
    ui->nmsLbl->setVisible(false);
    ui->pppoeLbl->setVisible(false);
    model->clear();
    statModel->clear();
    int exchId = ui->siteCB->currentData().toInt();
    QSqlQuery *query = dbMan->selectDevex(exchId);
    //`id`, `exchange_id`, `saloon`, `device_id`, `rack_row`, `rack_no`, `shelf_no`, `device_name`
    ui->deviceCB->setEnabled(true);
    ui->deviceCB->blockSignals(true);
    ui->deviceCB->clear();
    ui->toolBox->setCurrentIndex(0);

    devexIdDeviceId.clear();
    devexIdSaloonNo.clear();

    ui->okBtn->setEnabled(false);

    int DEVEXID,DEVICEID, SALOON;
    QString DEVICENAME, TYPE,SN,PUR;
    while(query->next())
    {
        DEVEXID = query->value(0).toInt();
        SALOON = query->value(2).toInt();
        DEVICEID = query->value(3).toInt();
        DEVICENAME = query->value(7).toString();

        TYPE = query->value(8).toString();
        PUR = query->value(9).toString();
        SN = query->value(10).toString();

        if(query->value(10).isNull())
            ui->deviceCB->addItem(DEVICENAME, DEVEXID);
        else
            ui->deviceCB->addItem(DEVICENAME+"-"+TYPE+"-"+PUR+"-"+SN, DEVEXID);

        devexIdDeviceId.insert(DEVEXID,DEVICEID);
        devexIdSaloonNo.insert(DEVEXID, SALOON);
    }

    ui->deviceCB->setCurrentIndex(-1);
    ui->deviceCB->blockSignals(false);
}

void ReportDeviceDialog::on_deviceCB_currentIndexChanged(int index)
{
    ui->saloonLbl->clear();
    ui->deviceLbl->clear();
    ui->nmsLbl->setVisible(false);
    ui->pppoeLbl->setVisible(false);
    model->clear();
    statModel->clear();
    ui->toolBox->setCurrentIndex(0);

    if(index > -1)
    {
        ui->okBtn->setEnabled(true);
        int devexId = ui->deviceCB->currentData().toInt();
        int deviceId = devexIdDeviceId.value(devexId);
        int saloonNo = devexIdSaloonNo.value(devexId);

        ui->saloonLbl->setText(dbMan->getSaloonName(saloonNo)+" Saloon");

        QString device = dbMan->getDeviceName(deviceId);
        ui->deviceLbl->setText(device);
        fillReportTV(devexId);

        bool dslamFlag = dbMan->isDeviceDslam(deviceId);
        if(dslamFlag)
        {
            QString nms = dbMan->getDslamMplsNMSIp(devexId);
            QList<int> pppoeList = dbMan->getDslamPppoeVlanList(devexId);
            QString vlanString = dbMan->listToRange(pppoeList);


            if(nms.isEmpty())
                ui->nmsLbl->setVisible(false);
            else
            {
                ui->nmsLbl->setVisible(true);
                ui->nmsLbl->setText("NMS IP: "+nms);
            }

            if(vlanString.isEmpty())
                ui->pppoeLbl->setVisible(false);
            else
            {
                ui->pppoeLbl->setVisible(true);
                ui->pppoeLbl->setText("PPPOE VLAN: "+vlanString);
            }
        }
    }
    else
    {
        ui->okBtn->setEnabled(false);
        //clear paint
    }

}

