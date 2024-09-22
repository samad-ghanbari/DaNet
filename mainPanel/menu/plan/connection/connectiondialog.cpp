#include "connectiondialog.h"
#include "ui_connectiondialog.h"
#include "lib/database/danetdbman.h"
#include <QSqlQueryModel>
#include <QSqlQuery>
#include "mainPanel/menu/plan/connection/addbrasaggdialog.h"
#include "mainPanel/menu/plan/connection/editbrasaggdialog.h"
#include "mainPanel/menu/plan/connection/removebrasaggdialog.h"
#include "mainPanel/menu/plan/connection/addaggcxdialog.h"
#include "mainPanel/menu/plan/connection/editaggcxdialog.h"
#include "mainPanel/menu/plan/connection/removeaggcxdialog.h"

#define ID 0
#define BRAS_EXCH_ID 1
#define BRAS_EXCH 2
#define BRAS_ID 3
#define AGG_EXCH_ID 4
#define AGG_EXCH 5
#define AGG1_ID 6
#define AGG1 7
#define AGG1_ETH 8
#define AGG2_ID 9
#define AGG2 10
#define AGG2_ETH 11


ConnectionDialog::ConnectionDialog(QWidget *parent, DanetDbMan *db) :
    QDialog(parent),
    ui(new Ui::ConnectionDialog),
    dbMan(db),
    model(new QSqlQueryModel(this)),
    aggCxModel(new QSqlQueryModel(this))
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowMinMaxButtonsHint);

    ui->brasTV->setModel(model);
    ui->brasTV->verticalHeader()->hide();
    ui->brasTV->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    ui->aggCxTV->setModel(aggCxModel);
    ui->aggCxTV->verticalHeader()->hide();
    ui->aggCxTV->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    //area
    ui->brasAreaCB->blockSignals(true);
    ui->brasAreaCB->addItem("2", 2);
    ui->brasAreaCB->addItem("3", 3);
    ui->brasAreaCB->addItem("4", 4);
    ui->brasAreaCB->addItem("5", 5);
    ui->brasAreaCB->addItem("6", 6);
    ui->brasAreaCB->addItem("7", 7);
    ui->brasAreaCB->addItem("8", 8);
    ui->brasAreaCB->addItem("Karaj", 9);
    ui->brasAreaCB->setCurrentIndex(-1);
    ui->brasAreaCB->blockSignals(false);
    ui->brasAddBtn->setEnabled(false);

    ui->aggCxAreaCB->blockSignals(true);
    ui->aggCxAreaCB->addItem("2", 2);
    ui->aggCxAreaCB->addItem("3", 3);
    ui->aggCxAreaCB->addItem("4", 4);
    ui->aggCxAreaCB->addItem("5", 5);
    ui->aggCxAreaCB->addItem("6", 6);
    ui->aggCxAreaCB->addItem("7", 7);
    ui->aggCxAreaCB->addItem("8", 8);
    ui->aggCxAreaCB->addItem("Karaj", 9);
    ui->aggCxAreaCB->setCurrentIndex(-1);
    ui->aggCxAreaCB->blockSignals(false);

    // context menu
    ui->brasTV->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->brasTV, SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(contextMenuSlot(QPoint)));
    contextMenu.addAction(ui->actionEdit_BRAS_Connection);
    contextMenu.addAction(ui->actionRemove_BRAS_Connection);

    ui->aggCxTV->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->aggCxTV, SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(aggCxContextMenuSlot(QPoint)));
    aggCxContextMenu.addAction(ui->actionEdit_Inter_Connection);
    aggCxContextMenu.addAction(ui->actionRemove_Inter_Connection);

    int workingArea = dbMan->getLoggedInDefaultArea();
    ui->brasAreaCB->setCurrentIndex(ui->brasAreaCB->findData(workingArea));
    ui->aggCxAreaCB->setCurrentIndex(ui->aggCxAreaCB->findData(workingArea));

}

ConnectionDialog::~ConnectionDialog()
{
    delete ui;
}


void ConnectionDialog::fillBrasTV()
{
    const int brasDevexId = ui->brasCB->currentData().toInt();
    QSqlQuery *query = dbMan->selectViewBras(brasDevexId);
    //0`id`, 1`bras_exchange_id`, 2`bras_abbr`, 3`bras_id`, 4`agg_exchange_id`, 5`agg_abbr`, 6`agg1_id`, 7`agg1`, 8`agg1_eth`, 9`agg2_id`, 10`agg2`, 11`agg2_eth`
    model->setQuery(*query);
    ui->brasTV->hideColumn(ID);
    ui->brasTV->hideColumn(BRAS_EXCH_ID);
    ui->brasTV->hideColumn(BRAS_ID);
    ui->brasTV->hideColumn(AGG_EXCH_ID);
    ui->brasTV->hideColumn(AGG1_ID);
    ui->brasTV->hideColumn(AGG2_ID);

    model->setHeaderData(BRAS_EXCH, Qt::Horizontal,"BRAS\nExchange Abbr");
    model->setHeaderData(AGG_EXCH, Qt::Horizontal,"Exchange Abbr");
    model->setHeaderData(AGG1, Qt::Horizontal,"Agg-1");
    model->setHeaderData(AGG1_ETH, Qt::Horizontal,"BRAS to Agg1\nEth-Trunk");
    model->setHeaderData(AGG2, Qt::Horizontal,"Agg-2");
    model->setHeaderData(AGG2_ETH, Qt::Horizontal,"BRAS to Agg2\nEth-Trunk");
}

void ConnectionDialog::on_brasAreaCB_currentIndexChanged(int index)
{
    index++;
    model->clear();
    int area = ui->brasAreaCB->currentData().toInt();
    QSqlQuery *query = dbMan->selectBras(area);
    //id, deviceName
    int id;
    QString name;
    ui->brasCB->blockSignals(true);
    ui->brasCB->clear();
    while(query->next())
    {
        id = query->value(0).toInt();
        name = query->value(1).toString();
        ui->brasCB->addItem(name, id);
    }
    ui->brasCB->setCurrentIndex(-1);
    ui->brasCB->blockSignals(false);
}

void ConnectionDialog::on_brasCB_currentIndexChanged(int index)
{
    if(index  > -1)
        ui->brasAddBtn->setEnabled(true);
    else
        ui->brasAddBtn->setEnabled(false);

    fillBrasTV();

}

void ConnectionDialog::contextMenuSlot(QPoint ptr)
{
    QModelIndex clickedIndex = ui->brasTV->indexAt(ptr);
    Row = clickedIndex.row();
    ptr.setY(ptr.y() + 30);
    //0`id`, 1`bras_exchange_id`, 2`bras_abbr`, 3`bras_id`, 4`agg_exchange_id`, 5`agg_abbr`, 6`agg1_id`, 7`agg1`, 8`agg1_eth`, 9`agg2_id`, 10`agg2`, 11`agg2_eth`

    clickedIndex = clickedIndex.sibling(clickedIndex.row(), ID); // id
    id = model->data(clickedIndex).toInt();
    if(id > 0)
    {
        clickedIndex = clickedIndex.sibling(clickedIndex.row(), BRAS_EXCH_ID);
        brasExchId = model->data(clickedIndex).toInt();

        clickedIndex = clickedIndex.sibling(clickedIndex.row(), BRAS_ID);
        brasId = model->data(clickedIndex).toInt();

        clickedIndex = clickedIndex.sibling(clickedIndex.row(), AGG_EXCH_ID);
        aggExchId = model->data(clickedIndex).toInt();

        clickedIndex = clickedIndex.sibling(clickedIndex.row(), AGG1);
        aggAbbr = model->data(clickedIndex).toString();

        clickedIndex = clickedIndex.sibling(clickedIndex.row(), AGG1_ID);
        agg1DevexId = model->data(clickedIndex).toInt();

        clickedIndex = clickedIndex.sibling(clickedIndex.row(), AGG1);
        agg1 = model->data(clickedIndex).toString();

        clickedIndex = clickedIndex.sibling(clickedIndex.row(), AGG1_ETH);
        eth1 = model->data(clickedIndex).toInt();

        clickedIndex = clickedIndex.sibling(clickedIndex.row(), AGG2_ID);
        agg2DevexId = model->data(clickedIndex).toInt();

        clickedIndex = clickedIndex.sibling(clickedIndex.row(), AGG2);
        agg2 = model->data(clickedIndex).toString();

        clickedIndex = clickedIndex.sibling(clickedIndex.row(), AGG2_ETH);
        eth2 = model->data(clickedIndex).toInt();

        contextMenu.popup(ui->brasTV->mapToGlobal(ptr));
    }
}

void ConnectionDialog::on_brasAddBtn_clicked()
{
    int area = ui->brasAreaCB->currentData().toInt();
    QString bras = ui->brasCB->currentText();
    int brasDevexId = ui->brasCB->currentData().toInt();

    AddBrasAggDialog *addBrasAggDialog = new AddBrasAggDialog(this, dbMan,area, brasDevexId, bras);
    addBrasAggDialog->setAttribute(Qt::WA_DeleteOnClose);
    connect(addBrasAggDialog, SIGNAL(finished(int)), this,SLOT(refreshBrasSlot(int)));
    addBrasAggDialog->show();
}

void ConnectionDialog::on_actionEdit_BRAS_Connection_triggered()
{
    int area = ui->brasAreaCB->currentData().toInt();
    QString brasName = ui->brasCB->currentText();

    EditBrasAggDialog *editBrasAggDialog = new EditBrasAggDialog(this,dbMan, id,brasId,area,brasName,aggExchId,agg1DevexId,eth1,agg2DevexId,eth2);
    editBrasAggDialog->setAttribute(Qt::WA_DeleteOnClose);
    connect(editBrasAggDialog, SIGNAL(finished(int)), this,SLOT(refreshBrasSlot(int)));
    editBrasAggDialog->show();
}

void ConnectionDialog::on_actionRemove_BRAS_Connection_triggered()
{
    QString brasName = ui->brasCB->currentText();

    RemoveBrasAggDialog *removeBrasAggDialog = new RemoveBrasAggDialog(this, dbMan, id,brasId, brasName, aggAbbr,agg1,eth1,agg2,eth2);
    removeBrasAggDialog->setAttribute(Qt::WA_DeleteOnClose);
    connect(removeBrasAggDialog, SIGNAL(finished(int)), this,SLOT(refreshBrasSlot(int)));
    removeBrasAggDialog->show();
}

//.....................................................

void ConnectionDialog::fillAggCxTV()
{
    int area = ui->aggCxAreaCB->currentData().toInt();
    QSqlQuery *query = dbMan->selectViewAggCx(area);
//0`id`,1`exchange_id`,2`abbr`,3`agg1_id`,4`agg1`,5`agg1_eth`,6`cx_id`,7`cx`,8`cx_eth`
    aggCxModel->setQuery(*query);
    ui->aggCxTV->hideColumn(0);
    ui->aggCxTV->hideColumn(1);
    ui->aggCxTV->hideColumn(3);
    ui->aggCxTV->hideColumn(6);

    aggCxModel->setHeaderData(2, Qt::Horizontal,"Exchange Abbr");
    aggCxModel->setHeaderData(4, Qt::Horizontal,"Aggregation 1");
    aggCxModel->setHeaderData(5, Qt::Horizontal,"AGG1 To CX600\nEther-Trunk");
    aggCxModel->setHeaderData(7, Qt::Horizontal,"CX600");
    aggCxModel->setHeaderData(8, Qt::Horizontal,"CX600 To AGG1\nEther-Trunk");
}

void ConnectionDialog::refreshBrasSlot(int done)
{

    done++;
    fillBrasTV();
    ui->brasTV->selectRow(Row);
}

void ConnectionDialog::refreshAggCxSlot(int done)
{
    done++;
    fillAggCxTV();
    ui->aggCxTV->selectRow(Row);
}

void ConnectionDialog::on_aggCxAreaCB_currentIndexChanged(int index)
{
    index++;
    aggCxModel->clear();
    fillAggCxTV();
}

void ConnectionDialog::aggCxContextMenuSlot(QPoint ptr)
{
    QModelIndex clickedIndex = ui->aggCxTV->indexAt(ptr);
    Row = clickedIndex.row();
    ptr.setY(ptr.y() + 30);
//0`id`,1`exchange_id`,2`abbr`,3`agg1_id`,4`agg1`,5`agg1_eth`,6`cx_id`,7`cx`,8`cx_eth`

    clickedIndex = clickedIndex.sibling(clickedIndex.row(), 0); // id
    Id = aggCxModel->data(clickedIndex).toInt();
    if(Id  > 0)
    {
        clickedIndex = clickedIndex.sibling(clickedIndex.row(), 1);
        ExchId = aggCxModel->data(clickedIndex).toInt();
        clickedIndex = clickedIndex.sibling(clickedIndex.row(), 2);
        Abbr = aggCxModel->data(clickedIndex).toString();
        clickedIndex = clickedIndex.sibling(clickedIndex.row(), 3);
        AggDevexId = aggCxModel->data(clickedIndex).toInt();
        clickedIndex = clickedIndex.sibling(clickedIndex.row(), 4);
        Agg = aggCxModel->data(clickedIndex).toString();
        clickedIndex = clickedIndex.sibling(clickedIndex.row(), 5);
        AggEth = aggCxModel->data(clickedIndex).toInt();
        clickedIndex = clickedIndex.sibling(clickedIndex.row(), 6);
        CxDevexId = aggCxModel->data(clickedIndex).toInt();
        clickedIndex = clickedIndex.sibling(clickedIndex.row(), 7);
        Cx = aggCxModel->data(clickedIndex).toString();
        clickedIndex = clickedIndex.sibling(clickedIndex.row(), 8);
        CxEth = aggCxModel->data(clickedIndex).toInt();

        aggCxContextMenu.popup(ui->aggCxTV->mapToGlobal(ptr));
    }
}

void ConnectionDialog::on_aggCxAddBtn_clicked()
{
    int area = ui->aggCxAreaCB->currentData().toInt();

    AddAggCxDialog *addAggCxDialog = new AddAggCxDialog(this, dbMan, area);
    addAggCxDialog->setAttribute(Qt::WA_DeleteOnClose);
    connect(addAggCxDialog, SIGNAL(finished(int)), this,SLOT(refreshAggCxSlot(int)));
    addAggCxDialog->show();
}

void ConnectionDialog::on_actionEdit_Inter_Connection_triggered()
{
    int area = ui->aggCxAreaCB->currentData().toInt();

    EditAggCxDialog *editAggCxDialog = new EditAggCxDialog(this, dbMan,Id,area,ExchId,AggDevexId,AggEth,CxDevexId,CxEth);
    editAggCxDialog->setAttribute(Qt::WA_DeleteOnClose);
    connect(editAggCxDialog, SIGNAL(finished(int)), this,SLOT(refreshAggCxSlot(int)));
    editAggCxDialog->show();
}

void ConnectionDialog::on_actionRemove_Inter_Connection_triggered()
{
    RemoveAggCxDialog *removeAggCxDialog = new RemoveAggCxDialog(this,dbMan, Id, Abbr,AggDevexId,Agg,AggEth,CxDevexId,Cx,CxEth);
    removeAggCxDialog->setAttribute(Qt::WA_DeleteOnClose);
    connect(removeAggCxDialog, SIGNAL(finished(int)), this,SLOT(refreshAggCxSlot(int)));
    removeAggCxDialog->show();
}
