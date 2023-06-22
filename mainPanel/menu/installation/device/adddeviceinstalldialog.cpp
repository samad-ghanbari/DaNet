#include "adddeviceinstalldialog.h"
#include "ui_adddeviceinstalldialog.h"
#include <QMessageBox>
#include "lib/database/danetdbman.h"
#include <QSqlQuery>

AddDeviceInstallDialog::AddDeviceInstallDialog(QWidget *parent, DanetDbMan *db, const int ExchId, const QString Abbr, bool DSLAM) :
    QDialog(parent),
    ui(new Ui::AddDeviceInstallDialog),
    DbMan(db),
    exchId(ExchId)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowMinMaxButtonsHint);

    ui->abbrLbl->setText(Abbr);

    ADDED = false;

    bool IS_SITE = DbMan->isSite(exchId);
    QSqlQuery *query;
    //saloon CB
    if(IS_SITE)
    {
        query = DbMan->selectSaloons(true,false);
        int saloonId;
        QString saloon;
        while(query->next())
        {
            saloonId = query->value(0).toInt();
            saloon = query->value(1).toString();
            ui->saloonCB->addItem(saloon, saloonId);
        }
    }
    else
    {
        query = DbMan->selectSaloons();
        int saloonId;
        QString saloonStr;
        while(query->next())
        {
            saloonId = query->value(0).toInt();
            saloonStr = query->value(1).toString();
            ui->saloonCB->addItem(saloonStr, saloonId);
        }
    }


    query = DbMan->selectPurchases();
    QString pur, purAbbr;
    while(query->next())
    {
        purAbbr = query->value(1).toString();
        pur = query->value(2).toString();

        ui->purchaseCB->addItem(pur+"("+purAbbr+")", purAbbr);
    }
    ui->purchaseCB->setCurrentIndex(2);


    query = DbMan->selectDevices(false, DSLAM);
    //`id`, `device`, `sr`, `type`
    int id;
    ui->deviceCB->blockSignals(true);
    QString dev,dvType0, dvType1;
    while(query->next())
    {
        id = query->value(0).toInt();
        dev = query->value(3).toString()+"-"+query->value(1).toString();;
        dvType1 = query->value(3).toString();
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


        ui->deviceCB->addItem(dev,id);

    }
    ui->deviceCB->setCurrentIndex(-1);
    ui->deviceCB->blockSignals(false);

    //    QMap<QString, int> deviceMap = DbMan->selectDeviceMap(false);

    //    foreach (QString k, deviceMap.keys())
    //        ui->deviceCB->addItem(k, deviceMap.value(k));

    //    ui->deviceCB->setCurrentIndex(-1);

    QList<int> saloonRowRackShelf = DbMan->getLastSaloonRowRackShelf(exchId);
    if( saloonRowRackShelf.count() == 4)
    {
        ui->saloonCB->setCurrentIndex(ui->saloonCB->findData(saloonRowRackShelf.at(0)));

        ui->rowSB->setValue(saloonRowRackShelf.at(1));

        if(saloonRowRackShelf.at(3) < 3) // shelf < 3
        {
            ui->rackSB->setValue(saloonRowRackShelf.at(2));
            ui->shelfSB->setValue(saloonRowRackShelf.at(3)+1);
        }
        else
        {
            ui->rackSB->setValue(saloonRowRackShelf.at(2)+1);
            ui->shelfSB->setValue(3);
        }
    }

}

AddDeviceInstallDialog::~AddDeviceInstallDialog()
{
    delete ui;
}

void AddDeviceInstallDialog::checkEntry()
{
    bool D = !ui->deviceCB->currentText().isEmpty();
    bool P = !ui->purchaseCB->currentText().isEmpty();
    bool S = !ui->saloonCB->currentText().isEmpty();


    if(D && P && S)
        ui->okBtn->setEnabled(true);
    else
        ui->okBtn->setEnabled(false);
}

bool AddDeviceInstallDialog::isDone()
{
    return ADDED;
}

int AddDeviceInstallDialog::getDevexId()
{
    return devexId;
}

void AddDeviceInstallDialog::on_cancelBtn_clicked()
{
    this->close();
}

void AddDeviceInstallDialog::on_deviceCB_currentIndexChanged(int index)
{
    index++;
    checkEntry();
}

void AddDeviceInstallDialog::on_purchaseCB_currentIndexChanged(int index)
{
    index++;
    checkEntry();
}

void AddDeviceInstallDialog::on_okBtn_clicked()
{
    int deviceId, saloon, row, rack, shelf;
    QString purchase, desc, deviceName;

    saloon = ui->saloonCB->currentData().toInt();
    row = ui->rowSB->value();
    rack = ui->rackSB->value();
    shelf = ui->shelfSB->value();
    deviceId = ui->deviceCB->currentData().toInt();
    purchase = ui->purchaseCB->currentData().toString();
    desc = ui->descTE->toPlainText();
    deviceName = DbMan->createDeviceName(exchId,row,rack,shelf,deviceId);
    QString username = DbMan->getLoggedinNameLastname();

//    bool DSLAM = DbMan->isDeviceDslam(deviceId);
//    if(DSLAM && (shelf < 1 || shelf > 3))
//    {
//        QMessageBox::information(this, "Incorrect Shelf No", "DSLAM can be installed in shelfs 1, 2 or 3.");
//        return;
//    }

    //    if(DbMan->siteDslamInsertPosibility(exchId, deviceId))
    //    {
    if(DbMan->insertDevex(exchId,saloon,deviceId,row, rack, shelf,deviceName,purchase, desc))
    {
        DbMan->insertLog(exchId, username, "Add Device: "+deviceName );

        devexId = DbMan->getDevexId(exchId,deviceId,row,rack,shelf);
        ADDED = true;
        this->close();
    }
    else
        QMessageBox::information(this, "Error", "Can not Install Device.\n"+DbMan->lastError());
    //    }
    //    else
    //        QMessageBox::information(this, "Error", "Due to limitations, each site can only have one DSLAM.\nYou have to create new abbr to add new DSLAM.");
}

void AddDeviceInstallDialog::on_saloonCB_currentIndexChanged(int index)
{
    index++;
    checkEntry();
}
