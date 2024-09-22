#include "usage_dialog.h"
#include "ui_usage_dialog.h"
#include "lib/database/danetdbman.h"
#include <QSqlQueryModel>
#include "mainPanel/menu/management/commodities/usage/usage_addedit.h"
#include "mainPanel/menu/management/commodities/usage/usage_remove.h"
#include <QSqlQueryModel>
#include "lib/sortfilterproxymodel.h"
#include "lib/headerview.h"

namespace Commodity {

namespace Usage {

#define UsageId 0
#define CommId 1
#define LetterId 2
#define Amount 3
#define Desc 4
#define TS 5
#define User_Id 6
#define User 7

Dialog::Dialog(DanetDbMan *db,const int COMM_ID, const QString COMM, const int TOTAL, const int LEFT, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog),
    dbMan(db),
    proxyModel(nullptr),
    header(nullptr),
    comm_id(COMM_ID)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowMinMaxButtonsHint);

    commodity = COMM;

    loggedIdUserId = dbMan->getLoggedinUserId();

    ui->commLbl->setText(COMM);
    ui->totalLbl->setText(QString::number(TOTAL));
    ui->leftLbl->setText(QString::number(LEFT));
    total = TOTAL;
    left = LEFT;

    proxyModel = new SortFilterProxyModel(this);
    header = new HeaderView(8,LetterId, ui->TV);
    ui->TV->setHorizontalHeader(header);
    header->hideWidgets(User_Id);
    header->hideWidgets(CommId);
    header->hideWidgets(UsageId);

    header->hideComboBox(LetterId);
    header->setLableText(LetterId, "Letter ID");
    header->setLineEditPlaceHolder(LetterId, "Search");

    header->hideComboBox(Amount);
    header->setLableText(Amount, "Amount");
    header->hideLineEdit(Amount);

    header->hideComboBox(Desc);
    header->setLableText(Desc, "Description");
    header->setLineEditPlaceHolder(Desc, "Search");

    header->hideComboBox(TS);
    header->setLableText(TS, "Date");
    header->setLineEditPlaceHolder(TS, "Search");

    header->hideComboBox(User);
    header->setLableText(User, "User");
    header->setLineEditPlaceHolder(User, "Search");

    fillTV();

    connect(header, SIGNAL(sectionsWidgetsSignal()), this, SLOT(headerWidgetsChagedSlot()));

    ///////////////////// context
    ui->TV->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->TV, SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(contextMenuSlot(QPoint)));
    contextMenu.addAction(ui->actionEdit_Commodity_Usage);
    contextMenu.addSeparator();
    contextMenu.addAction(ui->actionRemove_Commodity_Usage);

}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_actionEdit_Commodity_Usage_triggered()
{
    Commodity::Usage::Addedit edit(comm_id,left, dbMan,this);
    edit.setValues(usage_id, letter_id,left, amount,desc);
    edit.setLables("Commodity Usage Modification", commodity);
    edit.exec();
    fillTV();
    emit changeSignal();
}

void Dialog::on_actionRemove_Commodity_Usage_triggered()
{
    Commodity::Usage::Remove  remove(usage_id, dbMan,this);
    remove.setLables("Remove Commodity Usage", commodity);
    remove.setValues(letter_id,amount,desc);
    remove.exec();
    fillTV();
    emit changeSignal();
}

void Dialog::on_okBtn_clicked()
{
    Commodity::Usage::Addedit add(comm_id,left, dbMan,this);
    add.setLables("New Commodity Usage", commodity);
    add.exec();
    fillTV();
    emit changeSignal();
}

void Dialog::fillTV()
{
    QSqlQueryModel *model = new QSqlQueryModel(this);
    model->setQuery(*dbMan->selectViewCommodityUsage(comm_id));
    proxyModel->setSourceModel(model);
    proxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);

    //0`id`, 1`comm_id`, 2`letter_id`, 3`amount`, 4`description` 5ts 6userid 7user
    ui->TV->setModel(proxyModel);
    ui->TV->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->TV->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->TV->verticalHeader()->hide();
    ui->TV->hideColumn(UsageId);
    ui->TV->hideColumn(CommId);
    ui->TV->hideColumn(User_Id);

    changeSlot();
}

void Dialog::contextMenuSlot(QPoint ptr)
{

    QModelIndex clickedIndex = ui->TV->indexAt(ptr);
    row = clickedIndex.row();

    clickedIndex = clickedIndex.sibling(clickedIndex.row(), UsageId);
    usage_id = proxyModel->data(clickedIndex).toInt();
    clickedIndex = clickedIndex.sibling(clickedIndex.row(), LetterId);
    letter_id = proxyModel->data(clickedIndex).toString();
    clickedIndex = clickedIndex.sibling(clickedIndex.row(), Amount);
    amount = proxyModel->data(clickedIndex).toInt();
    clickedIndex = clickedIndex.sibling(clickedIndex.row(), Desc);
    desc = proxyModel->data(clickedIndex).toString();
    clickedIndex = clickedIndex.sibling(clickedIndex.row(), User_Id);
    user_id = proxyModel->data(clickedIndex).toInt();
    clickedIndex = clickedIndex.sibling(clickedIndex.row(), User);
    user = proxyModel->data(clickedIndex).toString();

    ptr.setY(ptr.y() + 80);
    if( (usage_id > 0) && (loggedIdUserId == user_id) )
    {
        contextMenu.popup(ui->TV->mapToGlobal(ptr));
    }
}

void Dialog::changeSlot()
{
    QList<int> totalLeft = dbMan->getCommodityAmounts(comm_id);
    total = totalLeft[0];
    left = totalLeft[1];
    ui->totalLbl->setText(QString::number(total));
    ui->leftLbl->setText(QString::number(left));

    if(left == 0)
    {
        ui->okBtn->setEnabled(false);
        ui->okBtn->setVisible(false);
    }
    else
    {
        ui->okBtn->setEnabled(true);
        ui->okBtn->setVisible(true);
    }
}

void Dialog::headerWidgetsChagedSlot()
{
    letter_id = header->sections[LetterId]->lineEdit->text();
    desc = header->sections[Desc]->lineEdit->text();
    ts = header->sections[TS]->lineEdit->text();
    user = header->sections[User]->lineEdit->text();

    QMap<int, QString> mapString;
    QMap<int, QRegExp> mapRegExp;
    QRegExp reg;
    reg.setCaseSensitivity(Qt::CaseInsensitive);

    if(!letter_id.isEmpty())
    {
        reg.setPattern(letter_id);
        mapRegExp.insert(LetterId, reg);
    }
    if(!desc.isEmpty())
    {
        reg.setPattern(desc);
        mapRegExp.insert(Desc, reg);
    }
    if(!ts.isEmpty())
    {
        reg.setPattern(ts);
        mapRegExp.insert(TS, reg);
    }
    if(!user.isEmpty())
    {
        reg.setPattern(user);
        mapRegExp.insert(User, reg);
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
    ui->TV->hideColumn(UsageId);
    ui->TV->hideColumn(CommId);
    ui->TV->hideColumn(User_Id);
}

} // namespace Usage
}
