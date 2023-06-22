#include "adddevicedialog.h"
#include "ui_adddevicedialog.h"
#include <QCryptographicHash>
#include "lib/database/danetdbman.h"
#include <QMessageBox>

AddDeviceDialog::AddDeviceDialog(QWidget *parent, DanetDbMan *db) :
    QDialog(parent),
    ui(new Ui::AddDeviceDialog),
    dbMan(db)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowMinMaxButtonsHint);

    ui->shelfChB->setChecked(false);
    ui->shelfSB->setVisible(false);
    ui->noShelfLbl->setVisible(true);

    ui->subslotChB->setChecked(true);
    ui->subslotSB->setVisible(true);
    ui->noSubSlotLbl->setVisible(false);
}

AddDeviceDialog::~AddDeviceDialog()
{
    delete ui;
}

void AddDeviceDialog::entryCheck()
{
    if(ui->deviceLE->text().isEmpty() || ui->typeLE->text().isEmpty() || ui->vendorLE->text().isEmpty())
        ui->okBtn->setEnabled(false);
    else
        ui->okBtn->setEnabled(true);
}

void AddDeviceDialog::on_cancelBtn_clicked()
{
    this->close();
}

void AddDeviceDialog::on_deviceLE_textChanged(const QString &arg1)
{
    if(arg1.isEmpty())
        entryCheck();
    else
        entryCheck();
}

void AddDeviceDialog::on_typeLE_textChanged(const QString &arg1)
{
    if(arg1.isEmpty())
        entryCheck();
    else
        entryCheck();
}

void AddDeviceDialog::on_vendorLE_textChanged(const QString &arg1)
{
    if(arg1.isEmpty())
        entryCheck();
    else
        entryCheck();
}

void AddDeviceDialog::on_slotCountSB_valueChanged(int arg1)
{
    ui->lineSlotCountSB->setValue(arg1);
    entryCheck();
}

void AddDeviceDialog::on_okBtn_clicked()
{
    QString device, type, vendor;
    int slotCount,lineSlotCount, startSlot, startShelf=-1, startSubslot=-1;
    bool sr;
    device = ui->deviceLE->text();
    sr = ui->srCB->isChecked();
    type = ui->typeLE->text();
    vendor = ui->vendorLE->text();   
    slotCount = ui->slotCountSB->value();
    lineSlotCount = ui->lineSlotCountSB->value();
    startSlot = ui->startSlotNoSB->value();
    if(ui->shelfChB->isChecked())
        startShelf = ui->shelfSB->value();
    if(ui->subslotChB->isChecked())
        startSubslot = ui->subslotSB->value();

    if(dbMan->insertDevice(device, sr, type, vendor,slotCount,lineSlotCount,startShelf, startSlot,startSubslot))
     {
        this->close();
    }
    else
        QMessageBox::information(this, "Error", "Can not add new device.\n"+dbMan->lastError());

}

void AddDeviceDialog::on_srCB_toggled(bool checked)
{
    if(checked)
        checked = true;

    entryCheck();
}

void AddDeviceDialog::on_startSlotNoSB_valueChanged(int arg1)
{
    arg1++;
    entryCheck();
}

void AddDeviceDialog::on_shelfChB_toggled(bool checked)
{
    if(checked)
    {
        ui->shelfSB->setVisible(true);
        ui->noShelfLbl->setVisible(false);
    }
    else
    {
        ui->shelfSB->setVisible(false);
        ui->noShelfLbl->setVisible(true);
    }
}

void AddDeviceDialog::on_subslotChB_toggled(bool checked)
{
    if(checked)
    {
        ui->subslotSB->setVisible(true);
        ui->noSubSlotLbl->setVisible(false);
    }
    else
    {
        ui->subslotSB->setVisible(false);
        ui->noSubSlotLbl->setVisible(true);
    }
}

void AddDeviceDialog::on_lineSlotCountSB_valueChanged(int arg1)
{
    arg1++;
    entryCheck();
}
