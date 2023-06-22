#include "lomdialog.h"
#include "ui_lomdialog.h"
#include "lib/database/danetdbman.h"
#include "lib/report/lom/Reportlom.h"
#include "mainPanel/menu/report/lom/lomitemdialog.h"
#include <QStandardItem>
#include <QSqlQuery>
#include <QMessageBox>
#include <QFileDialog>
#include <QFile>
#include <QDesktopServices>
#include <QTableView>
#include<QtMath>

#define COM_TYPE 0
#define COM_INFO 1
#define AMOUNT 2
#define LOCATION 3
#define PURCHASE 4
#define CODE 5
#define DESC 6

LomDialog::LomDialog(QWidget *parent, DanetDbMan *db) :
    QDialog(parent),
    ui(new Ui::LomDialog),
    dbMan(db),
    model(new QStandardItemModel(this)),
    modelRow(0)
{

    init();
    path = "";
}

LomDialog::LomDialog(QWidget *parent, DanetDbMan *db, const int Area, const int ExchId, const int Type, const int SiteId, const QString Path):
    QDialog(parent),
    ui(new Ui::LomDialog),
    dbMan(db),
    model(new QStandardItemModel(this)),
    modelRow(0)
{
    init();

    path = Path;
    ui->areaCB->setCurrentIndex(ui->areaCB->findData(Area));
    ui->abbrCB->setCurrentIndex(ui->abbrCB->findData(ExchId));
    ui->typeCB->setCurrentIndex(ui->typeCB->findData(Type));
    if(SiteId > 0)
        ui->siteCB->setCurrentIndex(ui->siteCB->findData(SiteId));

}

LomDialog::~LomDialog()
{
    delete ui;
}

void LomDialog::init()
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

    //type
    ui->typeCB->blockSignals(true);
    ui->typeCB->addItem("Exchange", 2);
    ui->typeCB->addItem("Site", 3);
    ui->typeCB->setCurrentIndex(-1);
    ui->typeCB->blockSignals(false);

    int workingArea = dbMan->getLoggedInDefaultArea();
    ui->areaCB->setCurrentIndex(ui->areaCB->findData(workingArea));
    ui->typeCB->setCurrentIndex(0);
    ui->siteFrame->setVisible(false);

    // model
    model->clear();
    model->setColumnCount(7);
    model->setHorizontalHeaderLabels(QStringList()<<"Commodity\nType"<<"Commodity"<<"Amount"<<"From\nLocation"<<"Purchase"
                                     <<"Commodity\nCode"<<"Description");

//    model->setHorizontalHeaderLabels(QStringList()<<"نوع المان"<<"المان"<<"تعداد"<<"محل تامین"<<"خرید"
//                                     <<"کد کالا"<<"توضیحات");

    ui->TV->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->TV->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->TV->verticalHeader()->hide();

    ///////////////////// context
    ui->TV->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->TV, SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(contextMenuSlot(QPoint)));
    connect(ui->TV, SIGNAL(doubleClicked(QModelIndex)),this,SLOT(doubleClickedSlot(QModelIndex)));
    contextMenu.addAction(ui->actionEdit_Item);
    contextMenu.addSeparator();
    contextMenu.addAction(ui->actionRemove_Item);


    ui->TV->setModel(model);
}

void LomDialog::on_areaCB_currentIndexChanged(int index)
{
    int area = index + 2;
    ui->siteFrame->setVisible(false);
    ui->siteCB->clear();
    ui->abbrCB->blockSignals(true);
    ui->abbrCB->clear();


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

    ui->abbrCB->setCurrentIndex(0);
}

void LomDialog::on_abbrCB_currentIndexChanged(int index)
{
    index++;
    ui->typeCB->blockSignals(true);
    ui->typeCB->setCurrentIndex(0);
    ui->typeCB->blockSignals(false);

    ui->siteCB->blockSignals(true);
    ui->siteCB->clear();
    ui->siteCB->blockSignals(false);
    ui->siteFrame->setVisible(false);
}

void LomDialog::on_typeCB_currentIndexChanged(int index)
{
    index++;
    ui->siteCB->blockSignals(true);
    ui->siteCB->clear();

    int type = ui->typeCB->currentData().toInt();

    if(type == 2) //exch
    {
        ui->siteFrame->setVisible(false);
        ui->siteCB->blockSignals(false);
    }
    else //site
    {
        ui->siteFrame->setVisible(true);

        int area = ui->areaCB->currentData().toInt();
        QString mother = ui->abbrCB->currentText();

        QSqlQuery *query = dbMan->selectSites(area,mother);
        //`id`,`abbr`

        int i;
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

        ui->siteCB->blockSignals(false);

    }

}

void LomDialog::contextMenuSlot(QPoint ptr)
{
    //`com_type`, `com_info`, `amount`, `location`, `purchase`, code , `description`

    QModelIndex clickedIndex = ui->TV->indexAt(ptr);

    row = clickedIndex.row();
    clickedIndex = clickedIndex.sibling(clickedIndex.row(), COM_TYPE);
    commodity_type = model->data(clickedIndex).toString();
    clickedIndex = clickedIndex.sibling(clickedIndex.row(), COM_INFO);
    commodity_info = model->data(clickedIndex).toString();
    clickedIndex = clickedIndex.sibling(clickedIndex.row(), AMOUNT);
    amount = model->data(clickedIndex).toInt();
    clickedIndex = clickedIndex.sibling(clickedIndex.row(), LOCATION);
    location = model->data(clickedIndex).toString();
    clickedIndex = clickedIndex.sibling(clickedIndex.row(), PURCHASE);
    purchase = model->data(clickedIndex).toString();
    clickedIndex = clickedIndex.sibling(clickedIndex.row(), CODE);
    code = model->data(clickedIndex).toString();
    clickedIndex = clickedIndex.sibling(clickedIndex.row(), DESC);
    desc = model->data(clickedIndex).toString();


    ptr.setY(ptr.y() + 30);
    if((!commodity_type.isEmpty()) || (!commodity_info.isEmpty()) )
    {
        contextMenu.popup(ui->TV->mapToGlobal(ptr));
    }
}

void LomDialog::doubleClickedSlot(QModelIndex ind)
{
    ind = ind.sibling(ind.row(), COM_TYPE);
    commodity_type = model->data(ind).toString();
    ind = ind.sibling(ind.row(), COM_INFO);
    commodity_info = model->data(ind).toString();
    ind = ind.sibling(ind.row(), AMOUNT);
    amount = model->data(ind).toInt();
    ind = ind.sibling(ind.row(), LOCATION);
    location = model->data(ind).toString();
    ind = ind.sibling(ind.row(), PURCHASE);
    purchase = model->data(ind).toString();
    ind = ind.sibling(ind.row(), CODE);
    code = model->data(ind).toString();
    ind = ind.sibling(ind.row(), DESC);
    desc = model->data(ind).toString();

    if((!commodity_type.isEmpty()) || (!commodity_info.isEmpty()) )
    {
        on_actionEdit_Item_triggered();
    }
}

void LomDialog::on_okBtn_clicked()
{
    if(model->rowCount() == 0)
    {
        QMessageBox::warning(this, "ERROR", "No item was added to the LOM list!");
        return;
    }

    QString customInfo = ui->descTE->toPlainText().trimmed();
    if(customInfo.isEmpty()) customInfo = "";

    //report
    if(path.isEmpty())
        path = QFileDialog::getSaveFileName(this, "Save DaNet LOM Report As", "","pdf (*.pdf)");

    if(!path.isEmpty())
    {
        if(!path.endsWith(".pdf")) path += ".pdf";

        int exchId = ui->abbrCB->currentData().toInt();
        if(ui->typeCB->currentData().toInt() == 3)
            exchId = ui->siteCB->currentData().toInt();

        QString abbr = dbMan->getExchangeAreaName(exchId);

        QString exchange = dbMan->getExchangeName(exchId);

        //"Equipment\nType"<<"Equipment\nInfo"<<"Quantity"<<"Location"<<"Purchase" << "Code"<<"Description"
        ReportLom reportLom(this,model,QList<int>()<<20<<40<<20<<40<<20<<40<<50, true, path, abbr, exchange, "DaNet LOM Report", customInfo );
        if(reportLom.print())
        {
            QDesktopServices::openUrl(path);
        }
        else
        {
            QMessageBox::warning(this, "ERROR", reportLom.lastError());
        }
    }

    this->close();
}

// add item
void LomDialog::on_addBtn_clicked()
{
    LomItemDialog item(dbMan);
    item.exec();

    if((item.Amount > 0) && (item.done == true) )
    {
        insertToModel(item.CommType, item.CommInfo,item.Amount,item.Location,item.Purchase,item.Code,item.Desc );
        // check for DSLAM
        if(item.CommInfo.compare("ADEE", Qt::CaseInsensitive) == 0)
        {
            //terminal, user cable
            insertToModel("Terminal", "Terminal-128",item.Amount,"-",item.Purchase,"-","-" );
            insertToModel("User Cable", "Cable-32X2",item.Amount*2,"-",item.Purchase,"-","-" );
        }
        else if(item.CommInfo.compare("ADLE", Qt::CaseInsensitive) == 0)
        {
            //terminal, user cable
            insertToModel("Terminal", "Terminal-128",qCeil(2*item.Amount*32.0/128),"-",item.Purchase,"-","-" );
            insertToModel("User Cable", "Cable-32",item.Amount*2,"-",item.Purchase,"-","-" );
        }
        else if(item.CommInfo.compare("ADPE", Qt::CaseInsensitive) == 0)
        {
            //terminal, user cable
            insertToModel("Terminal", "Terminal-128",qCeil(2*item.Amount*64.0/128),"-",item.Purchase,"-","-" );
            insertToModel("User Cable", "Cable-32X2",item.Amount*2,"-",item.Purchase,"-","-" );
        }
        else if(item.CommInfo.compare("SHEB", Qt::CaseInsensitive) == 0)
        {
            //terminal, user cable
            insertToModel("Terminal", "Terminal-128",qCeil(item.Amount*32.0/128),"-",item.Purchase,"-","-" );
            insertToModel("User Cable", "Cable-32",item.Amount,"-",item.Purchase,"-","-" );

        }
        else if(item.CommInfo.compare("SCUB", Qt::CaseInsensitive) == 0)
        {
            //o2gs, o4gs, coupler, module, patch cord
            insertToModel("Pinch Board", "O2GS", item.Amount,item.Location,item.Purchase,"-","-" );
            insertToModel("Pinch Board", "O4GS", item.Amount,item.Location,item.Purchase,"-","-" );
            insertToModel("Patch Cord", "MM SC/LC", 12,item.Location,item.Purchase,"-","-" );
            insertToModel("Coupler", "MM 2X2 LC/LC", 2,item.Location,item.Purchase,"-","-" );
            insertToModel("Module", "SX-500m",20,"-",item.Purchase,"-","-" );
        }
        else if(item.CommInfo.compare("MA5616", Qt::CaseInsensitive) == 0)
        {
            //control, power
            insertToModel("Control Board", "CCUB", 1,item.Location,item.Purchase,"-","-" );
            insertToModel("Power Board", "PDIA", 1,item.Location,item.Purchase,"-","-" );

        }
        else if(item.CommInfo.compare("MA5616H", Qt::CaseInsensitive) == 0)
        {
            //control, power
            insertToModel("Control Board", "CCUE", 1,item.Location,item.Purchase,"-","-" );
            insertToModel("Power Board", "PAIC", 1,item.Location,item.Purchase,"-","-" );

        }
        else if(item.CommInfo.compare("MA5616V", Qt::CaseInsensitive) == 0)
        {
            //control, power
            insertToModel("Control Board", "CCUE", 1,item.Location,item.Purchase,"-","-" );
            insertToModel("Power Board", "PAIC", 1,item.Location,item.Purchase,"-","-" );

        }
        else if(item.CommInfo.compare("VCPE", Qt::CaseInsensitive) == 0)// 64
        {
            //terminal, user cable
            insertToModel("Terminal", "Terminal-128",qCeil(2*item.Amount*64.0/128),"-",item.Purchase,"-","-" );
            insertToModel("User Cable", "Cable-32X2",item.Amount*2,"-",item.Purchase,"-","-" );

        }
        else if(item.CommInfo.compare("VDSE", Qt::CaseInsensitive) == 0) //32
        {
            //terminal, user cable
            insertToModel("Terminal", "Terminal-128",qCeil(2*item.Amount*32.0/128),"-",item.Purchase,"-","-" );
            insertToModel("User Cable", "Cable-32",item.Amount*2,"-",item.Purchase,"-","-" );
        }

    }
}

// context menu
void LomDialog::on_actionEdit_Item_triggered()
{
    LomItemDialog item(dbMan);
    item.setLabel("LOM Item Modification");
    item.setOkBtnText("Modify");
    item.setValues(commodity_type, commodity_info,amount,purchase,location,code,desc);
    item.exec();

    //modify
    if(item.done)
    {
        QModelIndex index = model->index(row,0);
        model->setData(index, item.CommType);
        index = index.sibling(row,1);
        model->setData(index, item.CommInfo);
        index = index.sibling(row,2);
        model->setData(index, item.Amount);
        index = index.sibling(row,3);
        model->setData(index, item.Location);
        index = index.sibling(row,4);
        model->setData(index, item.Purchase);
        index = index.sibling(row,5);
        model->setData(index, item.Code);
        index = index.sibling(row,6);
        model->setData(index, item.Desc);
    }

}

void LomDialog::on_actionRemove_Item_triggered()
{
    model->removeRow(row);
    modelRow = model->rowCount();
}

void LomDialog::insertToModel(QString cat, QString comm, int amt, QString loc, QString pur,QString cod, QString des)
{
    model->insertRow(modelRow);
    QModelIndex index = model->index(modelRow,0); // cat
    model->setData(index, cat);
    index = index.sibling(modelRow,1); // comm
    model->setData(index, comm);
    index = index.sibling(modelRow,2); //amount
    model->setData(index, amt);
    index = index.sibling(modelRow,3); //location
    model->setData(index, loc);
    index = index.sibling(modelRow,4); //purchase
    model->setData(index, pur);
    index = index.sibling(modelRow,5); //code
    model->setData(index, cod);
    index = index.sibling(modelRow,6); //desc
    model->setData(index, des);
    modelRow++;
}

