#include "saloonmandialog.h"
#include "ui_saloonmandialog.h"
#include <QSqlQuery>
#include <QSqlQueryModel>
#include "lib/database/danetdbman.h"
#include "mainPanel/menu/management/saloon/addsaloondialog.h"
#include "mainPanel/menu/management/saloon/editsaloondialog.h"
#include "mainPanel/menu/management/saloon/removesaloondialog.h"

SaloonManDialog::SaloonManDialog(QWidget *parent, DanetDbMan *db) :
    QDialog(parent),
    ui(new Ui::SaloonManDialog),
    dbMan(db),
    model(new QSqlQueryModel(this))
{

    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowMinMaxButtonsHint);

    ui->saloonTV->setModel(model);
    ui->saloonTV->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->saloonTV->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    ui->saloonTV->verticalHeader()->hide();


    ui->saloonTV->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->saloonTV, SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(contextMenuSlot(QPoint)));

    contextMenu.addAction(ui->actionEdit_Saloon);
    contextMenu.addSeparator();
    contextMenu.addAction(ui->actionRemove_Saloon);

    fillTV();
    AdminUserFlag = false;
    ui->addSaloonBtn->setEnabled(false);

}

SaloonManDialog::~SaloonManDialog()
{
    delete ui;
}

void SaloonManDialog::fillTV()
{
    QSqlQuery *query = dbMan->selectSaloons();
    //`id`,`saloon`
    model->setQuery(*query);
    model->setHeaderData(1,Qt::Horizontal,"Saloon");
    ui->saloonTV->hideColumn(0);
}

void SaloonManDialog::refreshSlot(int done)
{
    done++;
    fillTV();
    ui->saloonTV->selectRow(row);
}

void SaloonManDialog::contextMenuSlot(QPoint ptr)
{
    if(!AdminUserFlag) return;

    QModelIndex clickedIndex = ui->saloonTV->indexAt(ptr);
    row = clickedIndex.row();
    clickedIndex = clickedIndex.sibling(clickedIndex.row(), 0); // id
    id = model->data(clickedIndex).toInt();
    clickedIndex = clickedIndex.sibling(clickedIndex.row(), 1); // sal
    saloon = model->data(clickedIndex).toString();


    if(id > 0)
        contextMenu.popup(ui->saloonTV->mapToGlobal(ptr));
}

void SaloonManDialog::on_addSaloonBtn_clicked()
{
    if(!AdminUserFlag) return;

    AddSaloonDialog *addSaloonDialog = new AddSaloonDialog(this, dbMan);
    addSaloonDialog->setAttribute(Qt::WA_DeleteOnClose);
    connect(addSaloonDialog, SIGNAL(finished(int)),this, SLOT(refreshSlot(int)));
    addSaloonDialog->show();
}

void SaloonManDialog::on_actionEdit_Saloon_triggered()
{
    if(!AdminUserFlag) return;

     EditSaloonDialog *editSaloonDialog = new EditSaloonDialog(this, dbMan,id,saloon);
     editSaloonDialog->setAttribute(Qt::WA_DeleteOnClose);
     connect(editSaloonDialog, SIGNAL(finished(int)),this, SLOT(refreshSlot(int)));
     editSaloonDialog->show();
}

void SaloonManDialog::on_actionRemove_Saloon_triggered()
{
    if(!AdminUserFlag) return;

    RemoveSaloonDialog *removeSaloonDialog = new RemoveSaloonDialog(this, dbMan,id,saloon);
    removeSaloonDialog->setAttribute(Qt::WA_DeleteOnClose);
    connect(removeSaloonDialog, SIGNAL(finished(int)),this, SLOT(refreshSlot(int)));
    removeSaloonDialog->show();
}

void SaloonManDialog::on_adminLE_textChanged(const QString &arg1)
{
    QString adminPass = dbMan->getUserHashedPassword("admin");
    QString hashedArg = dbMan->hashPassword(arg1);

    if(hashedArg.compare(adminPass,Qt::CaseSensitive) == 0)
    {
        AdminUserFlag = true;
        ui->addSaloonBtn->setEnabled(true);
    }
    else
    {
        AdminUserFlag = false;
        ui->addSaloonBtn->setEnabled(false);
    }
}

