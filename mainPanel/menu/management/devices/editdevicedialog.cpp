#include "editdevicedialog.h"
#include "ui_editdevicedialog.h"
#include "lib/database/danetdbman.h"
#include <QCryptographicHash>
#include <QMessageBox>

EditDeviceDialog::EditDeviceDialog(QWidget *parent, DanetDbMan *db, const int Id, const QString device, const bool sr, const QString type, const QString vendor, const int slotCount, const int lineSlotCount, const int startShelf, const int startSlot, const int startSubslot) :
    QDialog(parent),
    ui(new Ui::EditDeviceDialog),
    dbMan(db)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowMinMaxButtonsHint);

    id = Id;
    ui->deviceLE->setText(device);
    if(sr)
        ui->srCB->setChecked(true);
    else
        ui->srCB->setChecked(false);
    ui->typeLE->setText(type);
    ui->vendorLE->setText(vendor);
    ui->slotCountSB->setValue(slotCount);
    ui->lineSlotCountSB->setValue(lineSlotCount);
    ui->slotStartNoSB->setValue(startSlot);

    if(startShelf == -1)
    {
        ui->shelfChB->setChecked(false);
        ui->shelfSB->setVisible(false);
        ui->noShelfLbl->setVisible(true);
    }
    else
    {
        ui->shelfChB->setChecked(true);
        ui->shelfSB->setVisible(true);
        ui->shelfSB->setValue(startShelf);
        ui->noShelfLbl->setVisible(false);
    }

    if(startSubslot == -1)
    {
        ui->subslotChB->setChecked(false);
        ui->subslotSB->setVisible(false);
        ui->noSubslotLbl->setVisible(true);
    }
    else
    {
        ui->subslotChB->setChecked(true);
        ui->subslotSB->setVisible(true);
        ui->subslotSB->setValue(startSubslot);
        ui->noSubslotLbl->setVisible(false);
    }

    ui->modifyBtn->setEnabled(false);

}

EditDeviceDialog::~EditDeviceDialog()
{
    delete ui;
}

void EditDeviceDialog::entryCheck()
{
    if(ui->deviceLE->text().isEmpty() || ui->typeLE->text().isEmpty() || ui->vendorLE->text().isEmpty())
        ui->modifyBtn->setEnabled(false);
    else
        ui->modifyBtn->setEnabled(true);
}

void EditDeviceDialog::on_cancelBtn_clicked()
{
    this->close();
}

void EditDeviceDialog::on_deviceLE_textChanged(const QString &arg1)
{
    if(arg1.isEmpty())
        entryCheck();
    else
        entryCheck();
}

void EditDeviceDialog::on_typeLE_textChanged(const QString &arg1)
{
    if(arg1.isEmpty())
        entryCheck();
    else
        entryCheck();
}

void EditDeviceDialog::on_vendorLE_textChanged(const QString &arg1)
{
    if(arg1.isEmpty())
        entryCheck();
    else
        entryCheck();
}

void EditDeviceDialog::on_slotCountSB_valueChanged(int arg1)
{
    ui->lineSlotCountSB->setValue(arg1);
    entryCheck();
}

void EditDeviceDialog::on_modifyBtn_clicked()
{
    int slotCount,lineSlotCount, startSlot, startShelf = -1, startSubslot = -1;
    QString device, type, vendor;
    bool sr;
    slotCount = ui->slotCountSB->value();
    lineSlotCount = ui->lineSlotCountSB->value();
    device = ui->deviceLE->text();
    sr = ui->srCB->isChecked();
    type = ui->typeLE->text();
    vendor = ui->vendorLE->text();
    startSlot = ui->slotStartNoSB->value();
    if(ui->shelfChB->isChecked())
        startShelf = ui->shelfSB->value();
    if(ui->subslotChB->isChecked())
        startSubslot = ui->subslotSB->value();

    if(dbMan->updateDevice(id, device,sr, type, vendor,slotCount,lineSlotCount,startShelf,startSlot,startSubslot) )
    {
        this->close();
    }
    else
        QMessageBox::information(this, "Error", "Can not edit device.\n"+dbMan->lastError());
}

void EditDeviceDialog::on_srCB_toggled(bool checked)
{
    if(checked)
        checked = true;
    entryCheck();
}

void EditDeviceDialog::on_slotStartNoSB_valueChanged(int arg1)
{
    arg1++;
    entryCheck();
}

void EditDeviceDialog::on_shelfChB_toggled(bool checked)
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

    entryCheck();
}

void EditDeviceDialog::on_subslotChB_toggled(bool checked)
{
    if(checked)
    {
        ui->subslotSB->setVisible(true);
        ui->noSubslotLbl->setVisible(false);
    }
    else
    {
        ui->subslotSB->setVisible(false);
        ui->noSubslotLbl->setVisible(true);
    }

    entryCheck();
}

void EditDeviceDialog::on_lineSlotCountSB_valueChanged(int arg1)
{
    arg1++;
    entryCheck();
}
