#include "exchangesitesreportdialog.h"
#include "ui_exchangesitesreportdialog.h"
#include "lib/database/danetdbman.h"
#include "lib/report/sites/ReportSites.h"
#include <QSqlQueryModel>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QFileDialog>
#include <QDir>
#include <QPrinter>
#include <QPainter>
#include <QDesktopServices>
#include <QMessageBox>

#include <QFontDatabase>

ExchangeSitesReportDialog::ExchangeSitesReportDialog(QWidget *parent, DanetDbMan *db) :
    QDialog(parent),
    ui(new Ui::ExchangeSitesReportDialog),
    dbMan(db),
    treeModel(new QStandardItemModel(this)),
    tableModel(new QStandardItemModel(this))
{
    ui->setupUi(this);

    QFontDatabase::addApplicationFont(":/font/tahoma.ttf");
    QFontDatabase::addApplicationFont(":/font/tahomabd.ttf");

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

    ui->paperCB->addItem("A0", 0);
    ui->paperCB->addItem("A1", 1);
    ui->paperCB->addItem("A2", 2);
    ui->paperCB->addItem("A3", 3);
    ui->paperCB->addItem("A4", 4);
    ui->paperCB->addItem("A5", 5);
    ui->paperCB->setCurrentIndex(ui->paperCB->findData(3));
    ui->compressSB->setValue(100);

    ui->orientationCB->addItem("Landscape", 0);
    ui->orientationCB->addItem("Portrait", 1);
    ui->orientationCB->setCurrentIndex(ui->orientationCB->findData(0));

    ui->fontStyleCB->addItem("Normal", 0);
    ui->fontStyleCB->addItem("Bold", 1);
    ui->fontStyleCB->setCurrentText("Normal");

    int workingArea = dbMan->getLoggedInDefaultArea();
    ui->areaCB->setCurrentIndex(ui->areaCB->findData(workingArea));

    ui->reportTable->verticalHeader()->hide();
    ui->reportTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->reportTable->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    //treeView
    QFont font;
    font.setFamily("tahoma");
    font.setPointSize(12);
    font.setBold(true);

    ui->reportTree->setFont(font);
    ui->reportTree->setModel(treeModel);
    ui->reportTree->setSelectionMode(QAbstractItemView::NoSelection);
    ui->reportTree->setEditTriggers(QTreeView::NoEditTriggers);
    ui->reportTree->setContentsMargins(20, 20,20, 20);
    ui->reportTree->setAnimated(true);
    ui->reportTree->setHeaderHidden(true);
    ui->reportTree->setIndentation(40);

    ui->okBtn->setEnabled(false);
}

ExchangeSitesReportDialog::~ExchangeSitesReportDialog()
{
    delete ui;
}

void ExchangeSitesReportDialog::initTreeModel(const QString Mother)
{
    QMap<int,QStandardItem*> items;
    //root
    QStandardItem *root = treeModel->invisibleRootItem();
    QStandardItem *motherItem = new QStandardItem(Mother);
    motherItem->setForeground(Qt::darkBlue);
    QStandardItem *temp;
    root->appendRow(motherItem);
    //`id`, `area`, 2`exchange`, 3`abbr`, `type`, 5`mother_exchange_id`, `mother_exchange_abbr`,
    //7`uplink_exchange_id`, 8`uplink_exchange_abbr`, 9`site_cascade`, 10`site_node`, `address`
    QString exchange, abbr;//motherAbbr, upAbbr;
    int id,motherId,uplinkId,cascade, node;
    QString info, eq, pppoe, ip;

    int r = 0;
    QModelIndex ind = tableModel->index(0,0);
    while(r < tableModel->rowCount())
    {
        ind = ind.sibling(r,0);
        id = tableModel->data(ind).toInt();

        ind = ind.sibling(r,2);
        exchange = tableModel->data(ind).toString();

        ind = ind.sibling(r,3);
        abbr = tableModel->data(ind).toString();

        ind = ind.sibling(r,5);
        motherId = tableModel->data(ind).toInt();

        ind = ind.sibling(r,7);
        uplinkId = tableModel->data(ind).toInt();

        ind = ind.sibling(r,9);
        cascade = tableModel->data(ind).toInt();

        ind = ind.sibling(r,10);
        node = tableModel->data(ind).toInt();

        ind = ind.sibling(r,12);
        eq = tableModel->data(ind).toString();

        ind = ind.sibling(r,13);
        ip = tableModel->data(ind).toString();

        ind = ind.sibling(r,14);
        pppoe = tableModel->data(ind).toString();

        info = eq;
        if(!ip.isEmpty())
            info +="* IP:"+ip;
        if(!pppoe.isEmpty())
            info +="* PPPOE:"+pppoe;

        if(!abbr.isEmpty())
        {
            info = QString::number(cascade)+"-"+QString::number(node)+"   "+abbr+"("+exchange+") " + info ;
            items.insert(id,new QStandardItem(info));

            if(motherId == uplinkId)
            {
                motherItem->appendRow(items.value(id));
                temp = items.value(id);
                temp->setForeground(Qt::darkGreen);
            }
            else
            {
                temp = items.value(uplinkId);
                temp->appendRow(items.value(id));
                int uplinkNode = getNode(uplinkId);
                temp = items.value(id);
                if(uplinkNode == 1)
                    temp->setForeground(Qt::darkMagenta);
                else if(uplinkNode == 2)
                    temp->setForeground(Qt::darkCyan);
                else if(uplinkNode == 3)
                    temp->setForeground(Qt::darkRed);
                else if(uplinkNode == 4)
                    temp->setForeground(Qt::darkYellow);
                else if(uplinkNode == 5)
                    temp->setForeground(Qt::darkGray);
                else if(uplinkNode == 6)
                    temp->setForeground(Qt::blue);
                else if(uplinkNode == 7)
                    temp->setForeground(Qt::green);
                else if(uplinkNode == 8)
                    temp->setForeground(Qt::red);
                else if(uplinkNode == 9)
                    temp->setForeground(Qt::cyan);
                else if(uplinkNode == 10)
                    temp->setForeground(Qt::yellow);
            }

        }
        r++;
    }
}

int ExchangeSitesReportDialog::getNode(const int uplinkId)
{
    int n = -1, u;
    QModelIndex index;
    for(int i = 0; i < tableModel->rowCount(); i++)
    {
        index = tableModel->index(i, 7);
        u = tableModel->data(index).toInt();
        if(u == uplinkId)
        {
            index = index.sibling(index.row(), 10);
            n = tableModel->data(index).toInt();
        }
    }
    return n;
}

void ExchangeSitesReportDialog::on_cancelBtn_clicked()
{
    this->close();
}

void ExchangeSitesReportDialog::on_okBtn_clicked()
{

    QString customInfo = ui->customInfoLE->text().trimmed();
    if(customInfo.isEmpty())
        customInfo="";

    QString motherAbbr = ui->areaCB->currentText()+"-"+ui->abbrCB->currentText();
    QString mother = dbMan->getExchangeName(ui->abbrCB->currentData().toInt());

    QString path = QFileDialog::getSaveFileName(this, "Save DaNet Exchange Sites Report As", "","pdf (*.pdf)");
    if(!path.isEmpty())
    {
        if(!path.endsWith(".pdf")) path += ".pdf";

        int compress = ui->compressSB->value();
        int ps = ui->paperCB->currentData().toInt();
        int orient = ui->orientationCB->currentData().toInt();
        int fontSize = ui->fontSiteSB->value();
        int fontStyle = ui->fontStyleCB->currentData().toInt();
        QFont font;
        font.setBold(fontStyle);
        font.setPointSize(fontSize);
        font.setFamily("Tahoma");

        ReportSites reportSites(this, tableModel , path, motherAbbr, mother, ps, compress, orient, font, customInfo);
        if(reportSites.print())
        {
            QDesktopServices::openUrl(path);
//            tableModel->clear();
//            ui->okBtn->setEnabled(false);
//            printFrameFlag = true;
//            ui->printFrame->setVisible(false);
//            ui->abbrCB->setCurrentIndex(-1);
        }
        else
        {
            QMessageBox::warning(this, "ERROR", reportSites.lastError());
        }

    this->close();

    }

}

void ExchangeSitesReportDialog::on_areaCB_currentIndexChanged(int index)
{
    int area = index + 2;
    tableModel->clear();
    treeModel->clear();
    QSqlQuery *query = dbMan->selectAreaAbbreviations(area,true);
    int exchId;
    QString abbr;
    ui->abbrCB->blockSignals(true);
    ui->abbrCB->clear();

    while(query->next())
    {
        exchId = query->value(0).toInt();
        abbr = query->value(1).toString();
        ui->abbrCB->addItem(abbr, exchId);
    }
    ui->abbrCB->setCurrentIndex(-1);
    ui->abbrCB->blockSignals(false);
    ui->okBtn->setEnabled(false);
}

void ExchangeSitesReportDialog::on_abbrCB_currentIndexChanged(int index)
{
    index++;
    tableModel->clear();
    treeModel->clear();
    ui->toolBox->setCurrentIndex(0);
    if(ui->abbrCB->currentText().isEmpty())
    {
        ui->okBtn->setEnabled(false);
    }
    else
    {
        QString mother = ui->areaCB->currentText() + "-" + ui->abbrCB->currentText();
        int motherId = ui->abbrCB->currentData().toInt();

        QSqlQuery *query = dbMan->selectExchangeSites(motherId);

        int r=0,exchId,dslamId, deviceId;
        QString info;
        QList<int> intList;
        tableModel->setColumnCount(15);//12+3 , device, IP, pppoe
        tableModel->setRowCount(0);
        QModelIndex ind;
        while(query->next())
        {
            //0`id`, 1`area`, 2`exchange`, 3`abbr`, 4`type`, 5`mother_exchange_id`, 6`mother_exchange_abbr`,
            //7`uplink_exchange_id`, 8`uplink_exchange_abbr`, 9`site_cascade`, 10`site_node`, 11`address`
            tableModel->insertRow(r);

            ind = tableModel->index(r,0);
            exchId = query->value(0).toInt();
            tableModel->setData(ind, exchId);

            ind = ind.sibling(r,1);
            tableModel->setData(ind, query->value(1));

            ind = ind.sibling(r,2);
            tableModel->setData(ind, query->value(2));

            ind = ind.sibling(r,3);
            tableModel->setData(ind, query->value(3));

            ind = ind.sibling(r,4);
            tableModel->setData(ind, query->value(4));

            ind = ind.sibling(r,5);
            tableModel->setData(ind, query->value(5));

            ind = ind.sibling(r,6);
            tableModel->setData(ind, query->value(6));

            ind = ind.sibling(r,7);
            tableModel->setData(ind, query->value(7));

            ind = ind.sibling(r,8);
            tableModel->setData(ind, query->value(8));

            ind = ind.sibling(r,9);
            tableModel->setData(ind, query->value(9));

            ind = ind.sibling(r,10);
            tableModel->setData(ind, query->value(10));

            ind = ind.sibling(r,11);
            tableModel->setData(ind, query->value(11));

            r++;
        }
        r = 0;
        while(r < tableModel->rowCount())
        {
            ind = ind.sibling(r,0);
            exchId = tableModel->data(ind).toInt();

            intList = dbMan->getExchangeSiteDslamIds(exchId);
            if(intList.count() > 0)
            {
                dslamId = intList.at(0);
                deviceId = dbMan->getDevexDeviceId(dslamId);
                info = dbMan->getDevice(deviceId, false);
                ind = ind.sibling(r,12);
                tableModel->setData(ind, info);

                info = dbMan->getDslamMplsNMSIp(dslamId);
                ind = ind.sibling(r,13);
                tableModel->setData(ind, info);

                intList = dbMan->getDslamPppoeVlanList(dslamId);
                info = dbMan->listToRange(intList);
                ind = ind.sibling(r,14);
                tableModel->setData(ind, info);
            }
            r++;
        }

        initTreeModel(mother);

        //0`id`, 1`area`, 2`exchange`, 3`abbr`, 4`type`, 5`mother_exchange_id`, 6`mother_exchange_abbr`,
        //7`uplink_exchange_id`, 8`uplink_exchange_abbr`, 9`site_cascade`, 10`site_node`, 11`address`
        tableModel->setHeaderData(2,Qt::Horizontal, "Site Name");
        tableModel->setHeaderData(3,Qt::Horizontal, "Abbr");
        tableModel->setHeaderData(8,Qt::Horizontal, "Uplink Exch/Site\nAbbr");
        tableModel->setHeaderData(9,Qt::Horizontal, "Cascade");
        tableModel->setHeaderData(10,Qt::Horizontal, "Node");
        tableModel->setHeaderData(11,Qt::Horizontal, "Address");
        tableModel->setHeaderData(12,Qt::Horizontal, "Device");
        tableModel->setHeaderData(13,Qt::Horizontal, "IP");
        tableModel->setHeaderData(14,Qt::Horizontal, "PPPOE");

        ui->reportTable->setModel(tableModel);
        ui->reportTable->hideColumn(0);
        ui->reportTable->hideColumn(1);
        ui->reportTable->hideColumn(4);
        ui->reportTable->hideColumn(5);
        ui->reportTable->hideColumn(6);
        ui->reportTable->hideColumn(7);

        ui->reportTree->collapseAll();
        ind = treeModel->index(0,0);
        ui->reportTree->expand(ind);

        ui->okBtn->setEnabled(true);

    }

}
