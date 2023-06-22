#include "devicecountdialog.h"
#include "ui_devicecountdialog.h"
#include <QSqlQuery>
#include "lib/database/danetdbman.h"


DeviceCountDialog::DeviceCountDialog(QWidget *parent, DanetDbMan *db) :
    QDialog(parent),
    ui(new Ui::DeviceCountDialog),
    dbMan(db)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowMinMaxButtonsHint);

    ui->areaCB->blockSignals(true);
    ui->areaCB->addItem("2", 2);
    ui->areaCB->addItem("3", 3);
    ui->areaCB->addItem("4", 4);
    ui->areaCB->addItem("5", 5);
    ui->areaCB->addItem("6", 6);
    ui->areaCB->addItem("7", 7);
    ui->areaCB->addItem("8", 8);
    ui->areaCB->addItem("Karaj", 9);
    ui->areaCB->addItem("All", 1);
    ui->areaCB->setCurrentIndex(-1);
    ui->areaCB->blockSignals(false);

    ui->exchFrame->setVisible(false);

    QSqlQuery *query = dbMan->selectDevices(true);
    ui->devCB->blockSignals(true);
    //`id`, `device`, `sr`, `type`, `vendor`,`slot_count`,`start_shelf`,
    //`start_slot`,`start_subslot`
    int id;
    QString dev, type0="", type1="";
    while(query->next())
    {
        id = query->value(0).toInt();
        dev = query->value(1).toString();
        type1 = query->value(3).toString();
        if(type1.contains("AGG", Qt::CaseInsensitive))
            type1 = "AGG";
        if(type1.contains("BRAS", Qt::CaseInsensitive))
            type1 = "BRAS";
        if(type0.isEmpty())
            type0 = type1;

        if(type0.compare(type1) != 0)
        {
            ui->devCB->insertSeparator(ui->devCB->count());
            type0 = type1;
        }

        ui->devCB->addItem(type1+ " " +dev,id);
    }
    ui->devCB->setCurrentIndex(-1);
    ui->devCB->blockSignals(false);
    ui->devCB->setEnabled(false);

}

DeviceCountDialog::~DeviceCountDialog()
{
    delete ui;
}

void DeviceCountDialog::on_CancelBtn_clicked()
{
    this->close();
}

void DeviceCountDialog::on_areaCB_currentIndexChanged(int index)
{
    index++;
    ui->abbrCB->clear();
    ui->cntLbl->clear();
    ui->devCB->setCurrentIndex(-1);

    int area = ui->areaCB->currentData().toInt();
    if(area > 1)
    {
        ui->exchFrame->setVisible(true);
        ui->devCB->setEnabled(false);
        QSqlQuery *query = dbMan->selectAreaAbbreviations(area, true);
        //`id`,`abbr`
        int id;
        QString abbr;
        ui->abbrCB->blockSignals(true);
        while(query->next())
        {
            id = query->value(0).toInt();
            abbr = query->value(1).toString();
            ui->abbrCB->addItem(abbr,id);

        }
        ui->abbrCB->addItem("All", 0);
        ui->abbrCB->setCurrentIndex(-1);
        ui->abbrCB->blockSignals(false);

    }
    else // all
    {
        ui->exchFrame->setVisible(false);
        ui->devCB->setEnabled(true);
    }
}

void DeviceCountDialog::on_devCB_currentIndexChanged(int index)
{
    index++;
    int area = ui->areaCB->currentData().toInt();
    int deviceId = ui->devCB->currentData().toInt();
    int exchId = -1;
    if(area > 1)
        exchId = ui->abbrCB->currentData().toInt();

    int cnt = dbMan->getDevexCount(area, exchId, deviceId);
    ui->cntLbl->setText(QString::number(cnt));
}

void DeviceCountDialog::on_abbrCB_currentIndexChanged(int index)
{
    ui->devCB->setCurrentIndex(-1);
    if(index > -1)
        ui->devCB->setEnabled(true);
    else
        ui->devCB->setEnabled(false);
}

