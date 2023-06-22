#include "editdevicenamedialog.h"
#include "ui_editdevicenamedialog.h"
#include "lib/database/danetdbman.h"
#include <QMessageBox>

EditDeviceNameDialog::EditDeviceNameDialog(QWidget *parent,DanetDbMan *DbMan,const int DevexId, QString DeviceName,QString AreaAbbr,QString Site,QString Device) :
    QDialog(parent),
    ui(new Ui::EditDeviceNameDialog),
    dbMan(DbMan),
    devexId(DevexId)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowMinMaxButtonsHint);

    ui->oldDeviceLbl->setText(DeviceName);
    ui->areaMotherLbl->setText(AreaAbbr+"-");
    ui->deviceTypeLbl->setText("- "+Device);
    ui->siteNameLE->setText(Site);

    ui->checkBox->setChecked(false);
    ui->okBtn->setEnabled(false);
}

EditDeviceNameDialog::~EditDeviceNameDialog()
{
    delete ui;
}

void EditDeviceNameDialog::on_cancelBtn_clicked()
{
    this->close();
}

void EditDeviceNameDialog::on_okBtn_clicked()
{
    QString temp = ui->areaMotherLbl->text();
    temp = temp.remove(" "); // 5-GN
    temp += " " + ui->siteNameLE->text().remove(" ");
    temp += ui->deviceTypeLbl->text();


    QString username = dbMan->getLoggedinNameLastname();
    int exchId = dbMan->getDevexExchangeId(devexId);
    //log
    dbMan->insertLog(exchId, username, "Modified Device Name to "+temp );

    if(dbMan->updateDevexDeviceName(devexId,temp))
    {
        this->close();
    }
    else
        QMessageBox::warning(this, "ERROR", "Cannot Edit Device Name.\n"+dbMan->lastError());
}

void EditDeviceNameDialog::on_checkBox_toggled(bool checked)
{
    if(checked)
    {
        if(ui->siteNameLE->text().isEmpty())
        {
            ui->okBtn->setEnabled(false);
            ui->checkBox->setChecked(false);
        }
        else
            ui->okBtn->setEnabled(true);
    }
    else
        ui->okBtn->setEnabled(false);
}

void EditDeviceNameDialog::on_siteNameLE_textChanged(const QString &arg1)
{
    ui->okBtn->setEnabled(false);
    ui->checkBox->setChecked(false);

    if(arg1.isEmpty())
        ui->checkBox->setEnabled(false);
    else
        ui->checkBox->setEnabled(true);
}
