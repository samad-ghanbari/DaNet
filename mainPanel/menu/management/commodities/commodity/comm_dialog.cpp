#include "comm_dialog.h"
#include "ui_comm_dialog.h"
#include "lib/database/danetdbman.h"
#include "mainPanel/menu/management/commodities/usage/usage_dialog.h"
#include "mainPanel/menu/management/commodities/commodity/comm_addedit.h"
#include "mainPanel/menu/management/commodities/commodity/comm_remove.h"
#include <QSqlQueryModel>
#include "lib/sortfilterproxymodel.h"
#include "lib/headerview.h"

//0`cat_id`, 1`cat`, 2`info_id`, 3`commodity`, 4`purchase_id`,
//5`purchase`, 6`code`, 7`location`, 8`total_amount`, 9`left_amount`

#define CAT_ID 0
#define CATEGORY 1
#define COMM_ID 2
#define COMMODITY 3
#define PURCHASE_ID 4
#define PURCHASE 5
#define CODE 6
#define LOCATION 7
#define TOTAL 8
#define LEFT 9
#define TS 10
#define ENABLED 11


namespace Commodity
{
Dialog::Dialog(DanetDbMan *db, const int ID, const QString CAT, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog),
    dbMan(db),
    proxyModel(nullptr),
    header(nullptr),
    cat_id(ID),
    cat(CAT)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowMinMaxButtonsHint);

    ui->catLbl->setText(CAT);

    QString username = dbMan->getLoggedinUserame();
    if(username.compare(ADMIN, Qt::CaseInsensitive) != 0)
    {
        adminUser = false;
        ui->addBtn->setEnabled(false);
        ui->addBtn->setVisible(false);
    }
    else
        adminUser = true;


    proxyModel = new SortFilterProxyModel(this);
    header = new HeaderView(12,COMMODITY, ui->TV);
    ui->TV->setHorizontalHeader(header);
    header->hideWidgets(CAT_ID);
    header->hideWidgets(CATEGORY);
    header->hideWidgets(COMM_ID);
    header->hideWidgets(PURCHASE_ID);

    header->hideComboBox(COMMODITY);
    header->setLableText(COMMODITY, "Commodity");
    header->setLineEditPlaceHolder(COMMODITY, "Search");

    header->hideComboBox(PURCHASE);
    header->setLableText(PURCHASE, "Purchase");
    header->setLineEditPlaceHolder(PURCHASE, "Search");

    header->hideComboBox(CODE);
    header->setLableText(CODE, "Commodity Code");
    header->setLineEditPlaceHolder(CODE, "Search");

    header->hideComboBox(LOCATION);
    header->setLableText(LOCATION, "Location");
    header->setLineEditPlaceHolder(LOCATION, "Search");

    header->hideComboBox(TOTAL);
    header->setLableText(TOTAL, "Total Amount");
    header->hideLineEdit(TOTAL);

    header->hideComboBox(LEFT);
    header->setLableText(LEFT, "Left Amount");
    header->hideLineEdit(LEFT);

    header->hideComboBox(TS);
    header->setLableText(TS, "Date");
    header->setLineEditPlaceHolder(TS, "Search");

    header->hideComboBox(ENABLED);
    header->setLableText(ENABLED, "Enabled");
    header->hideLineEdit(ENABLED);

    fillTV();

    connect(header, SIGNAL(sectionsWidgetsSignal()), this, SLOT(headerWidgetsChagedSlot()));

    ///////////////////// context
    ui->TV->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->TV, SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(contextMenuSlot(QPoint)));
    connect(ui->TV, SIGNAL(doubleClicked(QModelIndex)),this,SLOT(doubleClickedSlot(QModelIndex)));
    contextMenu.addAction(ui->actionEdit_Commodity);
    contextMenu.addSeparator();
    contextMenu.addAction(ui->actionRemove_Commodity);

}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::contextMenuSlot(QPoint ptr)
{
    if(!adminUser) return;

    QModelIndex clickedIndex = ui->TV->indexAt(ptr);
    row = clickedIndex.row();

    clickedIndex = clickedIndex.sibling(clickedIndex.row(), COMM_ID);
    comm_id = proxyModel->data(clickedIndex).toInt();
    clickedIndex = clickedIndex.sibling(clickedIndex.row(), COMMODITY);
    commodity = proxyModel->data(clickedIndex).toString();
    clickedIndex = clickedIndex.sibling(clickedIndex.row(), PURCHASE_ID);
    purchase_id = proxyModel->data(clickedIndex).toInt();
    clickedIndex = clickedIndex.sibling(clickedIndex.row(), PURCHASE);
    purchase = proxyModel->data(clickedIndex).toString();
    clickedIndex = clickedIndex.sibling(clickedIndex.row(), CODE);
    code = proxyModel->data(clickedIndex).toString();
    clickedIndex = clickedIndex.sibling(clickedIndex.row(), LOCATION);
    location = proxyModel->data(clickedIndex).toString();
    clickedIndex = clickedIndex.sibling(clickedIndex.row(), TOTAL);
    total = proxyModel->data(clickedIndex).toInt();
    clickedIndex = clickedIndex.sibling(clickedIndex.row(), LEFT);
    left = proxyModel->data(clickedIndex).toInt();
    clickedIndex = clickedIndex.sibling(clickedIndex.row(), ENABLED);
    QString en = proxyModel->data(clickedIndex).toString();
    enabled = (en.compare("Enabled", Qt::CaseInsensitive) == 0)?1:0;

    ptr.setY(ptr.y() + 80);
    if(comm_id > 0)
    {
        contextMenu.popup(ui->TV->mapToGlobal(ptr));
    }
}

void Dialog::doubleClickedSlot(QModelIndex ind)
{
    ind = ind.sibling(ind.row(), COMM_ID);
    comm_id = proxyModel->data(ind).toInt();
    ind = ind.sibling(ind.row(), COMMODITY);
    commodity = proxyModel->data(ind).toString();
    ind = ind.sibling(ind.row(), TOTAL);
    total = proxyModel->data(ind).toInt();
    ind = ind.sibling(ind.row(), LEFT);
    left = proxyModel->data(ind).toInt();
    ind = ind.sibling(ind.row(), ENABLED);
    QString en = proxyModel->data(ind).toString();
    enabled = (en.compare("Enabled", Qt::CaseInsensitive) == 0)?1:0;

    if((comm_id > 0) && (enabled))
    {
        //usage
        Commodity::Usage::Dialog *uDialog = new Commodity::Usage::Dialog(dbMan,comm_id, commodity,total, left, this);
        connect(uDialog,SIGNAL(changeSignal()), this, SLOT(changeSlot()));
        connect(this, SIGNAL(changeSignal()), uDialog, SLOT(changeSlot()));
        uDialog->showMaximized();
    }

}

void Dialog::fillTV()
{
    QSqlQueryModel *model = new QSqlQueryModel(this);
    model->setQuery(*dbMan->selectViewCommodity(cat_id, false));
    proxyModel->setSourceModel(model);
    proxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);

    //0`cat_id`, 1`cat`, 2`comm_id`, 3`commodity`, 4`purchase_id`,
    //5`purchase`, 6`code`, 7`location`, 8`total_amount`, 9`left_amount`, 10 ts
    ui->TV->setModel(proxyModel);
    ui->TV->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->TV->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->TV->verticalHeader()->hide();
    ui->TV->hideColumn(CAT_ID);
    ui->TV->hideColumn(CATEGORY);
    ui->TV->hideColumn(COMM_ID);
    ui->TV->hideColumn(PURCHASE_ID);
}

void Dialog::on_addBtn_clicked()
{
    Commodity::AddEdit add(cat_id,dbMan, this);
    add.setLables("New Commodity", cat);
    add.exec();
    fillTV();
}

void Dialog::on_actionEdit_Commodity_triggered()
{
    Commodity::AddEdit edit(cat_id, dbMan, this);
    edit.setLables("Commodity Modification", cat);
    edit.setValues(comm_id, commodity,purchase_id,location,code,total, left,enabled);
    edit.exec();
    fillTV();
    emit changeSignal();
}

void Dialog::on_actionRemove_Commodity_triggered()
{
    Commodity::Remove remove(comm_id, dbMan, this);
    remove.setLables("Remove Commodity", cat);
    remove.setValues(commodity,purchase,location,code,total);
    remove.exec();
    fillTV();
}

void Dialog::changeSlot()
{
    fillTV();
}

void Dialog::headerWidgetsChagedSlot()
{
    commodity = header->sections[COMMODITY]->lineEdit->text();
    purchase = header->sections[PURCHASE]->lineEdit->text();
    code = header->sections[CODE]->lineEdit->text();
    location = header->sections[LOCATION]->lineEdit->text();
    ts = header->sections[TS]->lineEdit->text();

    QMap<int, QString> mapString;
    QMap<int, QRegExp> mapRegExp;
    QRegExp reg;
    reg.setCaseSensitivity(Qt::CaseInsensitive);

    if(!commodity.isEmpty())
    {
        reg.setPattern(commodity);
        mapRegExp.insert(COMMODITY, reg);
    }
    if(!purchase.isEmpty())
    {
        reg.setPattern(purchase);
        mapRegExp.insert(PURCHASE, reg);
    }
    if(!location.isEmpty())
    {
        reg.setPattern(location);
        mapRegExp.insert(LOCATION, reg);
    }
    if(!code.isEmpty())
    {
        reg.setPattern(code);
        mapRegExp.insert(CODE, reg);
    }
    if(!ts.isEmpty())
    {
        reg.setPattern(ts);
        mapRegExp.insert(TS, reg);
    }

    proxyModel->setColumnsFilter(mapString, mapRegExp);
    ui->TV->setModel(proxyModel);
    ui->TV->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->TV->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->TV->verticalHeader()->hide();
    ui->TV->hideColumn(CAT_ID);
    ui->TV->hideColumn(CATEGORY);
    ui->TV->hideColumn(COMM_ID);
    ui->TV->hideColumn(PURCHASE_ID);

}

}
