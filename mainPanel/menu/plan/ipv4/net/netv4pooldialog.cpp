#include "netv4pooldialog.h"
#include "ui_netv4pooldialog.h"
#include "lib/headerview.h"
#include "lib/database/danetdbman.h"
#include <QSqlQuery>
#include <QSqlQueryModel>
#include "lib/sortfilterproxymodel.h"
#include "mainPanel/menu/plan/ipv4/net/addnetv4dialog.h"
#include "mainPanel/menu/plan/ipv4/net/editnetv4dialog.h"
#include "mainPanel/menu/plan/ipv4/net/removenetv4dialog.h"
#include "mainPanel/menu/plan/ipv4/net/subnetv4searchdialog.h"
#include "mainPanel/menu/plan/ipv4/subnet1/subnet1v4pooldialog.h"
#include "mainPanel/menu/plan/ipv4/net/netv4assignmentdialog.h"
#include "lib/report/ipv4plan/net/ReportNetworkIpV4Plan.h"
#include <QFileDialog>
#include <QDesktopServices>
#include <QMessageBox>

#include <QBrush>

//`id`, `net`, `prefix`, `mask`, `bc`, `hosts`,`description`

#define ID 0
#define NET 1
#define PREFIX 2
#define MASK 3
#define BC 4
#define HOST 5
#define DESC 6


NetV4PoolDialog::NetV4PoolDialog(QWidget *parent, DanetDbMan *db) :
    QDialog(parent),
    ui(new Ui::NetV4PoolDialog),
    dbMan(db),
    proxyModel(nullptr),
    header(nullptr)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowMinMaxButtonsHint);

    proxyModel = new SortFilterProxyModel(this);
    header = new HeaderView(7,MASK, ui->ipTV);
    ui->ipTV->setHorizontalHeader(header);
    ui->ipTV->setModel(proxyModel);

    //    header->blockSignals(true);

    header->hideWidgets(ID);

    header->hideComboBox(NET);
    header->setLableText(NET, "Network");
    header->setLineEditPlaceHolder(NET, "Search");

    header->hideComboBox(PREFIX);
    header->setLableText(PREFIX, "Prefix");
    header->setLineEditPlaceHolder(PREFIX, "Search");

    header->hideComboBox(MASK);
    header->setLableText(MASK, "Mask");
    header->setLineEditPlaceHolder(MASK, "Search");

    header->hideComboBox(BC);
    header->setLableText(BC, "Broadcast");
    header->setLineEditPlaceHolder(BC, "Search");

    header->hideComboBox(HOST);
    header->setLableText(HOST, "Host No");
    header->setLineEditPlaceHolder(HOST, "Search");

    //    header->hideWidgets(DEPARTMENT_NO);

    //    header->hideLineEdit(DEPARTMENT);
    //    header->setLableText(DEPARTMENT,"Department");
    //    header->addComboBoxItem(DEPARTMENT,"Data", 1);
    //    header->addComboBoxItem(DEPARTMENT,"Switch", 2);
    //    header->addComboBoxItem(DEPARTMENT,"Access", 3);
    //    header->addComboBoxItem(DEPARTMENT,"Transmission", 4);
    //    header->addComboBoxItem(DEPARTMENT,"All",0);

    //    header->setComboBoxCurrentData(DEPARTMENT, 1);


    header->hideComboBox(DESC);
    header->setLableText(DESC, "Description");
    header->setLineEditPlaceHolder(DESC, "Search");

    connect(header, SIGNAL(sectionsWidgetsSignal()), this, SLOT(headerWidgetsChagedSlot()));

    fillIpTV();

    ui->ipTV->verticalHeader()->hide();
    ui->ipTV->horizontalHeader()->setSectionResizeMode(NET,QHeaderView::Stretch);
    ui->ipTV->horizontalHeader()->setSectionResizeMode(MASK,QHeaderView::Stretch);
    ui->ipTV->horizontalHeader()->setSectionResizeMode(BC,QHeaderView::Stretch);
    ui->ipTV->horizontalHeader()->setSectionResizeMode(DESC,QHeaderView::Stretch);
    ui->ipTV->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    /////////////////////

    ui->ipTV->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->ipTV, SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(contextMenuSlot(QPoint)));
    connect(ui->ipTV, SIGNAL(doubleClicked(QModelIndex)),this,SLOT(doubleClickedSlot(QModelIndex)));

    contextMenu.addAction(ui->actionRefresh);
    contextMenu.addSeparator();
    contextMenu.addAction(ui->actionSubNetworkPool);
    contextMenu.addSeparator();
    contextMenu.addAction(ui->actionNetwork_Assignment);
    contextMenu.addAction(ui->actionEditNetwork);
    contextMenu.addSeparator();
    contextMenu.addAction(ui->actionRemveNetwork);
    contextMenu.addSeparator();
    contextMenu.addAction(ui->actionExport_PDF);

}

NetV4PoolDialog::~NetV4PoolDialog()
{
    delete ui;
}

void NetV4PoolDialog::fillIpTV()
{
    //
    Net = header->sections[NET]->lineEdit->text();
    Prefix = header->sections[PREFIX]->lineEdit->text().toInt();
    Mask = header->sections[MASK]->lineEdit->text();
    Bc = header->sections[BC]->lineEdit->text();
    Host = header->sections[HOST]->lineEdit->text().toInt();
    Desc = header->sections[DESC]->lineEdit->text();

    QMap<int, QString> mapString;
    QMap<int, QRegExp> mapRegExp;
    QRegExp reg;
    reg.setCaseSensitivity(Qt::CaseInsensitive);

    if(!Net.isEmpty())
    {
        reg.setPattern(Net);
        mapRegExp.insert(NET, reg);
    }
    if(Prefix > 0)
        mapString.insert(PREFIX, QString::number(Prefix));
    if(!Mask.isEmpty())
    {
        reg.setPattern(Mask);
        mapRegExp.insert(MASK, reg);
    }
    if(!Bc.isEmpty())
    {
        reg.setPattern(Bc);
        mapRegExp.insert(BC, reg);
    }
    if(Host > 0)
        mapString.insert(HOST, QString::number(Host));
    if(!Desc.isEmpty())
    {
        reg.setPattern(Desc);
        mapRegExp.insert(DESC, reg);
    }


    //

    QSqlQuery *query = dbMan->selectNetPool();
    //`id`, `net`, `prefix`, `mask`, `bc`, `hosts`, `description`
    QSqlQueryModel *model = new QSqlQueryModel(this);
    model->setQuery(*query);

//    QBrush blueBrush;
//    blueBrush.setColor(Qt::darkBlue);
//    QModelIndex ind = model->index(0,1);
//    model->setData(ind,blueBrush, Qt::ForegroundRole);

    proxyModel->setSourceModel(model);
    proxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    ui->ipTV->hideColumn(ID);
    proxyModel->setColumnsFilter(mapString, mapRegExp);

    ui->ipTV->verticalHeader()->hide();
    ui->ipTV->horizontalHeader()->setSectionResizeMode(NET,QHeaderView::Stretch);
    ui->ipTV->horizontalHeader()->setSectionResizeMode(MASK,QHeaderView::Stretch);
    ui->ipTV->horizontalHeader()->setSectionResizeMode(BC,QHeaderView::Stretch);
    ui->ipTV->horizontalHeader()->setSectionResizeMode(DESC,QHeaderView::Stretch);
    ui->ipTV->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
}

void NetV4PoolDialog::refreshSlot(int done)
{
    done++;
    fillIpTV();
    ui->ipTV->selectRow(row);
}

void NetV4PoolDialog::headerWidgetsChagedSlot()
{
    Net = header->sections[NET]->lineEdit->text();
    Prefix = header->sections[PREFIX]->lineEdit->text().toInt();
    Mask = header->sections[MASK]->lineEdit->text();
    Bc = header->sections[BC]->lineEdit->text();
    Host = header->sections[HOST]->lineEdit->text().toInt();
    Desc = header->sections[DESC]->lineEdit->text();

    QMap<int, QString> mapString;
    QMap<int, QRegExp> mapRegExp;
    QRegExp reg;
    reg.setCaseSensitivity(Qt::CaseInsensitive);

    if(Prefix > 0)
        mapString.insert(PREFIX, QString::number(Prefix));
    if(Host > 0)
        mapString.insert(HOST, QString::number(Host));

    if(!Net.isEmpty())
    {
        reg.setPattern(Net);
        mapRegExp.insert(NET, reg);
    }
    if(!Mask.isEmpty())
    {
        reg.setPattern(Mask);
        mapRegExp.insert(MASK, reg);
    }
    if(!Bc.isEmpty())
    {
        reg.setPattern(Bc);
        mapRegExp.insert(BC, reg);
    }
    if(!Desc.isEmpty())
    {
        reg.setPattern(Desc);
        mapRegExp.insert(DESC, reg);
    }

    proxyModel->setColumnsFilter(mapString, mapRegExp);

    ui->ipTV->hideColumn(ID);

    ui->ipTV->verticalHeader()->hide();
    ui->ipTV->horizontalHeader()->setSectionResizeMode(NET,QHeaderView::Stretch);
    ui->ipTV->horizontalHeader()->setSectionResizeMode(MASK,QHeaderView::Stretch);
    ui->ipTV->horizontalHeader()->setSectionResizeMode(BC,QHeaderView::Stretch);
    ui->ipTV->horizontalHeader()->setSectionResizeMode(DESC,QHeaderView::Stretch);
    ui->ipTV->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

}

void NetV4PoolDialog::contextMenuSlot(QPoint ptr)
{

    //`id`, `net`, `prefix`, `mask`, `bc`, `hosts`,`department_no`, `department`, `description`

    QModelIndex clickedIndex = ui->ipTV->indexAt(ptr);

    row = clickedIndex.row();
    clickedIndex = clickedIndex.sibling(clickedIndex.row(), ID);
    Id = proxyModel->data(clickedIndex).toInt();
    clickedIndex = clickedIndex.sibling(clickedIndex.row(), NET);
    Net = proxyModel->data(clickedIndex).toString();
    clickedIndex = clickedIndex.sibling(clickedIndex.row(), PREFIX);
    Prefix = proxyModel->data(clickedIndex).toInt();
    clickedIndex = clickedIndex.sibling(clickedIndex.row(), MASK);
    Mask = proxyModel->data(clickedIndex).toString();
    clickedIndex = clickedIndex.sibling(clickedIndex.row(), BC);
    Bc = proxyModel->data(clickedIndex).toString();
    clickedIndex = clickedIndex.sibling(clickedIndex.row(), HOST);
    Host = proxyModel->data(clickedIndex).toInt();
    clickedIndex = clickedIndex.sibling(clickedIndex.row(), DESC);
    Desc = proxyModel->data(clickedIndex).toString();


    ptr.setY(ptr.y() + 80);
    if(Id > 0)
    {
        contextMenu.popup(ui->ipTV->mapToGlobal(ptr));

        // has Ip pool or subnet pool 1 assiged >> no edit no remove

        int ipC = dbMan->getIpPoolCount(Id,0);
        bool assignedSubnet = dbMan->isSubnetPoolAssigned(1, Id, 0);

        if( (ipC > 0) || assignedSubnet)
        {
            ui->actionEditNetwork->setEnabled(false);
            ui->actionRemveNetwork->setEnabled(false);
        }
        else
        {
            ui->actionEditNetwork->setEnabled(true);
            ui->actionRemveNetwork->setEnabled(true);
        }

    }
}

void NetV4PoolDialog::doubleClickedSlot(QModelIndex ind)
{
    ind = ind.sibling(ind.row(), ID);
    Id = proxyModel->data(ind).toInt();
    ind = ind.sibling(ind.row(), NET);
    QString net = proxyModel->data(ind).toString();
    ind = ind.sibling(ind.row(), PREFIX);
    net += "/" + QString::number(proxyModel->data(ind).toInt());
    ind = ind.sibling(ind.row(), DESC);
    Desc = proxyModel->data(ind).toString();


    Subnet1V4PoolDialog *subnet1V4PoolDialog = new Subnet1V4PoolDialog(this,dbMan,Id, net, Desc);
    subnet1V4PoolDialog->setAttribute(Qt::WA_DeleteOnClose);
    connect(subnet1V4PoolDialog, SIGNAL(finished(int)), this, SLOT(refreshSlot(int)));
    subnet1V4PoolDialog->showMaximized();

}

void NetV4PoolDialog::on_addBtn_clicked()
{
    AddNetV4Dialog *addNetV4Dialog = new AddNetV4Dialog(this, dbMan);
    addNetV4Dialog->setAttribute(Qt::WA_DeleteOnClose);
    connect(addNetV4Dialog, SIGNAL(finished(int)), this, SLOT(refreshSlot(int)));
    addNetV4Dialog->show();
}

void NetV4PoolDialog::on_actionEditNetwork_triggered()
{
    EditNetV4Dialog *editNetV4Dialog = new EditNetV4Dialog(this,dbMan,Id,Net,Prefix);
    editNetV4Dialog->setAttribute(Qt::WA_DeleteOnClose);
    connect(editNetV4Dialog, SIGNAL(finished(int)), this, SLOT(refreshSlot(int)));
    editNetV4Dialog->show();
}

void NetV4PoolDialog::on_actionRemveNetwork_triggered()
{
    RemoveNetV4Dialog *removeNetV4Dialog = new RemoveNetV4Dialog(this,dbMan,Id,Net,Prefix,Mask,Bc,Host);
    removeNetV4Dialog->setAttribute(Qt::WA_DeleteOnClose);
    connect(removeNetV4Dialog, SIGNAL(finished(int)), this, SLOT(refreshSlot(int)));
    removeNetV4Dialog->show();
}

void NetV4PoolDialog::on_actionSubNetworkPool_triggered()
{
    QString net = Net + " / "+QString::number(Prefix);

    Subnet1V4PoolDialog *subnet1V4PoolDialog = new Subnet1V4PoolDialog(this,dbMan,Id, net, Desc);
    subnet1V4PoolDialog->setAttribute(Qt::WA_DeleteOnClose);
    subnet1V4PoolDialog->showMaximized();
}

void NetV4PoolDialog::on_actionNetwork_Assignment_triggered()
{
    NetV4AssignmentDialog *netV4AssignmentDialog = new NetV4AssignmentDialog(this,dbMan,Id,Net,Prefix, Desc);
    netV4AssignmentDialog->setAttribute(Qt::WA_DeleteOnClose);
    connect(netV4AssignmentDialog, SIGNAL(finished(int)), this, SLOT(refreshSlot(int)));
    netV4AssignmentDialog->show();
}

void NetV4PoolDialog::on_actionRefresh_triggered()
{
    fillIpTV();
}

void NetV4PoolDialog::on_actionExport_PDF_triggered()
{
    QString path = QFileDialog::getSaveFileName(this, "Save DaNet Network Report As", "","pdf (*.pdf)");
    if(!path.isEmpty())
    {
        if(!path.endsWith(".pdf", Qt::CaseInsensitive)) path += ".pdf";


        ReportNetworkIpV4Plan reportNetworkIpPlan(this,proxyModel, path);
        if(reportNetworkIpPlan.print())
        {
            QDesktopServices::openUrl(path);
        }
        else
        {
            QMessageBox::warning(this, "ERROR", reportNetworkIpPlan.lastError());
        }
    }

}

void NetV4PoolDialog::on_searchBtn_clicked()
{
    SubnetV4SearchDialog *subnetV4SearchDialog = new SubnetV4SearchDialog(this, dbMan);
    subnetV4SearchDialog->setAttribute(Qt::WA_DeleteOnClose);
    subnetV4SearchDialog->show();
}
