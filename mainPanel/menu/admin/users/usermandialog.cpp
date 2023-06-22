#include "usermandialog.h"
#include "ui_usermandialog.h"
#include <lib/database/danetdbman.h>
#include <QSqlQueryModel>
#include <QModelIndex>
#include "mainPanel/menu/admin/users/adduserdialog.h"
#include "mainPanel/menu/admin/users/removeuserdialog.h"
#include "mainPanel/menu/admin/users/edituserdialog.h"

UserManDialog::UserManDialog(QWidget *parent, DanetDbMan *db) :
    QDialog(parent),
    ui(new Ui::UserManDialog),
    dbMan(db),
    userModel(nullptr)
{
    ui->setupUi(this);
    resize(parent->width()-50, parent->height()-50);
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint);

    fillUserTableView(dbMan->selectUsers());


    ui->usersTV->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->usersTV, SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(contextMenuSlot(QPoint)));

    ui->usersTV->verticalHeader()->hide();
    contextMenu.addAction(ui->actionEditUser);
    contextMenu.addSeparator();
    contextMenu.addAction(ui->actionRemove);

}

UserManDialog::~UserManDialog()
{
    delete ui;
}

void UserManDialog::fillUserTableView(QSqlQuery *query)
{
    userModel = new QSqlQueryModel(this);
    userModel->setQuery(*query);
    ui->usersTV->setModel(userModel);
    ui->usersTV->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->usersTV->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    ui->usersTV->horizontalHeader()->setSectionResizeMode(4, QHeaderView::Stretch);
    ui->usersTV->horizontalHeader()->setSectionResizeMode(5, QHeaderView::Stretch);
}

//////
void UserManDialog::contextMenuSlot(QPoint ptr)
{
    QModelIndex clickedIndex = ui->usersTV->indexAt(ptr);
    row = clickedIndex.row();

    clickedIndex = clickedIndex.sibling(clickedIndex.row(), 0); // id
    id = userModel->data(clickedIndex).toInt();
    clickedIndex = clickedIndex.sibling(clickedIndex.row(), 1); // name
    name = userModel->data(clickedIndex).toString();
    clickedIndex = clickedIndex.sibling(clickedIndex.row(), 2); // lastname
    lastname = userModel->data(clickedIndex).toString();
    clickedIndex = clickedIndex.sibling(clickedIndex.row(), 3); // area
    area = userModel->data(clickedIndex).toInt();
    clickedIndex = clickedIndex.sibling(clickedIndex.row(), 4); // username
    username = userModel->data(clickedIndex).toString();

    if(id > 0)
        contextMenu.popup(ui->usersTV->mapToGlobal(ptr));
}

void UserManDialog::on_addNewUserBtn_clicked()
{
    AddUserDialog addUserDialog(this, dbMan);
    addUserDialog.exec();
    fillUserTableView(dbMan->selectUsers());
}

void UserManDialog::on_actionRemove_triggered()
{
    RemoveUserDialog removeUserDialog(this, dbMan, id, name, lastname, area, username);
    removeUserDialog.exec();
    fillUserTableView(dbMan->selectUsers());
    ui->usersTV->selectRow(row);
}

void UserManDialog::on_actionEditUser_triggered()
{
    EditUserDialog editUserDialog(this, dbMan, id, name, lastname, area, username);
    editUserDialog.exec();
    fillUserTableView(dbMan->selectUsers());
    ui->usersTV->selectRow(row);
}
