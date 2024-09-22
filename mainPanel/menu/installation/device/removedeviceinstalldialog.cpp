#include "removedeviceinstalldialog.h"
#include "ui_removedeviceinstalldialog.h"
#include "lib/database/danetdbman.h"
#include <QMessageBox>

RemoveDeviceInstallDialog::RemoveDeviceInstallDialog(QWidget *parent, DanetDbMan *db, const int DevexId, const QString abbr, const QString saloon, const QString rowRackShelf, const QString device, const QString deviceName) :
    QDialog(parent),
    ui(new Ui::RemoveDeviceInstallDialog),
    dbMan(db),
    devexId(DevexId)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowMinMaxButtonsHint);

    ui->abbrLbl->setText(abbr);
    ui->saloonLbl->setText(saloon);
    ui->rowRackShelfLbl->setText(rowRackShelf);

    ui->deviceLbl->setText(device);
    ui->deviceNameLbl->setText(deviceName);

    if(dbMan->isDevexDslam(devexId))
    {
        //dslam should have no plan
        if(dbMan->dslamExistance(devexId))
        {
            ui->okBtn->setEnabled(false);
            ui->errorLbl->setText("you have to remove DSLAM PLAN first");
            ui->errorLbl->setVisible(true);
            ui->checkBox->setChecked(false);
            ui->checkBox->setEnabled(false);
        }
        else
        {
            ui->okBtn->setEnabled(false);
            ui->errorLbl->setVisible(false);
            ui->checkBox->setChecked(false);
            ui->checkBox->setEnabled(true);
        }
    }
    else
    {
        if(dbMan->isDevexRemovable(devexId))
        {
            ui->okBtn->setEnabled(false);
            ui->errorLbl->setVisible(false);
            ui->checkBox->setChecked(false);
            ui->checkBox->setEnabled(true);
        }
        else
        {
            ui->okBtn->setEnabled(false);
            ui->errorLbl->setText("you have to remove device cards first");
            ui->errorLbl->setVisible(true);
            ui->checkBox->setChecked(false);
            ui->checkBox->setEnabled(false);
        }
    }
}

RemoveDeviceInstallDialog::~RemoveDeviceInstallDialog()
{
    delete ui;
}

void RemoveDeviceInstallDialog::on_okBtn_clicked()
{
    QString username = dbMan->getLoggedinNameLastname();
    int exchId = dbMan->getDevexExchangeId(devexId);
    QString device = ui->deviceNameLbl->text();

    bool v = dbMan->deleteDevexVlans(devexId);
    bool e = dbMan->deleteDevexEtherTrunks(devexId);
    bool vpn = dbMan->deleteDevexVpns(devexId);

    if(v && e && vpn)
    {
        if(dbMan->deleteDevex(devexId))
        {
            dbMan->insertLog(exchId, username, "Remove Device: "+device);

            this->close();
        }
        else
            QMessageBox::warning(this, "Error","Can not remove device.\n"+dbMan->lastError());
    }
    else
    {
        ui->okBtn->setEnabled(false);
        ui->errorLbl->setText("you have to remove device cards first");
        ui->errorLbl->setVisible(true);
        ui->checkBox->setChecked(false);
        ui->checkBox->setEnabled(false);
    }
}

void RemoveDeviceInstallDialog::on_cancelBtn_clicked()
{
     this->close();
}

void RemoveDeviceInstallDialog::on_checkBox_toggled(bool checked)
{
    if(checked)
        ui->okBtn->setEnabled(true);
    else
        ui->okBtn->setEnabled(false);
}

