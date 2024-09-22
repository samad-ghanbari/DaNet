#include "deviceinstalldialog.h"
#include "ui_deviceinstalldialog.h"
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QModelIndex>
#include <lib/database/danetdbman.h>
#include "mainPanel/menu/installation/device/adddeviceinstalldialog.h"
#include "mainPanel/menu/installation/device/editdevicepositiondialog.h"
#include "mainPanel/menu/installation/device/removedeviceinstalldialog.h"
#include "mainPanel/menu/installation/device/editdevicetypedialog.h"
#include "mainPanel/menu/installation/device/editdevicedescdialog.h"
#include "mainPanel/menu/installation/device/editdevicepurchasedialog.h"
#include "mainPanel/menu/installation/device/editdevicenamedialog.h"

/*
 0:`devex_id`, 1:`exchange_id`, 2:`saloon_no`, 3:`device_id`, 4:`saloon_name`,
5 `row`, 6`rack`, 7`shelf`, 8`device`, 9`device_name`, 10`purchase`, 11`description`
*/
#define DEVEX_ID 0
#define EXCHANGE_ID 1
#define SALOON_NO 2
#define DEVICE_ID 3
#define SALOON 4
#define ROW 5
#define RACK 6
#define SHELF 7
#define TYPE 8
#define DEVICE 9
#define DEVICE_NAME 10
#define PURCHASE 11
#define DESCRIPTION 12


DeviceInstallDialog::DeviceInstallDialog(QWidget *parent, DanetDbMan *db) :
    QDialog(parent),
    ui(new Ui::DeviceInstallDialog),
    dbMan(db),
    model(new QSqlQueryModel(this))
{
    ui->setupUi(this);
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


    //device
    ui->devicesTV->setModel(model);
    ui->devicesTV->verticalHeader()->hide();

    // context menu devices
    ui->devicesTV->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->devicesTV, SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(deviceContextMenuSlot(QPoint)));

    deviceContextMenu.addAction(ui->actionEdit_Description);
    deviceContextMenu.addSeparator();
    deviceContextMenu.addAction(ui->actionEdit_Purchase);
    deviceContextMenu.addSeparator();
    deviceContextMenu.addAction(ui->actionEdit_Device_Name);
    deviceContextMenu.addSeparator();
    deviceContextMenu.addAction(ui->actionEditDevicePosition);
    deviceContextMenu.addSeparator();
    deviceContextMenu.addAction(ui->actionChangeDevice);
    deviceContextMenu.addSeparator();
    deviceContextMenu.addAction(ui->actionRemoveDevice);

    ui->frame->setEnabled(false);

    int workingArea = dbMan->getLoggedInDefaultArea();
    ui->areaCB->setCurrentIndex(ui->areaCB->findData(workingArea));
}

DeviceInstallDialog::~DeviceInstallDialog()
{
    delete ui;
    delete model;
    model = nullptr;
}

void DeviceInstallDialog::fillDevicesTV()
{
    model->clear();
/*
 0:`devex_id`, 1:`exchange_id`, 2:`saloon_no`, 3:`device_id`, 4:`saloon_name`,
5 `row`, 6`rack`, 7`shelf`, 8`type`,9`device`, 10`device_name`, 11`purchase`, 12`description`
*/
    int exchId = ui->abbrCB->currentData().toInt();
    if(ui->typeCB->currentData().toInt() == 3)
        exchId = ui->siteCB->currentData().toInt();

    QSqlQuery *query = dbMan->selectViewDevex(exchId);
    model->setQuery(*query);
    model->setHeaderData(SALOON, Qt::Horizontal,"Saloon");
    model->setHeaderData(ROW, Qt::Horizontal,"Rack\nRow");
    model->setHeaderData(RACK, Qt::Horizontal,"Rack\nNo");
    model->setHeaderData(SHELF, Qt::Horizontal,"Shelf\nNo");
    model->setHeaderData(TYPE, Qt::Horizontal,"Type");
    model->setHeaderData(DEVICE, Qt::Horizontal,"Device");
    model->setHeaderData(DEVICE_NAME, Qt::Horizontal,"Device Name");
    model->setHeaderData(PURCHASE, Qt::Horizontal,"Purchase");
    model->setHeaderData(DESCRIPTION, Qt::Horizontal,"Description");

    ui->devicesTV->hideColumn(DEVEX_ID);
    ui->devicesTV->hideColumn(EXCHANGE_ID);
    ui->devicesTV->hideColumn(SALOON_NO);
    ui->devicesTV->hideColumn(DEVICE_ID);


    ui->devicesTV->horizontalHeader()->setSectionResizeMode(DEVICE, QHeaderView::Stretch);
    ui->devicesTV->horizontalHeader()->setSectionResizeMode(DEVICE_NAME, QHeaderView::Stretch);
    ui->devicesTV->horizontalHeader()->setSectionResizeMode(DESCRIPTION, QHeaderView::Stretch);

    ui->devicesTV->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

}

void DeviceInstallDialog::refreshSlot(int confirm)
{
    confirm++;
    fillDevicesTV();
    ui->devicesTV->selectRow(recordRow);
}

void DeviceInstallDialog::on_areaCB_currentIndexChanged(int index)
{
    ui->frame->setEnabled(false);
    model->clear();

    ui->abbrCB->blockSignals(true);
    ui->abbrCB->clear();

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

void DeviceInstallDialog::on_abbrCB_currentIndexChanged(const QString &arg1)
{
    if(arg1.isEmpty()) return;
    ui->frame->setEnabled(false);
    model->clear();

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

void DeviceInstallDialog::on_typeCB_currentIndexChanged(int index)
{
    model->clear();
    ui->frame->setEnabled(false);

    ui->siteCB->blockSignals(true);
    ui->siteCB->clear();
    ui->siteFrame->setEnabled(false);
    ui->siteFrame->setVisible(false);


    if(index == 1) // site
    {
        ui->siteFrame->setEnabled(true);
        ui->siteFrame->setVisible(true);


        int area = ui->areaCB->currentData().toInt();
        QString mother = ui->abbrCB->currentText();

        QSqlQuery *query = dbMan->selectSites(area, mother);
        //`id`,`abbr`, `exchange`, `site_cascade`, `site_node`
        int i;
        QString a, s, c, n;
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

        ui->frame->setEnabled(true);
        fillDevicesTV();

    }
}

void DeviceInstallDialog::on_siteCB_currentIndexChanged(int index)
{
    model->clear();

    index++;
    if(ui->typeCB->currentData().toInt() == 3)
    {
        ui->frame->setEnabled(true);
        fillDevicesTV();
    }
}

void DeviceInstallDialog::deviceContextMenuSlot(QPoint ptr)
{
    /*
     0:`devex_id`, 1:`exchange_id`, 2:`saloon_no`, 3:`device_id`, 4:`saloon_name`,
    5 `row`, 6`rack`, 7`shelf`, 8`device`, 9`device_name`, 10`purchase`, 11`description`
    */

    QModelIndex clickedIndex = ui->devicesTV->indexAt(ptr);
    recordRow = clickedIndex.row();

    ptr.setY(ptr.y() + 10);

    clickedIndex = clickedIndex.sibling(clickedIndex.row(), DEVEX_ID);
    clickedDevexId = model->data(clickedIndex).toInt();
    if(clickedDevexId > 0)
    {
        clickedIndex = clickedIndex.sibling(clickedIndex.row(), DEVEX_ID);
        clickedDevexId = model->data(clickedIndex).toInt();

        clickedIndex = clickedIndex.sibling(clickedIndex.row(), SALOON_NO);
        clickedSaloonNo = model->data(clickedIndex).toInt();

        clickedIndex = clickedIndex.sibling(clickedIndex.row(), DEVICE_ID);
        clickedDeviceId = model->data(clickedIndex).toInt();

        clickedIndex = clickedIndex.sibling(clickedIndex.row(), SALOON);
        clickedSaloon = model->data(clickedIndex).toString();

        clickedIndex = clickedIndex.sibling(clickedIndex.row(), ROW);
        clickedRow = model->data(clickedIndex).toInt();

        clickedIndex = clickedIndex.sibling(clickedIndex.row(), RACK);
        clickedRack = model->data(clickedIndex).toInt();

        clickedIndex = clickedIndex.sibling(clickedIndex.row(), SHELF);
        clickedShelf = model->data(clickedIndex).toInt();

        clickedIndex = clickedIndex.sibling(clickedIndex.row(), DEVICE);
        clickedDevice = model->data(clickedIndex).toString();

        clickedIndex = clickedIndex.sibling(clickedIndex.row(), DEVICE_NAME);
        clickedDeviceName = model->data(clickedIndex).toString();

        clickedIndex = clickedIndex.sibling(clickedIndex.row(), PURCHASE);
        clickedPurchase = model->data(clickedIndex).toString();

        clickedIndex = clickedIndex.sibling(clickedIndex.row(), DESCRIPTION);
        clickedDesc = model->data(clickedIndex).toString();

        if(clickedDeviceId > 0)
            deviceContextMenu.popup(ui->devicesTV->mapToGlobal(ptr));

        if(ui->typeCB->currentData().toInt() == 3)
            ui->actionEdit_Device_Name->setEnabled(true);
        else
            ui->actionEdit_Device_Name->setEnabled(false);
    }
}

void DeviceInstallDialog::on_addDeviceBtn_clicked()
{
    int exchId;
    exchId = ui->abbrCB->currentData().toInt();
    QString abbr = ui->areaCB->currentText()+'-';
    abbr += ui->abbrCB->currentText();
    if(ui->typeCB->currentData().toInt() == 3) // site
    {
        exchId = ui->siteCB->currentData().toInt();
        abbr += " -"+ui->siteCB->currentText();
    }

    AddDeviceInstallDialog *addDeviceInstallDialog = new AddDeviceInstallDialog(this, dbMan,exchId,abbr);
    addDeviceInstallDialog->setAttribute(Qt::WA_DeleteOnClose);
    connect(addDeviceInstallDialog, SIGNAL(finished(int)), this, SLOT(refreshSlot(int)));
    addDeviceInstallDialog->show();
}

void DeviceInstallDialog::on_actionEditDevicePosition_triggered()
{
    int exchId = ui->abbrCB->currentData().toInt();
    QString abbr = ui->areaCB->currentText()+'-';
    abbr += ui->abbrCB->currentText();
    if(ui->typeCB->currentData().toInt() == 3) // site
    {
        exchId = ui->siteCB->currentData().toInt();
        abbr += '-'+ui->siteCB->currentText();
    }

    EditDevicePositionDialog *editDevicePositionDialog = new EditDevicePositionDialog(this,dbMan,exchId, abbr,clickedDevexId,clickedDeviceId, clickedSaloonNo,clickedRow, clickedRack,clickedShelf,clickedDeviceName);
    editDevicePositionDialog->setAttribute(Qt::WA_DeleteOnClose);
    connect(editDevicePositionDialog, SIGNAL(finished(int)), this, SLOT(refreshSlot(int)));
    editDevicePositionDialog->show();
}

void DeviceInstallDialog::on_actionRemoveDevice_triggered()
{
    QString abbr = ui->areaCB->currentText()+'-';
    abbr += ui->abbrCB->currentText();
    if(ui->typeCB->currentData().toInt() == 3) // site
        abbr += '-'+ui->siteCB->currentText();

    QString rowRackShelf = QString::number(clickedRow)+'*'+QString::number(clickedRack)+'*'+QString::number(clickedShelf);

    RemoveDeviceInstallDialog *removeDeviceInstallDialog = new RemoveDeviceInstallDialog(this,dbMan,clickedDevexId,abbr, clickedSaloon,rowRackShelf,clickedDevice,clickedDeviceName);
    removeDeviceInstallDialog->setAttribute(Qt::WA_DeleteOnClose);
    connect(removeDeviceInstallDialog, SIGNAL(finished(int)), this, SLOT(refreshSlot(int)));
    removeDeviceInstallDialog->show();
}

void DeviceInstallDialog::on_actionChangeDevice_triggered()
{
    QString abbr = ui->areaCB->currentText()+'-';
    int exchId = ui->abbrCB->currentData().toInt();
    abbr += ui->abbrCB->currentText();
    if(ui->typeCB->currentData().toInt() == 3) // site
    {
        exchId = ui->siteCB->currentData().toInt();
        abbr += " -"+ui->siteCB->currentText();
    }
    // abbr : 5-BS  5-BS -AKAB

    EditDeviceTypeDialog *editDeviceTypeDialog = new EditDeviceTypeDialog(this,dbMan,exchId,abbr, clickedSaloonNo,clickedRow, clickedRack, clickedShelf,clickedDevexId, clickedPurchase,clickedDeviceId,clickedDeviceName);
    editDeviceTypeDialog->setAttribute(Qt::WA_DeleteOnClose);
    connect(editDeviceTypeDialog, SIGNAL(finished(int)), this, SLOT(refreshSlot(int)));
    editDeviceTypeDialog->show();
}

void DeviceInstallDialog::on_actionEdit_Description_triggered()
{
    QString abbr = ui->areaCB->currentText()+'-';
    abbr += ui->abbrCB->currentText();
    if(ui->typeCB->currentData().toInt() == 3) // site
    {
        abbr += '-'+ui->siteCB->currentText();
    }

    EditDeviceDescDialog *editDeviceDescDialog = new EditDeviceDescDialog(this,dbMan,abbr,clickedSaloon, clickedDevexId,clickedRow, clickedRack,clickedShelf,clickedDeviceName,clickedDesc);
    editDeviceDescDialog->setAttribute(Qt::WA_DeleteOnClose);
    connect(editDeviceDescDialog, SIGNAL(finished(int)), this, SLOT(refreshSlot(int)));
    editDeviceDescDialog->show();
}

void DeviceInstallDialog::on_actionEdit_Purchase_triggered()
{
    QString abbr = ui->areaCB->currentText()+'-';
    abbr += ui->abbrCB->currentText();
    if(ui->typeCB->currentData().toInt() == 3) // site
    {
        abbr += '-'+ui->siteCB->currentText();
    }

    EditDevicePurchaseDialog *editDevicePurchaseDialog = new EditDevicePurchaseDialog(this,dbMan,abbr,clickedSaloon ,clickedDevexId,clickedRow, clickedRack,clickedShelf,clickedDeviceName,clickedPurchase);
    editDevicePurchaseDialog->setAttribute(Qt::WA_DeleteOnClose);
    connect(editDevicePurchaseDialog, SIGNAL(finished(int)), this, SLOT(refreshSlot(int)));
    editDevicePurchaseDialog->show();

}

void DeviceInstallDialog::on_actionEdit_Device_Name_triggered()
{
    if(ui->typeCB->currentData().toInt() == 3)
    {
        QString areaAbbr = ui->areaCB->currentData().toString();
        areaAbbr = areaAbbr + "-"+ ui->abbrCB->currentText();
        int siteId = ui->siteCB->currentData().toInt();
        QString site = dbMan->getExchangeAbbr(siteId);

        EditDeviceNameDialog *editDeviceNameDialog = new EditDeviceNameDialog(this, dbMan, clickedDevexId, clickedDeviceName, areaAbbr,site,clickedDevice);
        editDeviceNameDialog->setAttribute(Qt::WA_DeleteOnClose);
        connect(editDeviceNameDialog, SIGNAL(finished(int)), this, SLOT(refreshSlot(int)));
        editDeviceNameDialog->show();
    }
}
