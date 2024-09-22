#include "dslamdialog.h"
#include "ui_dslamdialog.h"
#include "lib/database/danetdbman.h"
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QStringListModel>
#include <QVBoxLayout>
#include "mainPanel/menu/plan/dslam/dslam5600/dslamma5600widget.h"
#include "mainPanel/menu/plan/dslam/dslam5616/dslamma5616widget.h"
#include "mainPanel/menu/plan/dslam/dslammxk819/dslammxk819widget.h"


DslamDialog::DslamDialog(QWidget *parent, DanetDbMan *Db) :
    QDialog(parent),
    ui(new Ui::DslamDialog),
    dbMan(Db)
{
    ui->setupUi(this);
    init();
}

DslamDialog::DslamDialog(QWidget *parent, DanetDbMan *Db, const int area, const int ExchId, const int type, const int SiteId, const int DevexId) :
    QDialog(parent),
    ui(new Ui::DslamDialog),
    dbMan(Db)
{
    ui->setupUi(this);

    init();

    ui->areaCB->setCurrentIndex(ui->areaCB->findData(area));
    ui->abbrCB->setCurrentIndex(ui->abbrCB->findData(ExchId));
    ui->typeCB->setCurrentIndex(ui->typeCB->findData(type));
    ui->siteCB->setCurrentIndex(ui->siteCB->findData(SiteId));
    ui->dslamCB->setCurrentIndex(ui->dslamCB->findData(DevexId));
}

DslamDialog::~DslamDialog()
{
    delete ui;
}

void DslamDialog::init()
{
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

    //type
    ui->typeCB->blockSignals(true);
    ui->typeCB->addItem("Exchange", 2);
    ui->typeCB->addItem("Site", 3);
    ui->typeCB->setCurrentIndex(-1);
    ui->typeCB->blockSignals(false);


    int workingArea = dbMan->getLoggedInDefaultArea();
    ui->areaCB->setCurrentIndex(ui->areaCB->findData(workingArea));

    dslamId = -1;
}

void DslamDialog::on_areaCB_currentIndexChanged(int index)
{
    int area = index + 2;
    qDeleteAll(ui->parentWidget->children());

    ui->abbrCB->blockSignals(true);
    ui->abbrCB->clear();

    ui->dslamCB->blockSignals(true);
    ui->dslamCB->clear();
    ui->dslamCB->blockSignals(false);

    QString abbr;
    int id;
    QSqlQuery *query = dbMan->selectExchanges(area);
    //`id`,`abbr`

    while(query->next())
    {
        id = query->value(0).toInt();
        abbr = query->value(1).toString();
        ui->abbrCB->addItem(abbr, id);
    }

    ui->abbrCB->setCurrentIndex(-1);
    ui->abbrCB->blockSignals(false);
    ui->typeCB->setCurrentIndex(0);

}

void DslamDialog::on_abbrCB_currentIndexChanged(int index)
{
    index++;
    qDeleteAll(ui->parentWidget->children());
    ui->typeCB->blockSignals(true);
    ui->typeCB->setCurrentIndex(0);
    ui->typeCB->blockSignals(false);

    ui->siteCB->blockSignals(true);
    ui->siteCB->clear();
    ui->siteCB->blockSignals(false);
    ui->frame->setVisible(false);

    ui->dslamCB->blockSignals(true);
    ui->dslamCB->clear();

    int exchId = ui->abbrCB->currentData().toInt();

    QSqlQuery *query = dbMan->selectExchangeDslams(exchId);
    //id,dslam
    int id;
    QString dslam,pur, t,sn;
    while(query->next())
    {
        id = query->value(0).toInt();
        dslam = query->value(1).toString();
        pur = query->value(2).toString();
        if(query->value(3).isNull() || query->value(4).isNull())
        {
            ui->dslamCB->addItem(dslam,id);
        }
        else
        {
            t = query->value(3).toString();
            sn = query->value(4).toString();
            dslam += "-"+t+"-"+pur+"-"+sn;
            ui->dslamCB->addItem(dslam,id);
        }
    }

    ui->dslamCB->setCurrentIndex(-1);
    ui->dslamCB->blockSignals(false);

}

void DslamDialog::on_typeCB_currentIndexChanged(int index)
{
    index++;
    qDeleteAll(ui->parentWidget->children());
    ui->siteCB->blockSignals(true);
    ui->siteCB->clear();

    int type = ui->typeCB->currentData().toInt();

    if(type == 2) //exch
    {
        ui->frame->setEnabled(false);
        ui->frame->setVisible(false);
        ui->siteCB->blockSignals(false);

        ui->dslamCB->blockSignals(true);
        ui->dslamCB->clear();

        int exchId = ui->abbrCB->currentData().toInt();

        QSqlQuery *query = dbMan->selectExchangeDslams(exchId);
        //id,dslam
        int id;
        QString dslam, pur, t,sn;
        while(query->next())
        {
            id = query->value(0).toInt();
            dslam = query->value(1).toString();
            pur = query->value(2).toString();
            if(query->value(3).isNull() || query->value(4).isNull())
            {
                ui->dslamCB->addItem(dslam,id);
            }
            else
            {
                t = query->value(3).toString();
                sn = query->value(4).toString();
                dslam += "-"+t+"-"+pur+"-"+sn;
                ui->dslamCB->addItem(dslam,id);
            }
        }

        ui->dslamCB->setCurrentIndex(-1);
        ui->dslamCB->blockSignals(false);

    }
    else //site
    {
        ui->frame->setEnabled(true);
        ui->frame->setVisible(true);
        ui->dslamCB->blockSignals(true);
        ui->dslamCB->clear();
        ui->dslamCB->blockSignals(false);

        int i;
        int area = ui->areaCB->currentData().toInt();
        QString mother = ui->abbrCB->currentText();

        QSqlQuery *query = dbMan->selectSites(area,mother);
        //`id`,`abbr`

        QString a, s,c,n;
        while(query->next())
        {
            i = query->value(0).toInt();
            a = query->value(1).toString();
            s = query->value(2).toString();
            c = query->value(3).toString();
            n = query->value(4).toString();
            ui->siteCB->addItem(c+"-"+n+"*"+a+"*"+s, i);
        }

        ui->siteCB->setCurrentIndex(-1);
        ui->siteCB->blockSignals(false);

    }


}

void DslamDialog::on_siteCB_currentIndexChanged(int index)
{
    index++;
    qDeleteAll(ui->parentWidget->children());
    ui->dslamCB->blockSignals(true);
    ui->dslamCB->clear();

    if(ui->typeCB->currentData().toInt() == 3) //site
    {
        int exchId = ui->siteCB->currentData().toInt();

        QSqlQuery *query = dbMan->selectExchangeDslams(exchId);
        //id,dslam,p,t,sn
        int id;
        QString dslam,pur,t,sn;
        while(query->next())
        {
            id = query->value(0).toInt();
            dslam = query->value(1).toString();
            pur = query->value(2).toString();
            if(query->value(3).isNull() || query->value(4).isNull())
            {
                ui->dslamCB->addItem(dslam,id);
            }
            else
            {
                t = query->value(3).toString();
                sn = query->value(4).toString();
                dslam += "-"+t+"-"+pur+"-"+sn;
                ui->dslamCB->addItem(dslam,id);
            }

        }
    }

    ui->dslamCB->setCurrentIndex(-1);
    ui->dslamCB->blockSignals(false);

    if(ui->dslamCB->count() > 0)
        ui->dslamCB->setCurrentIndex(0);

}

void DslamDialog::on_dslamCB_currentIndexChanged(int index)
{
    index++;
    qDeleteAll(ui->parentWidget->children());
    QVBoxLayout *layout = new QVBoxLayout(ui->parentWidget);
    layout->setMargin(0);

    area = -1;
    dslamId = -1;
    dslamName = "";

    area = ui->areaCB->currentData().toInt();
    if(ui->typeCB->currentData().toInt() == 2)//exch
        exchId = ui->abbrCB->currentData().toInt();
    else
        exchId = ui->siteCB->currentData().toInt();
    dslamId = ui->dslamCB->currentData().toInt();
    dslamName = ui->dslamCB->currentText();

    if(dslamName.isEmpty()) dslamId = -1;

    int dslamType = dbMan->getDevexDslamType(dslamId);// 1:MA5300, 2:MA5600, 3:Zhone, 4:MA5616

    if(dslamType == 2) // MA5600
    {
        DslamMa5600Widget *dslam5600 = new DslamMa5600Widget(ui->parentWidget,dbMan,area,exchId,dslamId);
        dslam5600->setAttribute(Qt::WA_DeleteOnClose);
        connect(dslam5600, SIGNAL(finished(int)), this, SIGNAL(finished(int)));
        connect(dslam5600, SIGNAL(dslamNameSignal(int)), this, SLOT(updateDslamCB(int)));
        layout->addWidget(dslam5600);
    }
    else if(dslamType == 3) // MXK819
    {
        DslamMxk819Widget *dslammxk = new DslamMxk819Widget(ui->parentWidget,dbMan,area,exchId,dslamId);
        dslammxk->setAttribute(Qt::WA_DeleteOnClose);
        connect(dslammxk, SIGNAL(finished(int)), this, SIGNAL(finished(int)));
        connect(dslammxk, SIGNAL(dslamNameSignal(int)), this, SLOT(updateDslamCB(int)));
        layout->addWidget(dslammxk);
    }
    else if(dslamType == 4) // MA5616
    {
        DslamMa5616Widget *dslam5616 = new DslamMa5616Widget(ui->parentWidget,dbMan,area,exchId,dslamId);
        dslam5616->setAttribute(Qt::WA_DeleteOnClose);
        connect(dslam5616, SIGNAL(finished(int)), this, SIGNAL(finished(int)));
        connect(dslam5616, SIGNAL(dslamNameSignal(int)), this, SLOT(updateDslamCB(int)));
        layout->addWidget(dslam5616);
    }
    
}

void DslamDialog::updateDslamCB(int i)
{

    i++;
    ui->dslamCB->blockSignals(true);

    int dsId = ui->dslamCB->currentData().toInt();
    ui->dslamCB->clear();

    int exchId = ui->abbrCB->currentData().toInt();
    if(ui->typeCB->currentData().toInt() == 3)
        exchId = ui->siteCB->currentData().toInt();

    QSqlQuery *query = dbMan->selectExchangeDslams(exchId);
    //id,dslam,p,t,sn
    int id;
    QString dslam,pur,t,sn;
    while(query->next())
    {
        id = query->value(0).toInt();
        dslam = query->value(1).toString();
        pur = query->value(2).toString();
        if(query->value(3).isNull() || query->value(4).isNull())
        {
            ui->dslamCB->addItem(dslam,id);
        }
        else
        {
            t = query->value(3).toString();
            sn = query->value(4).toString();
            dslam += "-"+t+"-"+pur+"-"+sn;
            ui->dslamCB->addItem(dslam,id);
        }

    }


    ui->dslamCB->setCurrentIndex(ui->dslamCB->findData(dsId));
    ui->dslamCB->blockSignals(false);

}
