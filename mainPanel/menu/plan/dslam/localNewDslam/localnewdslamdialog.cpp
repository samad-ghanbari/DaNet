#include "localnewdslamdialog.h"
#include "ui_localnewdslamdialog.h"
#include "lib/database/danetdbman.h"
#include "mainPanel/menu/installation/device/adddeviceinstalldialog.h"
#include "mainPanel/menu/installation/card/cardinstalldialog.h"
#include "mainPanel/menu/plan/dslam/dslamdialog.h"
#include <QSqlQuery>
#include <QDebug>

LocalNewDslamDialog::LocalNewDslamDialog(QWidget *Parent, DanetDbMan *db) :
    QDialog(Parent),
    ui(new Ui::LocalNewDslamDialog),
    dbMan(db),
    parent(Parent),
    addDeviceInstallDialog(NULL),
    cardInstallDialog(NULL),
    dslamDialog(NULL)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowMinMaxButtonsHint);

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

    int workingArea = dbMan->getLoggedInDefaultArea();
    ui->areaCB->setCurrentIndex(ui->areaCB->findData(workingArea));

    ui->okBtn->setEnabled(false);

}

LocalNewDslamDialog::~LocalNewDslamDialog()
{
    delete ui;
}

void LocalNewDslamDialog::on_okBtn_clicked()
{
    this->hide();
    area = ui->areaCB->currentData().toInt();
    QString abbr = ui->abbrCB->currentText();
    exchId = ui->abbrCB->currentData().toInt();

    //######### Add Device
    delete addDeviceInstallDialog;
    addDeviceInstallDialog = new AddDeviceInstallDialog(this,dbMan,exchId,abbr,true);
    connect(addDeviceInstallDialog, SIGNAL(finished(int)), this, SLOT(deviceSlot(int)));
    addDeviceInstallDialog->show();
//    addDeviceInstallDialog->activateWindow();
}

void LocalNewDslamDialog::deviceSlot(int DONE)
{
    DONE++;
    if(addDeviceInstallDialog->isDone())
    {
        devexId = addDeviceInstallDialog->getDevexId();


        //######## Add Card

        delete cardInstallDialog;
        cardInstallDialog = new CardInstallDialog(this,dbMan, area, exchId, 2, -1, devexId);
        connect(cardInstallDialog, SIGNAL(finished(int)), this, SLOT(cardSlot(int)));
        cardInstallDialog->showMaximized();
//        cardInstallDialog->activateWindow();
    }
    else
        this->close();
}

void LocalNewDslamDialog::cardSlot(int done)
{
    //######## Plan DSLAM

    done++;
    delete dslamDialog;
    dslamDialog = new DslamDialog(this,dbMan, area, exchId, 2, -1, devexId);
    connect(dslamDialog, SIGNAL(finished(int)), this, SIGNAL(finished(int)));
    dslamDialog->showMaximized();
//    dslamDialog->activateWindow();
}

void LocalNewDslamDialog::on_areaCB_currentIndexChanged(int index)
{
    int area = index+2;
    ui->okBtn->setEnabled(false);
    ui->abbrCB->blockSignals(true);
    ui->abbrCB->clear();

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

void LocalNewDslamDialog::on_abbrCB_currentIndexChanged(int index)
{
    if(index > -1)
        ui->okBtn->setEnabled(true);
    else
        ui->okBtn->setEnabled(false);
}
