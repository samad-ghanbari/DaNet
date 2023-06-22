#include "lettersdialog.h"
#include "ui_lettersdialog.h"
#include "lib/database/danetdbman.h"
#include <QSqlQueryModel>
#include "lib/sortfilterproxymodel.h"
#include "lib/headerview.h"
#include "mainPanel/menu/management/letters/letteraddeditdialog.h"
#include "mainPanel/menu/management/letters/letterremovedialog.h"
#include "mainPanel/menu/management/letters/letterviewdialog.h"

#define ID 0
#define USER_ID 1
#define LETTER_ID 2
#define LETTER_NO 3
#define FROM_DEP 4
#define TO_DEP 5
#define DESC 6
#define TS 7

LettersDialog::LettersDialog(DanetDbMan *db, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LettersDialog),
    dbMan(db),
    proxyModel(nullptr),
    header(nullptr)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowMinMaxButtonsHint);

    proxyModel = new SortFilterProxyModel(this);
    header = new HeaderView(8,LETTER_NO, ui->TV);
    ui->TV->setHorizontalHeader(header);
    header->hideWidgets(ID);
    header->hideWidgets(USER_ID);

    header->hideComboBox(LETTER_ID);
    header->setLableText(LETTER_ID, "Letter ID");
    header->setLineEditPlaceHolder(LETTER_ID, "Search");

    header->hideComboBox(LETTER_NO);
    header->setLableText(LETTER_NO, "Letter NO");
    header->setLineEditPlaceHolder(LETTER_NO, "Search");

    header->hideComboBox(FROM_DEP);
    header->setLableText(FROM_DEP, "From\nDepartment");
    header->setLineEditPlaceHolder(FROM_DEP, "Search");

    header->hideComboBox(TO_DEP);
    header->setLableText(TO_DEP, "To\nDepartment");
    header->setLineEditPlaceHolder(TO_DEP, "Search");

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
    connect(ui->TV, SIGNAL(doubleClicked(QModelIndex)),this,SLOT(doubleClickedSlot(QModelIndex)));
    contextMenu.addAction(ui->actionView);
    contextMenu.addSeparator();
    contextMenu.addAction(ui->actionEdit);
    contextMenu.addSeparator();
    contextMenu.addAction(ui->actionRemove);

}

LettersDialog::~LettersDialog()
{
    delete ui;
}

void LettersDialog::fillTV()
{
    QSqlQueryModel *model = new QSqlQueryModel(this);
    model->setQuery(*dbMan->selectLetters());
    proxyModel->setSourceModel(model);
    proxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    ui->TV->setModel(proxyModel);
    ui->TV->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->TV->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->TV->verticalHeader()->hide();
    ui->TV->hideColumn(ID);
    ui->TV->hideColumn(USER_ID);
}

void LettersDialog::contextMenuSlot(QPoint ptr)
{
    QModelIndex clickedIndex = ui->TV->indexAt(ptr);
    row = clickedIndex.row();

    clickedIndex = clickedIndex.sibling(clickedIndex.row(), ID);
    id = proxyModel->data(clickedIndex).toInt();
    clickedIndex = clickedIndex.sibling(clickedIndex.row(), LETTER_ID);
    letter_id = proxyModel->data(clickedIndex).toString();
    clickedIndex = clickedIndex.sibling(clickedIndex.row(), LETTER_NO);
    letter_no = proxyModel->data(clickedIndex).toString();
    clickedIndex = clickedIndex.sibling(clickedIndex.row(),FROM_DEP);
    from_dep = proxyModel->data(clickedIndex).toString();
    clickedIndex = clickedIndex.sibling(clickedIndex.row(), TO_DEP);
    to_dep = proxyModel->data(clickedIndex).toString();
    clickedIndex = clickedIndex.sibling(clickedIndex.row(),DESC);
    desc = proxyModel->data(clickedIndex).toString();
    clickedIndex = clickedIndex.sibling(clickedIndex.row(),TS);
    ts = proxyModel->data(clickedIndex).toString();

    ptr.setY(ptr.y() + 80);
    if(id > 0)
    {
        contextMenu.popup(ui->TV->mapToGlobal(ptr));
    }
}

void LettersDialog::doubleClickedSlot(QModelIndex ind)
{
    ind = ind.sibling(ind.row(), ID);
    id = proxyModel->data(ind).toInt();
    ind = ind.sibling(ind.row(), LETTER_ID);
    letter_id = proxyModel->data(ind).toString();
    ind = ind.sibling(ind.row(), LETTER_NO);
    letter_no = proxyModel->data(ind).toString();
    ind = ind.sibling(ind.row(), FROM_DEP);
    from_dep = proxyModel->data(ind).toString();
    ind = ind.sibling(ind.row(), TO_DEP);
    to_dep = proxyModel->data(ind).toString();
    ind = ind.sibling(ind.row(), DESC);
    desc = proxyModel->data(ind).toString();
    ind = ind.sibling(ind.row(), TS);
    ts = proxyModel->data(ind).toString();

    if(id > 0)
    {
        //usage
        LetterViewDialog vDialog(letter_id, letter_no, from_dep, to_dep, desc, ts, this);
        vDialog.exec();
    }
}

void LettersDialog::headerWidgetsChagedSlot()
{
    letter_id = header->sections[LETTER_ID]->lineEdit->text();
    letter_no = header->sections[LETTER_NO]->lineEdit->text();
    from_dep = header->sections[FROM_DEP]->lineEdit->text();
    to_dep = header->sections[TO_DEP]->lineEdit->text();
    desc = header->sections[DESC]->lineEdit->text();
    ts = header->sections[TS]->lineEdit->text();

    QMap<int, QString> mapString;
    QMap<int, QRegExp> mapRegExp;
    QRegExp reg;
    reg.setCaseSensitivity(Qt::CaseInsensitive);

    if(!letter_id.isEmpty())
    {
        reg.setPattern(letter_id);
        mapRegExp.insert(LETTER_ID, reg);
    }
    if(!letter_no.isEmpty())
    {
        reg.setPattern(letter_no);
        mapRegExp.insert(LETTER_NO, reg);
    }
    if(!from_dep.isEmpty())
    {
        reg.setPattern(from_dep);
        mapRegExp.insert(FROM_DEP, reg);
    }
    if(!to_dep.isEmpty())
    {
        reg.setPattern(to_dep);
        mapRegExp.insert(TO_DEP, reg);
    }
    if(!desc.isEmpty())
    {
        reg.setPattern(desc);
        mapRegExp.insert(DESC, reg);
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
    ui->TV->hideColumn(ID);
    ui->TV->hideColumn(USER_ID);
}

void LettersDialog::on_addBtn_clicked()
{
    LetterAddEditDialog add(dbMan, this);
    add.exec();
    fillTV();
}

void LettersDialog::on_actionView_triggered()
{
    LetterViewDialog view(letter_id, letter_no, from_dep, to_dep, desc, ts, this);
    view.exec();
}

void LettersDialog::on_actionEdit_triggered()
{
    LetterAddEditDialog edit(dbMan, this);
    edit.setValues(id,letter_id,letter_no,from_dep, to_dep,desc);
    edit.exec();
    fillTV();
    ui->TV->selectRow(row);
}

void LettersDialog::on_actionRemove_triggered()
{
    LetterRemoveDialog rem(dbMan, id, letter_id, letter_no, from_dep, to_dep, desc, ts, this);
    rem.exec();
    fillTV();
}
