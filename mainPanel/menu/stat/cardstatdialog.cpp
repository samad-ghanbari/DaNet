#include "cardstatdialog.h"
#include "ui_cardstatdialog.h"
#include <QSqlQuery>
#include "lib/database/danetdbman.h"
#include <QStandardItemModel>
#include <QDebug>

CardStatDialog::CardStatDialog(QWidget *parent, DanetDbMan *db) :
    QDialog(parent),
    ui(new Ui::CardStatDialog),
    dbMan(db),
    model(new QStandardItemModel(this))
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
    ui->siteFrame->setVisible(false);

    ui->typeCB->blockSignals(true);
    ui->typeCB->addItem("Exchange", 2);
    ui->typeCB->addItem("Site", 3);
    ui->typeCB->blockSignals(false);

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

    ui->TV->setModel(model);
}

CardStatDialog::~CardStatDialog()
{
    delete ui;
}

void CardStatDialog::on_areaCB_currentIndexChanged(int index)
{
    index++;
    ui->abbrCB->clear();
    ui->siteCB->clear();
    ui->cardCB->clear();
    ui->typeCB->blockSignals(true);
    ui->typeCB->setCurrentIndex(0);
    ui->typeCB->blockSignals(false);
    ui->siteFrame->setVisible(false);
    ui->devCB->setCurrentIndex(-1);
    int area = ui->areaCB->currentData().toInt();
    if(area > 1)
    {
        ui->exchFrame->setVisible(true);
        ui->typeFrame->setVisible(false);
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
    else
    {
        ui->exchFrame->setVisible(false);
        ui->devCB->setEnabled(true);
    }
    model->clear();
}

void CardStatDialog::on_abbrCB_currentIndexChanged(int index)
{
    index++;
    ui->devCB->setCurrentIndex(-1);
    int exchId = ui->abbrCB->currentData().toInt();
    ui->siteCB->clear();
    ui->cardCB->clear();
    ui->typeCB->blockSignals(true);
    ui->typeCB->setCurrentIndex(0);
    ui->typeCB->blockSignals(false);
    ui->siteFrame->setVisible(false);
    ui->devCB->setEnabled(true);
    if(exchId == 0)
    {
        //all
        ui->typeFrame->setVisible(false);

    }
    else
    {
        ui->typeFrame->setVisible(true);
    }

    model->clear();
}

void CardStatDialog::on_typeCB_currentIndexChanged(int index)
{
    model->clear();
    ui->siteCB->clear();
    ui->cardCB->clear();
    ui->devCB->setCurrentIndex(-1);

    if(index == 0)
    {
        //exch
        ui->siteFrame->setVisible(false);
        ui->devCB->setEnabled(true);
    }
    else
    {
        //site
        ui->siteFrame->setVisible(true);
        ui->devCB->setEnabled(false);
        int exchId = ui->abbrCB->currentData().toInt();
        int id;
        QString site;
        QSqlQuery *query = dbMan->selectExchangeSites(exchId);
        //`id`, `area`, `exchange`, `abbr`
        ui->siteCB->blockSignals(true);
        while(query->next())
        {
            id = query->value(0).toInt();
            site = query->value(2).toString();
            ui->siteCB->addItem(site, id);
        }
        ui->siteCB->setCurrentIndex(-1);
        ui->siteCB->blockSignals(false);

    }

    model->clear();
}

void CardStatDialog::on_siteCB_currentIndexChanged(int index)
{
    model->clear();
    ui->cardCB->clear();
    ui->devCB->setCurrentIndex(-1);

    if(index > -1)
        ui->devCB->setEnabled(true);
    else
        ui->devCB->setEnabled(false);

    model->clear();
}

void CardStatDialog::on_devCB_currentIndexChanged(int index)
{
    ui->cardCB->clear();
    model->clear();
    if(index > -1)
    {
        int deviceId = ui->devCB->currentData().toInt();
        QSqlQuery *query = dbMan->selectDeviceCards(deviceId);
        int id;
        QString card;
        ui->cardCB->blockSignals(true);
        while(query->next())
        {
            id = query->value(0).toInt();
            card = query->value(1).toString();
            ui->cardCB->addItem(card,id);
        }
        ui->cardCB->setCurrentIndex(-1);
        ui->cardCB->blockSignals(false);
    }
}

void CardStatDialog::on_cardCB_currentIndexChanged(int index)
{
    model->clear();

    if(index > -1)
    {
        int area = ui->areaCB->currentData().toInt();
        int exchId = ui->abbrCB->currentData().toInt();
        if(ui->typeCB->currentData().toInt() == 3)
            exchId = ui->siteCB->currentData().toInt();

        int deviceId = ui->devCB->currentData().toInt();
        int cardId = ui->cardCB->currentData().toInt();
        QString card = ui->cardCB->currentText();

        if(dbMan->isDeviceDslam(deviceId))
        {
            model->setColumnCount(1);
            model->setRowCount(1);
            model->setHeaderData(0,Qt::Horizontal,"Card Count");
            model->setHeaderData(0,Qt::Vertical,card);

            int cnt = dbMan->getDslamCardCount(area,exchId,deviceId,cardId);
            QModelIndex ind = model->index(0,0);
            model->setData(ind,cnt);
            ui->TV->setModel(model);
            ui->TV->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
            ui->TV->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
            ui->TV->verticalHeader()->show();
        }
        else
        {
            QSqlQuery query = *(dbMan->selectSlotsStatistics(area,exchId,deviceId,cardId));
            model->setColumnCount(5);
            model->setHeaderData(0,Qt::Horizontal,"ID");
            model->setHeaderData(1,Qt::Horizontal,"Abbr");
            model->setHeaderData(2,Qt::Horizontal,"Slot");
            model->setHeaderData(3,Qt::Horizontal,"Subslot");
            model->setHeaderData(4,Qt::Horizontal,"Status");
            int id,s,sub, row=-1;

            QString st, abbr;
            QModelIndex ind;

            QBrush greenBrush, redBrush;
            greenBrush.setColor(Qt::darkGreen);
            redBrush.setColor(Qt::darkRed);
            bool FREE;

            while(query.next())
            {
                FREE = false;
                id = query.value(0).toInt();
                abbr = query.value(1).toString();
                s = query.value(2).toInt();
                sub = query.value(3).toInt();
                st = query.value(4).toString();
                if(dbMan->cardRemovable(id))
                {
                    st = "Free";
                    FREE = true;
                }

                row ++;
                model->insertRow(row);
                ind = model->index(row,0);
                model->setData(ind, id);
                ind = ind.sibling(row,1);
                model->setData(ind,abbr);
                ind = ind.sibling(row,2);
                model->setData(ind, s);
                ind = ind.sibling(row,3);
                model->setData(ind,sub);
                ind = ind.sibling(row,4);
                model->setData(ind, st);

                if(FREE)
                    model->setData(ind,greenBrush,Qt::ForegroundRole);
                else
                    model->setData(ind,redBrush,Qt::ForegroundRole);
            }

            ui->TV->setModel(model);
            ui->TV->hideColumn(0);
            ui->TV->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
            ui->TV->verticalHeader()->hide();
        }
    }        
}

void CardStatDialog::on_CancelBtn_clicked()
{
    this->close();
}

