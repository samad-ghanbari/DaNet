#include "devicemanagementdialog.h"
#include "ui_devicemanagementdialog.h"
#include "lib/database/danetdbman.h"
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include "lib/mapmodel.h"
#include <QMessageBox>
#include <QStringListModel>
#include <QStandardItemModel>
#include "mainPanel/menu/management/DeviceManagement/addvlandialog.h"
#include "mainPanel/menu/management/DeviceManagement/editvlandialog.h"
#include "mainPanel/menu/management/DeviceManagement/removevlandialog.h"
#include "mainPanel/menu/management/DeviceManagement/addethertrunkdialog.h"
#include "mainPanel/menu/management/DeviceManagement/editethertrunkdialog.h"
#include "mainPanel/menu/management/DeviceManagement/removeethertrunkdialog.h"
#include "mainPanel/menu/management/DeviceManagement/addvpndialog.h"
#include "mainPanel/menu/management/DeviceManagement/editvpndialog.h"
#include "mainPanel/menu/management/DeviceManagement/removevpndialog.h"

#include "mainPanel/deviceWidgets/ASR9010/asr9010widget.h"
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

DeviceManagementDialog::DeviceManagementDialog(QWidget *parent, DanetDbMan *db) :
    QDialog(parent),
    ui(new Ui::DeviceManagementDialog),
    dbMan(db),
    vlanModel(new QSqlQueryModel(this)),
    etherModel(new QSqlQueryModel(this)),
    vpnModel(new QSqlQueryModel(this)),
    slotModel(new QSqlQueryModel(this)),
    newVlanModel(NULL),
    newEtherModel(NULL),
    newVpnModel(new QStandardItemModel(this)),
    moduleModel(new QStandardItemModel(this)),
    expModel(new QStringListModel(this)),
    impModel(new QStringListModel(this)),
    newExpModel(new QStringListModel(this)),
    newImpModel(new QStringListModel(this))
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowMinMaxButtonsHint);

    ui->huaweiConfigLoadBtn->setEnabled(false);
    ui->ciscoConfigLoadBtn->setEnabled(false);
    ui->okBtn->setEnabled(false);
    ui->checkBox->setEnabled(false);
    ui->tabWidget->setEnabled(false);
    ui->tabWidget->setCurrentIndex(0);

    ui->currentVlanTV->horizontalHeader()->setStretchLastSection(true);
    ui->currentVlanTV->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->currentVlanTV->verticalHeader()->hide();
    ui->currentVlanTV->setModel(vlanModel);

    ui->vlanTV->horizontalHeader()->setStretchLastSection(true);
    ui->vlanTV->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->vlanTV->verticalHeader()->hide();
    ui->vlanTV->setModel(vlanModel);

    ui->currentEtherTV->horizontalHeader()->setStretchLastSection(true);
    ui->currentEtherTV->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->currentEtherTV->verticalHeader()->hide();
    ui->currentEtherTV->setModel(etherModel);

    ui->etherTV->horizontalHeader()->setStretchLastSection(true);
    ui->etherTV->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->etherTV->verticalHeader()->hide();
    ui->etherTV->setModel(etherModel);

    ui->newVlanTV->horizontalHeader()->setStretchLastSection(true);
    ui->newVlanTV->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->newVlanTV->verticalHeader()->hide();

    ui->newEtherTV->horizontalHeader()->setStretchLastSection(true);
    ui->newEtherTV->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->newEtherTV->verticalHeader()->hide();

    ui->vpnTV->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->vpnTV->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->vpnTV->verticalHeader()->hide();
    ui->vpnTV->setModel(vpnModel);

    ui->expLV->setModel(expModel);
    ui->impLV->setModel(impModel);

    ui->currVpnTV->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->currVpnTV->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->currVpnTV->verticalHeader()->hide();
    ui->currVpnTV->setModel(vpnModel);

    ui->modTV->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->modTV->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->modTV->verticalHeader()->hide();
    ui->modTV->setModel(moduleModel);

    ui->currExpLV->setModel(expModel);
    ui->currImpLV->setModel(impModel);

    ui->newVpnTV->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->newVpnTV->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->newVpnTV->verticalHeader()->hide();

    newVpnModel->setColumnCount(4);
    //row,vpn,rd, desc
    newVpnModel->setHeaderData(1, Qt::Horizontal,"VPN");
    newVpnModel->setHeaderData(2, Qt::Horizontal,"Route Distinguisher");
    newVpnModel->setHeaderData(3, Qt::Horizontal,"Description");

    ui->newVpnTV->setModel(newVpnModel);
    ui->newVpnTV->hideColumn(0);

    ui->newExpLV->setModel(newExpModel);
    ui->newImpLV->setModel(newImpModel);

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
    ui->infoLbl->clear();

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
    ui->modChB->setChecked(false);
    ui->modChB->setEnabled(false);
    ui->modOkBtn->setEnabled(false);
    DeviceVendor = 0;


    ui->vlanTV->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->vlanTV, SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(vlanContextMenuSlot(QPoint)));
    vlanContextMenu.addAction(ui->actionEditVlan);
    vlanContextMenu.addSeparator();
    vlanContextMenu.addAction(ui->actionRemoveVlan);

    ui->etherTV->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->etherTV, SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(etherContextMenuSlot(QPoint)));
    etherContextMenu.addAction(ui->actionEditEtherTrunk);
    etherContextMenu.addSeparator();
    etherContextMenu.addAction(ui->actionRemoveEtherTrunk);

    ui->vpnTV->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->vpnTV, SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(vpnContextMenuSlot(QPoint)));
    vpnContextMenu.addAction(ui->actionEditVPN);
    vpnContextMenu.addSeparator();
    vpnContextMenu.addAction(ui->actionRemoveVPN);


    int workingArea = dbMan->getLoggedInDefaultArea();
    ui->areaCB->setCurrentIndex(ui->areaCB->findData(workingArea));

}

DeviceManagementDialog::~DeviceManagementDialog()
{
    delete ui;

    delete vlanModel;
    delete etherModel;
    delete vpnModel;
    delete expModel;
    delete impModel;
    vlanModel = NULL;
    etherModel = NULL;
    vpnModel = NULL;
    expModel = NULL;
    impModel =NULL;
}

void DeviceManagementDialog::clearModels()
{
    delete newVlanModel;
    newVlanModel = NULL;
    delete newEtherModel;
    newEtherModel = NULL;

    idModuleMap.clear();
    moduleModel->clear();
    vlanModel->clear();
    etherModel->clear();
    vpnModel->clear();
}

void DeviceManagementDialog::fillVlanTV()
{
    const int devexId = ui->deviceCB->currentData().toInt();
    QSqlQuery *query = dbMan->selectVlans(devexId);
    vlanModel->setQuery(*query);

    ui->vlanTV->hideColumn(0);
    ui->currentVlanTV->hideColumn(0);

    vlanModel->setHeaderData(1, Qt::Horizontal,"VLAN");
    vlanModel->setHeaderData(2, Qt::Horizontal,"Description");

}

void DeviceManagementDialog::fillEtherTrunkTV()
{
    const int devexId = ui->deviceCB->currentData().toInt();
    QSqlQuery *query = dbMan->selectEtherTrunks(devexId);
    etherModel->setQuery(*query);

    ui->etherTV->hideColumn(0);
    ui->currentEtherTV->hideColumn(0);

    etherModel->setHeaderData(1, Qt::Horizontal,"Ether Trunk");
    etherModel->setHeaderData(2, Qt::Horizontal,"Description");

}

void DeviceManagementDialog::fillVpnViews()
{
    const int devexId = ui->deviceCB->currentData().toInt();
    QSqlQuery *query = dbMan->selectVPN(devexId);
    //`id`, `vpn`, `rd`, `description`
    vpnModel->setQuery(*query);
    ui->vpnTV->hideColumn(0);
    ui->currVpnTV->hideColumn(0);

    vpnModel->setHeaderData(1, Qt::Horizontal,"VPN");
    vpnModel->setHeaderData(2, Qt::Horizontal,"Route Distinguisher");
    vpnModel->setHeaderData(3, Qt::Horizontal,"Description");

    QModelIndex ind = vpnModel->index(0, 0);
    ui->vpnTV->selectRow(0);
    ui->vpnTV->clicked(ind);

    ui->currVpnTV->selectRow(0);
    ui->currVpnTV->clicked(ind);
}

void DeviceManagementDialog::fillModulesTV()
{
    moduleModel->clear();
    idModuleMap.clear();

    const int devexId = ui->deviceCB->currentData().toInt();
    QSqlQuery *query = dbMan->selectDeviceModules(devexId);
    //`int_id`,`device`, `interface_type`,`interface`,`module` newModule(empty)

    moduleModel->setColumnCount(6);

    ui->modTV->hideColumn(0);
    moduleModel->setHeaderData(1, Qt::Horizontal,"Device");
    moduleModel->setHeaderData(2, Qt::Horizontal,"Interface Type");
    moduleModel->setHeaderData(3, Qt::Horizontal,"Interface");
    moduleModel->setHeaderData(4, Qt::Horizontal,"Current Module");
    moduleModel->setHeaderData(5, Qt::Horizontal,"New Module");

    int Row = -1, intId;
    QString device, intType,inte,mod;
    QModelIndex ind;
    while(query->next())
    {
        intId = query->value(0).toInt();
        device = query->value(1).toString();
        intType = query->value(2).toString();
        inte = query->value(3).toString();
        mod = query->value(4).toString();

        Row++;
        moduleModel->insertRow(Row);
        ind = moduleModel->index(Row, 0);
        moduleModel->setData(ind,intId);
        ind = ind.sibling(Row,1);
        moduleModel->setData(ind,device);
        ind = ind.sibling(Row,2);
        moduleModel->setData(ind,intType);
        ind = ind.sibling(Row,3);
        moduleModel->setData(ind,inte);
        ind = ind.sibling(Row,4);
        moduleModel->setData(ind,mod);


    }
}

void DeviceManagementDialog::vlanContextMenuSlot(QPoint ptr)
{
    QModelIndex clickedIndex = ui->vlanTV->indexAt(ptr);
    row = clickedIndex.row();

    clickedIndex = clickedIndex.sibling(clickedIndex.row(), 0); // id
    vlanId = vlanModel->data(clickedIndex).toInt();
    clickedIndex = clickedIndex.sibling(clickedIndex.row(), 1); // vlan
    vlan = vlanModel->data(clickedIndex).toInt();
    clickedIndex = clickedIndex.sibling(clickedIndex.row(), 2); // description
    vlanDesc = vlanModel->data(clickedIndex).toString();

    if(vlanId > 0)
        vlanContextMenu.popup(ui->vlanTV->mapToGlobal(ptr));
}

void DeviceManagementDialog::etherContextMenuSlot(QPoint ptr)
{
    QModelIndex clickedIndex = ui->etherTV->indexAt(ptr);
    row = clickedIndex.row();

    clickedIndex = clickedIndex.sibling(clickedIndex.row(), 0); // id
    etherId = etherModel->data(clickedIndex).toInt();
    clickedIndex = clickedIndex.sibling(clickedIndex.row(), 1); // ether
    ether = etherModel->data(clickedIndex).toInt();
    clickedIndex = clickedIndex.sibling(clickedIndex.row(), 2); // description
    etherDesc = etherModel->data(clickedIndex).toString();

    if(etherId > 0)
        etherContextMenu.popup(ui->etherTV->mapToGlobal(ptr));
}

void DeviceManagementDialog::vpnContextMenuSlot(QPoint ptr)
{
    //`id`, `vpn`, `rd`, `description`
    QModelIndex clickedIndex = ui->vpnTV->indexAt(ptr);
    row = clickedIndex.row();

    clickedIndex = clickedIndex.sibling(clickedIndex.row(), 0); // id
    ui->vpnTV->selectRow(clickedIndex.row());
    ui->vpnTV->clicked(clickedIndex);
    vpnId = vpnModel->data(clickedIndex).toInt();
    if(vpnId > 0)
    {
        clickedIndex = clickedIndex.sibling(clickedIndex.row(), 1); // vpn
        vpn = vpnModel->data(clickedIndex).toString();
        clickedIndex = clickedIndex.sibling(clickedIndex.row(), 2); // rd
        vpnRD = vpnModel->data(clickedIndex).toString();
        clickedIndex = clickedIndex.sibling(clickedIndex.row(), 3); // description
        vpnDesc = vpnModel->data(clickedIndex).toString();

        vpnContextMenu.popup(ui->vpnTV->mapToGlobal(ptr));
    }
}

void DeviceManagementDialog::on_okBtn_clicked()
{
    bool done = true;
    int devexId = ui->deviceCB->currentData().toInt();
    //vlan update
    QMapIterator<int, QString> ite(vlanDescMap);
    while(ite.hasNext())
    {
        ite.next();
        if(!dbMan->insertUpdateVlan(devexId,ite.key(),ite.value()))
        {
            QMessageBox::warning(this,"ERROR","Operation Failed.\nUnable to update new VLANs.\n"+dbMan->lastError());
            done = false;
        }
    }

    //update ether trunk
    QMapIterator<int, QString> iter(etherDescMap);
    while(iter.hasNext())
    {
        iter.next();
        if(!dbMan->insertUpdateEtherTrunk(devexId,iter.key(),iter.value()))
        {
            QMessageBox::warning(this,"ERROR","Operation Failed.\nUnable to update new Eth-Trunks.\n"+dbMan->lastError());
            done = false;
        }
    }

    //update VPN
    QModelIndex vpnIndex;
    int Row = 0, row;
    QString vpn, rd,desc, exp,imp;
    while(Row < newVpnModel->rowCount())
    {
        vpnIndex = newVpnModel->index(Row,0);//row
        row = newVpnModel->data(vpnIndex).toInt();
        vpnIndex = newVpnModel->index(Row,1);//vpn
        vpn = newVpnModel->data(vpnIndex).toString();
        vpnIndex = newVpnModel->index(Row,2);//rd
        rd = newVpnModel->data(vpnIndex).toString();
        vpnIndex = newVpnModel->index(Row,3);//desc
        desc = newVpnModel->data(vpnIndex).toString();

        exp = newExpMap.value(row);
        imp = newImpMap.value(row);

        if(!dbMan->insertUpdateVpn(devexId,vpn, rd, desc, exp, imp))
        {
            QMessageBox::warning(this,"ERROR","Operation Failed.\nUnable to update new VPN.\n"+dbMan->lastError());
            done = false;
        }

        Row++;
    }


    if(done)
    {
        QMessageBox::information(this,"DONE","Successfully done.\nnew VLANs, Eth-Trunks and VPNs are updated");
        ui->checkBox->setChecked(false);
        ui->okBtn->setEnabled(false);
        int i = ui->deviceCB->currentIndex();
        ui->deviceCB->setCurrentIndex(-1);
        ui->deviceCB->setCurrentIndex(i);
        ui->infoLbl->clear();
    }
    else
        QMessageBox::information(this,"Error","Cannot Update.");


}

void DeviceManagementDialog::paintDevice(const QString device)
{
    ui->tabWidget->setTabEnabled(0,true);
    ui->deviceTab->setEnabled(true);

    if(device.contains("ASR9010", Qt::CaseInsensitive))
        drawASR9010();
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
    else if(device.contains("MA5600", Qt::CaseInsensitive))
        drawdslam16s("Huawei DSLAM MA5600");
    else if(device.contains("MA5616", Qt::CaseInsensitive))
        drawDslam4s();
    else if(device.contains("MXK819", Qt::CaseInsensitive))
        drawdslam16s("Zhone DSLAM MXK819");
    else
    {
        ui->tabWidget->setTabEnabled(0,false);
        ui->tabWidget->setCurrentIndex(1);
    }
}

void DeviceManagementDialog::on_checkBox_toggled(bool checked)
{
    QString usr = dbMan->getLoggedinUserame();
    if( (usr.compare(ADMIN) == 0) && checked)
        ui->okBtn->setEnabled(true);
    else
        ui->okBtn->setEnabled(false);
}

void DeviceManagementDialog::on_currentTab_currentChanged(int index)
{
    if(index == 0)
        ui->newTab->setCurrentIndex(0);
    else if(index == 1)
        ui->newTab->setCurrentIndex(1);
    else if(index == 2)
        ui->newTab->setCurrentIndex(2);
}

void DeviceManagementDialog::on_newTab_currentChanged(int index)
{
    if(index == 0)
        ui->currentTab->setCurrentIndex(0);
    else if(index == 1)
        ui->currentTab->setCurrentIndex(1);
    else if(index == 2)
        ui->currentTab->setCurrentIndex(2);
}

void DeviceManagementDialog::on_areaCB_currentIndexChanged(int index)
{
    ui->huaweiConfigLoadBtn->setEnabled(false);
    ui->ciscoConfigLoadBtn->setEnabled(false);
    ui->tabWidget->setEnabled(false);
    ui->tabWidget->setCurrentIndex(0);
    ui->abbrCB->blockSignals(true);
    ui->abbrCB->clear();
    ui->infoLbl->clear();
    vlanDescMap.clear();
    etherDescMap.clear();
    ui->checkBox->setEnabled(false);
    clearModels();
    ui->slotLbl->clear();
    qDeleteAll(ui->parentWidget->children());

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

void DeviceManagementDialog::on_abbrCB_currentIndexChanged(int index)
{
    index++;
    vlanDescMap.clear();
    etherDescMap.clear();
    ui->checkBox->setEnabled(false);
    ui->tabWidget->setEnabled(false);
    ui->tabWidget->setCurrentIndex(0);
    clearModels();
    ui->huaweiConfigLoadBtn->setEnabled(false);
    ui->ciscoConfigLoadBtn->setEnabled(false);
    ui->infoLbl->clear();
    ui->slotLbl->clear();
    ui->typeCB->setCurrentIndex(-1);
    ui->typeCB->setEnabled(true);
    qDeleteAll(ui->parentWidget->children());


    ui->siteCB->blockSignals(true);
    ui->siteCB->clear();
    ui->siteCB->blockSignals(false);
    ui->siteFrame->setEnabled(false);
    ui->siteFrame->setVisible(false);

    ui->deviceCB->clear();
    ui->deviceCB->setEnabled(false);

    ui->typeCB->setCurrentIndex(0);

}

void DeviceManagementDialog::on_typeCB_currentIndexChanged(int index)
{
    vlanDescMap.clear();
    etherDescMap.clear();
    ui->checkBox->setEnabled(false);
    ui->tabWidget->setEnabled(false);
    ui->tabWidget->setCurrentIndex(0);
    clearModels();
    ui->slotLbl->clear();
    qDeleteAll(ui->parentWidget->children());

    ui->huaweiConfigLoadBtn->setEnabled(false);
    ui->ciscoConfigLoadBtn->setEnabled(false);
    ui->infoLbl->clear();
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


        vlanDescMap.clear();
        etherDescMap.clear();
        ui->checkBox->setEnabled(false);
        clearModels();
        ui->huaweiConfigLoadBtn->setEnabled(false);
        ui->ciscoConfigLoadBtn->setEnabled(false);
        ui->infoLbl->clear();
        ui->deviceCB->setEnabled(true);
        ui->deviceCB->blockSignals(true);
        ui->deviceCB->clear();
        devexIdDeviceId.clear();
        devexIdSaloon.clear();

        if(ui->typeCB->currentData().toInt() == 2) //exch
        {
            int exchId = ui->abbrCB->currentData().toInt();

            QSqlQuery *query = dbMan->selectDevex(exchId, true);
        //0dx.`devex_id`, 1dx.`exchange_id`, 2dx.`saloon_name`, 3dx.`device_id`,
        //4dx.`row`, 5dx.`rack`, 6dx.`shelf`, 7dx.`device_name`,8ds.type,9dx.purchase,
        //10ds.sn,11dv.type
            int DEVEXID,DEVICEID;
            QString DEVICENAME, SALOON, pur, t,sn,dvType0, dvType1;
            while(query->next())
            {
                DEVEXID = query->value(0).toInt();
                SALOON = query->value(2).toString();
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
                devexIdSaloon.insert(DEVEXID, SALOON);
            }

            ui->deviceCB->setCurrentIndex(-1);
            ui->deviceCB->blockSignals(false);
        }



    }
}

void DeviceManagementDialog::on_siteCB_currentIndexChanged(int index)
{
    index++;
    vlanDescMap.clear();
    etherDescMap.clear();
    ui->checkBox->setEnabled(false);
    ui->tabWidget->setEnabled(false);
    ui->tabWidget->setCurrentIndex(0);
    clearModels();
    ui->slotLbl->clear();
    qDeleteAll(ui->parentWidget->children());
    ui->huaweiConfigLoadBtn->setEnabled(false);
    ui->ciscoConfigLoadBtn->setEnabled(false);
    ui->infoLbl->clear();
    int exchId = ui->siteCB->currentData().toInt();
    QSqlQuery *query = dbMan->selectDevex(exchId, true);
    //`id`, `exchange_id`, `saloon`, `device_id`, `rack_row`, `rack_no`, `shelf_no`, `device_name`
    ui->deviceCB->setEnabled(true);
    ui->deviceCB->blockSignals(true);
    ui->deviceCB->clear();

    devexIdDeviceId.clear();
    devexIdSaloon.clear();


    int DEVEXID,DEVICEID;
    QString DEVICENAME, SALOON, pur, t,sn,dvType0, dvType1;
    while(query->next())
    {
        DEVEXID = query->value(0).toInt();
        SALOON = query->value(2).toString();
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
        devexIdSaloon.insert(DEVEXID, SALOON);
    }

    ui->deviceCB->setCurrentIndex(-1);
    ui->deviceCB->blockSignals(false);


}

void DeviceManagementDialog::on_deviceCB_currentIndexChanged(int index)
{
    vlanDescMap.clear();
    etherDescMap.clear();
    ui->checkBox->setEnabled(false);
    clearModels();
    ui->slotLbl->clear();
    qDeleteAll(ui->parentWidget->children());

    ui->modChB->setChecked(false);
    ui->modChB->setEnabled(false);
    ui->modOkBtn->setEnabled(false);
    ui->modInfoLbl->clear();

    SR = dbMan->isDevexSR(ui->deviceCB->currentData().toInt());
    if(SR)
    {
        ui->modBrowsBtn->setEnabled(true);
        ui->modTab->setEnabled(true);
        fillModulesTV();
    }
    else
    {
        ui->modBrowsBtn->setEnabled(false);
        ui->modTab->setEnabled(false);
    }



    if(index > -1)
    {
        ui->tabWidget->setEnabled(true);
        ui->tabWidget->setCurrentIndex(0);

        int devexId = ui->deviceCB->currentData().toInt();
        int deviceId = devexIdDeviceId.value(devexId);

        QString slotString, device;

        QSqlQuery *query = dbMan->selectDevice(deviceId);
        //`id`, `device`, `sr`, `type`, `vendor`,  `slot_count`, `start_slot`
        if(query->next())
        {
            slotString = "Slot Count: "+query->value(5).toString()+"      Slot Start No: "+query->value(6).toString();
            device = query->value(1).toString();
        }

        ui->slotLbl->setText(slotString);

        paintDevice(device);
        fillVlanTV();
        fillEtherTrunkTV();
        fillVpnViews();

        bool huaweiDevice = dbMan->isDeviceHuaweiVendor(deviceId);
        bool ciscoDevice = dbMan->isDeviceCiscoVendor(deviceId);
        if(huaweiDevice)
        {
            ui->huaweiConfigLoadBtn->setEnabled(true);
            ui->ciscoConfigLoadBtn->setEnabled(false);

            DeviceVendor = 1;
            if(SR)
                ui->modCmLbl->setText("Capture Command:   display interface");
            else
                ui->modCmLbl->clear();
        }
        else if(ciscoDevice)
        {
            ui->huaweiConfigLoadBtn->setEnabled(false);
            ui->ciscoConfigLoadBtn->setEnabled(true);

            DeviceVendor = 2;
            if(SR)
                ui->modCmLbl->setText("Capture Command:   show interface");
            else
                ui->modCmLbl->clear();
        }
        else
        {
            ui->infoLbl->setText("Only CISCO and HUAWEI devices are supported!");
            ui->huaweiConfigLoadBtn->setEnabled(false);
            ui->ciscoConfigLoadBtn->setEnabled(false);

            DeviceVendor = 0;
            ui->modCmLbl->setText("Only CISCO and HUAWEI devices are supported!");
            ui->modTab->setEnabled(false);
            ui->modChB->setChecked(false);
            ui->modChB->setEnabled(false);
            ui->modOkBtn->setEnabled(false);
            ui->modBrowsBtn->setEnabled(false);
        }
    }
    else
    {
        ui->tabWidget->setEnabled(false);
        ui->tabWidget->setCurrentIndex(0);

    }

}

void DeviceManagementDialog::on_addVlanBtn_clicked()
{
    int devexId = ui->deviceCB->currentData().toInt();
    QString abbr = ui->abbrCB->currentText();
    QString saloon = devexIdSaloon.value(devexId);
    QString device = ui->deviceCB->currentText();

    AddVlanDialog *addVlanDialog = new AddVlanDialog(this,dbMan,devexId, abbr, saloon, device);
    addVlanDialog->setAttribute(Qt::WA_DeleteOnClose);
    connect(addVlanDialog, SIGNAL(finished(int)), this, SLOT(refreshVlanSlot(int)));
    addVlanDialog->show();
}

void DeviceManagementDialog::on_addEtherBtn_clicked()
{
    int devexId = ui->deviceCB->currentData().toInt();
    QString abbr = ui->abbrCB->currentText();
    QString saloon = devexIdSaloon.value(devexId);
    QString device = ui->deviceCB->currentText();

    AddEtherTrunkDialog *addEtherTrunkDialog = new AddEtherTrunkDialog(this,dbMan,devexId, abbr, saloon, device);
    addEtherTrunkDialog->setAttribute(Qt::WA_DeleteOnClose);
    connect(addEtherTrunkDialog, SIGNAL(finished(int)), this, SLOT(refreshEtherSlot(int)));
    addEtherTrunkDialog->show();
}

void DeviceManagementDialog::on_addVpnBtn_clicked()
{
    int devexId = ui->deviceCB->currentData().toInt();
    QString abbr = ui->areaCB->currentText()+'-'+ui->abbrCB->currentText(); // 2-ba
    if(ui->typeCB->currentData().toInt() == 3) // site
        abbr += "-"+ui->siteCB->currentText();
    QString saloon = devexIdSaloon.value(devexId);
    QString device = ui->deviceCB->currentText();

    AddVPNDialog *addVPNDialog = new AddVPNDialog(this, dbMan, devexId,abbr,saloon, device);
    addVPNDialog->setAttribute(Qt::WA_DeleteOnClose);
    connect(addVPNDialog, SIGNAL(finished(int)), this, SLOT(refreshVpnSlot(int)));
    addVPNDialog->show();
}

void DeviceManagementDialog::on_actionEditVlan_triggered()
{
    int devexId = ui->deviceCB->currentData().toInt();
    QString abbr = ui->abbrCB->currentText();
    QString saloon = devexIdSaloon.value(devexId);
    QString device = ui->deviceCB->currentText();

    EditVlanDialog *editVlanDialog = new EditVlanDialog(this,dbMan,vlanId, abbr, saloon, device, vlan, vlanDesc);
    editVlanDialog->setAttribute(Qt::WA_DeleteOnClose);
    connect(editVlanDialog, SIGNAL(finished(int)), this, SLOT(refreshVlanSlot(int)));
    editVlanDialog->show();
}

void DeviceManagementDialog::on_actionRemoveVlan_triggered()
{
    int devexId = ui->deviceCB->currentData().toInt();
    QString abbr = ui->abbrCB->currentText();
    QString saloon =  devexIdSaloon.value(devexId);
    QString device = ui->deviceCB->currentText();

    RemoveVlanDialog *removeVlanDialog = new RemoveVlanDialog(this,dbMan,vlanId, abbr, saloon, device, vlan, vlanDesc);
    removeVlanDialog->setAttribute(Qt::WA_DeleteOnClose);
    connect(removeVlanDialog, SIGNAL(finished(int)), this, SLOT(refreshVlanSlot(int)));
    removeVlanDialog->show();
}

void DeviceManagementDialog::on_actionEditEtherTrunk_triggered()
{
    int devexId = ui->deviceCB->currentData().toInt();
    QString abbr = ui->abbrCB->currentText();
    QString saloon = devexIdSaloon.value(devexId);
    QString device = ui->deviceCB->currentText();

    EditEtherTrunkDialog *editEtherTrunkDialog = new EditEtherTrunkDialog(this,dbMan,etherId, abbr, saloon, device, ether, etherDesc);
    editEtherTrunkDialog->setAttribute(Qt::WA_DeleteOnClose);
    connect(editEtherTrunkDialog, SIGNAL(finished(int)), this, SLOT(refreshEtherSlot(int)));
    editEtherTrunkDialog->show();
}

void DeviceManagementDialog::on_actionRemoveEtherTrunk_triggered()
{
    int devexId = ui->deviceCB->currentData().toInt();
    QString abbr = ui->abbrCB->currentText();
    QString saloon = devexIdSaloon.value(devexId);
    QString device = ui->deviceCB->currentText();

    RemoveEtherTrunkDialog *removeEtherTrunkDialog = new RemoveEtherTrunkDialog(this,dbMan,etherId, abbr, saloon, device, ether, etherDesc);
    removeEtherTrunkDialog->setAttribute(Qt::WA_DeleteOnClose);
    connect(removeEtherTrunkDialog, SIGNAL(finished(int)), this, SLOT(refreshEtherSlot(int)));
    removeEtherTrunkDialog->show();
}

void DeviceManagementDialog::on_actionEditVPN_triggered()
{
    int devexId = ui->deviceCB->currentData().toInt();
    QString abbr = ui->areaCB->currentText()+'-'+ui->abbrCB->currentText(); // 2-ba
    if(ui->typeCB->currentData().toInt() == 3) // site
        abbr += "-"+ui->siteCB->currentText();
    QString saloon = devexIdSaloon.value(devexId);
    QString device = ui->deviceCB->currentText();

    QString exp = expModel->stringList().join(" ");
    QString imp = impModel->stringList().join(" ");

    EditVPNDialog *editVPNDialog = new EditVPNDialog(this, dbMan,abbr,saloon, device, vpnId, vpn, vpnRD, vpnDesc,exp, imp);
    editVPNDialog->setAttribute(Qt::WA_DeleteOnClose);
    connect(editVPNDialog, SIGNAL(finished(int)), this, SLOT(refreshVpnSlot(int)));
    editVPNDialog->show();
}

void DeviceManagementDialog::on_actionRemoveVPN_triggered()
{
    int devexId = ui->deviceCB->currentData().toInt();
    QString abbr = ui->areaCB->currentText()+'-'+ui->abbrCB->currentText(); // 2-ba
    if(ui->typeCB->currentData().toInt() == 3) // site
        abbr += "-"+ui->siteCB->currentText();
    QString saloon = devexIdSaloon.value(devexId);
    QString device = ui->deviceCB->currentText();

    QString exp = expModel->stringList().join("  ");
    QString imp = impModel->stringList().join("  ");

    RemoveVPNDialog *removeVPNDialog = new RemoveVPNDialog(this, dbMan,abbr,saloon, device, vpnId, vpn, vpnRD, vpnDesc,exp, imp);
    removeVPNDialog->setAttribute(Qt::WA_DeleteOnClose);
    connect(removeVPNDialog, SIGNAL(finished(int)), this, SLOT(refreshVpnSlot(int)));
    removeVPNDialog->show();
}

void DeviceManagementDialog::on_vpnTV_clicked(const QModelIndex &index)
{
    QModelIndex ind = index.sibling(index.row(), 0); // vpn id index
    int vpnId = vpnModel->data(ind).toInt();
    QStringList exp,imp;

    if(vpnId > 0)
    {
        QSqlQuery *query = dbMan->selectVpnRT(vpnId);
        //`id`,`exp_imp`,`rt`

        bool expImp;
        while(query->next())
        {
            expImp = query->value(1).toBool();
            if(expImp) // imp
            {
                imp.append(query->value(2).toString());
            }
            else //exp
            {
                exp.append(query->value(2).toString());
            }
        }
    }

    expModel->setStringList(exp);
    impModel->setStringList(imp);
}

void DeviceManagementDialog::on_currVpnTV_clicked(const QModelIndex &index)
{
    QModelIndex ind = index.sibling(index.row(), 0); // vpn id index
    int vpnId = vpnModel->data(ind).toInt();
    QStringList exp,imp;

    if(vpnId > 0)
    {
        QSqlQuery *query = dbMan->selectVpnRT(vpnId);
        //`id`,`exp_imp`,`rt`

        bool expImp;
        while(query->next())
        {
            expImp = query->value(1).toBool();
            if(expImp) // imp
            {
                imp.append(query->value(2).toString());
            }
            else //exp
            {
                exp.append(query->value(2).toString());
            }
        }
    }

    expModel->setStringList(exp);
    impModel->setStringList(imp);
}

void DeviceManagementDialog::on_tabWidget_currentChanged(int index)
{
    QModelIndex ind = vpnModel->index(0,0);
    if(index == 3) // vpn
    {
        ui->vpnTV->selectRow(0);
        ui->vpnTV->clicked(ind);
    }else if(index == 4) //config
    {
        ui->currVpnTV->selectRow(0);
        ui->currVpnTV->clicked(ind);
    }
}

void DeviceManagementDialog::on_newVpnTV_clicked(const QModelIndex &index)
{
    QModelIndex ind = index.sibling(index.row(), 0); // row
    int row = newVpnModel->data(ind).toInt();
    QStringList expL,impL;

    expL = newExpMap.value(row).split(" ",QString::SkipEmptyParts);
    impL = newImpMap.value(row).split(" ",QString::SkipEmptyParts);

    newExpModel->setStringList(expL);
    newImpModel->setStringList(impL);
}

void DeviceManagementDialog::on_huaweiConfigLoadBtn_clicked()
{
    vlanDescMap.clear();
    etherDescMap.clear();
    ui->checkBox->setEnabled(false);
    bool intOK;
    delete newVlanModel;
    newVlanModel = NULL;
    delete newEtherModel;
    newEtherModel = NULL;
    ui->infoLbl->clear();

    newVpnModel->clear();
    newVpnModel->setColumnCount(4);
    //row,vpn,rd, desc
    newVpnModel->setHeaderData(1, Qt::Horizontal,"VPN");
    newVpnModel->setHeaderData(2, Qt::Horizontal,"Route Distinguisher");
    newVpnModel->setHeaderData(3, Qt::Horizontal,"Description");
    ui->newVpnTV->hideColumn(0);

    newExpMap.clear();
    newImpMap.clear();
    newExpModel->setStringList(QStringList{});
    newImpModel->setStringList(QStringList{});

    QString configFileName = QFileDialog::getOpenFileName(this,"HUAWEI Device Current Configuration");
    if(configFileName.isEmpty()) return;

    QFile configFile(configFileName);
    QTextStream configStream(&configFile);

    QModelIndex newVpnIndex;
    int newVpnRow = 0;

    newEtherModel = new MapModel(this, QStringList()<<"Ether-Trunk"<<"Description");
    newVlanModel = new MapModel(this,QStringList()<<"VLAN"<<"Description");

    if(configFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QString line, sysname,desc, vpn, rd, exp, imp;
        int ind, vlan,ether;

        line = configStream.readLine().trimmed();
        bool SYSNAME = true;
        while(!line.isNull())
        {
            //purify remove ---- More ----  and space
            if(line.contains("---- More ----",Qt::CaseInsensitive))
                line = line.remove("---- More ----",Qt::CaseInsensitive);
            line = line.trimmed();

            if(SYSNAME)
            {
                if(line.startsWith("sysname", Qt::CaseInsensitive))
                {
                    sysname = line;
                    ui->infoLbl->setText(sysname);
                    SYSNAME = false;
                }
            }

            //check vlan 4010 desc ...
            if(line.startsWith("vlan", Qt::CaseInsensitive) && !line.contains("batch", Qt::CaseInsensitive))
            {
                line = line.remove("vlan", Qt::CaseInsensitive).trimmed();
                vlan = line.toInt(&intOK);
                line = configStream.readLine().trimmed();
                if(intOK == false)
                {
                    continue;
                }

                //purify remove ---- More ----  and space
                if(line.contains("---- More ----",Qt::CaseInsensitive))
                    line = line.remove("---- More ----",Qt::CaseInsensitive).trimmed();

                while( !(line.startsWith("#") || line.startsWith("interface", Qt::CaseInsensitive) || line.isNull() ))
                {
                    if(line.contains("description"))
                    {
                        ind = line.indexOf("description",0,Qt::CaseInsensitive);
                        if(ind > 0)
                            line = line.remove(0,ind-1).trimmed();
                        line = line.remove("description",Qt::CaseInsensitive);
                        desc = line;
                        vlanDescMap.insert(vlan,desc);
                        break;
                    }

                    line = configStream.readLine().trimmed();
                    //purify remove ---- More ----  and space
                    if(line.contains("---- More ----",Qt::CaseInsensitive))
                        line = line.remove("---- More ----",Qt::CaseInsensitive).trimmed();
                }

                continue;
            }
            //check vlan batch 10 11 4020 to 4030 > put to array by splitting based on space and ...
            else if(line.startsWith("vlan batch", Qt::CaseInsensitive))
            {
                line = line.remove("vlan batch", Qt::CaseInsensitive).trimmed();
                QStringList list = line.split(" ",QString::SkipEmptyParts,Qt::CaseInsensitive);
                int i,val,nextVal;
                QString Str;
                i = 0;
                while(i < list.length())
                {
                    if(i < (list.length()-1))
                    {
                        Str = list[i+1];
                        if(Str.compare("to", Qt::CaseInsensitive) == 0)
                        { // 10 to 20
                            Str = list[i];
                            val = Str.toInt(&intOK);
                            if(intOK == false)
                            {
                                i++;
                                continue;
                            }

                            Str = list[i+1];
                            nextVal = Str.toInt(&intOK);
                            if(intOK == false)
                            {
                                i++;
                                continue;
                            }

                            for(int j = val; j <= nextVal; j++)
                            {
                                desc = "VLAN-"+QString::number(j);
                                vlanDescMap.insert(j, desc);
                            }
                            i += 2;
                        }else
                        {
                            Str = list[i];
                            vlan = Str.toInt(&intOK);
                            if(intOK == false)
                            {
                                i++;
                                continue;
                            }else
                            {
                                desc = "VLAN-"+Str;
                                vlanDescMap.insert(vlan, desc);
                            }
                        }

                    }
                    else
                    {
                        Str = list[i];
                        vlan = Str.toInt(&intOK);
                        if(intOK == false)
                        {
                            i++;
                            continue;
                        }else
                        {
                            desc = "VLAN-"+Str;
                            vlanDescMap.insert(vlan, desc);
                        }
                    }

                    i++;
                }
            }
            // check VLANIF
            else if(line.startsWith("interface Vlanif", Qt::CaseInsensitive))
            {
                ind = line.indexOf("interface Vlanif",0,Qt::CaseInsensitive);
                if(ind > 0)
                    line = line.remove(0,ind-1).trimmed();
                line = line.remove("interface Vlanif",Qt::CaseInsensitive).trimmed();
                vlan = line.toInt(&intOK);
                line = configStream.readLine().trimmed();
                if(intOK == false)
                {
                    continue;
                }

                //purify remove ---- More ----  and space
                if(line.contains("---- More ----",Qt::CaseInsensitive))
                    line  = line.remove("---- More ----",Qt::CaseInsensitive).trimmed();
                while( !(line.startsWith("#") || line.startsWith("interface", Qt::CaseInsensitive) || line.isNull() ))
                {
                    if(line.contains("description"))
                    {
                        ind = line.indexOf("description",0,Qt::CaseInsensitive);
                        if(ind > 0)
                            line = line.remove(0,ind-1).trimmed();
                        line = line.remove("description",Qt::CaseInsensitive);
                        desc = line;
                        vlanDescMap.insert(vlan,desc);
                        break;
                    }

                    line = configStream.readLine().trimmed();
                    //purify remove ---- More ----  and space
                    if(line.contains("---- More ----",Qt::CaseInsensitive))
                        line = line.remove("---- More ----",Qt::CaseInsensitive).trimmed();
                }

                continue;
            }
            // check Ether-Trunk
            else if(line.startsWith("interface Eth-Trunk", Qt::CaseInsensitive))
            {
                if(!line.contains("."))
                {
                    ind = line.indexOf("interface Eth-Trunk",0,Qt::CaseInsensitive);
                    if(ind > 0)
                        line = line.remove(0,ind-1).trimmed();
                    line = line.remove("interface Eth-Trunk",Qt::CaseInsensitive);
                    ether = line.toInt(&intOK);
                    line = configStream.readLine().trimmed();
                    if(intOK == false)
                    {
                        continue;
                    }

                    //purify remove ---- More ----  and space
                    if(line.contains("---- More ----",Qt::CaseInsensitive))
                        line = line.remove("---- More ----",Qt::CaseInsensitive);
                    line = line.trimmed();
                    while( !(line.startsWith("#") || line.startsWith("interface", Qt::CaseInsensitive) || line.isNull() ))
                    {
                        if(line.contains("description"))
                        {
                            ind = line.indexOf("description",0,Qt::CaseInsensitive);
                            if(ind > 0)
                                line = line.remove(0,ind-1).trimmed();
                            line = line.remove("description",Qt::CaseInsensitive);
                            desc = line;
                            etherDescMap.insert(ether,desc);
                            break;
                        }

                        line = configStream.readLine().trimmed();
                        //purify remove ---- More ----  and space
                        if(line.contains("---- More ----",Qt::CaseInsensitive))
                            line = line.remove("---- More ----",Qt::CaseInsensitive);
                        line = line.trimmed();

                    }

                    continue;
                }
            }
            // check VPN
            else if(line.startsWith("ip vpn-instance", Qt::CaseInsensitive))
            {
                ind = line.indexOf("ip vpn-instance",0,Qt::CaseInsensitive);
                if(ind > 0)
                    line = line.remove(0,ind-1).trimmed();
                line = line.remove("ip vpn-instance",Qt::CaseInsensitive).trimmed();
                vpn = line;
                line = configStream.readLine().trimmed();
                //purify remove ---- More ----  and space
                if(line.contains("---- More ----",Qt::CaseInsensitive))
                    line = line.remove("---- More ----",Qt::CaseInsensitive);
                line = line.trimmed();

                desc = "";
                rd = "";
                exp = "";
                imp = "";

                while( !(line.startsWith("#") || line.startsWith("ip vpn-instance", Qt::CaseInsensitive) || line.isNull() ) )
                {

                    if(line.contains("description"))
                    {
                        line = line.remove("description",Qt::CaseInsensitive).trimmed();
                        desc = line;
                    }
                    else if(line.contains("route-distinguisher"))
                    {
                        line = line.remove("route-distinguisher",Qt::CaseInsensitive).trimmed();
                        rd = line;
                    }
                    else if(line.contains("vpn-target"))
                    {
                        line = line.remove("vpn-target",Qt::CaseInsensitive).trimmed();

                        if(line.contains("export-extcommunity"))
                        {
                            line = line.remove("export-extcommunity",Qt::CaseInsensitive).trimmed();
                            exp = line;
                        }
                        else if(line.contains("import-extcommunity"))
                        {
                            line = line.remove("import-extcommunity",Qt::CaseInsensitive).trimmed();
                            imp = line;
                        }
                    }



                    line = configStream.readLine().trimmed();
                    //purify remove ---- More ----  and space
                    if(line.contains("---- More ----",Qt::CaseInsensitive))
                        line = line.remove("---- More ----",Qt::CaseInsensitive);
                    line = line.trimmed();
                }

                newVpnModel->insertRow(newVpnRow);

                newVpnIndex = newVpnModel->index(newVpnRow, 0); //row
                newVpnModel->setData(newVpnIndex,newVpnRow);

                newVpnIndex = newVpnModel->index(newVpnRow, 1); //vpn
                newVpnModel->setData(newVpnIndex,vpn);

                newVpnIndex = newVpnModel->index(newVpnRow, 2); //rd
                newVpnModel->setData(newVpnIndex,rd);

                newVpnIndex = newVpnModel->index(newVpnRow, 3); //desc
                newVpnModel->setData(newVpnIndex,desc);

                newExpMap.insert(newVpnRow, exp);
                newImpMap.insert(newVpnRow, imp);

                newVpnRow++;

                continue;
            }

            line = configStream.readLine().trimmed();
        }

        newVlanModel->setMap(&vlanDescMap);
        ui->newVlanTV->setModel(newVlanModel);
        newEtherModel->setMap(&etherDescMap);
        ui->newEtherTV->setModel(newEtherModel);

        if(!SYSNAME)
            ui->checkBox->setEnabled(true);
        else
            ui->checkBox->setEnabled(false);
    }

    configFile.close();

    QString usr = dbMan->getLoggedinUserame();
    if(usr.compare(ADMIN) == 0)
        ui->checkBox->setEnabled(true);
    else
        ui->checkBox->setEnabled(false);

}

void DeviceManagementDialog::on_ciscoConfigLoadBtn_clicked()
{
    QMessageBox::warning(this, "Information", "This feature is not supported yet!");
}

void DeviceManagementDialog::drawASR9010()
{
    slotModel->clear();


    int devexId = ui->deviceCB->currentData().toInt();

    QSqlQuery *query = dbMan->selectViewSlotsBrief(devexId, true);
    //`shelf`, `slot`, `sub_slot`, `card`
    slotModel->setQuery(*query);

    QHBoxLayout *layout = new QHBoxLayout(ui->parentWidget);
    layout->addItem(new QSpacerItem(10,10,QSizePolicy::Expanding,QSizePolicy::Minimum));
    ASR9010Widget *asr = new ASR9010Widget(this, slotModel);
    layout->addWidget(asr);
    layout->addItem(new QSpacerItem(10,10,QSizePolicy::Expanding,QSizePolicy::Minimum));

}

void DeviceManagementDialog::drawC7613()
{
    slotModel->clear();

    int devexId = ui->deviceCB->currentData().toInt();

    QSqlQuery *query = dbMan->selectViewSlotsBrief(devexId, true);
    //`shelf`, `slot`, `sub_slot`, `card`
    slotModel->setQuery(*query);

    QHBoxLayout *layout = new QHBoxLayout(ui->parentWidget);
    layout->addItem(new QSpacerItem(10,10,QSizePolicy::Expanding,QSizePolicy::Minimum));
    C7613Widget *c7613 = new C7613Widget(this,slotModel);
    layout->addWidget(c7613);
    layout->addItem(new QSpacerItem(10,10,QSizePolicy::Expanding,QSizePolicy::Minimum));
}

void DeviceManagementDialog::drawCX200()
{
    slotModel->clear();



    QHBoxLayout *layout = new QHBoxLayout(ui->parentWidget);
    layout->addItem(new QSpacerItem(10,10,QSizePolicy::Expanding,QSizePolicy::Minimum));
    CX200Widget *cx200 = new CX200Widget(this);
    layout->addWidget(cx200);
    layout->addItem(new QSpacerItem(10,10,QSizePolicy::Expanding,QSizePolicy::Minimum));
}

void DeviceManagementDialog::drawME60X16()
{
    slotModel->clear();

    int devexId = ui->deviceCB->currentData().toInt();

    QSqlQuery *query = dbMan->selectViewSlotsBrief(devexId, true);
    //`shelf`, `slot`, `sub_slot`, `card`
    slotModel->setQuery(*query);

    QHBoxLayout *layout = new QHBoxLayout(ui->parentWidget);
    layout->addItem(new QSpacerItem(10,10,QSizePolicy::Expanding,QSizePolicy::Minimum));
    ME60X16Widget *me60x16 = new ME60X16Widget(this, slotModel);
    layout->addWidget(me60x16);
    layout->addItem(new QSpacerItem(10,10,QSizePolicy::Expanding,QSizePolicy::Minimum));
}

void DeviceManagementDialog::drawME60X8()
{
    slotModel->clear();

    int devexId = ui->deviceCB->currentData().toInt();

    QSqlQuery *query = dbMan->selectViewSlotsBrief(devexId, true);
    //`shelf`, `slot`, `sub_slot`, `card`
    slotModel->setQuery(*query);

    QHBoxLayout *layout = new QHBoxLayout(ui->parentWidget);
    layout->addItem(new QSpacerItem(10,10,QSizePolicy::Expanding,QSizePolicy::Minimum));
    ME60X8Widget *me60x8 = new ME60X8Widget(this, slotModel);
    layout->addWidget(me60x8);
    layout->addItem(new QSpacerItem(10,10,QSizePolicy::Expanding,QSizePolicy::Minimum));
}

void DeviceManagementDialog::drawNE40X16()
{
    slotModel->clear();

    int devexId = ui->deviceCB->currentData().toInt();

    QSqlQuery *query = dbMan->selectViewSlotsBrief(devexId, true);
    //`shelf`, `slot`, `sub_slot`, `card`
    slotModel->setQuery(*query);

    QHBoxLayout *layout = new QHBoxLayout(ui->parentWidget);
    layout->addItem(new QSpacerItem(10,10,QSizePolicy::Expanding,QSizePolicy::Minimum));
    NE40X16Widget *ne40x16 = new NE40X16Widget(this,true, slotModel);
    layout->addWidget(ne40x16);
    layout->addItem(new QSpacerItem(10,10,QSizePolicy::Expanding,QSizePolicy::Minimum));
}

void DeviceManagementDialog::drawNE40X8()
{
    slotModel->clear();

    int devexId = ui->deviceCB->currentData().toInt();

    QSqlQuery *query = dbMan->selectViewSlotsBrief(devexId, true);
    //`shelf`, `slot`, `sub_slot`, `card`
    slotModel->setQuery(*query);

    QHBoxLayout *layout = new QHBoxLayout(ui->parentWidget);
    layout->addItem(new QSpacerItem(10,10,QSizePolicy::Expanding,QSizePolicy::Minimum));
    NE40X8Widget *ne40x8 = new NE40X8Widget(this,true, slotModel);
    layout->addWidget(ne40x8);
    layout->addItem(new QSpacerItem(10,10,QSizePolicy::Expanding,QSizePolicy::Minimum));
}

void DeviceManagementDialog::drawCX600X16()
{
    slotModel->clear();

    int devexId = ui->deviceCB->currentData().toInt();

    QSqlQuery *query = dbMan->selectViewSlotsBrief(devexId, true);
    //`shelf`, `slot`, `sub_slot`, `card`
    slotModel->setQuery(*query);

    QHBoxLayout *layout = new QHBoxLayout(ui->parentWidget);
    layout->addItem(new QSpacerItem(10,10,QSizePolicy::Expanding,QSizePolicy::Minimum));
    NE40X16Widget *cx600x16 = new NE40X16Widget(this,false, slotModel);
    layout->addWidget(cx600x16);
    layout->addItem(new QSpacerItem(10,10,QSizePolicy::Expanding,QSizePolicy::Minimum));
}

void DeviceManagementDialog::drawCX600X8()
{
    slotModel->clear();

    int devexId = ui->deviceCB->currentData().toInt();

    QSqlQuery *query = dbMan->selectViewSlotsBrief(devexId, true);
    //`shelf`, `slot`, `sub_slot`, `card`
    slotModel->setQuery(*query);

    QHBoxLayout *layout = new QHBoxLayout(ui->parentWidget);
    layout->addItem(new QSpacerItem(10,10,QSizePolicy::Expanding,QSizePolicy::Minimum));
    NE40X8Widget *cx600x8 = new NE40X8Widget(this,false, slotModel);
    layout->addWidget(cx600x8);
    layout->addItem(new QSpacerItem(10,10,QSizePolicy::Expanding,QSizePolicy::Minimum));
}

void DeviceManagementDialog::drawNE5000X16A()
{
    slotModel->clear();

    int devexId = ui->deviceCB->currentData().toInt();

    QSqlQuery *query = dbMan->selectViewSlotsBrief(devexId, true);
    //`shelf`, `slot`, `sub_slot`, `card`
    slotModel->setQuery(*query);

    QHBoxLayout *layout = new QHBoxLayout(ui->parentWidget);
    layout->addItem(new QSpacerItem(10,10,QSizePolicy::Expanding,QSizePolicy::Minimum));
    NE5000X16AWidget *ne5000X16AWidget = new NE5000X16AWidget(this,true, slotModel);
    layout->addWidget(ne5000X16AWidget);
    layout->addItem(new QSpacerItem(10,10,QSizePolicy::Expanding,QSizePolicy::Minimum));
}

void DeviceManagementDialog::drawNE5000X16()
{
    slotModel->clear();

    int devexId = ui->deviceCB->currentData().toInt();

    QSqlQuery *query = dbMan->selectViewSlotsBrief(devexId, true);
    //`shelf`, `slot`, `sub_slot`, `card`
    slotModel->setQuery(*query);

    QHBoxLayout *layout = new QHBoxLayout(ui->parentWidget);
    layout->addItem(new QSpacerItem(10,10,QSizePolicy::Expanding,QSizePolicy::Minimum));
    NE5000X16Widget *ne5000 = new NE5000X16Widget(this, slotModel);
    layout->addWidget(ne5000);
    layout->addItem(new QSpacerItem(10,10,QSizePolicy::Expanding,QSizePolicy::Minimum));
}

void DeviceManagementDialog::drawS2403H()
{
    slotModel->clear();


    QHBoxLayout *layout = new QHBoxLayout(ui->parentWidget);
    layout->addItem(new QSpacerItem(10,10,QSizePolicy::Expanding,QSizePolicy::Minimum));
    S2403HWidget *s2403 = new S2403HWidget(this);
    layout->addWidget(s2403);
    layout->addItem(new QSpacerItem(10,10,QSizePolicy::Expanding,QSizePolicy::Minimum));
}

void DeviceManagementDialog::drawS5300()
{
    slotModel->clear();



    QHBoxLayout *layout = new QHBoxLayout(ui->parentWidget);
    layout->addItem(new QSpacerItem(10,10,QSizePolicy::Expanding,QSizePolicy::Minimum));
    S5300Widget *s5300 = new S5300Widget(this);
    layout->addWidget(s5300);
    layout->addItem(new QSpacerItem(10,10,QSizePolicy::Expanding,QSizePolicy::Minimum));
}

void DeviceManagementDialog::drawS9303()
{
    slotModel->clear();

    int devexId = ui->deviceCB->currentData().toInt();

    QSqlQuery *query = dbMan->selectViewSlotsBrief(devexId, true);
    //`shelf`, `slot`, `sub_slot`, `card`
    slotModel->setQuery(*query);

    QHBoxLayout *layout = new QHBoxLayout(ui->parentWidget);
    layout->addItem(new QSpacerItem(10,10,QSizePolicy::Expanding,QSizePolicy::Minimum));
    S9303Widget *s9303 = new S9303Widget(this, slotModel);
    layout->addWidget(s9303);
    layout->addItem(new QSpacerItem(10,10,QSizePolicy::Expanding,QSizePolicy::Minimum));
}

void DeviceManagementDialog::drawS9306()
{
    slotModel->clear();

    int devexId = ui->deviceCB->currentData().toInt();

    QSqlQuery *query = dbMan->selectViewSlotsBrief(devexId, true);
    //`shelf`, `slot`, `sub_slot`, `card`
    slotModel->setQuery(*query);

    QHBoxLayout *layout = new QHBoxLayout(ui->parentWidget);
    layout->addItem(new QSpacerItem(10,10,QSizePolicy::Expanding,QSizePolicy::Minimum));
    S9306Widget *s9306 = new S9306Widget(this, slotModel);
    layout->addWidget(s9306);
    layout->addItem(new QSpacerItem(10,10,QSizePolicy::Expanding,QSizePolicy::Minimum));
}

void DeviceManagementDialog::drawDslam4s()
{
    slotModel->clear();

    int devexId = ui->deviceCB->currentData().toInt();

    QSqlQuery *query = dbMan->selectViewSlotsBrief(devexId, true);
    //`shelf`, `slot`, `sub_slot`, `card`
    slotModel->setQuery(*query);

    QHBoxLayout *layout = new QHBoxLayout(ui->parentWidget);
    layout->addItem(new QSpacerItem(10,10,QSizePolicy::Expanding,QSizePolicy::Minimum));
    Dslam4sWidget *dslam4s = new Dslam4sWidget(this, slotModel);
    layout->addWidget(dslam4s);
    layout->addItem(new QSpacerItem(10,10,QSizePolicy::Expanding,QSizePolicy::Minimum));
}

void DeviceManagementDialog::drawdslam16s(const QString DeviceName)
{
    slotModel->clear();

    int devexId = ui->deviceCB->currentData().toInt();

    QSqlQuery *query = dbMan->selectViewSlotsBrief(devexId, true);
    //`shelf`, `slot`, `sub_slot`, `card`
    slotModel->setQuery(*query);

    QHBoxLayout *layout = new QHBoxLayout(ui->parentWidget);
    layout->addItem(new QSpacerItem(10,10,QSizePolicy::Expanding,QSizePolicy::Minimum));
    Dslam16sWidget *dslam16s = new Dslam16sWidget(this, DeviceName, slotModel);
    layout->addWidget(dslam16s);
    layout->addItem(new QSpacerItem(10,10,QSizePolicy::Expanding,QSizePolicy::Minimum));
}

void DeviceManagementDialog::huaweiModulesAnalyser()
{
    int ind;
    idModuleMap.clear();


    QTextStream configStream(&configFile);

    QString Line, Interface, Module;

    if(configFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        Line = configStream.readLine().trimmed();
        while(!Line.isNull())
        {
            //purify remove ---- More ----  and space
            if(Line.contains("---- More ----",Qt::CaseInsensitive))
                Line = Line.remove("---- More ----",Qt::CaseInsensitive);
            Line = Line.trimmed();

            //interfaces start
            if( (Line.contains("/", Qt::CaseInsensitive)) && (Line.contains("current state :", Qt::CaseInsensitive)) && (!Line.contains("protocol current state :", Qt::CaseInsensitive)) )
            {
                Interface = "";
                Module = "";

                ind = Line.indexOf("current state :",Qt::CaseInsensitive);
                Line = Line.remove(ind,Line.length());
                Interface = Line.trimmed();
                Interface = Interface.remove("GigabitEthernet", Qt::CaseInsensitive);
                Interface = Interface.remove("TenGigE", Qt::CaseInsensitive);
                Interface = Interface.remove("Ethernet", Qt::CaseInsensitive);
                Interface = Interface.remove("XGigabitEthernet", Qt::CaseInsensitive);
                Interface = Interface.remove("TenGigabitEthernet", Qt::CaseInsensitive);
                ind = Interface.indexOf("net",Qt::CaseInsensitive);
                Interface = Interface.remove(0,ind);
                Interface = Interface.remove("net", Qt::CaseInsensitive);
                Interface = Interface.trimmed();

                Line = configStream.readLine().trimmed();
                while( (!Line.isNull()) && (Line.contains("protocol current state :", Qt::CaseInsensitive)) )
                    Line = configStream.readLine().trimmed();

                //purify remove ---- More ----  and space
                if(Line.contains("---- More ----",Qt::CaseInsensitive))
                    Line = Line.remove("---- More ----",Qt::CaseInsensitive);
                Line = Line.trimmed();

                while( (!Line.isNull()) && (!Line.contains("current state :", Qt::CaseInsensitive)))
                {
                    if(Line.contains("Distance:", Qt::CaseInsensitive))
                    {
                        if(Line.contains("300m", Qt::CaseInsensitive))
                            Module = "SX-300m";
                        else if(Line.contains("500m", Qt::CaseInsensitive))
                            Module = "SX-500m";
                        else if(Line.contains("10km", Qt::CaseInsensitive))
                            Module = "LX-10Km";
                        else if(Line.contains("15km", Qt::CaseInsensitive))
                            Module = "LX-15Km";
                        else if(Line.contains("20km", Qt::CaseInsensitive))
                            Module = "LX-20Km";
                        else if(Line.contains("40km", Qt::CaseInsensitive))
                            Module = "LX-40Km";
                        else if(Line.contains("80km", Qt::CaseInsensitive))
                            Module = "ZX-80Km";
                        else if(Line.contains("100km", Qt::CaseInsensitive))
                            Module = "ZX-100Km";
                        else if(Line.contains("120km", Qt::CaseInsensitive))
                            Module = "ZX-120Km";
                    }

                    Line = configStream.readLine().trimmed();
                    //purify remove ---- More ----  and space
                    if(Line.contains("---- More ----",Qt::CaseInsensitive))
                        Line = Line.remove("---- More ----",Qt::CaseInsensitive);
                    Line = Line.trimmed();

                }

                if( (!Interface.isEmpty()) && (!Module.isEmpty()) )
                    setModelModule(Interface, Module);
            }
            else
                Line = configStream.readLine().trimmed();
        }
    }

    configFile.close();

    if(idModuleMap.count() > 0)
        ui->modChB->setEnabled(true);
    else
        ui->modChB->setEnabled(false);
}

void DeviceManagementDialog::IOSModulesAnalyser()
{
    int ind;
    idModuleMap.clear();


    QTextStream configStream(&configFile);

    QString Line, Interface, Module;

    if(configFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        Line = configStream.readLine().trimmed();
        while(!Line.isNull())
        {
            //purify remove ---- More ----  and space
            if(Line.contains("--More--",Qt::CaseInsensitive))
                Line = Line.remove("--More--",Qt::CaseInsensitive);
            Line = Line.trimmed();

            //interfaces start
            if( (Line.contains("/", Qt::CaseInsensitive)) && (Line.contains("line protocol is", Qt::CaseInsensitive)) )
            {
                Interface = "";
                Module = "";

                ind = Line.indexOf("is",Qt::CaseInsensitive);
                Line = Line.remove(ind,Line.length());

                Interface = Line.trimmed();
                Interface = Interface.remove("GigabitEthernet", Qt::CaseInsensitive);
                Interface = Interface.remove("TenGigE", Qt::CaseInsensitive);
                Interface = Interface.remove("Ethernet", Qt::CaseInsensitive);
                Interface = Interface.remove("XGigabitEthernet", Qt::CaseInsensitive);
                Interface = Interface.remove("TenGigabitEthernet", Qt::CaseInsensitive);
                ind = Interface.indexOf("net",Qt::CaseInsensitive);
                Interface = Interface.remove(0,ind);
                Interface = Interface.remove("net", Qt::CaseInsensitive);
                Interface = Interface.trimmed();

                Line = configStream.readLine().trimmed();

                //purify remove ---- More ----  and space
                if(Line.contains("--More--",Qt::CaseInsensitive))
                    Line = Line.remove("--More--",Qt::CaseInsensitive);
                Line = Line.trimmed();

                while( (!Line.isNull()) && (!Line.contains("line protocol is", Qt::CaseInsensitive)) )
                {

                    if((Line.contains("duplex", Qt::CaseInsensitive)) && (Line.contains("b/s", Qt::CaseInsensitive)))
                    {
                        ind = Line.indexOf("b/s", Qt::CaseInsensitive);
                        Line = Line.remove(0,ind);
                        ind = Line.indexOf("link type", Qt::CaseInsensitive);
                        Line = Line.remove(ind,Line.length());

                        if(Line.contains("SR", Qt::CaseInsensitive))
                            Module = "SR";
                        else if(Line.contains("LR", Qt::CaseInsensitive))
                            Module = "LR";
                        else if(Line.contains("ER", Qt::CaseInsensitive))
                            Module = "ER";
                        else if(Line.contains("ZR", Qt::CaseInsensitive))
                            Module = "ZR";
                    }


                    Line = configStream.readLine().trimmed();
                    //purify remove ---- More ----  and space
                    if(Line.contains("--More--",Qt::CaseInsensitive))
                        Line = Line.remove("--More--",Qt::CaseInsensitive);
                    Line = Line.trimmed();

                }

                if( (!Interface.isEmpty()) && (!Module.isEmpty()) )
                    setModelModule(Interface, Module);
            }
            else
                Line = configStream.readLine().trimmed();
        }
    }

    configFile.close();

    if(idModuleMap.count() > 0)
        ui->modChB->setEnabled(true);
    else
        ui->modChB->setEnabled(false);
}

void DeviceManagementDialog::IOSXRModulesAnalyser()
{
    int ind;
    idModuleMap.clear();


    QTextStream configStream(&configFile);

    QString Line, Interface, Module;

    if(configFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        Line = configStream.readLine().trimmed();
        while(!Line.isNull())
        {
            //purify remove ---- More ----  and space
            if(Line.contains("--More--",Qt::CaseInsensitive))
                Line = Line.remove("--More--",Qt::CaseInsensitive);
            Line = Line.trimmed();

            //interfaces start
            if( (Line.contains("/", Qt::CaseInsensitive)) && (Line.contains("line protocol is", Qt::CaseInsensitive)) )
            {
                Interface = "";
                Module = "";

                ind = Line.indexOf("is",Qt::CaseInsensitive);
                Line = Line.remove(ind,Line.length());

                Interface = Line.trimmed();
                Interface = Interface.remove("GigabitEthernet", Qt::CaseInsensitive);
                Interface = Interface.remove("TenGigE", Qt::CaseInsensitive);
                Interface = Interface.remove("Ethernet", Qt::CaseInsensitive);
                Interface = Interface.remove("XGigabitEthernet", Qt::CaseInsensitive);
                Interface = Interface.remove("TenGigabitEthernet", Qt::CaseInsensitive);
                ind = Interface.indexOf("net",Qt::CaseInsensitive);
                Interface = Interface.remove(0,ind);
                Interface = Interface.remove("net", Qt::CaseInsensitive);
                Interface = Interface.trimmed();

                Line = configStream.readLine().trimmed();

                //purify remove ---- More ----  and space
                if(Line.contains("--More--",Qt::CaseInsensitive))
                    Line = Line.remove("--More--",Qt::CaseInsensitive);
                Line = Line.trimmed();

                while( (!Line.isNull()) && (!Line.contains("line protocol is", Qt::CaseInsensitive)) )
                {

                    if((Line.contains("duplex", Qt::CaseInsensitive)) && (Line.contains("b/s", Qt::CaseInsensitive)))
                    {
                        ind = Line.indexOf("b/s", Qt::CaseInsensitive);
                        Line = Line.remove(0,ind);
                        ind = Line.indexOf("link type", Qt::CaseInsensitive);
                        Line = Line.remove(ind,Line.length());

                        if(Line.contains("SR", Qt::CaseInsensitive))
                            Module = "SR";
                        else if(Line.contains("LR", Qt::CaseInsensitive))
                            Module = "LR";
                        else if(Line.contains("ER", Qt::CaseInsensitive))
                            Module = "ER";
                        else if(Line.contains("ZR", Qt::CaseInsensitive))
                            Module = "ZR";
                    }


                    Line = configStream.readLine().trimmed();
                    //purify remove ---- More ----  and space
                    if(Line.contains("--More--",Qt::CaseInsensitive))
                        Line = Line.remove("--More--",Qt::CaseInsensitive);
                    Line = Line.trimmed();

                }

                if( (!Interface.isEmpty()) && (!Module.isEmpty()) )
                    setModelModule(Interface, Module);
            }
            else
                Line = configStream.readLine().trimmed();
        }
    }

    configFile.close();

    if(idModuleMap.count() > 0)
        ui->modChB->setEnabled(true);
    else
        ui->modChB->setEnabled(false);
}

void DeviceManagementDialog::setModelModule(const QString interface, const QString module)
{
    int id;
    QString INTER,mod;
    QModelIndex ind = moduleModel->index(0,0);
    QBrush redBrush, blackBrush;
    redBrush.setColor(Qt::darkRed);
    blackBrush.setColor(Qt::black);
    for(int r = 0; r < moduleModel->rowCount(); r++)
    {
        ind = ind.sibling(r, 3);
        INTER = moduleModel->data(ind).toString().trimmed();
        if(interface.compare(INTER, Qt::CaseInsensitive) == 0)
        {
            ind = ind.sibling(r,0);
            id = moduleModel->data(ind).toInt();

            ind = ind.sibling(r,4);
            mod = moduleModel->data(ind).toString();

            ind = ind.sibling(r,5);

            if(mod.compare(module, Qt::CaseInsensitive) == 0)
            {
                moduleModel->setData(ind,blackBrush,Qt::ForegroundRole);
                moduleModel->setData(ind, module);


                ind = ind.sibling(r,4);
                moduleModel->setData(ind,blackBrush,Qt::ForegroundRole);

                ind = ind.sibling(r,3);
                moduleModel->setData(ind,blackBrush,Qt::ForegroundRole);

                ind = ind.sibling(r,2);
                moduleModel->setData(ind,blackBrush,Qt::ForegroundRole);

                ind = ind.sibling(r,1);
                moduleModel->setData(ind,blackBrush,Qt::ForegroundRole);

            }
            else
            {
                moduleModel->setData(ind,redBrush,Qt::ForegroundRole);
                moduleModel->setData(ind, module);

                ind = ind.sibling(r,4);
                moduleModel->setData(ind,redBrush,Qt::ForegroundRole);

                ind = ind.sibling(r,3);
                moduleModel->setData(ind,redBrush,Qt::ForegroundRole);

                ind = ind.sibling(r,2);
                moduleModel->setData(ind,redBrush,Qt::ForegroundRole);

                ind = ind.sibling(r,1);
                moduleModel->setData(ind,redBrush,Qt::ForegroundRole);

                if(!module.isEmpty())
                    idModuleMap.insert(id,module);
            }

            break;
        }
    }
}

void DeviceManagementDialog::refreshVlanSlot(int i)
{
    i++;
    fillVlanTV();
    ui->vlanTV->selectRow(row);
}

void DeviceManagementDialog::refreshEtherSlot(int i)
{
    i++;
    fillEtherTrunkTV();
    ui->etherTV->selectRow(row);
}

void DeviceManagementDialog::refreshVpnSlot(int i)
{
    i++;
    fillVpnViews();
    ui->vpnTV->selectRow(row);
}

void DeviceManagementDialog::on_modBrowsBtn_clicked()
{
    QString configFileName = QFileDialog::getOpenFileName(this,"Select Captured File(dis/show int)");
    if(!configFileName.isEmpty())
    {
        idModuleMap.clear();
        //clear new Modules
        QModelIndex ind = moduleModel->index(0,5);
        int r;
        for(r=0; r<moduleModel->rowCount(); r++)
        {
            ind = ind.sibling(r, 5);
            moduleModel->setData(ind, "");
        }
        //

        ui->modInfoLbl->setText(configFileName);

        configFile.setFileName(configFileName);

        switch (DeviceVendor)
        {
        case 1:
            huaweiModulesAnalyser();
            break;
        case 2:
            IOSModulesAnalyser();
            break;
        case 3:
            IOSXRModulesAnalyser();
            break;
        }
    }

}

void DeviceManagementDialog::on_modChB_toggled(bool checked)
{
    if(checked && idModuleMap.count() > 0)
        ui->modOkBtn->setEnabled(true);
    else
        ui->modOkBtn->setEnabled(false);
}

void DeviceManagementDialog::on_modOkBtn_clicked()
{
    QString mod;
    bool ok = true;
    foreach (int intId, idModuleMap.keys())
    {
        mod = idModuleMap.value(intId);
        if(mod.isEmpty())
            continue;

        if(!dbMan->updateInterfaceModule(intId,mod))
            ok = false;
    }
    if(ok)
        this->close();
    else
        QMessageBox::warning(this,"Error", "Interfaces module could not updated completely.");

}
