#include "cardinstalldialog.h"
#include "ui_cardinstalldialog.h"
#include <lib/database/danetdbman.h>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QMessageBox>
#include "mainPanel/menu/installation/card/adddevicecarddialog.h"
#include "mainPanel/menu/installation/card/removedevicecarddialog.h"
#include "mainPanel/menu/installation/card/editcarddescdialog.h"
#include "mainPanel/menu/installation/card/editcardpurchasedialog.h"
#include "mainPanel/menu/installation/card/changedslamcarddialog.h"

#include "mainPanel/deviceWidgets/ASR9010/asr9010widget.h"
#include "mainPanel/deviceWidgets/ASR1013/asr1013widget.h"
#include "mainPanel/deviceWidgets/ASR1002/asr1002widget.h"
#include "mainPanel/deviceWidgets/C7613/c7613widget.h"
#include "mainPanel/deviceWidgets/CX200/cx200widget.h"
#include "mainPanel/deviceWidgets/ME60X8/me60x8widget.h"
#include "mainPanel/deviceWidgets/ME60X16/me60x16widget.h"
#include "mainPanel/deviceWidgets/NE40X8/ne40x8widget.h"
#include "mainPanel/deviceWidgets/NE40X16/ne40x16widget.h"
#include "mainPanel/deviceWidgets/NE5000X16/ne5000X16widget.h"
#include "mainPanel/deviceWidgets/NE5000X16A/ne5000X16Awidget.h"
#include "mainPanel/deviceWidgets/S2403H/s2403hwidget.h"
#include "mainPanel/deviceWidgets/S5300/s5300widget.h"
#include "mainPanel/deviceWidgets/S9303/s9303widget.h"
#include "mainPanel/deviceWidgets/S9306/s9306widget.h"
#include "mainPanel/deviceWidgets/DSLAM4S/dslam4swidget.h"
#include "mainPanel/deviceWidgets/DSLAM16S/dslam16swidget.h"

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


CardInstallDialog::CardInstallDialog(QWidget *parent, DanetDbMan *db) :
    QDialog(parent),
    ui(new Ui::CardInstallDialog),
    dbMan(db),
    slotModel(new QSqlQueryModel(this)),
    deviceModel(new QSqlQueryModel(this))
{
    ui->setupUi(this);
    init();
}

CardInstallDialog::CardInstallDialog(QWidget *parent, DanetDbMan *db, const int area, const int ExchId, const int type, const int SiteId, const int DevexId)  :
    QDialog(parent),
    ui(new Ui::CardInstallDialog),
    dbMan(db),
    slotModel(new QSqlQueryModel(this)),
    deviceModel(new QSqlQueryModel(this))
{
    ui->setupUi(this);
    init();

    ui->areaCB->setCurrentIndex(ui->areaCB->findData(area));
    ui->abbrCB->setCurrentIndex(ui->abbrCB->findData(ExchId));
    ui->typeCB->setCurrentIndex(ui->typeCB->findData(type));
    ui->siteCB->setCurrentIndex(ui->siteCB->findData(SiteId));
    ui->devexCB->setCurrentIndex(ui->devexCB->findData(DevexId));
}

CardInstallDialog::~CardInstallDialog()
{
    delete ui;
    delete slotModel;
    slotModel = nullptr;
}

void CardInstallDialog::init()
{
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowMinMaxButtonsHint);

    ui->siteFrame->setVisible(false);

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
    ui->typeCB->setEnabled(false);

    ui->siteCB->blockSignals(true);
    ui->siteCB->clear();
    ui->siteCB->blockSignals(false);
    ui->siteFrame->setEnabled(false);
    ui->siteFrame->setVisible(false);

    ui->devexCB->blockSignals(true);
    ui->devexCB->clear();
    ui->devexCB->blockSignals(false);
    ui->devexCB->setEnabled(false);

    //slot
    ui->slotsTV->setModel(slotModel);
    ui->slotsTV->verticalHeader()->hide();

    // context menu slot
    ui->slotsTV->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->slotsTV, SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(slotContextMenuSlot(QPoint)));
    slotContextMenu.addAction(ui->actionEdit_Description);
    slotContextMenu.addSeparator();
    slotContextMenu.addAction(ui->actionEdit_Purchase);
    slotContextMenu.addSeparator();
    slotContextMenu.addAction(ui->actionchange_Card_Type);
    slotContextMenu.addSeparator();
    slotContextMenu.addAction(ui->actionRemove_Card);


    int workingArea = dbMan->getLoggedInDefaultArea();
    ui->areaCB->setCurrentIndex(ui->areaCB->findData(workingArea));

    BtnMan(false,false,false);
}

void CardInstallDialog::fillSlotsTV(const int devexId)
{
    /*
`slots_id`, `devex_id`, `card_id`, `shelf`, `slot`, `sub_slot`, `card`,
`adsl`, `vdsl`, `shdsl`, `e`, `g`, `g10`, `g40`, `g100`, `purchase`, `description`
*/

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

    slotModel->setQuery(*query);

    ui->slotsTV->hideColumn(SLOTS_ID);
    ui->slotsTV->hideColumn(DEVEX_ID);
    ui->slotsTV->hideColumn(CARD_ID);

    slotModel->setHeaderData(SHELF,Qt::Horizontal,"Shelf\nNo");
    slotModel->setHeaderData(SLOT_NO,Qt::Horizontal,"Slot\nNo");
    slotModel->setHeaderData(SUB_SLOT,Qt::Horizontal,"Sub-Slot\nNo");
    slotModel->setHeaderData(CARD,Qt::Horizontal,"Card");
    slotModel->setHeaderData(ADSL,Qt::Horizontal,"ADSL\nPort");
    slotModel->setHeaderData(VDSL,Qt::Horizontal,"VDSL\nPort");
    slotModel->setHeaderData(SHDSL,Qt::Horizontal,"SHDSL\nPort");
    slotModel->setHeaderData(E,Qt::Horizontal,"E\nPort");
    slotModel->setHeaderData(G,Qt::Horizontal,"1G\nPort");
    slotModel->setHeaderData(G10,Qt::Horizontal,"10G\nPort");
    slotModel->setHeaderData(G40,Qt::Horizontal,"40G\nPort");
    slotModel->setHeaderData(G100,Qt::Horizontal,"100G\nPort");
    slotModel->setHeaderData(PURCHASE,Qt::Horizontal,"Purchase");
    slotModel->setHeaderData(DESCRIPTION,Qt::Horizontal,"Card\nDescription");

    ui->slotsTV->horizontalHeader()->setSectionResizeMode(CARD,QHeaderView::Stretch);
    ui->slotsTV->horizontalHeader()->setSectionResizeMode(DESCRIPTION,QHeaderView::Stretch);
    ui->slotsTV->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);


    if(SHELF_flag)
        ui->slotsTV->showColumn(SHELF);
    else
        ui->slotsTV->hideColumn(SHELF);

    if(SUBSLOT_flag)
        ui->slotsTV->showColumn(SUB_SLOT);
    else
        ui->slotsTV->hideColumn(SUB_SLOT);

    if(ADSL_flag)
        ui->slotsTV->showColumn(ADSL);
    else
        ui->slotsTV->hideColumn(ADSL);

    if(VDSL_flag)
        ui->slotsTV->showColumn(VDSL);
    else
        ui->slotsTV->hideColumn(VDSL);

    if(SHDSL_flag)
        ui->slotsTV->showColumn(SHDSL);
    else
        ui->slotsTV->hideColumn(SHDSL);

    if(E_flag)
        ui->slotsTV->showColumn(E);
    else
        ui->slotsTV->hideColumn(E);

    if(G_flag)
        ui->slotsTV->showColumn(G);
    else
        ui->slotsTV->hideColumn(G);

    if(G10_flag)
        ui->slotsTV->showColumn(G10);
    else
        ui->slotsTV->hideColumn(G10);

    if(G40_flag)
        ui->slotsTV->showColumn(G40);
    else
        ui->slotsTV->hideColumn(G40);

    if(G100_flag)
        ui->slotsTV->showColumn(G100);
    else
        ui->slotsTV->hideColumn(G100);
}

void CardInstallDialog::BtnMan(bool FillBtn, bool DepleteBtn, bool addBtn)
{

    if(FillBtn)
    {
        ui->fillDslamBtn->setEnabled(true);
        ui->fillDslamBtn->setVisible(true);
    }
    else
    {
        ui->fillDslamBtn->setEnabled(false);
        ui->fillDslamBtn->setVisible(false);
    }

    if(DepleteBtn)
    {
        ui->emptyDslamBtn->setEnabled(true);
        ui->emptyDslamBtn->setVisible(true);
    }
    else
    {
        ui->emptyDslamBtn->setEnabled(false);
        ui->emptyDslamBtn->setVisible(false);
    }

    if(addBtn)
        ui->addCardBtn->setEnabled(true);
    else
        ui->addCardBtn->setEnabled(false);

}

void CardInstallDialog::paintDevice(const QString device)
{
    ui->tabWidget->setTabEnabled(0,true);
    ui->deviceTab->setEnabled(true);
    qDeleteAll(ui->parentWidget->children());

    if(device.contains("ASR9010", Qt::CaseInsensitive))
        drawASR9010();
    else if(device.contains("ASR1013", Qt::CaseInsensitive))
        drawASR1013();
    else if(device.contains("ASR1002", Qt::CaseInsensitive))
        drawASR1002();
    else if(device.contains("7613", Qt::CaseInsensitive))
        drawC7613();
    else if(device.contains("CX200", Qt::CaseInsensitive))
        drawCX200();
    else if(device.contains("ME60X16", Qt::CaseInsensitive))
        drawME60X16();
    else if(device.contains("ME60X8", Qt::CaseInsensitive))
        drawME60X8();
    else if(device.contains("NE40X16", Qt::CaseInsensitive) || device.contains("NE40EX16", Qt::CaseInsensitive))
        drawNE40X16();
    else if(device.contains("NE40X8", Qt::CaseInsensitive) || device.contains("NE40EX8", Qt::CaseInsensitive))
        drawNE40X8();
    else if(device.contains("CX600X16", Qt::CaseInsensitive))
        drawCX600X16();
    else if(device.contains("CX600X8", Qt::CaseInsensitive))
        drawCX600X8();
    else if(device.contains("NE5000X16A", Qt::CaseInsensitive))
        drawNE5000X16A();
    else if(device.contains("NE5000X16", Qt::CaseInsensitive))
        drawNE5000X16();
    else if(device.contains("S2403H", Qt::CaseInsensitive))
        drawS2403H();
    else if(device.contains("S5300", Qt::CaseInsensitive))
        drawS5300();
    else if(device.contains("S9303", Qt::CaseInsensitive))
        drawS9303();
    else if(device.contains("S9306", Qt::CaseInsensitive))
        drawS9306();
    else if(device.contains("MA5300", Qt::CaseInsensitive))
        drawdslam16s("Huawei DSLAM MA5300");
    else if(device.contains("MA5616", Qt::CaseInsensitive))
        drawDslam4s();
    else if(device.contains("MA5600", Qt::CaseInsensitive))
        drawdslam16s("Huawei DSLAM MA5600");
    else if(device.contains("MXK819", Qt::CaseInsensitive))
        drawdslam16s("Zhone DSLAM MXK819");
    else
    {
        ui->tabWidget->setTabEnabled(0,false);
        ui->tabWidget->setCurrentIndex(1);
    }
}

void CardInstallDialog::on_areaCB_currentIndexChanged(int index)
{
    ui->slotFrame->setEnabled(false);
    BtnMan(false, false, false);
    slotModel->clear();
    deviceModel->clear();
    qDeleteAll(ui->parentWidget->children());
    ui->tabWidget->setCurrentIndex(0);

    ui->abbrCB->blockSignals(true);
    ui->abbrCB->clear();

    ui->devexCB->blockSignals(true);
    ui->devexCB->clear();
    ui->devexCB->blockSignals(false);
    ui->devexCB->setEnabled(false);

    ui->typeCB->blockSignals(true);
    ui->typeCB->setCurrentIndex(-1);
    ui->typeCB->blockSignals(false);
    ui->typeCB->setEnabled(false);

    ui->siteCB->blockSignals(true);
    ui->siteCB->clear();
    ui->siteCB->blockSignals(false);
    ui->siteCB->setEnabled(false);
    ui->siteFrame->setEnabled(false);
    ui->siteFrame->setVisible(false);


    int area = index + 2;
    QSqlQuery *query = dbMan->selectExchanges(area);
    //`id`,`abbr`
    int i;
    QString a;
    while(query->next())
    {
        i = query->value(0).toInt();
        a = query->value(1).toString();

        ui->abbrCB->addItem(a, i);
    }

    ui->abbrCB->setCurrentIndex(-1);
    ui->abbrCB->blockSignals(false);
}

void CardInstallDialog::on_abbrCB_currentTextChanged(const QString &arg1)
{
    ui->slotFrame->setEnabled(false);
    slotModel->clear();
    deviceModel->clear();
    qDeleteAll(ui->parentWidget->children());
    ui->tabWidget->setCurrentIndex(0);
    BtnMan(false,false,false);

    ui->devexCB->blockSignals(true);
    ui->devexCB->clear();
    ui->devexCB->blockSignals(false);
    ui->devexCB->setEnabled(false);

    mother = arg1;

    ui->siteCB->blockSignals(true);
    ui->siteCB->clear();
    ui->siteCB->blockSignals(false);
    ui->siteFrame->setEnabled(false);
    ui->siteFrame->setVisible(false);

    ui->typeCB->blockSignals(true);
    ui->typeCB->setCurrentIndex(-1);
    ui->typeCB->blockSignals(false);
    ui->typeCB->setEnabled(true);
    ui->typeCB->setCurrentIndex(0);
}

void CardInstallDialog::on_typeCB_currentIndexChanged(int index)
{
    slotModel->clear();
    deviceModel->clear();
    qDeleteAll(ui->parentWidget->children());
    ui->tabWidget->setCurrentIndex(0);
    BtnMan(false, false, false);
    ui->slotFrame->setEnabled(false);

    ui->siteCB->blockSignals(true);
    ui->siteCB->clear();
    ui->siteFrame->setEnabled(false);
    ui->siteFrame->setVisible(false);

    ui->devexCB->blockSignals(true);
    ui->devexCB->clear();
    ui->devexCB->blockSignals(false);
    ui->devexCB->setEnabled(false);


    if(index == 1) // site
    {
        ui->siteFrame->setEnabled(true);
        ui->siteFrame->setVisible(true);


        int area = ui->areaCB->currentData().toInt();
        mother = ui->abbrCB->currentText();

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
        ui->siteCB->setEnabled(true);
    }
    else // exchange
    {
        ui->siteCB->blockSignals(true);
        ui->siteCB->clear();
        ui->siteCB->blockSignals(false);
        ui->siteFrame->setEnabled(false);
        ui->siteFrame->setVisible(false);

        if(ui->typeCB->currentData().toInt() == 3) //site
            exchSiteId = ui->siteCB->currentData().toInt();
        else
            exchSiteId = ui->abbrCB->currentData().toInt();

        QSqlQuery *query = dbMan->selectBriefDevex(exchSiteId);
        //`id`, `device_name`
        int id;
        ui->devexCB->blockSignals(true);
        QString dev, pur, t,sn, dvType0, dvType1;
        while(query->next())
        {
            id = query->value(0).toInt();
            dev = query->value(1).toString();
            pur = query->value(2).toString();
            dvType1 = query->value(5).toString();
            if(dvType1.contains("AGG", Qt::CaseInsensitive))
                dvType1 = "AGG";
            if(dvType1.contains("BRAS", Qt::CaseInsensitive))
                dvType1 = "BRAS";
            if(dvType0.isEmpty())
                dvType0 = dvType1;
            if(dvType1.compare(dvType0) != 0)
            {
                ui->devexCB->insertSeparator(ui->devexCB->count());
                dvType0 = dvType1;
            }

            if(query->value(3).isNull() || query->value(4).isNull())
            {
                ui->devexCB->addItem(dev,id);
            }
            else
            {
                t = query->value(3).toString();
                sn = query->value(4).toString();
                dev += "-"+t+"-"+pur+"-"+sn;
                ui->devexCB->addItem(dev,id);
            }
        }


        ui->devexCB->setCurrentIndex(-1);
        ui->devexCB->blockSignals(false);
        ui->devexCB->setEnabled(true);
    }
}

void CardInstallDialog::on_siteCB_currentIndexChanged(int index)
{
    index++;
    BtnMan(false, false, false);
    ui->slotFrame->setEnabled(false);
    slotModel->clear();
    deviceModel->clear();
    qDeleteAll(ui->parentWidget->children());
    ui->tabWidget->setCurrentIndex(0);

    ui->devexCB->blockSignals(true);
    ui->devexCB->clear();
    ui->devexCB->blockSignals(false);
    ui->devexCB->setEnabled(true);

    if(ui->typeCB->currentData().toInt() == 3) //site
    {
        exchSiteId = ui->siteCB->currentData().toInt();

        QSqlQuery *query = dbMan->selectBriefDevex(exchSiteId);
        //dx.`id`, dx.`device_name`,dx.purchase,ds.t,ds.sn, dv.type
        int id;
        ui->devexCB->blockSignals(true);
        QString dev, pur, t,sn,dvType0, dvType1;
        while(query->next())
        {
            id = query->value(0).toInt();
            dev = query->value(1).toString();
            pur = query->value(2).toString();
            dvType1 = query->value(5).toString();
            if(dvType1.contains("AGG", Qt::CaseInsensitive))
                dvType1 = "AGG";
            if(dvType0.isEmpty())
                dvType0 = dvType1;
            if((dvType0 != dvType1))
            {
                ui->devexCB->insertSeparator(ui->devexCB->count());
                dvType0 = dvType1;
            }

            if(query->value(3).isNull() || query->value(4).isNull())
            {
                ui->devexCB->addItem(dev,id);
            }
            else
            {
                t = query->value(3).toString();
                sn = query->value(4).toString();
                dev += "-"+t+"-"+pur+"-"+sn;
                ui->devexCB->addItem(dev,id);
            }
        }
        ui->devexCB->setCurrentIndex(-1);
        ui->devexCB->blockSignals(false);

        if(ui->devexCB->count() == 1)
            ui->devexCB->setCurrentIndex(0);
    }

}

void CardInstallDialog::on_devexCB_currentIndexChanged(int index)
{
    index++;
    slotModel->clear();
    deviceModel->clear();
    qDeleteAll(ui->parentWidget->children());
    ui->tabWidget->setCurrentIndex(0);
    ui->slotFrame->setEnabled(true);

    int devId = ui->devexCB->currentData().toInt();
    int dId = dbMan->getDevexDeviceId(devId);
    bool DSLAM = dbMan->isDeviceDslam(dId);


    BtnMan(DSLAM, DSLAM, true);

    fillSlotsTV(devId);
    QString name = ui->devexCB->currentText();
    paintDevice(name);

}

void CardInstallDialog::on_emptyDslamBtn_clicked()
{
    int devexId = ui->devexCB->currentData().toInt();
    if(dbMan->isDevexDslam(devexId))
    {
        QString device = ui->devexCB->currentText();
        QString abbr = ui->areaCB->currentText()+'-';
        abbr += ui->abbrCB->currentText();
        if(ui->typeCB->currentData().toInt() == 3) // site
            abbr += '-'+ui->siteCB->currentText();


        RemoveDeviceCardDialog *removeDeviceCardDialog = new RemoveDeviceCardDialog(this, dbMan, abbr, devexId, device);
        removeDeviceCardDialog->setAttribute(Qt::WA_DeleteOnClose);
        connect(removeDeviceCardDialog, SIGNAL(finished(int)), this, SLOT(refreshSlot(int)));
        removeDeviceCardDialog->show();
    }
}

void CardInstallDialog::on_fillDslamBtn_clicked()
{
    int exchId = ui->abbrCB->currentData().toInt();
    int devexId = ui->devexCB->currentData().toInt();
    QString abbr = ui->areaCB->currentText()+'-';
    abbr += ui->abbrCB->currentText();
    if(ui->typeCB->currentData().toInt() == 3) // site
        abbr += '-'+ui->siteCB->currentText();

    QString device = ui->devexCB->currentText();

    AddDeviceCardDialog *addDeviceCardDialog = new AddDeviceCardDialog(this, dbMan,exchId, abbr, devexId, device, true);
    addDeviceCardDialog->setAttribute(Qt::WA_DeleteOnClose);
    connect(addDeviceCardDialog, SIGNAL(finished(int)), this, SLOT(refreshSlot(int)));
    addDeviceCardDialog->show();
}

void CardInstallDialog::slotContextMenuSlot(QPoint ptr)
{
    QModelIndex clickedIndex = ui->slotsTV->indexAt(ptr);
    clickedRow = clickedIndex.row();

    ptr.setY(ptr.y() + 25);
    /*
    `slots_id`, `devex_id`, `card_id`, `shelf`, `slot`, `sub_slot`, `card`,
    `adsl`, `vdsl`, `shdsl`, `e`, `g`, `g10`, `g40`, `g100`, `purchase`, `description`
    */
    clickedIndex = clickedIndex.sibling(clickedIndex.row(), SLOTS_ID);
    clickedSlotsId = slotModel->data(clickedIndex).toInt();

    if(clickedSlotsId > 0)
    {

        clickedIndex = clickedIndex.sibling(clickedIndex.row(), DEVEX_ID);
        clickedDevexId = slotModel->data(clickedIndex).toInt();

        clickedIndex = clickedIndex.sibling(clickedIndex.row(), CARD_ID);
        if(slotModel->data(clickedIndex).isNull())
            clickedCardId = -1;
        else
            clickedCardId = slotModel->data(clickedIndex).toInt();

        clickedIndex = clickedIndex.sibling(clickedIndex.row(), SHELF);
        if(slotModel->data(clickedIndex).isNull())
            clickedShelf = -1;
        else
            clickedShelf = slotModel->data(clickedIndex).toInt();

        clickedIndex = clickedIndex.sibling(clickedIndex.row(), SLOT_NO);
        clickedSlot = slotModel->data(clickedIndex).toInt();

        clickedIndex = clickedIndex.sibling(clickedIndex.row(), SUB_SLOT);
        if(slotModel->data(clickedIndex).isNull())
            clickedSubSlot = -1;
        else
            clickedSubSlot = slotModel->data(clickedIndex).toInt();

        clickedIndex = clickedIndex.sibling(clickedIndex.row(), CARD);
        clickedCard = slotModel->data(clickedIndex).toString();

        clickedIndex = clickedIndex.sibling(clickedIndex.row(), DESCRIPTION);
        clickedDesc = slotModel->data(clickedIndex).toString();

        clickedIndex = clickedIndex.sibling(clickedIndex.row(), PURCHASE);
        clickedPurchase = slotModel->data(clickedIndex).toString();


        if(clickedCardId > -1)
        {
            if(dbMan->isDevexDslam(clickedDevexId))
                ui->actionchange_Card_Type->setEnabled(true);
            else
                ui->actionchange_Card_Type->setEnabled(false);

            slotContextMenu.popup(ui->slotsTV->mapToGlobal(ptr));
        }

    }
}

void CardInstallDialog::on_actionRemove_Card_triggered()
{
    int devexId = ui->devexCB->currentData().toInt();
    QString device = ui->devexCB->currentText();

    QString abbr = ui->areaCB->currentText()+'-';
    abbr += ui->abbrCB->currentText();
    if(ui->typeCB->currentData().toInt() == 3) // site
        abbr += '-'+ui->siteCB->currentText();

    RemoveDeviceCardDialog *removeDeviceCardDialog = new RemoveDeviceCardDialog(this, dbMan, abbr, devexId, device,clickedSlotsId, clickedCard,clickedShelf, clickedSlot, clickedSubSlot);
    removeDeviceCardDialog->setAttribute(Qt::WA_DeleteOnClose);
    connect(removeDeviceCardDialog, SIGNAL(finished(int)), this, SLOT(refreshSlot(int)));
    removeDeviceCardDialog->show();
}

void CardInstallDialog::on_addCardBtn_clicked()
{
    int exchId;
    int devexId = ui->devexCB->currentData().toInt();
    QString abbr = ui->areaCB->currentText()+'-';
    exchId = ui->abbrCB->currentData().toInt();
    abbr += ui->abbrCB->currentText();
    if(ui->typeCB->currentData().toInt() == 3) // site
    {
        exchId = ui->siteCB->currentData().toInt();
        abbr += '-'+ui->siteCB->currentText();
    }


    QString device = ui->devexCB->currentText();

    AddDeviceCardDialog *addDeviceCardDialog = new AddDeviceCardDialog(this, dbMan, exchId, abbr,devexId,device,false);
    addDeviceCardDialog->setAttribute(Qt::WA_DeleteOnClose);
    connect(addDeviceCardDialog, SIGNAL(finished(int)), this, SLOT(refreshSlot(int)));
    addDeviceCardDialog->show();
}

void CardInstallDialog::drawASR9010()
{
    deviceModel->clear();


    int devexId = ui->devexCB->currentData().toInt();

    QSqlQuery *query = dbMan->selectViewSlotsBrief(devexId, true);
    //`shelf`, `slot`, `sub_slot`, `card`
    deviceModel->setQuery(*query);

    QHBoxLayout *layout = new QHBoxLayout(ui->parentWidget);
    layout->addItem(new QSpacerItem(10,10,QSizePolicy::Expanding,QSizePolicy::Minimum));
    ASR9010Widget *asr = new ASR9010Widget(this, deviceModel);
    layout->addWidget(asr);
    layout->addItem(new QSpacerItem(10,10,QSizePolicy::Expanding,QSizePolicy::Minimum));

}

void CardInstallDialog::drawASR1013()
{
    deviceModel->clear();


    int devexId = ui->devexCB->currentData().toInt();

    QSqlQuery *query = dbMan->selectViewSlotsBrief(devexId, true);
    //`shelf`, `slot`, `sub_slot`, `card`
    deviceModel->setQuery(*query);

    QHBoxLayout *layout = new QHBoxLayout(ui->parentWidget);
    layout->addItem(new QSpacerItem(10,10,QSizePolicy::Expanding,QSizePolicy::Minimum));
    ASR1013Widget *asr = new ASR1013Widget(this, deviceModel);
    layout->addWidget(asr);
    layout->addItem(new QSpacerItem(10,10,QSizePolicy::Expanding,QSizePolicy::Minimum));

}

void CardInstallDialog::drawASR1002()
{
    deviceModel->clear();


    int devexId = ui->devexCB->currentData().toInt();

    QSqlQuery *query = dbMan->selectViewSlotsBrief(devexId, true);
    //`shelf`, `slot`, `sub_slot`, `card`
    deviceModel->setQuery(*query);

    QHBoxLayout *layout = new QHBoxLayout(ui->parentWidget);
    layout->addItem(new QSpacerItem(10,10,QSizePolicy::Expanding,QSizePolicy::Minimum));
    ASR1002Widget *asr = new ASR1002Widget(this, deviceModel);
    layout->addWidget(asr);
    layout->addItem(new QSpacerItem(10,10,QSizePolicy::Expanding,QSizePolicy::Minimum));

}

void CardInstallDialog::drawC7613()
{
    deviceModel->clear();

    int devexId = ui->devexCB->currentData().toInt();

    QSqlQuery *query = dbMan->selectViewSlotsBrief(devexId, true);
    //`shelf`, `slot`, `sub_slot`, `card`
    deviceModel->setQuery(*query);

    QHBoxLayout *layout = new QHBoxLayout(ui->parentWidget);
    layout->addItem(new QSpacerItem(10,10,QSizePolicy::Expanding,QSizePolicy::Minimum));
    C7613Widget *c7613 = new C7613Widget(this,deviceModel);
    layout->addWidget(c7613);
    layout->addItem(new QSpacerItem(10,10,QSizePolicy::Expanding,QSizePolicy::Minimum));
}

void CardInstallDialog::drawCX200()
{
    deviceModel->clear();

    QHBoxLayout *layout = new QHBoxLayout(ui->parentWidget);
    layout->addItem(new QSpacerItem(10,10,QSizePolicy::Expanding,QSizePolicy::Minimum));
    CX200Widget *cx200 = new CX200Widget(this);
    layout->addWidget(cx200);
    layout->addItem(new QSpacerItem(10,10,QSizePolicy::Expanding,QSizePolicy::Minimum));
}

void CardInstallDialog::drawME60X16()
{
    deviceModel->clear();

    int devexId = ui->devexCB->currentData().toInt();

    QSqlQuery *query = dbMan->selectViewSlotsBrief(devexId, true);
    //`shelf`, `slot`, `sub_slot`, `card`
    deviceModel->setQuery(*query);

    QHBoxLayout *layout = new QHBoxLayout(ui->parentWidget);
    layout->addItem(new QSpacerItem(10,10,QSizePolicy::Expanding,QSizePolicy::Minimum));
    ME60X16Widget *me60x16 = new ME60X16Widget(this, deviceModel);
    layout->addWidget(me60x16);
    layout->addItem(new QSpacerItem(10,10,QSizePolicy::Expanding,QSizePolicy::Minimum));
}

void CardInstallDialog::drawME60X8()
{
    deviceModel->clear();

    int devexId = ui->devexCB->currentData().toInt();

    QSqlQuery *query = dbMan->selectViewSlotsBrief(devexId, true);
    //`shelf`, `slot`, `sub_slot`, `card`
    deviceModel->setQuery(*query);

    QHBoxLayout *layout = new QHBoxLayout(ui->parentWidget);
    layout->addItem(new QSpacerItem(10,10,QSizePolicy::Expanding,QSizePolicy::Minimum));
    ME60X8Widget *me60x8 = new ME60X8Widget(this, deviceModel);
    layout->addWidget(me60x8);
    layout->addItem(new QSpacerItem(10,10,QSizePolicy::Expanding,QSizePolicy::Minimum));
}

void CardInstallDialog::drawNE40X16()
{
    deviceModel->clear();

    int devexId = ui->devexCB->currentData().toInt();

    QSqlQuery *query = dbMan->selectViewSlotsBrief(devexId, true);
    //`shelf`, `slot`, `sub_slot`, `card`
    deviceModel->setQuery(*query);

    QHBoxLayout *layout = new QHBoxLayout(ui->parentWidget);
    layout->addItem(new QSpacerItem(10,10,QSizePolicy::Expanding,QSizePolicy::Minimum));
    NE40X16Widget *ne40x16 = new NE40X16Widget(this,true, deviceModel);
    layout->addWidget(ne40x16);
    layout->addItem(new QSpacerItem(10,10,QSizePolicy::Expanding,QSizePolicy::Minimum));
}

void CardInstallDialog::drawNE40X8()
{
    deviceModel->clear();

    int devexId = ui->devexCB->currentData().toInt();

    QSqlQuery *query = dbMan->selectViewSlotsBrief(devexId, true);
    //`shelf`, `slot`, `sub_slot`, `card`
    deviceModel->setQuery(*query);

    QHBoxLayout *layout = new QHBoxLayout(ui->parentWidget);
    layout->addItem(new QSpacerItem(10,10,QSizePolicy::Expanding,QSizePolicy::Minimum));
    NE40X8Widget *ne40x8 = new NE40X8Widget(this,true, deviceModel);
    layout->addWidget(ne40x8);
    layout->addItem(new QSpacerItem(10,10,QSizePolicy::Expanding,QSizePolicy::Minimum));
}

void CardInstallDialog::drawCX600X16()
{
    deviceModel->clear();

    int devexId = ui->devexCB->currentData().toInt();

    QSqlQuery *query = dbMan->selectViewSlotsBrief(devexId, true);
    //`shelf`, `slot`, `sub_slot`, `card`
    deviceModel->setQuery(*query);

    QHBoxLayout *layout = new QHBoxLayout(ui->parentWidget);
    layout->addItem(new QSpacerItem(10,10,QSizePolicy::Expanding,QSizePolicy::Minimum));
    NE40X16Widget *cx600x16 = new NE40X16Widget(this,false, deviceModel);
    layout->addWidget(cx600x16);
    layout->addItem(new QSpacerItem(10,10,QSizePolicy::Expanding,QSizePolicy::Minimum));
}

void CardInstallDialog::drawCX600X8()
{
    deviceModel->clear();

    int devexId = ui->devexCB->currentData().toInt();

    QSqlQuery *query = dbMan->selectViewSlotsBrief(devexId, true);
    //`shelf`, `slot`, `sub_slot`, `card`
    deviceModel->setQuery(*query);

    QHBoxLayout *layout = new QHBoxLayout(ui->parentWidget);
    layout->addItem(new QSpacerItem(10,10,QSizePolicy::Expanding,QSizePolicy::Minimum));
    NE40X8Widget *cx600x8 = new NE40X8Widget(this,false, deviceModel);
    layout->addWidget(cx600x8);
    layout->addItem(new QSpacerItem(10,10,QSizePolicy::Expanding,QSizePolicy::Minimum));
}

void CardInstallDialog::drawNE5000X16A()
{
    deviceModel->clear();

    int devexId = ui->devexCB->currentData().toInt();

    QSqlQuery *query = dbMan->selectViewSlotsBrief(devexId, true);
    //`shelf`, `slot`, `sub_slot`, `card`
    deviceModel->setQuery(*query);

    QHBoxLayout *layout = new QHBoxLayout(ui->parentWidget);
    layout->addItem(new QSpacerItem(10,10,QSizePolicy::Expanding,QSizePolicy::Minimum));
    NE5000X16AWidget *ne5000X16AWidget = new NE5000X16AWidget(this,true, deviceModel);
    layout->addWidget(ne5000X16AWidget);
    layout->addItem(new QSpacerItem(10,10,QSizePolicy::Expanding,QSizePolicy::Minimum));
}

void CardInstallDialog::drawNE5000X16()
{
    deviceModel->clear();

    int devexId = ui->devexCB->currentData().toInt();

    QSqlQuery *query = dbMan->selectViewSlotsBrief(devexId, true);
    //`shelf`, `slot`, `sub_slot`, `card`
    deviceModel->setQuery(*query);

    QHBoxLayout *layout = new QHBoxLayout(ui->parentWidget);
    layout->addItem(new QSpacerItem(10,10,QSizePolicy::Expanding,QSizePolicy::Minimum));
    NE5000X16Widget *ne5000 = new NE5000X16Widget(this, deviceModel);
    layout->addWidget(ne5000);
    layout->addItem(new QSpacerItem(10,10,QSizePolicy::Expanding,QSizePolicy::Minimum));
}

void CardInstallDialog::drawS2403H()
{
    deviceModel->clear();


    QHBoxLayout *layout = new QHBoxLayout(ui->parentWidget);
    layout->addItem(new QSpacerItem(10,10,QSizePolicy::Expanding,QSizePolicy::Minimum));
    S2403HWidget *s2403 = new S2403HWidget(this);
    layout->addWidget(s2403);
    layout->addItem(new QSpacerItem(10,10,QSizePolicy::Expanding,QSizePolicy::Minimum));
}

void CardInstallDialog::drawS5300()
{
    deviceModel->clear();



    QHBoxLayout *layout = new QHBoxLayout(ui->parentWidget);
    layout->addItem(new QSpacerItem(10,10,QSizePolicy::Expanding,QSizePolicy::Minimum));
    S5300Widget *s5300 = new S5300Widget(this);
    layout->addWidget(s5300);
    layout->addItem(new QSpacerItem(10,10,QSizePolicy::Expanding,QSizePolicy::Minimum));
}

void CardInstallDialog::drawS9303()
{
    deviceModel->clear();

    int devexId = ui->devexCB->currentData().toInt();

    QSqlQuery *query = dbMan->selectViewSlotsBrief(devexId, true);
    //`shelf`, `slot`, `sub_slot`, `card`
    deviceModel->setQuery(*query);

    QHBoxLayout *layout = new QHBoxLayout(ui->parentWidget);
    layout->addItem(new QSpacerItem(10,10,QSizePolicy::Expanding,QSizePolicy::Minimum));
    S9303Widget *s9303 = new S9303Widget(this, deviceModel);
    layout->addWidget(s9303);
    layout->addItem(new QSpacerItem(10,10,QSizePolicy::Expanding,QSizePolicy::Minimum));
}

void CardInstallDialog::drawS9306()
{
    deviceModel->clear();

    int devexId = ui->devexCB->currentData().toInt();

    QSqlQuery *query = dbMan->selectViewSlotsBrief(devexId, true);
    //`shelf`, `slot`, `sub_slot`, `card`
    deviceModel->setQuery(*query);

    QHBoxLayout *layout = new QHBoxLayout(ui->parentWidget);
    layout->addItem(new QSpacerItem(10,10,QSizePolicy::Expanding,QSizePolicy::Minimum));
    S9306Widget *s9306 = new S9306Widget(this, deviceModel);
    layout->addWidget(s9306);
    layout->addItem(new QSpacerItem(10,10,QSizePolicy::Expanding,QSizePolicy::Minimum));
}

void CardInstallDialog::drawDslam4s()
{
    deviceModel->clear();

    int devexId = ui->devexCB->currentData().toInt();

    QSqlQuery *query = dbMan->selectViewSlotsBrief(devexId, true);
    //`shelf`, `slot`, `sub_slot`, `card`
    deviceModel->setQuery(*query);

    QHBoxLayout *layout = new QHBoxLayout(ui->parentWidget);
    layout->addItem(new QSpacerItem(10,10,QSizePolicy::Expanding,QSizePolicy::Minimum));
    Dslam4sWidget *dslam4s = new Dslam4sWidget(this, deviceModel);
    layout->addWidget(dslam4s);
    layout->addItem(new QSpacerItem(10,10,QSizePolicy::Expanding,QSizePolicy::Minimum));
}

void CardInstallDialog::drawdslam16s(const QString DeviceName)
{
    deviceModel->clear();

    int devexId = ui->devexCB->currentData().toInt();

    QSqlQuery *query = dbMan->selectViewSlotsBrief(devexId, true);
    //`shelf`, `slot`, `sub_slot`, `card`
    deviceModel->setQuery(*query);

    QHBoxLayout *layout = new QHBoxLayout(ui->parentWidget);
    layout->addItem(new QSpacerItem(10,10,QSizePolicy::Expanding,QSizePolicy::Minimum));
    Dslam16sWidget *dslam16s = new Dslam16sWidget(this, DeviceName, deviceModel);
    layout->addWidget(dslam16s);
    layout->addItem(new QSpacerItem(10,10,QSizePolicy::Expanding,QSizePolicy::Minimum));
}

void CardInstallDialog::refreshSlot(int done)
{
    done++;
    int devexId = ui->devexCB->currentData().toInt();
    QString device = ui->devexCB->currentText();
    fillSlotsTV(devexId);
    paintDevice(device);

    ui->slotsTV->selectRow(clickedRow);
}

void CardInstallDialog::on_actionEdit_Purchase_triggered()
{
    int devexId = ui->devexCB->currentData().toInt();
    QString device = ui->devexCB->currentText();

    QString abbr = ui->areaCB->currentText()+'-';
    abbr += ui->abbrCB->currentText();
    if(ui->typeCB->currentData().toInt() == 3) // site
        abbr += '-'+ui->siteCB->currentText();

    EditCardPurchaseDialog *editCardPurchaseDialog = new EditCardPurchaseDialog(this, dbMan, abbr, devexId, device,clickedSlotsId, clickedCard,clickedShelf, clickedSlot, clickedSubSlot, clickedPurchase);
    editCardPurchaseDialog->setAttribute(Qt::WA_DeleteOnClose);
    connect(editCardPurchaseDialog, SIGNAL(finished(int)), this, SLOT(refreshSlot(int)));
    editCardPurchaseDialog->show();
}

void CardInstallDialog::on_actionEdit_Description_triggered()
{
    int devexId = ui->devexCB->currentData().toInt();
    QString device = ui->devexCB->currentText();

    QString abbr = ui->areaCB->currentText()+'-';
    abbr += ui->abbrCB->currentText();
    if(ui->typeCB->currentData().toInt() == 3) // site
        abbr += '-'+ui->siteCB->currentText();

    EditCardDescDialog *editCardDescDialog = new EditCardDescDialog(this, dbMan, abbr, devexId, device,clickedSlotsId, clickedCard,clickedShelf, clickedSlot, clickedSubSlot, clickedDesc);
    editCardDescDialog->setAttribute(Qt::WA_DeleteOnClose);
    connect(editCardDescDialog, SIGNAL(finished(int)), this, SLOT(refreshSlot(int)));
    editCardDescDialog->show();
}

void CardInstallDialog::on_actionchange_Card_Type_triggered()
{
    int devexId = ui->devexCB->currentData().toInt();
    QString device = ui->devexCB->currentText();

    QString abbr = ui->areaCB->currentText()+'-';
    abbr += ui->abbrCB->currentText();
    if(ui->typeCB->currentData().toInt() == 3) // site
        abbr += '-'+ui->siteCB->currentText();

    ChangeDslamCardDialog *changeDslamCardDialog = new ChangeDslamCardDialog(this, dbMan, abbr, devexId, device,clickedSlotsId, clickedCardId,clickedShelf, clickedSlot, clickedSubSlot);
    changeDslamCardDialog->setAttribute(Qt::WA_DeleteOnClose);
    connect(changeDslamCardDialog, SIGNAL(finished(int)), this, SLOT(refreshSlot(int)));
    changeDslamCardDialog->show();
}
