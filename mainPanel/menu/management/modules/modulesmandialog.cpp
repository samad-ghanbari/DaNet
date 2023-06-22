#include "modulesmandialog.h"
#include "ui_modulesmandialog.h"
#include "lib/database/danetdbman.h"
#include "mainPanel/menu/management/modules/addmoduledialog.h"
#include "mainPanel/menu/management/modules/editmodulesdialog.h"
#include "mainPanel/menu/management/modules/removemoduledialog.h"
#include <QSqlQuery>
#include <QSqlQueryModel>

ModulesManDialog::ModulesManDialog(QWidget *parent, DanetDbMan *db) :
    QDialog(parent),
    ui(new Ui::ModulesManDialog),
    dbMan(db),
    model(new QSqlQueryModel(this))
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowMinMaxButtonsHint);

    ui->modulesTV->setModel(model);
    ui->modulesTV->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->modulesTV->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    ui->modulesTV->verticalHeader()->hide();


    ui->modulesTV->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->modulesTV, SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(contextMenuSlot(QPoint)));

    contextMenu.addAction(ui->actionEdit_Module);
    contextMenu.addSeparator();
    contextMenu.addAction(ui->actionRemove_Module);

    fillTV();
    AdminUserFlag = false;
    ui->addModuleBtn->setEnabled(false);

}

ModulesManDialog::~ModulesManDialog()
{
    delete ui;
}

void ModulesManDialog::fillTV()
{
    QSqlQuery *query = dbMan->selectModules();
    // `id`, `module`, `vendor`, `km`
    model->setQuery(*query);
    model->setHeaderData(1,Qt::Horizontal,"Module");
    model->setHeaderData(2,Qt::Horizontal,"Vendor");
    model->setHeaderData(3,Qt::Horizontal,"Km");
    ui->modulesTV->hideColumn(0);
}

void ModulesManDialog::refreshSlot(int i)
{
    i++;
    fillTV();
    ui->modulesTV->selectRow(row);
}

void ModulesManDialog::contextMenuSlot(QPoint ptr)
{
    if(!AdminUserFlag) return;

    QModelIndex clickedIndex = ui->modulesTV->indexAt(ptr);
    row = clickedIndex.row();
    clickedIndex = clickedIndex.sibling(clickedIndex.row(), 0); // id
    id = model->data(clickedIndex).toInt();
    clickedIndex = clickedIndex.sibling(clickedIndex.row(), 1); // module
    module = model->data(clickedIndex).toString();
    clickedIndex = clickedIndex.sibling(clickedIndex.row(), 2); // vendor
    vendor = model->data(clickedIndex).toString();
    clickedIndex = clickedIndex.sibling(clickedIndex.row(), 3); // km
    km = model->data(clickedIndex).toDouble();

    if( !module.isEmpty() )
        contextMenu.popup(ui->modulesTV->mapToGlobal(ptr));
}

void ModulesManDialog::on_addModuleBtn_clicked()
{
    if(!AdminUserFlag) return;

    AddModuleDialog *addModuleDialog = new AddModuleDialog(this, dbMan);
    addModuleDialog->setAttribute(Qt::WA_DeleteOnClose);
    connect(addModuleDialog, SIGNAL(finished(int)), this, SLOT(refreshSlot(int)));
    addModuleDialog->show();
}

void ModulesManDialog::on_actionEdit_Module_triggered()
{
    if(!AdminUserFlag) return;

    EditModulesDialog *editModulesDialog = new EditModulesDialog(this,dbMan,id,module, vendor, km);
    editModulesDialog->setAttribute(Qt::WA_DeleteOnClose);
    connect(editModulesDialog, SIGNAL(finished(int)), this, SLOT(refreshSlot(int)));
    editModulesDialog->show();
}

void ModulesManDialog::on_actionRemove_Module_triggered()
{
    if(!AdminUserFlag) return;

    RemoveModuleDialog *removeModuleDialog = new RemoveModuleDialog(this, dbMan, id, module, vendor, km);
    removeModuleDialog->setAttribute(Qt::WA_DeleteOnClose);
    connect(removeModuleDialog, SIGNAL(finished(int)), this, SLOT(refreshSlot(int)));
    removeModuleDialog->show();
}

void ModulesManDialog::on_adminLE_textChanged(const QString &arg1)
{
    QString adminPass = dbMan->getUserHashedPassword("admin");
    QString hashedArg = dbMan->hashPassword(arg1);

    if(hashedArg.compare(adminPass,Qt::CaseSensitive) == 0)
    {
        AdminUserFlag = true;
        ui->addModuleBtn->setEnabled(true);
    }
    else
    {
        AdminUserFlag = false;
        ui->addModuleBtn->setEnabled(false);
    }

}

