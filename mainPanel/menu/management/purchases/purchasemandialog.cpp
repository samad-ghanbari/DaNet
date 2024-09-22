#include "purchasemandialog.h"
#include "ui_purchasemandialog.h"
#include <QSqlQuery>
#include <QSqlQueryModel>
#include "lib/database/danetdbman.h"
#include "mainPanel/menu/management/purchases/addpurchasedialog.h"
#include "mainPanel/menu/management/purchases/editpurchasedialog.h"
#include "mainPanel/menu/management/purchases/removepurchasedialog.h"

PurchaseManDialog::PurchaseManDialog(QWidget *parent, DanetDbMan *db) :
    QDialog(parent),
    ui(new Ui::PurchaseManDialog),
    dbMan(db),
    model(new QSqlQueryModel(this))
{

    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowMinMaxButtonsHint);

    ui->purchaseTV->setModel(model);
    ui->purchaseTV->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->purchaseTV->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    ui->purchaseTV->verticalHeader()->hide();


    ui->purchaseTV->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->purchaseTV, SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(contextMenuSlot(QPoint)));

    contextMenu.addAction(ui->actionEdit_Purchase);
    contextMenu.addSeparator();
    contextMenu.addAction(ui->actionRemove_Purchase);

    fillTV();
    AdminUserFlag = false;
    ui->addPurchaseBtn->setEnabled(false);

}

PurchaseManDialog::~PurchaseManDialog()
{
    delete ui;
}

void PurchaseManDialog::fillTV()
{
    QSqlQuery *query = dbMan->selectPurchases();
    //`id`,`abbr`,`purchase`
    model->setQuery(*query);
    model->setHeaderData(1,Qt::Horizontal,"Abbreviation");
    model->setHeaderData(2,Qt::Horizontal,"Purchase");
    ui->purchaseTV->hideColumn(0);

}

void PurchaseManDialog::refreshSlot(int i)
{
    i++;
    fillTV();
    ui->purchaseTV->selectRow(row);
}

void PurchaseManDialog::contextMenuSlot(QPoint ptr)
{
    if(!AdminUserFlag) return;

    QModelIndex clickedIndex = ui->purchaseTV->indexAt(ptr);
    row = clickedIndex.row();
    clickedIndex = clickedIndex.sibling(clickedIndex.row(), 0); // id
    id = model->data(clickedIndex).toInt();
    clickedIndex = clickedIndex.sibling(clickedIndex.row(), 1); // abbr
    abbr = model->data(clickedIndex).toString();
    clickedIndex = clickedIndex.sibling(clickedIndex.row(), 2); // purchase
    purchase = model->data(clickedIndex).toString();

    if(id > 0)
        contextMenu.popup(ui->purchaseTV->mapToGlobal(ptr));
}

void PurchaseManDialog::on_addPurchaseBtn_clicked()
{
    if(!AdminUserFlag) return;

    AddPurchaseDialog *addPurchaseDialog = new AddPurchaseDialog(this, dbMan);
    addPurchaseDialog->setAttribute(Qt::WA_DeleteOnClose);
    connect(addPurchaseDialog, SIGNAL(finished(int)), this, SLOT(refreshSlot(int)));
    addPurchaseDialog->show();
}

void PurchaseManDialog::on_actionEdit_Purchase_triggered()
{
    if(!AdminUserFlag) return;

    EditPurchaseDialog *editPurchaseDialog = new EditPurchaseDialog(this, dbMan,id,abbr,purchase);
    editPurchaseDialog->setAttribute(Qt::WA_DeleteOnClose);
    connect(editPurchaseDialog, SIGNAL(finished(int)), this, SLOT(refreshSlot(int)));
    editPurchaseDialog->show();
}

void PurchaseManDialog::on_actionRemove_Purchase_triggered()
{
    if(!AdminUserFlag) return;

    RemovePurchaseDialog *removePurchaseDialog = new RemovePurchaseDialog(this, dbMan,id,abbr,purchase);
    removePurchaseDialog->setAttribute(Qt::WA_DeleteOnClose);
    connect(removePurchaseDialog, SIGNAL(finished(int)), this, SLOT(refreshSlot(int)));
    removePurchaseDialog->show();
}

void PurchaseManDialog::on_adminLE_textChanged(const QString &arg1)
{
    QString adminPass = dbMan->getUserHashedPassword("admin");
    QString hashedArg = dbMan->hashPassword(arg1);

    if(hashedArg.compare(adminPass,Qt::CaseSensitive) == 0)
    {
        AdminUserFlag = true;
        ui->addPurchaseBtn->setEnabled(true);
    }
    else
    {
        AdminUserFlag = false;
        ui->addPurchaseBtn->setEnabled(false);
    }
}
