#include "removedevicedialog.h"
#include "ui_removedevicedialog.h"
#include "lib/database/danetdbman.h"
#include <QCryptographicHash>
#include <QMessageBox>

RemoveDeviceDialog::RemoveDeviceDialog(QWidget *parent, DanetDbMan *db,int Id, QString device, QString type, QString vendor) :
    QDialog(parent),
    ui(new Ui::RemoveDeviceDialog),
    dbMan(db)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowMinMaxButtonsHint);

    id = Id;
    ui->deviceLbl->setText(device);
    ui->typeLbl->setText(type);
    ui->vendorLbl->setText(vendor);
}

RemoveDeviceDialog::~RemoveDeviceDialog()
{
    delete ui;

}

void RemoveDeviceDialog::on_cancelBtn_clicked()
{
    this->close();
}

void RemoveDeviceDialog::on_removeBtn_clicked()
{
    if(dbMan->deleteDevice(id))
    {
        this->close();
    }
    else
        QMessageBox::information(this, "Error", "Can not remove device.\n"+dbMan->lastError());
}


void RemoveDeviceDialog::on_checkBox_toggled(bool checked)
{
    if(checked)
        ui->removeBtn->setEnabled(true);
    else
        ui->removeBtn->setEnabled(false);
}
