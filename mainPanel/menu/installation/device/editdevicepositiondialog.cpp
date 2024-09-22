#include "editdevicepositiondialog.h"
#include "ui_editdevicepositiondialog.h"
#include "lib/database/danetdbman.h"
#include <QMessageBox>
#include <QSqlQuery>

EditDevicePositionDialog::EditDevicePositionDialog(QWidget *parent, DanetDbMan *db, const int ExchId, const QString abbr,  const int DevexId, const int DeviceId, const int Saloon,
                                                   const int Row, const int Rack, const int Shelf, const QString DeviceName) :
    QDialog(parent),
    ui(new Ui::EditDevicePositionDialog),
    dbMan(db),
    exchId(ExchId),
    saloon(Saloon),
    devexId(DevexId),
    deviceId(DeviceId),
    row(Row),
    rack(Rack),
    shelf(Shelf),
    oldDeviceName(DeviceName)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowMinMaxButtonsHint);

    ui->abbrLbl->setText(abbr);
    ui->deviceLbl->setText(DeviceName);
    ui->saloonLbl->setText(dbMan->getSaloonName(saloon));

    ui->RRSLbl->setText(QString::number(Row)+'*'+QString::number(Rack)+'*'+QString::number(Shelf));

    ui->rowSB->setValue(Row);
    ui->rackSB->setValue(Rack);
    ui->shelfSB->setValue(Shelf);

    bool IS_SITE = dbMan->isSite(exchId);
    //saloon CB
    if(IS_SITE)
    {
        QSqlQuery *query = dbMan->selectSaloons();
        int saloonId;
        QString saloonStr;
        while(query->next())
        {
            saloonId = query->value(0).toInt();
            saloonStr = query->value(1).toString();
            ui->saloonCB->addItem(saloonStr, saloonId);
        }
    }
    else
    {
        QSqlQuery *query = dbMan->selectSaloons(false,true);
        int saloonId;
        QString saloon;
        while(query->next())
        {
            saloonId = query->value(0).toInt();
            saloon = query->value(1).toString();
            ui->saloonCB->addItem(saloon, saloonId);
        }
    }
    ui->saloonCB->setCurrentIndex(ui->saloonCB->findData(saloon));

    ui->okBtn->setEnabled(false);
    ui->checkBox->setChecked(false);
    ui->checkBox->setEnabled(false);
}

EditDevicePositionDialog::~EditDevicePositionDialog()
{
    delete ui;
}

void EditDevicePositionDialog::entryCheck()
{
    bool sa = (ui->saloonCB->currentData().toInt() == saloon)? false : true;
    bool ro = (ui->rowSB->value() == row)? false : true;
    bool ra = (ui->rackSB->value() == rack)? false : true;
    bool sh = (ui->shelfSB->value() == shelf)? false : true;

    if(sa || ro || ra || sh)
        ui->checkBox->setEnabled(true);
    else
    {
        ui->checkBox->setChecked(false);
        ui->checkBox->setEnabled(false);
        ui->okBtn->setEnabled(false);
    }
}

void EditDevicePositionDialog::on_cancelBtn_clicked()
{
    this->close();
}

void EditDevicePositionDialog::on_okBtn_clicked()
{
    int SALOON = ui->saloonCB->currentData().toInt();
    int ROW = ui->rowSB->value();
    int RACK = ui->rackSB->value();
    int SHELF = ui->shelfSB->value();

//    bool DSLAM = dbMan->isDeviceDslam(deviceId);
//    if(DSLAM && (SHELF < 1 || SHELF > 3))
//    {
//        QMessageBox::information(this, "Incorrect Shelf No", "DSLAM can be installed in shelfs 1, 2 or 3.");
//        return;
//    }

    QString newDeviceName = dbMan->createDeviceName(exchId,ROW,RACK,SHELF,deviceId);

    QString username = dbMan->getLoggedinNameLastname();

    if(ROW == row && RACK == rack && SHELF == shelf && saloon != SALOON)
    {
        if(dbMan->updateDevex(devexId, SALOON))
        {
            dbMan->insertLog(exchId, username, "Edit Device To: "+newDeviceName);
            this->close();
        }
        else
            QMessageBox::warning(this, "Error", "Can not move device.\n"+dbMan->lastError());
    }
    else if(ROW != row || RACK != rack || SHELF != shelf)
    {
        if(dbMan->updateDevex(devexId,SALOON,ROW, RACK, SHELF,newDeviceName))
        {
            if(dbMan->updateLabel(oldDeviceName, newDeviceName))
            {
                dbMan->insertLog(exchId, username, "Edit Device To: "+newDeviceName);
                this->close();
            }
            else
                QMessageBox::warning(this, "Error", "Can not update peer labels.\n"+dbMan->lastError());
        }
        else
            QMessageBox::warning(this, "Error", "Can not move device.\n"+dbMan->lastError());
    }


}

void EditDevicePositionDialog::on_saloonCB_currentIndexChanged(int index)
{
    index++;
    entryCheck();
}

void EditDevicePositionDialog::on_rowSB_valueChanged(int arg1)
{
    arg1++;
    entryCheck();
}

void EditDevicePositionDialog::on_rackSB_valueChanged(int arg1)
{
    arg1++;
    entryCheck();
}

void EditDevicePositionDialog::on_shelfSB_valueChanged(int arg1)
{
    arg1++;
    entryCheck();
}

void EditDevicePositionDialog::on_checkBox_toggled(bool checked)
{
    if(checked)
        ui->okBtn->setEnabled(true);
    else
        ui->okBtn->setEnabled(false);
}


