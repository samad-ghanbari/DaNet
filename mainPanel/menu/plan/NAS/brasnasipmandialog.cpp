#include "brasnasipmandialog.h"
#include "ui_brasnasipmandialog.h"
#include "lib/database/danetdbman.h"
#include <QSqlQuery>
#include <QSqlQueryModel>
#include "mainPanel/menu/plan/NAS/addnasipdialog.h"
#include "mainPanel/menu/plan/NAS/editnasipdialog.h"
#include "mainPanel/menu/plan/NAS/removenasipdialog.h"

BrasNasIpManDialog::BrasNasIpManDialog(QWidget *parent, DanetDbMan *db) :
    QDialog(parent),
    ui(new Ui::BrasNasIpManDialog),
    dbMan(db),
    model(new QSqlQueryModel(this))
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowMinMaxButtonsHint);

    ui->nasTV->setModel(model);
    ui->nasTV->verticalHeader()->hide();
    ui->nasTV->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    // context menu
    ui->nasTV->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->nasTV, SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(contextMenuSlot(QPoint)));
    contextMenu.addAction(ui->actionEdit_NAS_IP);
    contextMenu.addAction(ui->actionRemove_NAS_IP);

    fillTV();
}

BrasNasIpManDialog::~BrasNasIpManDialog()
{
    delete ui;
}

void BrasNasIpManDialog::fillTV()
{
    QSqlQuery *query = dbMan->selectViewNasIp();
    //`id`, `bras_id`, `area`, `abbr`, `device_name`, `nas_ip`
    model->setQuery(*query);
    ui->nasTV->hideColumn(0);
    ui->nasTV->hideColumn(1);

    model->setHeaderData(2, Qt::Horizontal,"Area");
    model->setHeaderData(3, Qt::Horizontal,"Exchange Abbr");
    model->setHeaderData(4, Qt::Horizontal,"BRAS");
    model->setHeaderData(5, Qt::Horizontal,"NAS IP");

}

void BrasNasIpManDialog::refreshSlot(int i)
{
    i++;
    fillTV();
    ui->nasTV->selectRow(row);
}

void BrasNasIpManDialog::contextMenuSlot(QPoint ptr)
{
    QModelIndex clickedIndex = ui->nasTV->indexAt(ptr);
    row = clickedIndex.row();
    //`id`, `bras_id`, `area`, `abbr`, `device_name`, `nas_ip`


    clickedIndex = clickedIndex.sibling(clickedIndex.row(), 0); // id
    clickedId = model->data(clickedIndex).toInt();
    if(clickedId > 0)
    {
        clickedIndex = clickedIndex.sibling(clickedIndex.row(), 1);
        clickedDevexId = model->data(clickedIndex).toInt();

        clickedIndex = clickedIndex.sibling(clickedIndex.row(), 2); // area
        clickedArea = model->data(clickedIndex).toInt();

        clickedIndex = clickedIndex.sibling(clickedIndex.row(), 4); // bras name
        clickedBRAS = model->data(clickedIndex).toString();

        clickedIndex = clickedIndex.sibling(clickedIndex.row(), 5); // nas
        clickedNas = model->data(clickedIndex).toString();


        contextMenu.popup(ui->nasTV->mapToGlobal(ptr));
    }
}

void BrasNasIpManDialog::on_actionEdit_NAS_IP_triggered()
{
    EditNasIpDialog *editNasIpDialog = new EditNasIpDialog(this, dbMan,clickedId, clickedDevexId,clickedArea, clickedNas);
    editNasIpDialog->setAttribute(Qt::WA_DeleteOnClose);
    connect(editNasIpDialog, SIGNAL(finished(int)), this, SLOT(refreshSlot(int)));
    editNasIpDialog->show();
}

void BrasNasIpManDialog::on_actionRemove_NAS_IP_triggered()
{
    RemoveNasIpDialog *removeNasIpDialog = new RemoveNasIpDialog(this,dbMan,clickedId, clickedArea,clickedBRAS, clickedNas);
    removeNasIpDialog->setAttribute(Qt::WA_DeleteOnClose);
    connect(removeNasIpDialog, SIGNAL(finished(int)), this, SLOT(refreshSlot(int)));
    removeNasIpDialog->show();
}

void BrasNasIpManDialog::on_addBtn_clicked()
{
    AddNasIpDialog *addNasIpDialog = new AddNasIpDialog(this, dbMan);
    addNasIpDialog->setAttribute(Qt::WA_DeleteOnClose);
    connect(addNasIpDialog, SIGNAL(finished(int)), this, SLOT(refreshSlot(int)));
    addNasIpDialog->show();
}





