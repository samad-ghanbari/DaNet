#include "interfacetypedialog.h"
#include "ui_interfacetypedialog.h"
#include "lib/database/danetdbman.h"
#include "mainPanel/menu/management/interfaceType/addinterfacetypedialog.h"
#include "mainPanel/menu/management/interfaceType/editinterfacetypedialog.h"
#include "mainPanel/menu/management/interfaceType/removeinterfacetypedialog.h"
#include <QSqlQuery>
#include <QSqlQueryModel>

InterfaceTypeDialog::InterfaceTypeDialog(QWidget *parent, DanetDbMan *db) :
    QDialog(parent),
    ui(new Ui::InterfaceTypeDialog),
    dbMan(db),
    model(new QSqlQueryModel(this))
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowMinMaxButtonsHint);

    ui->intTypeTV->setModel(model);
    ui->intTypeTV->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->intTypeTV->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    ui->intTypeTV->verticalHeader()->hide();


    ui->intTypeTV->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->intTypeTV, SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(contextMenuSlot(QPoint)));

    contextMenu.addAction(ui->actionEdit_IntType);
    contextMenu.addSeparator();
    contextMenu.addAction(ui->actionRemove_IntType);

    fillTV();
    AdminUserFlag = false;
    ui->addIntTypeBtn->setEnabled(false);

}

InterfaceTypeDialog::~InterfaceTypeDialog()
{
    delete ui;
}

void InterfaceTypeDialog::fillTV()
{
    QSqlQuery *query = dbMan->selectInterfaceTypes();
    // `id`, `int type`, `speed`, `media`
    model->setQuery(*query);
    model->setHeaderData(1,Qt::Horizontal,"Interface\nType");
    model->setHeaderData(2,Qt::Horizontal,"Speed (Gbps)");
    model->setHeaderData(3,Qt::Horizontal,"Media");
    ui->intTypeTV->hideColumn(0);
}

void InterfaceTypeDialog::refreshSlot(int done)
{
    done++;
    fillTV();
    ui->intTypeTV->selectRow(row);
}

void InterfaceTypeDialog::contextMenuSlot(QPoint ptr)
{
    if(!AdminUserFlag) return;

    QModelIndex clickedIndex = ui->intTypeTV->indexAt(ptr);
    row = clickedIndex.row();
    clickedIndex = clickedIndex.sibling(clickedIndex.row(), 0); // id
    id = model->data(clickedIndex).toInt();
    clickedIndex = clickedIndex.sibling(clickedIndex.row(), 1); // int type
    intType = model->data(clickedIndex).toString();
    clickedIndex = clickedIndex.sibling(clickedIndex.row(), 2); // speed
    speed = model->data(clickedIndex).toDouble();
    clickedIndex = clickedIndex.sibling(clickedIndex.row(), 3); // media
    media = model->data(clickedIndex).toString();

    if( !intType.isEmpty() )
        contextMenu.popup(ui->intTypeTV->mapToGlobal(ptr));
}

void InterfaceTypeDialog::on_adminLE_textChanged(const QString &arg1)
{
    QString adminPass = dbMan->getUserHashedPassword("admin");
    QString hashedArg = dbMan->hashPassword(arg1);

    if(hashedArg.compare(adminPass,Qt::CaseSensitive) == 0)
    {
        AdminUserFlag = true;
        ui->addIntTypeBtn->setEnabled(true);
    }
    else
    {
        AdminUserFlag = false;
        ui->addIntTypeBtn->setEnabled(false);
    }

}

void InterfaceTypeDialog::on_actionEdit_IntType_triggered()
{
    if(!AdminUserFlag) return;

    EditInterfaceTypeDialog *editInterfaceTypeDialog = new EditInterfaceTypeDialog(this,dbMan,id,intType, speed, media);
    editInterfaceTypeDialog->setAttribute(Qt::WA_DeleteOnClose);
    connect(editInterfaceTypeDialog, SIGNAL(finished(int)),this,SLOT(refreshSlot(int)));
    editInterfaceTypeDialog->show();
}

void InterfaceTypeDialog::on_actionRemove_IntType_triggered()
{
    if(!AdminUserFlag) return;

    RemoveInterfaceTypeDialog *removeInterfaceTypeDialog = new RemoveInterfaceTypeDialog(this, dbMan, id, intType, speed, media);
    removeInterfaceTypeDialog->setAttribute(Qt::WA_DeleteOnClose);
    connect(removeInterfaceTypeDialog, SIGNAL(finished(int)),this,SLOT(refreshSlot(int)));
    removeInterfaceTypeDialog->show();
}

void InterfaceTypeDialog::on_addIntTypeBtn_clicked()
{
    if(!AdminUserFlag) return;

    AddInterfaceTypeDialog *addInterfaceTypeDialog = new AddInterfaceTypeDialog(this, dbMan);
    addInterfaceTypeDialog->setAttribute(Qt::WA_DeleteOnClose);
    connect(addInterfaceTypeDialog, SIGNAL(finished(int)),this,SLOT(refreshSlot(int)));
    addInterfaceTypeDialog->show();
}
