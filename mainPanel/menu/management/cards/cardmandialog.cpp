#include "cardmandialog.h"
#include "ui_cardmandialog.h"
#include "lib/database/danetdbman.h"
#include <QSqlQueryModel>
#include <QSqlQuery>
#include "mainPanel/menu/management/cards/addcarddialog.h"
#include "mainPanel/menu/management/cards/editcarddialog.h"
#include "mainPanel/menu/management/cards/removecarddialog.h"

CardManDialog::CardManDialog(QWidget *parent, DanetDbMan *db) :
    QDialog(parent),
    ui(new Ui::CardManDialog),
    dbMan(db),
    model(new QSqlQueryModel(this))
{
    ui->setupUi(this);
    resize(parent->width()-50, parent->height()-50);
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowMinMaxButtonsHint);

    ui->cardTV->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->cardTV, SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(contextMenuSlot(QPoint)));

    contextMenu.addAction(ui->actionAdminEditCard);
    contextMenu.addSeparator();
    contextMenu.addAction(ui->actionAdminRemoveCard);

     ui->cardTV->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
     ui->cardTV->verticalHeader()->hide();
     ui->cardTV->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);


    fillCardTV();
    AdminUserFlag = false;
    ui->adminAddCardBtn->setEnabled(false);
}

CardManDialog::~CardManDialog()
{
    delete ui;
}

void CardManDialog::fillCardTV()
{
    QSqlQuery *query = dbMan->selectCards();
    //`id`, `card`, `device`, `type`, `adsl`, `vdsl`, `shdsl`, `e`, `G`, `G10`, `G40`, `G100`
    model->setQuery(*query);
    model->setHeaderData(1,Qt::Horizontal,"Card Name");
    model->setHeaderData(2,Qt::Horizontal,"Device");
    model->setHeaderData(3,Qt::Horizontal,"Card Type");
    model->setHeaderData(4,Qt::Horizontal,"ADSL\nPort Count");
    model->setHeaderData(5,Qt::Horizontal,"VDSL\nPort Count");
    model->setHeaderData(6,Qt::Horizontal,"SHDSL\nPort Count");
    model->setHeaderData(7,Qt::Horizontal,"E\nPort Count");
    model->setHeaderData(8,Qt::Horizontal,"1G\nPort Count");
    model->setHeaderData(9,Qt::Horizontal,"10G\nPort Count");
    model->setHeaderData(10,Qt::Horizontal,"40G\nPort Count");
    model->setHeaderData(11,Qt::Horizontal,"100G\nPort Count");
    ui->cardTV->setModel(model);
    ui->cardTV->hideColumn(0);
}

void CardManDialog::fillCardTV(const QString Dev)
{
    QSqlQuery *query = dbMan->selectCards(Dev);
    //`id`, `card`, `device`, `type`, `adsl`, `vdsl`, `shdsl`, `e`, `G`, `G10`, `G40`, `G100`
    model->setQuery(*query);
    model->setHeaderData(1,Qt::Horizontal,"Card Name");
    model->setHeaderData(2,Qt::Horizontal,"Device");
    model->setHeaderData(3,Qt::Horizontal,"Card Type");
    model->setHeaderData(4,Qt::Horizontal,"ADSL\nPort Count");
    model->setHeaderData(5,Qt::Horizontal,"VDSL\nPort Count");
    model->setHeaderData(6,Qt::Horizontal,"SHDSL\nPort Count");
    model->setHeaderData(7,Qt::Horizontal,"E\nPort Count");
    model->setHeaderData(8,Qt::Horizontal,"1G\nPort Count");
    model->setHeaderData(9,Qt::Horizontal,"10G\nPort Count");
    model->setHeaderData(10,Qt::Horizontal,"40G\nPort Count");
    model->setHeaderData(11,Qt::Horizontal,"100G\nPort Count");
    ui->cardTV->setModel(model);
    ui->cardTV->hideColumn(0);
}

void CardManDialog::refreshSlot(int i)
{
    i++;
    if(ui->filterLE->text().isEmpty())
        fillCardTV();
    else
        fillCardTV(ui->filterLE->text().trimmed());
    ui->cardTV->selectRow(row);
}

void CardManDialog::contextMenuSlot(QPoint ptr)
{
    if(!AdminUserFlag) return;

    QModelIndex clickedIndex = ui->cardTV->indexAt(ptr);
    row = clickedIndex.row();
    clickedIndex = clickedIndex.sibling(clickedIndex.row(), 0); // id
    id = model->data(clickedIndex).toInt();
    clickedIndex = clickedIndex.sibling(clickedIndex.row(), 1); // card
    card = model->data(clickedIndex).toString();
    clickedIndex = clickedIndex.sibling(clickedIndex.row(), 2); // device
    device = model->data(clickedIndex).toString();
    clickedIndex = clickedIndex.sibling(clickedIndex.row(), 3); // type
    type = model->data(clickedIndex).toString();
    clickedIndex = clickedIndex.sibling(clickedIndex.row(), 4); // adsl
    adsl = model->data(clickedIndex).toInt();
    clickedIndex = clickedIndex.sibling(clickedIndex.row(), 5); // vdsl
    vdsl = model->data(clickedIndex).toInt();
    clickedIndex = clickedIndex.sibling(clickedIndex.row(), 6); // shdsl
    shdsl = model->data(clickedIndex).toInt();
    clickedIndex = clickedIndex.sibling(clickedIndex.row(), 7); // e
    e = model->data(clickedIndex).toInt();
    clickedIndex = clickedIndex.sibling(clickedIndex.row(), 8); // G
    G = model->data(clickedIndex).toInt();
    clickedIndex = clickedIndex.sibling(clickedIndex.row(), 9); // G10
    G10 = model->data(clickedIndex).toInt();
    clickedIndex = clickedIndex.sibling(clickedIndex.row(), 10); // G40
    G40 = model->data(clickedIndex).toInt();
    clickedIndex = clickedIndex.sibling(clickedIndex.row(), 11); // G100
    G100 = model->data(clickedIndex).toInt();

    contextMenu.popup(ui->cardTV->mapToGlobal(ptr));
}

void CardManDialog::on_adminAddCardBtn_clicked()
{
    if(!AdminUserFlag) return;

    AddCardDialog *addCardDialog = new AddCardDialog(this, dbMan);
    addCardDialog->setAttribute(Qt::WA_DeleteOnClose);
    connect(addCardDialog,SIGNAL(finished(int)), this, SLOT(refreshSlot(int)));
    addCardDialog->show();
}

void CardManDialog::on_actionAdminEditCard_triggered()
{
    if(!AdminUserFlag) return;

    EditCardDialog *editCardDialog = new EditCardDialog(this, dbMan,  id, card, device, type, adsl,  vdsl,  shdsl,  e,  G,  G10,  G40, G100);
    editCardDialog->setAttribute(Qt::WA_DeleteOnClose);
    connect(editCardDialog,SIGNAL(finished(int)), this, SLOT(refreshSlot(int)));
    editCardDialog->show();
}

void CardManDialog::on_actionAdminRemoveCard_triggered()
{
    if(!AdminUserFlag) return;

    RemoveCardDialog *removeCardDialog = new RemoveCardDialog(this, dbMan, id, card, device, type);
    removeCardDialog->setAttribute(Qt::WA_DeleteOnClose);
    connect(removeCardDialog,SIGNAL(finished(int)), this, SLOT(refreshSlot(int)));
    removeCardDialog->show();
}

void CardManDialog::on_filterLE_textChanged(const QString &arg1)
{
    if(arg1.trimmed().isEmpty())
        fillCardTV();
    else
        fillCardTV(arg1);
}

void CardManDialog::on_adminLE_textChanged(const QString &arg1)
{
    QString adminPass = dbMan->getUserHashedPassword("admin");
    QString hashedArg = dbMan->hashPassword(arg1);

    if(hashedArg.compare(adminPass,Qt::CaseSensitive) == 0)
    {
        AdminUserFlag = true;
        ui->adminAddCardBtn->setEnabled(true);
    }
    else
    {
        AdminUserFlag = false;
        ui->adminAddCardBtn->setEnabled(false);
    }

}
