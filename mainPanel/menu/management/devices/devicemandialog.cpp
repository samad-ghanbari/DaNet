#include "devicemandialog.h"
#include "ui_devicemandialog.h"
#include "lib/database/danetdbman.h"
#include <QSqlQueryModel>
#include <QModelIndex>
#include "mainPanel/menu/management/devices/adddevicedialog.h"
#include "mainPanel/menu/management/devices/removedevicedialog.h"
#include "mainPanel/menu/management/devices/editdevicedialog.h"


DeviceManDialog::DeviceManDialog(QWidget *parent, DanetDbMan *db) :
    QDialog(parent),
    ui(new Ui::DeviceManDialog),
    dbMan(db),
    model(new QSqlQueryModel(this))
{
    ui->setupUi(this);
    resize(parent->width()-50, parent->height()-50);
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowMinMaxButtonsHint);

    ui->deviceTV->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->deviceTV, SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(contextMenuSlot(QPoint)));

    contextMenu.addAction(ui->actionAdminEditDevice);
    contextMenu.addSeparator();
    contextMenu.addAction(ui->actionAdminRemoveDevice);

    ui->deviceTV->setModel(model);
    ui->deviceTV->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->deviceTV->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    netDevices = true;
    fillDeviceTV();
    AdminUserFlag = false;
    ui->adminAddDeviceBtn->setEnabled(false);

}

DeviceManDialog::~DeviceManDialog()
{
    delete ui;
}

void DeviceManDialog::fillDeviceTV()
{
//`id`, `device`, `sr`, `type`, `vendor`,`slot_count`,`line_slot_count`,`start_shelf`,  `start_slot`,`start_subslot`

    model->clear();

    QSqlQuery *query = dbMan->selectDevices(netDevices);
    model->setQuery(*query);
    model->setHeaderData(1, Qt::Horizontal,"Device");
    model->setHeaderData(2, Qt::Horizontal,"SR");
    model->setHeaderData(3, Qt::Horizontal,"Type");
    model->setHeaderData(4, Qt::Horizontal,"Vendor");
    model->setHeaderData(5, Qt::Horizontal,"Slot\nCount");
    model->setHeaderData(6, Qt::Horizontal,"Line Slot\nCount");
    model->setHeaderData(7, Qt::Horizontal,"Shelf\nStart No");
    model->setHeaderData(8, Qt::Horizontal,"Slot\nStart No");
    model->setHeaderData(9, Qt::Horizontal,"Sub-slot\nStart No");

    ui->deviceTV->verticalHeader()->hide();
    ui->deviceTV->hideColumn(0);
    ui->deviceTV->hideColumn(2);
}

void DeviceManDialog::refreshSlot(int i)
{
    i++;
    fillDeviceTV();
    ui->deviceTV->selectRow(row);
}

void DeviceManDialog::contextMenuSlot(QPoint ptr)
{
    if(!AdminUserFlag) return;

    QModelIndex clickedIndex = ui->deviceTV->indexAt(ptr);
    row = clickedIndex.row();
//`id`, `device`, `sr`, `type`, `vendor`, `slot_count`, `slot_count`, `start_shelf`, `start_slot`,`start_subslot`
    clickedIndex = clickedIndex.sibling(clickedIndex.row(), 0); // id
    id = model->data(clickedIndex).toInt();
    clickedIndex = clickedIndex.sibling(clickedIndex.row(), 1); // device
    device = model->data(clickedIndex).toString();
    clickedIndex = clickedIndex.sibling(clickedIndex.row(), 2); // sr
    sr = model->data(clickedIndex).toBool();
    clickedIndex = clickedIndex.sibling(clickedIndex.row(), 3); // type
    type = model->data(clickedIndex).toString();
    clickedIndex = clickedIndex.sibling(clickedIndex.row(), 4); // vendor
    vendor = model->data(clickedIndex).toString();
    clickedIndex = clickedIndex.sibling(clickedIndex.row(), 5); // slot count
    slotCount = model->data(clickedIndex).toInt();
    clickedIndex = clickedIndex.sibling(clickedIndex.row(), 6); // line slot count
    lineSlotCount = model->data(clickedIndex).toInt();
    clickedIndex = clickedIndex.sibling(clickedIndex.row(), 7); // start shelf
    if(model->data(clickedIndex).isNull())
        startShelf = -1;
    else
        startShelf = model->data(clickedIndex).toInt();
    clickedIndex = clickedIndex.sibling(clickedIndex.row(), 8); // start slot
    if(model->data(clickedIndex).isNull())
        startSlot = -1;
    else
        startSlot = model->data(clickedIndex).toInt();
    clickedIndex = clickedIndex.sibling(clickedIndex.row(), 9); // start subslot
    if(model->data(clickedIndex).isNull())
        startSubslot = -1;
    else
        startSubslot = model->data(clickedIndex).toInt();

    if(id > 0)
        contextMenu.popup(ui->deviceTV->mapToGlobal(ptr));
}

void DeviceManDialog::on_adminAddDeviceBtn_clicked()
{
    if(!AdminUserFlag) return;

    AddDeviceDialog *addDeviceDialog = new AddDeviceDialog(this, dbMan);
    addDeviceDialog->setAttribute(Qt::WA_DeleteOnClose);
    connect(addDeviceDialog, SIGNAL(finished(int)), this, SLOT(refreshSlot(int)));
    addDeviceDialog->show();
}

void DeviceManDialog::on_actionAdminEditDevice_triggered()
{
    if(!AdminUserFlag) return;

    EditDeviceDialog *editDeviceDialog = new EditDeviceDialog(this, dbMan,id, device, sr, type, vendor,slotCount,lineSlotCount,startShelf, startSlot,startSubslot);
    editDeviceDialog->setAttribute(Qt::WA_DeleteOnClose);
    connect(editDeviceDialog, SIGNAL(finished(int)), this, SLOT(refreshSlot(int)));
    editDeviceDialog->show();
}

void DeviceManDialog::on_actionAdminRemoveDevice_triggered()
{
    if(!AdminUserFlag) return;

    RemoveDeviceDialog *removeDeviceDialog = new RemoveDeviceDialog(this, dbMan, id, device, type, vendor );
    removeDeviceDialog->setAttribute(Qt::WA_DeleteOnClose);
    connect(removeDeviceDialog, SIGNAL(finished(int)), this, SLOT(refreshSlot(int)));
    removeDeviceDialog->show();
}

void DeviceManDialog::on_netDevicesCB_toggled(bool checked)
{
    if(checked)
    {
        netDevices = true;
        ui->netDevicesCB->setText("Network Devices");
    }
    else
    {
        netDevices = false;
        ui->netDevicesCB->setText("All");
    }

    fillDeviceTV();
}

void DeviceManDialog::on_adminLE_textChanged(const QString &arg1)
{
    QString adminPass = dbMan->getUserHashedPassword("admin");
    QString hashedArg = dbMan->hashPassword(arg1);

    if(hashedArg.compare(adminPass,Qt::CaseSensitive) == 0)
    {
        AdminUserFlag = true;
        ui->adminAddDeviceBtn->setEnabled(true);
    }
    else
    {
        AdminUserFlag = false;
        ui->adminAddDeviceBtn->setEnabled(false);
    }
}
