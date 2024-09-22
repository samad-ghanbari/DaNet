#include "cat_dialog.h"
#include "ui_cat_dialog.h"
#include "lib/database/danetdbman.h"
#include "lib/centeralignedmodel.h"
#include "mainPanel/menu/management/commodities/cat/cat_addedit.h"
#include "mainPanel/menu/management/commodities/cat/cat_remove.h"
#include "mainPanel/menu/management/commodities/commodity/comm_dialog.h"

namespace Commodity
{
namespace Cat {

Dialog::Dialog(DanetDbMan *db, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog),
    dbMan(db),
    model(nullptr)
{
    ui->setupUi(this);
    QString username = dbMan->getLoggedinUserame();
    if(username.compare(ADMIN, Qt::CaseInsensitive) != 0)
    {
        adminUser = false;
        ui->addBtn->setEnabled(false);
        ui->addBtn->setVisible(false);
    }
    else
        adminUser = true;


    model = new CenterAlignedModel(this);
    fillTV();

    ///////////////////// context
    ui->TV->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->TV, SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(contextMenuSlot(QPoint)));
    connect(ui->TV, SIGNAL(doubleClicked(QModelIndex)),this,SLOT(doubleClickedSlot(QModelIndex)));
    contextMenu.addAction(ui->actionEdit_Category);
    contextMenu.addSeparator();
    contextMenu.addAction(ui->actionRemove_Category);

}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::fillTV()
{
    model->setQuery(*dbMan->selectCommodityCat());
    ui->TV->setModel(model);
    ui->TV->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->TV->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->TV->horizontalHeader()->hide();
    ui->TV->verticalHeader()->hide();
    ui->TV->hideColumn(0);
}

void Dialog::contextMenuSlot(QPoint ptr)
{
    if(!adminUser) return;

    QModelIndex clickedIndex = ui->TV->indexAt(ptr);
    row = clickedIndex.row();

    clickedIndex = clickedIndex.sibling(clickedIndex.row(), 0); // id
    id = model->data(clickedIndex).toInt();
    clickedIndex = clickedIndex.sibling(clickedIndex.row(), 1); // cat
    cat = model->data(clickedIndex).toString();

    if(id > 0)
    {
        contextMenu.popup(ui->TV->mapToGlobal(ptr));
    }
}

void Dialog::doubleClickedSlot(QModelIndex ind)
{
    ind = ind.sibling(ind.row(), 0);
    id = model->data(ind).toInt();
    ind = ind.sibling(ind.row(), 1);
    cat = model->data(ind).toString();

    if(id > 0)
    {
        Commodity::Dialog *dialog = new Commodity::Dialog(dbMan,id , cat, this);
        dialog->showMaximized();
    }
}

void Dialog::on_addBtn_clicked()
{
    Commodity::Cat::AddEdit add(dbMan,this);
    add.exec();
    fillTV();
}

void Dialog::on_actionEdit_Category_triggered()
{
    Commodity::Cat::AddEdit edit(dbMan,this);
    edit.setValues(id, cat);
    edit.exec();
    fillTV();
}

void Dialog::on_actionRemove_Category_triggered()
{
    Commodity::Cat::Remove  remove(dbMan,id, cat, this);
    remove.exec();
    fillTV();
}
}
}


