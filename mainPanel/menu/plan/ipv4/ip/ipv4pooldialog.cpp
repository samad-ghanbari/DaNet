#include "ipv4pooldialog.h"
#include "ui_ipv4pooldialog.h"
#include <QSqlQuery>
#include <QSortFilterProxyModel>
#include <QSqlQueryModel>
#include "lib/database/danetdbman.h"
#include "mainPanel/menu/plan/ipv4/ip/ipv4assigndialog.h"
#include "mainPanel/menu/plan/ipv4/ip/ipv4releasedialog.h"
#include "mainPanel/menu/plan/ipv4/ip/addipv4pooldialog.h"
#include "mainPanel/menu/plan/ipv4/ip/removeipv4pooldialog.h"
#include <QFileDialog>
#include <QDesktopServices>
#include "lib/report/ipv4plan/ip/ReportIpV4Plan.h"
#include <QMessageBox>

//`id`, `ip`, `description`, `editable`
#define ID 0
#define IP 1
#define DESC 2
#define ED 3

IpV4PoolDialog::IpV4PoolDialog(QWidget *parent, DanetDbMan *db, const int SubnetNo, const int SubnetId, const QString Subnet, const int Prefix, const QString Mask, const QString Bc, const int Host, const int Area, const QString Abbr, const QString App, const QString Desc) :
    QDialog(parent),
    ui(new Ui::IpV4PoolDialog),
    dbMan(db),
    model(new QSortFilterProxyModel(this)),
    m(new QSqlQueryModel(this)),
    subnetId(SubnetId),
    prefix(Prefix),
    host(Host),
    subnet(Subnet),
    mask(Mask),
    bc(Bc),
    abbr(Abbr),
    app(App),
    desc(Desc),
    subnetNo(SubnetNo)
{
    ui->setupUi(this);
    resize(parent->width()-50, parent->height()-50);
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowMinMaxButtonsHint);

    ui->subnetLbl->setText(subnet);
    ui->prefixLbl->setText(QString::number(Prefix));
    ui->maskLbl->setText(Mask);
    ui->bcLbl->setText(Bc);
    ui->hostLbl->setText(QString::number(Host));
    QString ab;
    if((Area > 1) && (Area < 9) )
        ab = "Area:"+QString::number(Area);
    else if(Area == 9)
        ab = "Karaj";
    if(!Abbr.isEmpty())
        ab = ab +"-"+Abbr;
    ui->abbrLbl->setText(ab);
    ui->appLbl->setText(App);
    ui->descLbl->setText(desc);

    ui->ipTV->setModel(model);
    ui->ipTV->horizontalHeader()->setStretchLastSection(true);
    ui->ipTV->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    ui->addIpBtn->setEnabled(false);
    ui->removeIpBtn->setEnabled(false);

    int ipC = -1;
    if(subnetNo == 1)
        ipC = dbMan->getIpPoolCount(subnetId, 1);
    else if(subnetNo == 2)
        ipC = dbMan->getIpPoolCount(subnetId, 2);
    else if(subnetNo == 3)
        ipC = dbMan->getIpPoolCount(subnetId, 3);


    if(ipC > 0)
    {
        ui->addIpBtn->setEnabled(false);
        ui->removeIpBtn->setEnabled(true);
    }
    else if(ipC == 0)
    {
        ui->addIpBtn->setEnabled(true);
        ui->removeIpBtn->setEnabled(false);
    }


    // subnet-prefix-mask-broadcast-hostcount-exch & desc
    subnetInfo << subnet <<QString::number(prefix) <<mask << Bc << QString::number(host) << abbr+" "+desc;

    contextMenu.addAction(ui->actionAssign_IP);
    contextMenu.addAction(ui->actionrelease_IP);
    contextMenu.addSeparator();
    contextMenu.addAction(ui->actionExport_PDF);
    contextMenu.addSeparator();
    contextMenu.addAction(ui->actionRefresh);

    ui->ipTV->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->ipTV, SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(contextMenuSlot(QPoint)));

    fillTV();

}

IpV4PoolDialog::~IpV4PoolDialog()
{
    delete ui;
}

void IpV4PoolDialog::fillTV()
{
     //`id`, `ip`, `description`, `editable`
    QSqlQuery *query = dbMan->selectIpPool(subnetNo, subnetId);
    m->setQuery(*query);
    model->setSourceModel(m);
    model->setHeaderData(IP,Qt::Horizontal,"IP");
    model->setHeaderData(DESC, Qt::Horizontal, "Description");
    ui->ipTV->verticalHeader()->hide();
    ui->ipTV->hideColumn(ID);
    ui->ipTV->hideColumn(ED);

    ui->ipTV->horizontalHeader()->setStretchLastSection(true);
    ui->ipTV->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
}

void IpV4PoolDialog::refreshSlot(int i)
{
    i++;
    fillTV();
    ui->ipTV->selectRow(Row);
}

void IpV4PoolDialog::refreshAddRemoveSlot(int i)
{
    i++;
    fillTV();

    ui->addIpBtn->setEnabled(false);
    ui->removeIpBtn->setEnabled(false);

    int ipC = -1;
    if(subnetNo == 1)
        ipC = dbMan->getIpPoolCount(subnetId, 1);
    else if(subnetNo == 2)
        ipC = dbMan->getIpPoolCount(subnetId, 2);
    else if(subnetNo == 3)
        ipC = dbMan->getIpPoolCount(subnetId, 3);


    if(ipC > 0)
    {
        ui->addIpBtn->setEnabled(false);
        ui->removeIpBtn->setEnabled(true);
    }
    else if(ipC == 0)
    {
        ui->addIpBtn->setEnabled(true);
        ui->removeIpBtn->setEnabled(false);
    }
}

void IpV4PoolDialog::contextMenuSlot(QPoint ptr)
{

    //`id`, `ip`, `description`, `editable`

    QModelIndex clickedIndex = ui->ipTV->indexAt(ptr);
    Row = clickedIndex.row();

    clickedIndex = clickedIndex.sibling(clickedIndex.row(), ID);
    ipId = model->data(clickedIndex).toInt();
    clickedIndex = clickedIndex.sibling(clickedIndex.row(), IP);
    ip = model->data(clickedIndex).toString();
    clickedIndex = clickedIndex.sibling(clickedIndex.row(), DESC);
    ipDesc = model->data(clickedIndex).toString();
    clickedIndex = clickedIndex.sibling(clickedIndex.row(), ED);
    editable = model->data(clickedIndex).toBool();

    if(ipId > 0)
    {
        contextMenu.popup(ui->ipTV->mapToGlobal(ptr));
        if(ipDesc.isEmpty())
            ui->actionrelease_IP->setEnabled(false);
        else
            ui->actionrelease_IP->setEnabled(true);

        if(editable)
        {
            ui->actionAssign_IP->setEnabled(true);
            ui->actionrelease_IP->setEnabled(true);
        }
        else
        {
            ui->actionAssign_IP->setEnabled(false);
            ui->actionrelease_IP->setEnabled(false);
        }
    }

    ui->ipTV->horizontalHeader()->setStretchLastSection(true);
    ui->ipTV->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
}

void IpV4PoolDialog::on_actionAssign_IP_triggered()
{
    IpV4AssignDialog *ipV4AssignDialog = new IpV4AssignDialog(this,dbMan,ipId,subnet,QString::number(prefix),mask,bc,QString::number(host),abbr,app,desc,ip, ipDesc);
    ipV4AssignDialog->setAttribute(Qt::WA_DeleteOnClose);
    connect(ipV4AssignDialog, SIGNAL(finished(int)), this, SLOT(refreshSlot(int)));
    ipV4AssignDialog->show();
}

void IpV4PoolDialog::on_actionrelease_IP_triggered()
{
    IpV4ReleaseDialog *ipV4ReleaseDialog = new IpV4ReleaseDialog(this,dbMan,ipId,subnet,QString::number(prefix),mask,bc,QString::number(host),abbr,app,desc,ip, ipDesc);
    ipV4ReleaseDialog->setAttribute(Qt::WA_DeleteOnClose);
    connect(ipV4ReleaseDialog, SIGNAL(finished(int)), this, SLOT(refreshSlot(int)));
    ipV4ReleaseDialog->show();
}

void IpV4PoolDialog::on_addIpBtn_clicked()
{
    AddIpV4PoolDialog *addIpV4PoolDialog = new AddIpV4PoolDialog(this, dbMan,subnetNo,subnetId,subnet,prefix,mask,bc,host,abbr,app,desc);
    addIpV4PoolDialog->setAttribute(Qt::WA_DeleteOnClose);
    connect(addIpV4PoolDialog, SIGNAL(finished(int)), this, SLOT(refreshAddRemoveSlot(int)));
    addIpV4PoolDialog->show();
}

void IpV4PoolDialog::on_removeIpBtn_clicked()
{
    RemoveIpV4PoolDialog *removeIpV4PoolDialog = new RemoveIpV4PoolDialog(this, dbMan,subnetNo,subnetId,subnet,prefix,mask,bc,host,abbr,app,desc);
    removeIpV4PoolDialog->setAttribute(Qt::WA_DeleteOnClose);
    connect(removeIpV4PoolDialog, SIGNAL(finished(int)), this, SLOT(refreshAddRemoveSlot(int)));
    removeIpV4PoolDialog->show();
}

void IpV4PoolDialog::on_actionRefresh_triggered()
{
    if(dbMan->refreshIpPool(subnetId, subnetNo))
        fillTV();
    else
        QMessageBox::warning(this, "ERROR", "cannot refresh IP pool. try again.\n"+dbMan->lastError());
}

void IpV4PoolDialog::on_searchLE_textChanged(const QString &arg1)
{
    QRegExp reg;
    reg.setCaseSensitivity(Qt::CaseInsensitive);
    reg.setPattern(arg1);
    model->setFilterKeyColumn(DESC);
    model->setFilterRegExp(reg);

}

void IpV4PoolDialog::on_actionExport_PDF_triggered()
{
    QString path = QFileDialog::getSaveFileName(this, "Save DaNet IP Pool Report As", "","pdf (*.pdf)");
    if(!path.isEmpty())
    {
        if(!path.endsWith(".pdf",Qt::CaseInsensitive)) path += ".pdf";

        ReportIpV4Plan reportIpPlan(this,model, subnetInfo, path);
        if(reportIpPlan.print())
        {
            QDesktopServices::openUrl(path);
        }
        else
        {
            QMessageBox::warning(this, "ERROR", reportIpPlan.lastError());
        }
    }

}
