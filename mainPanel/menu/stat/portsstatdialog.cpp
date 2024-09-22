#include "portsstatdialog.h"
#include "ui_portsstatdialog.h"
#include <QSqlQuery>
#include "lib/database/danetdbman.h"
#include <QStandardItemModel>

PortsStatDialog::PortsStatDialog(QWidget *parent, DanetDbMan *db) :
    QDialog(parent),
    ui(new Ui::PortsStatDialog),
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
    ui->areaCB->addItem("All", 1);
    ui->areaCB->setCurrentIndex(-1);
    ui->areaCB->blockSignals(false);

    ui->exchFrame->setVisible(false);
    ui->siteFrame->setVisible(false);

    ui->typeCB->blockSignals(true);
    ui->typeCB->addItem("Exchange", 2);
    ui->typeCB->addItem("Site", 3);
    ui->typeCB->blockSignals(false);

    QSqlQuery *query = dbMan->selectDevices(true);
    ui->devCB->blockSignals(true);
    //`id`, `device`, `sr`, `type`, `vendor`,`slot_count`,`start_shelf`,
    //`start_slot`,`start_subslot`
    int id;
    QString dev, type0="", type1="";
    while(query->next())
    {
        id = query->value(0).toInt();
        dev = query->value(1).toString();
        type1 = query->value(3).toString();
        if(type1.contains("AGG", Qt::CaseInsensitive))
            type1 = "AGG";
        if(type1.contains("BRAS", Qt::CaseInsensitive))
            type1 = "BRAS";
        if(type0.isEmpty())
            type0 = type1;

        if(type0.compare(type1) != 0)
        {
            ui->devCB->insertSeparator(ui->devCB->count());
            type0 = type1;
        }

        ui->devCB->addItem(type1+ " " +dev,id);
    }
    ui->devCB->setCurrentIndex(-1);
    ui->devCB->blockSignals(false);
    ui->devCB->setEnabled(false);


    ui->TV->setModel(model);
}

PortsStatDialog::~PortsStatDialog()
{
    delete ui;
}

void PortsStatDialog::on_CancelBtn_clicked()
{
    this->close();
}

void PortsStatDialog::on_areaCB_currentIndexChanged(int index)
{
    index++;
    ui->abbrCB->clear();
    ui->siteCB->clear();
    ui->typeCB->blockSignals(true);
    ui->typeCB->setCurrentIndex(0);
    ui->typeCB->blockSignals(false);
    ui->siteFrame->setVisible(false);
    ui->devCB->setCurrentIndex(-1);
    int area = ui->areaCB->currentData().toInt();
    if(area > 1)
    {
        ui->exchFrame->setVisible(true);
        ui->typeFrame->setVisible(false);
        ui->devCB->setEnabled(false);
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
    else
    {
        ui->exchFrame->setVisible(false);
        ui->devCB->setEnabled(true);
    }
    model->clear();
}

void PortsStatDialog::on_abbrCB_currentIndexChanged(int index)
{
    index++;
    ui->devCB->setCurrentIndex(-1);
    int exchId = ui->abbrCB->currentData().toInt();
    ui->siteCB->clear();
    ui->typeCB->blockSignals(true);
    ui->typeCB->setCurrentIndex(0);
    ui->typeCB->blockSignals(false);
    ui->siteFrame->setVisible(false);
    ui->devCB->setEnabled(true);
    if(exchId == 0)
    {
        //all
        ui->typeFrame->setVisible(false);

    }
    else
    {
        ui->typeFrame->setVisible(true);
    }

    model->clear();
}

void PortsStatDialog::on_typeCB_currentIndexChanged(int index)
{
    model->clear();
    ui->siteCB->clear();
    ui->devCB->setCurrentIndex(-1);

    if(index == 0)
    {
        //exch
        ui->siteFrame->setVisible(false);
        ui->devCB->setEnabled(true);
    }
    else
    {
        //site
        ui->siteFrame->setVisible(true);
        ui->devCB->setEnabled(false);
        int exchId = ui->abbrCB->currentData().toInt();
        int id;
        QString site;
        QSqlQuery *query = dbMan->selectExchangeSites(exchId);
        //`id`, `area`, `exchange`, `abbr`
        ui->siteCB->blockSignals(true);
        while(query->next())
        {
            id = query->value(0).toInt();
            site = query->value(2).toString();
            ui->siteCB->addItem(site, id);
        }
        ui->siteCB->setCurrentIndex(-1);
        ui->siteCB->blockSignals(false);

    }

    model->clear();
}

void PortsStatDialog::on_siteCB_currentIndexChanged(int index)
{
    model->clear();
    ui->devCB->setCurrentIndex(-1);

    if(index > -1)
        ui->devCB->setEnabled(true);
    else
        ui->devCB->setEnabled(false);

    model->clear();
}

void PortsStatDialog::on_devCB_currentIndexChanged(int index)
{
    if(index > -1)
    {
        model->clear();

        int area = ui->areaCB->currentData().toInt();
        int exchId = ui->abbrCB->currentData().toInt();
        if(ui->typeCB->currentData().toInt() == 3)
            exchId = ui->siteCB->currentData().toInt();

        int deviceId = ui->devCB->currentData().toInt();

        if(dbMan->isDeviceDslam(deviceId))
        {
            model->setColumnCount(1);
            model->setRowCount(3);
            model->setHeaderData(0,Qt::Horizontal,"Total Count");

            model->setHeaderData(0,Qt::Vertical,"ADSL");
            model->setHeaderData(1,Qt::Vertical,"SHDSL");
            model->setHeaderData(2,Qt::Vertical,"VDSL");

            int cnt ;
            QModelIndex ind = model->index(0,0);//adsl
            cnt = dbMan->getTotalDslPortCount(area,exchId,deviceId, "ADSL");
            model->setData(ind, cnt);

            ind = ind.sibling(1,0);//shdl
            cnt = dbMan->getTotalDslPortCount(area,exchId,deviceId, "SHDSL");
            model->setData(ind, cnt);

            ind = ind.sibling(2,0);//vdsl
            cnt = dbMan->getTotalDslPortCount(area,exchId,deviceId, "VDSL");
            model->setData(ind, cnt);

            ui->TV->setModel(model);
            ui->TV->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
            ui->TV->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

        }
        else
        {

            model->setColumnCount(3);
            model->setRowCount(5);
            //model->setHeaderData(0,Qt::Horizontal,"Port Type");
            model->setHeaderData(0,Qt::Horizontal,"Total");
            model->setHeaderData(1,Qt::Horizontal,"Used");
            model->setHeaderData(2,Qt::Horizontal,"Free");

            model->setHeaderData(0,Qt::Vertical,"E");
            model->setHeaderData(1,Qt::Vertical,"1G");
            model->setHeaderData(2,Qt::Vertical,"10G");
            model->setHeaderData(3,Qt::Vertical,"40G");
            model->setHeaderData(4,Qt::Vertical,"100G");

            int cnt ;
            QModelIndex ind = model->index(0,0);
            //E
            ind = ind.sibling(0,0);
            cnt = dbMan->getTotalPortCount(area,exchId,deviceId, "RJ45");
            model->setData(ind, cnt);
            ind = ind.sibling(0,1);
            cnt = dbMan->getUsedPortCount(area,exchId,deviceId, "RJ45");
            model->setData(ind, cnt);
            ind = ind.sibling(0,2);
            cnt = dbMan->getFreePortCount(area,exchId,deviceId, "RJ45");
            model->setData(ind, cnt);
            //1G
            ind = ind.sibling(1,0);
            cnt = dbMan->getTotalPortCount(area,exchId,deviceId, "1G");
            model->setData(ind, cnt);
            ind = ind.sibling(1,1);
            cnt = dbMan->getUsedPortCount(area,exchId,deviceId, "1G");
            model->setData(ind, cnt);
            ind = ind.sibling(1,2);
            cnt = dbMan->getFreePortCount(area,exchId,deviceId, "1G");
            model->setData(ind, cnt);
            //10G
            ind = ind.sibling(2,0);
            cnt = dbMan->getTotalPortCount(area,exchId,deviceId, "10G");
            model->setData(ind, cnt);
            ind = ind.sibling(2,1);
            cnt = dbMan->getUsedPortCount(area,exchId,deviceId, "10G");
            model->setData(ind, cnt);
            ind = ind.sibling(2,2);
            cnt = dbMan->getFreePortCount(area,exchId,deviceId, "10G");
            model->setData(ind, cnt);
            //40G
            ind = ind.sibling(3,0);
            cnt = dbMan->getTotalPortCount(area,exchId,deviceId, "40G");
            model->setData(ind, cnt);
            ind = ind.sibling(3,1);
            cnt = dbMan->getUsedPortCount(area,exchId,deviceId, "40G");
            model->setData(ind, cnt);
            ind = ind.sibling(3,2);
            cnt = dbMan->getFreePortCount(area,exchId,deviceId, "40G");
            model->setData(ind, cnt);
            //100G
            ind = ind.sibling(4,0);
            cnt = dbMan->getTotalPortCount(area,exchId,deviceId, "100G");
            model->setData(ind, cnt);
            ind = ind.sibling(4,1);
            cnt = dbMan->getUsedPortCount(area,exchId,deviceId, "100G");
            model->setData(ind, cnt);
            ind = ind.sibling(4,2);
            cnt = dbMan->getFreePortCount(area,exchId,deviceId, "100G");
            model->setData(ind, cnt);


            ui->TV->setModel(model);
            ui->TV->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
            ui->TV->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        }
    }
    else
        model->clear();
}

