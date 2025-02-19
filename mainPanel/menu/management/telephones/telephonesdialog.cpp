#include "telephonesdialog.h"
#include "ui_telephonesdialog.h"
#include <QSqlQueryModel>
#include "lib/sortfilterproxymodel.h"
#include "lib/headerview.h"
#include "lib/database/danetdbman.h"
#include "mainPanel/menu/management/telephones/telephoneviewdialog.h"
#include "mainPanel/menu/management/telephones/telephoneaddeditdialog.h"
#include "mainPanel/menu/management/telephones/telephoneremovedialog.h"

#define ID 0
#define CONTACT_NAME 1
#define DEP 2
#define POS 3
#define PHONES 4

TelephonesDialog::TelephonesDialog(DanetDbMan *db, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TelephonesDialog),
    dbMan(db),
    proxyModel(nullptr),
    header(nullptr)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowMinMaxButtonsHint);

    proxyModel = new SortFilterProxyModel(this);
    header = new HeaderView(5,CONTACT_NAME, ui->TV);
    ui->TV->setHorizontalHeader(header);
    header->hideWidgets(ID);

    header->hideComboBox(CONTACT_NAME);
    header->setLableText(CONTACT_NAME, "Contact\nName");
    header->setLineEditPlaceHolder(CONTACT_NAME, "Search");

    header->hideComboBox(DEP);
    header->setLableText(DEP, "Department");
    header->setLineEditPlaceHolder(DEP, "Search");

    header->hideComboBox(POS);
    header->setLableText(POS, "Position");
    header->setLineEditPlaceHolder(POS, "Search");

    header->hideComboBox(PHONES);
    header->setLableText(PHONES, "Phones");
    header->setLineEditPlaceHolder(PHONES, "Search");

    fillTV();
    connect(header, SIGNAL(sectionsWidgetsSignal()), this, SLOT(headerWidgetsChagedSlot()));
    ///////////////////// context
    ///     QString dep = QString(DEPARTMENT);
    if((dep.compare("tarahi") == 0))
    {

    ui->TV->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->TV, SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(contextMenuSlot(QPoint)));
    connect(ui->TV, SIGNAL(doubleClicked(QModelIndex)),this,SLOT(doubleClickedSlot(QModelIndex)));
    contextMenu.addAction(ui->actionView);
    contextMenu.addSeparator();
    contextMenu.addAction(ui->actionEdit);
    contextMenu.addSeparator();
    contextMenu.addAction(ui->actionRemove);
    }
    else
        ui->addBtn->setEnabled(false);
}

TelephonesDialog::~TelephonesDialog()
{
    delete ui;
}

void TelephonesDialog::fillTV()
{
    QSqlQueryModel *model = new QSqlQueryModel(this);
    model->setQuery(*dbMan->selectTelephones());
    proxyModel->setSourceModel(model);
    proxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    ui->TV->setModel(proxyModel);
    ui->TV->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->TV->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->TV->verticalHeader()->hide();
    ui->TV->hideColumn(ID);
}

void TelephonesDialog::contextMenuSlot(QPoint ptr)
{
    QModelIndex clickedIndex = ui->TV->indexAt(ptr);
    row = clickedIndex.row();

    clickedIndex = clickedIndex.sibling(clickedIndex.row(), ID);
    id = proxyModel->data(clickedIndex).toInt();
    clickedIndex = clickedIndex.sibling(clickedIndex.row(), CONTACT_NAME);
    contactName = proxyModel->data(clickedIndex).toString();
    clickedIndex = clickedIndex.sibling(clickedIndex.row(), DEP);
    dep = proxyModel->data(clickedIndex).toString();
    clickedIndex = clickedIndex.sibling(clickedIndex.row(), POS);
    pos = proxyModel->data(clickedIndex).toString();
    clickedIndex = clickedIndex.sibling(clickedIndex.row(), PHONES);
    phones = proxyModel->data(clickedIndex).toString();

    ptr.setY(ptr.y() + 80);
    if(id > 0)
    {
        contextMenu.popup(ui->TV->mapToGlobal(ptr));
    }
}

void TelephonesDialog::doubleClickedSlot(QModelIndex ind)
{
    ind = ind.sibling(ind.row(), ID);
    id = proxyModel->data(ind).toInt();
    ind = ind.sibling(ind.row(), CONTACT_NAME);
    contactName = proxyModel->data(ind).toString();
    ind = ind.sibling(ind.row(), DEP);
    dep = proxyModel->data(ind).toString();
    ind = ind.sibling(ind.row(), POS);
    pos = proxyModel->data(ind).toString();
    ind = ind.sibling(ind.row(), PHONES);
    phones = proxyModel->data(ind).toString();

    if(id > 0)
    {
        //usage
        TelephoneViewDialog tDialog(contactName, dep,pos,phones, this);
        tDialog.exec();
    }
}

void TelephonesDialog::on_addBtn_clicked()
{
    TelephoneAddEditDialog add(dbMan,this);
    add.exec();
    fillTV();
}

void TelephonesDialog::on_actionEdit_triggered()
{
    TelephoneAddEditDialog edit(dbMan, this);
    edit.setValues(id,contactName, dep,pos,phones);
    edit.exec();
    fillTV();
    ui->TV->selectRow(row);
}

void TelephonesDialog::on_actionRemove_triggered()
{
    TelephoneRemoveDialog rem(dbMan, id, contactName,dep,pos,phones, this);
    rem.exec();
    fillTV();
}

void TelephonesDialog::headerWidgetsChagedSlot()
{
    contactName = header->sections[CONTACT_NAME]->lineEdit->text();
    dep = header->sections[DEP]->lineEdit->text();
    pos = header->sections[POS]->lineEdit->text();
    phones = header->sections[PHONES]->lineEdit->text();

    QMap<int, QString> mapString;
    QMap<int, QRegExp> mapRegExp;
    QRegExp reg;
    reg.setCaseSensitivity(Qt::CaseInsensitive);

    if(!contactName.isEmpty())
    {
        reg.setPattern(contactName);
        mapRegExp.insert(CONTACT_NAME, reg);
    }
    if(!dep.isEmpty())
    {
        reg.setPattern(dep);
        mapRegExp.insert(DEP, reg);
    }
    if(!pos.isEmpty())
    {
        reg.setPattern(pos);
        mapRegExp.insert(POS, reg);
    }
    if(!phones.isEmpty())
    {
        reg.setPattern(phones);
        mapRegExp.insert(PHONES, reg);
    }

    proxyModel->setColumnsFilter(mapString, mapRegExp);
    ui->TV->setModel(proxyModel);
    ui->TV->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->TV->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->TV->verticalHeader()->hide();
    ui->TV->hideColumn(ID);
}

void TelephonesDialog::on_actionView_triggered()
{
    TelephoneViewDialog view(contactName,dep,pos,phones, this);
    view.exec();
}
