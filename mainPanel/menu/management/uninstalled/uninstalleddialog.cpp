#include "uninstalleddialog.h"
#include "ui_uninstalleddialog.h"
#include "lib/database/danetdbman.h"
#include <QSqlQueryModel>
#include "lib/sortfilterproxymodel.h"
#include "lib/headerview.h"
#include "mainPanel/menu/management/uninstalled/uninstalledaddeditdialog.h"
#include "mainPanel/menu/management/uninstalled/uninstalledremovedialog.h"

//`id`, `user_id`, `user`, `area`, `exchange`, `commodity`, `amount`, `letter_id`, `description`, `ts`
#define ID 0
#define USER_ID 1
#define USER 2
#define AREA 3
#define EXCHANGE 4
#define COMMODITY 5
#define AMOUNT 6
#define LETTER 7
#define DESC 8
#define TS 9

UninstalledDialog::UninstalledDialog(DanetDbMan *db, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UninstalledDialog),
    dbMan(db),
    proxyModel(nullptr),
    header(nullptr)
{
    ui->setupUi(this);

    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowMinMaxButtonsHint);

        proxyModel = new SortFilterProxyModel(this);
        header = new HeaderView(10,LETTER, ui->TV);
        loggedInUserId = dbMan->getLoggedinUserId();

        ui->TV->setHorizontalHeader(header);
        header->hideWidgets(ID);
        header->hideWidgets(USER_ID);

        header->hideComboBox(USER);
        header->setLableText(USER, "User");
        header->setLineEditPlaceHolder(USER, "Search");

        header->hideComboBox(AREA);
        header->setLableText(AREA, "Area");
        header->setLineEditPlaceHolder(AREA, "Search");

        header->hideComboBox(EXCHANGE);
        header->setLableText(EXCHANGE, "Exchange/Site");
        header->setLineEditPlaceHolder(EXCHANGE, "Search");

        header->hideComboBox(COMMODITY);
        header->setLableText(COMMODITY, "Commodity");
        header->setLineEditPlaceHolder(COMMODITY, "Search");

        header->hideComboBox(AMOUNT);
        header->setLableText(AMOUNT, "Amount");
        header->setLineEditPlaceHolder(AMOUNT, "Search");

        header->hideComboBox(LETTER);
        header->setLableText(LETTER, "Letter");
        header->setLineEditPlaceHolder(LETTER, "Search");

        header->hideComboBox(DESC);
        header->setLableText(DESC, "Description");
        header->setLineEditPlaceHolder(DESC, "Search");

        header->hideComboBox(TS);
        header->setLableText(TS, "Date");
        header->setLineEditPlaceHolder(TS, "Search");

        fillTV();
        connect(header, SIGNAL(sectionsWidgetsSignal()), this, SLOT(headerWidgetsChagedSlot()));
        ///////////////////// context
        ui->TV->setContextMenuPolicy(Qt::CustomContextMenu);
        connect(ui->TV, SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(contextMenuSlot(QPoint)));
        contextMenu.addAction(ui->actionEdit);
        contextMenu.addSeparator();
        contextMenu.addAction(ui->actionRemove);
}

UninstalledDialog::~UninstalledDialog()
{
    delete ui;
}

void UninstalledDialog::fillTV()
{
    QSqlQueryModel *model = new QSqlQueryModel(this);
    model->setQuery(*dbMan->selectUninstalled());
    proxyModel->setSourceModel(model);
    proxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    ui->TV->setModel(proxyModel);
    ui->TV->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->TV->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->TV->verticalHeader()->hide();
    ui->TV->hideColumn(ID);
    ui->TV->hideColumn(USER_ID);
}

void UninstalledDialog::contextMenuSlot(QPoint ptr)
{
    QModelIndex clickedIndex = ui->TV->indexAt(ptr);
    row = clickedIndex.row();

    clickedIndex = clickedIndex.sibling(clickedIndex.row(), ID);
    id = proxyModel->data(clickedIndex).toInt();
    clickedIndex = clickedIndex.sibling(clickedIndex.row(), USER_ID);
    user_id = proxyModel->data(clickedIndex).toInt();
    clickedIndex = clickedIndex.sibling(clickedIndex.row(), AREA);
    area = proxyModel->data(clickedIndex).toInt();
    clickedIndex = clickedIndex.sibling(clickedIndex.row(), EXCHANGE);
    exchange = proxyModel->data(clickedIndex).toString();
    clickedIndex = clickedIndex.sibling(clickedIndex.row(),COMMODITY);
    commodity = proxyModel->data(clickedIndex).toString();
    clickedIndex = clickedIndex.sibling(clickedIndex.row(), AMOUNT);
    amount = proxyModel->data(clickedIndex).toInt();
    clickedIndex = clickedIndex.sibling(clickedIndex.row(),LETTER);
    letter = proxyModel->data(clickedIndex).toString();
    clickedIndex = clickedIndex.sibling(clickedIndex.row(),DESC);
    desc = proxyModel->data(clickedIndex).toString();
    clickedIndex = clickedIndex.sibling(clickedIndex.row(),TS);
    ts = proxyModel->data(clickedIndex).toString();

    ptr.setY(ptr.y() + 80);
    if(user_id == loggedInUserId)
    {
        contextMenu.popup(ui->TV->mapToGlobal(ptr));
    }
}

void UninstalledDialog::headerWidgetsChagedSlot()
{
    QString User, Area, Exch, Commodity, Letter, Amount,Desc,Ts;
        User = header->sections[USER]->lineEdit->text();
        Area = header->sections[AREA]->lineEdit->text();
        Exch = header->sections[EXCHANGE]->lineEdit->text();
        Commodity = header->sections[COMMODITY]->lineEdit->text();
        Letter = header->sections[LETTER]->lineEdit->text();
        Desc = header->sections[DESC]->lineEdit->text();
        Ts = header->sections[TS]->lineEdit->text();

        QMap<int, QString> mapString;
        QMap<int, QRegExp> mapRegExp;
        QRegExp reg;
        reg.setCaseSensitivity(Qt::CaseInsensitive);

        if(!User.isEmpty())
        {
            reg.setPattern(User);
            mapRegExp.insert(USER, reg);
        }
        if(!Area.isEmpty())
        {
            reg.setPattern(Area);
            mapRegExp.insert(AREA, reg);
        }
        if(!Exch.isEmpty())
        {
            reg.setPattern(Exch);
            mapRegExp.insert(EXCHANGE, reg);
        }
        if(!Commodity.isEmpty())
        {
            reg.setPattern(Commodity);
            mapRegExp.insert(COMMODITY, reg);
        }
        if(!Letter.isEmpty())
        {
            reg.setPattern(Letter);
            mapRegExp.insert(LETTER, reg);
        }
        if(!Desc.isEmpty())
        {
            reg.setPattern(Desc);
            mapRegExp.insert(DESC, reg);
        }
        if(!Ts.isEmpty())
        {
            reg.setPattern(Ts);
            mapRegExp.insert(TS, reg);
        }

        proxyModel->setColumnsFilter(mapString, mapRegExp);
        ui->TV->setModel(proxyModel);
        ui->TV->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        ui->TV->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
        ui->TV->verticalHeader()->hide();
        ui->TV->hideColumn(ID);
        ui->TV->hideColumn(USER_ID);
}

void UninstalledDialog::on_addBtn_clicked()
{
    UninstalledAddEditDialog add(dbMan, this);
    add.exec();
    fillTV();
}

void UninstalledDialog::on_actionEdit_triggered()
{
    UninstalledAddEditDialog edit(dbMan, this);
    edit.setValues(id,area,exchange,commodity,amount,letter,desc);
    edit.exec();
    fillTV();
    ui->TV->selectRow(row);
}

void UninstalledDialog::on_actionRemove_triggered()
{
    UninstalledRemoveDialog rem(dbMan, id,area,exchange,commodity,amount,letter,desc,ts, this);
    rem.exec();
    fillTV();
}
