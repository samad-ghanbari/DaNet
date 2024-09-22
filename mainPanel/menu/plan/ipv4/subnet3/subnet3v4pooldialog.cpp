#include "subnet3v4pooldialog.h"
#include "ui_subnet3v4pooldialog.h"
#include "lib/headerview.h"
#include "lib/database/danetdbman.h"
#include <QSqlQuery>
#include <QSqlQueryModel>
#include "lib/sortfilterproxymodel.h"
#include "mainPanel/menu/plan/ipv4/subnet3/subnet3v4assignmentdialog.h"
#include "mainPanel/menu/plan/ipv4/subnet3/subnet3v4releasedialog.h"
#include "mainPanel/menu/plan/ipv4/subnet3/splitsubnet3v4dialog.h"
#include "mainPanel/menu/plan/ipv4/subnet3/mergesubnet3v4dialog.h"
#include "mainPanel/menu/plan/ipv4/ip/ipv4pooldialog.h"
#include <QFile>
#include <QFileDialog>
#include <QDesktopServices>
#include "lib/report/ipv4plan/subnet/ReportSubnetIpV4Plan.h"
#include <QMessageBox>

//`id`, `subnet`, `prefix`, `mask`, `bc`, `hosts`, `area_no`, `area`, `exchange_id`,
//`abbr`, `exchange`, `application`, `description`

#define ID 0
#define SUBNET 1
#define PREFIX 2
#define MASK 3
#define BC 4
#define HOST 5
#define AREA_NO 6
#define AREA 7
#define EXCHANGE_ID 8
#define ABBR 9
#define EXCHANGE 10
#define APPLICATION 11
#define DESC 12


Subnet3V4PoolDialog::Subnet3V4PoolDialog(QWidget *parent, DanetDbMan *db, const int SubnetPool2Id, const QString Subnet, const QString SubnetDesc) :
    QDialog(parent),
    ui(new Ui::Subnet3V4PoolDialog),
    dbMan(db),
    model(new QSqlQueryModel(this)),
    proxyModel(nullptr),
    header(nullptr),
    subnetPool2Id(SubnetPool2Id)
{
    ui->setupUi(this);
    resize(parent->width()-50, parent->height()-50);
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowMinMaxButtonsHint);

    proxyModel = new SortFilterProxyModel(this);
    header = new HeaderView(13,MASK, ui->ipTV);
    ui->ipTV->setHorizontalHeader(header);
    ui->ipTV->setModel(proxyModel);

    ui->netLbl->setText(Subnet);
    ui->descLbl->setText(SubnetDesc);


    currentArea = -1;

    //    header->blockSignals(true);

    header->hideWidgets(ID);

    header->hideComboBox(SUBNET);
    header->setLableText(SUBNET, "Subnet");
    header->setLineEditPlaceHolder(SUBNET, "Search");

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

    header->hideWidgets(AREA_NO);

    header->hideLineEdit(AREA);
    header->setLableText(AREA, "Area");
    header->addComboBoxItem(AREA,"2", 2);
    header->addComboBoxItem(AREA,"3", 3);
    header->addComboBoxItem(AREA,"4", 4);
    header->addComboBoxItem(AREA,"5", 5);
    header->addComboBoxItem(AREA,"6", 6);
    header->addComboBoxItem(AREA,"7", 7);
    header->addComboBoxItem(AREA,"8", 8);
    header->addComboBoxItem(AREA,"Karaj", 9);
    header->addComboBoxItem(AREA,"All", 0);
    header->setComboBoxCurrentData(AREA, -1);

    header->hideWidgets(EXCHANGE_ID);


    header->hideComboBox(ABBR);
    header->setLableText(ABBR, "Abbr");
    header->setLineEditPlaceHolder(ABBR, "Search");

    header->hideComboBox(EXCHANGE);
    header->setLableText(EXCHANGE, "Exchange");
    header->setLineEditPlaceHolder(EXCHANGE, "Search");


    header->hideComboBox(APPLICATION);
    header->setLableText(APPLICATION, "Application");
    header->setLineEditPlaceHolder(APPLICATION, "Search");

    header->hideComboBox(DESC);
    header->setLableText(DESC, "Description");
    header->setLineEditPlaceHolder(DESC, "Search");

    connect(header, SIGNAL(sectionsWidgetsSignal()), this, SLOT(headerWidgetsChagedSlot()));


    header->setComboBoxCurrentData(AREA, 0);

    ui->ipTV->verticalHeader()->hide();
    ui->ipTV->horizontalHeader()->setSectionResizeMode(SUBNET,QHeaderView::Stretch);
    ui->ipTV->horizontalHeader()->setSectionResizeMode(MASK,QHeaderView::Stretch);
    ui->ipTV->horizontalHeader()->setSectionResizeMode(BC,QHeaderView::Stretch);
    ui->ipTV->horizontalHeader()->setSectionResizeMode(EXCHANGE,QHeaderView::Stretch);
    ui->ipTV->horizontalHeader()->setSectionResizeMode(APPLICATION,QHeaderView::Stretch);
    ui->ipTV->horizontalHeader()->setSectionResizeMode(DESC,QHeaderView::Stretch);

    ui->ipTV->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    /////////////////////

    ui->ipTV->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->ipTV, SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(contextMenuSlot(QPoint)));
    connect(ui->ipTV, SIGNAL(doubleClicked(QModelIndex)),this,SLOT(doubleClickedSlot(QModelIndex)));


    contextMenu.addAction(ui->actionRefresh);
    contextMenu.addSeparator();
    contextMenu.addAction(ui->actionNetwork_Assignment);
    contextMenu.addAction(ui->actionNetwork_Release);
    contextMenu.addSeparator();
    contextMenu.addAction(ui->actionSplitNetwork);
    contextMenu.addAction(ui->actionMergenets);
    contextMenu.addSeparator();
    contextMenu.addAction(ui->actionView_Ip_Pool);
    contextMenu.addSeparator();
    contextMenu.addAction(ui->actionExport_PDF);

    int c1 = dbMan->getIpPoolCount(subnetPool2Id, 2);
    int c2 = dbMan->getSubnetPoolCount(3,subnetPool2Id, 2);

    if((c2 == 0) && (c1 == 0) )
    {
        ui->addSubnetBtn->setEnabled(true);
        ui->RemoveSubnetBtn->setEnabled(false);
    }
    else
    {
        ui->addSubnetBtn->setEnabled(false);
        ui->RemoveSubnetBtn->setEnabled(true);
    }
}

Subnet3V4PoolDialog::~Subnet3V4PoolDialog()
{
    delete ui;
}

void Subnet3V4PoolDialog::fillIpTV(int area)
{
//`id`, `subnet`, `prefix`, `mask`, `bc`, `hosts`, `area_no`, `area`, `exchange_id`,
//`abbr`, `exchange`, `application`, `description`

    QString subnet,mask, bc, abb, exch, ap,desc;
    int prefix, host;
    currentArea = area;

    subnet = header->sections[SUBNET]->lineEdit->text();
    prefix = header->sections[PREFIX]->lineEdit->text().toInt();
    mask = header->sections[MASK]->lineEdit->text();
    bc = header->sections[BC]->lineEdit->text();
    host = header->sections[HOST]->lineEdit->text().toInt();
    abb = header->sections[ABBR]->lineEdit->text();
    exch = header->sections[EXCHANGE]->lineEdit->text();
    ap = header->sections[APPLICATION]->lineEdit->text();
    desc = header->sections[DESC]->lineEdit->text();

    QSqlQuery *query;
    if(area > 1)
        query = dbMan->selectSubnetPool3View(area,subnetPool2Id);
    else
        query = dbMan->selectSubnetPool3View(subnetPool2Id);

    header->blockSignals(true);
    model->setQuery(*query);
    proxyModel->setSourceModel(model);
    proxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    ui->ipTV->hideColumn(ID);
    ui->ipTV->hideColumn(AREA_NO);
    ui->ipTV->hideColumn(EXCHANGE_ID);
    header->resetHeaderWidgets();

    if(area < 2)
        header->setComboBoxCurrentData(AREA,0);
    else
        header->setComboBoxCurrentData(AREA,area);

    header->blockSignals(false);
    if(!subnet.isEmpty())
        header->setLineEditText(SUBNET,subnet);
    if(prefix > 2)
        header->setLineEditText(PREFIX, QString::number(prefix));
    if(host > 0)
        header->setLineEditText(HOST, QString::number(host));
    if(!mask.isEmpty())
        header->setLineEditText(MASK, mask);
    if(!bc.isEmpty())
        header->setLineEditText(BC, bc);
    if(!abb.isEmpty())
        header->setLineEditText(ABBR, abb);
    if(!exch.isEmpty())
        header->setLineEditText(EXCHANGE, exch);
    if(!ap.isEmpty())
        header->setLineEditText(APPLICATION, ap);
    if(!desc.isEmpty())
        header->setLineEditText(DESC, desc);

    ui->ipTV->verticalHeader()->hide();
    ui->ipTV->horizontalHeader()->setSectionResizeMode(SUBNET,QHeaderView::Stretch);
    ui->ipTV->horizontalHeader()->setSectionResizeMode(MASK,QHeaderView::Stretch);
    ui->ipTV->horizontalHeader()->setSectionResizeMode(BC,QHeaderView::Stretch);
    ui->ipTV->horizontalHeader()->setSectionResizeMode(EXCHANGE,QHeaderView::Stretch);
    ui->ipTV->horizontalHeader()->setSectionResizeMode(APPLICATION,QHeaderView::Stretch);
    ui->ipTV->horizontalHeader()->setSectionResizeMode(DESC,QHeaderView::Stretch);

    ui->ipTV->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
}

void Subnet3V4PoolDialog::refreshCurrentAreaSlot(int i)
{
    i++;
    fillIpTV(currentArea);
    ui->ipTV->selectRow(Row);
}

void Subnet3V4PoolDialog::headerWidgetsChagedSlot()
{
    Area = header->sections[AREA]->comboBox->currentData().toInt();

    if( Area == currentArea )
    {
        Subnet = header->sections[SUBNET]->lineEdit->text();
        Prefix = header->sections[PREFIX]->lineEdit->text().toInt();
        Mask = header->sections[MASK]->lineEdit->text();
        Bc = header->sections[BC]->lineEdit->text();
        Host = header->sections[HOST]->lineEdit->text().toInt();
        Abbr = header->sections[ABBR]->lineEdit->text();
        Exchange = header->sections[EXCHANGE]->lineEdit->text();
        Application = header->sections[APPLICATION]->lineEdit->text();
        Desc = header->sections[DESC]->lineEdit->text();


        QMap<int, QString> mapString;
        QMap<int, QRegExp> mapRegExp;
        QRegExp reg;
        reg.setCaseSensitivity(Qt::CaseInsensitive);

        if(Prefix > 0)
            mapString.insert(PREFIX, QString::number(Prefix));
        if(Host > 0)
            mapString.insert(HOST, QString::number(Host));

        if(!Subnet.isEmpty())
        {
            reg.setPattern(Subnet);
            mapRegExp.insert(SUBNET, reg);
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
        if(!Exchange.isEmpty())
        {
            reg.setPattern(Exchange);
            mapRegExp.insert(EXCHANGE, reg);
        }
        if(!Abbr.isEmpty())
        {
            reg.setPattern(Abbr);
            mapRegExp.insert(ABBR, reg);
        }
        if(!Desc.isEmpty())
        {
            reg.setPattern(Desc);
            mapRegExp.insert(DESC, reg);
        }
        if(!Application.isEmpty())
        {
            reg.setPattern(Application);
            mapRegExp.insert(APPLICATION, reg);
        }

        proxyModel->setColumnsFilter(mapString, mapRegExp);

        ui->ipTV->hideColumn(ID);
        ui->ipTV->hideColumn(AREA_NO);
        ui->ipTV->hideColumn(EXCHANGE_ID);
    }
    else
    {
        fillIpTV(Area);
    }

    ui->ipTV->verticalHeader()->hide();
    ui->ipTV->horizontalHeader()->setSectionResizeMode(SUBNET,QHeaderView::Stretch);
    ui->ipTV->horizontalHeader()->setSectionResizeMode(MASK,QHeaderView::Stretch);
    ui->ipTV->horizontalHeader()->setSectionResizeMode(BC,QHeaderView::Stretch);
    ui->ipTV->horizontalHeader()->setSectionResizeMode(EXCHANGE,QHeaderView::Stretch);
    ui->ipTV->horizontalHeader()->setSectionResizeMode(APPLICATION,QHeaderView::Stretch);
    ui->ipTV->horizontalHeader()->setSectionResizeMode(DESC,QHeaderView::Stretch);

    ui->ipTV->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
}

void Subnet3V4PoolDialog::contextMenuSlot(QPoint ptr)
{

    //Id,Area,ExchId,Dep,Net,Prefix,Mask,Bc,Host,Desc

    QModelIndex clickedIndex = ui->ipTV->indexAt(ptr);
    Row = clickedIndex.row();

    clickedIndex = clickedIndex.sibling(clickedIndex.row(), ID);
    Id = proxyModel->data(clickedIndex).toInt();
    clickedIndex = clickedIndex.sibling(clickedIndex.row(), SUBNET);
    Subnet = proxyModel->data(clickedIndex).toString();
    clickedIndex = clickedIndex.sibling(clickedIndex.row(), PREFIX);
    Prefix = proxyModel->data(clickedIndex).toInt();
    clickedIndex = clickedIndex.sibling(clickedIndex.row(), MASK);
    Mask = proxyModel->data(clickedIndex).toString();
    clickedIndex = clickedIndex.sibling(clickedIndex.row(), BC);
    Bc = proxyModel->data(clickedIndex).toString();
    clickedIndex = clickedIndex.sibling(clickedIndex.row(), HOST);
    Host = proxyModel->data(clickedIndex).toInt();
    clickedIndex = clickedIndex.sibling(clickedIndex.row(), AREA_NO);
    if(proxyModel->data(clickedIndex).isNull())
        Area = 1;
    else
        Area = proxyModel->data(clickedIndex).toInt();
    clickedIndex = clickedIndex.sibling(clickedIndex.row(), EXCHANGE_ID);
    if(proxyModel->data(clickedIndex).isNull())
        ExchId = -2;
    else
        ExchId = proxyModel->data(clickedIndex).toInt();
    clickedIndex = clickedIndex.sibling(clickedIndex.row(), ABBR);
    Abbr = proxyModel->data(clickedIndex).toString();
    clickedIndex = clickedIndex.sibling(clickedIndex.row(), APPLICATION);
    Ap = proxyModel->data(clickedIndex).toString();
    clickedIndex = clickedIndex.sibling(clickedIndex.row(), DESC);
    Desc = proxyModel->data(clickedIndex).toString();

    ptr.setY(ptr.y() + 80);
    if(Id > 0)
    {
        bool assigned ;
        bool des = !Desc.isEmpty();
        bool ap = !Ap.isEmpty();
        bool a = (Area > 1)? true : false;
        assigned = a || ap || des;

        contextMenu.popup(ui->ipTV->mapToGlobal(ptr));
        if(assigned)
        {
            ui->actionNetwork_Release->setEnabled(true);
            ui->actionView_Ip_Pool->setEnabled(true);
        }

        int ipC = dbMan->getIpPoolCount(Id,3);
        if(ipC == 0)
        {
            ui->actionSplitNetwork->setEnabled(true);
            ui->actionMergenets->setEnabled(true);
            ui->actionNetwork_Release->setEnabled(true);
        }
        else
        {
            ui->actionSplitNetwork->setEnabled(false);
            ui->actionMergenets->setEnabled(false);
            ui->actionNetwork_Release->setEnabled(false);
        }


        if(!assigned)
        {
            ui->actionNetwork_Release->setEnabled(false);
            ui->actionView_Ip_Pool->setEnabled(false);
        }

    }
}

void Subnet3V4PoolDialog::doubleClickedSlot(QModelIndex clickedIndex)
{
    clickedIndex = clickedIndex.sibling(clickedIndex.row(), ID);
    Id = proxyModel->data(clickedIndex).toInt();
    clickedIndex = clickedIndex.sibling(clickedIndex.row(), SUBNET);
    Subnet = proxyModel->data(clickedIndex).toString();
    clickedIndex = clickedIndex.sibling(clickedIndex.row(), PREFIX);
    Prefix = proxyModel->data(clickedIndex).toInt();
    clickedIndex = clickedIndex.sibling(clickedIndex.row(), MASK);
    Mask = proxyModel->data(clickedIndex).toString();
    clickedIndex = clickedIndex.sibling(clickedIndex.row(), BC);
    Bc = proxyModel->data(clickedIndex).toString();
    clickedIndex = clickedIndex.sibling(clickedIndex.row(), HOST);
    Host = proxyModel->data(clickedIndex).toInt();
    clickedIndex = clickedIndex.sibling(clickedIndex.row(), AREA);
    Area = proxyModel->data(clickedIndex).toInt();
    clickedIndex = clickedIndex.sibling(clickedIndex.row(), ABBR);
    Abbr = proxyModel->data(clickedIndex).toString();
    clickedIndex = clickedIndex.sibling(clickedIndex.row(), APPLICATION);
    Ap = proxyModel->data(clickedIndex).toString();
    clickedIndex = clickedIndex.sibling(clickedIndex.row(), DESC);
    Desc = proxyModel->data(clickedIndex).toString();

    bool assigned ;
    bool des = !Desc.isEmpty();
    bool ap = !Ap.isEmpty();
    assigned = ap || des;

    if(assigned)
    {
        int IP_POOL = dbMan->getIpPoolCount(Id, 3);
        if(IP_POOL > 0)
        {
            IpV4PoolDialog *ipV4PoolDialog = new IpV4PoolDialog(this,dbMan,3, Id,Subnet,Prefix,Mask,Bc,Host,Area,Abbr,Ap,Desc);
            ipV4PoolDialog->setAttribute(Qt::WA_DeleteOnClose);
            ipV4PoolDialog->showMaximized();
        }
    }

}

void Subnet3V4PoolDialog::on_actionNetwork_Assignment_triggered()
{
    Subnet3V4AssignmentDialog *subnet3V4AssignmentDialog = new Subnet3V4AssignmentDialog(this,dbMan,Id,Subnet,Prefix, Area,ExchId, Ap, Desc);
    subnet3V4AssignmentDialog->setAttribute(Qt::WA_DeleteOnClose);
    connect(subnet3V4AssignmentDialog, SIGNAL(finished(int)), this, SLOT(refreshCurrentAreaSlot(int)));
    subnet3V4AssignmentDialog->show();
}

void Subnet3V4PoolDialog::on_actionNetwork_Release_triggered()
{
    Subnet3V4ReleaseDialog *subnet3V4ReleaseDialog = new Subnet3V4ReleaseDialog(this,dbMan,Id,Subnet,Prefix,Mask,Bc,Host, Area,Abbr, Ap,Desc);
    subnet3V4ReleaseDialog->setAttribute(Qt::WA_DeleteOnClose);
    connect(subnet3V4ReleaseDialog, SIGNAL(finished(int)), this, SLOT(refreshCurrentAreaSlot(int)));
    subnet3V4ReleaseDialog->show();
}

void Subnet3V4PoolDialog::on_actionSplitNetwork_triggered()
{
    QString abbr;
    if( (Area > 2) && (Area < 9) )
        abbr = QString::number(Area)+"-"+Abbr;
    else if(Area == 9)
        abbr = QString("Karaj")+"-"+Abbr;
    else if(Area == 1)
        abbr = "General";

    SplitSubnet3V4Dialog *splitSubnet3V4Dialog = new SplitSubnet3V4Dialog(this,dbMan,subnetPool2Id, Id,abbr,Subnet,Prefix, Area, Ap, Desc);
    splitSubnet3V4Dialog->setAttribute(Qt::WA_DeleteOnClose);
    connect(splitSubnet3V4Dialog, SIGNAL(finished(int)), this, SLOT(refreshCurrentAreaSlot(int)));
    splitSubnet3V4Dialog->show();
}

void Subnet3V4PoolDialog::on_actionMergenets_triggered()
{
    QString abbr;
    if( (Area > 2) && (Area < 9) )
        abbr = QString::number(Area)+"-"+Abbr;
    else if(Area == 9)
        abbr = QString("Karaj")+"-"+Abbr;
    else if(Area == 1)
        abbr = "General";

    MergeSubnet3V4Dialog *mergeSubnet3V4Dialog = new MergeSubnet3V4Dialog(this, dbMan,Id,abbr,Ap,Subnet,Prefix);
    mergeSubnet3V4Dialog->setAttribute(Qt::WA_DeleteOnClose);
    connect(mergeSubnet3V4Dialog, SIGNAL(finished(int)), this, SLOT(refreshCurrentAreaSlot(int)));
    mergeSubnet3V4Dialog->show();
}

void Subnet3V4PoolDialog::on_actionView_Ip_Pool_triggered()
{
    IpV4PoolDialog *ipV4PoolDialog = new IpV4PoolDialog(this,dbMan,3, Id,Subnet,Prefix,Mask,Bc,Host,Area,Abbr,Ap,Desc);
    ipV4PoolDialog->setAttribute(Qt::WA_DeleteOnClose);
    ipV4PoolDialog->show();
}

void Subnet3V4PoolDialog::on_actionExport_PDF_triggered()
{

    QString path = QFileDialog::getSaveFileName(this, "Save DaNet Subnet Report As", "","pdf (*.pdf)");
    if(!path.isEmpty())
    {
        if(!path.endsWith(".pdf", Qt::CaseInsensitive)) path += ".pdf";

        ReportSubnetIpV4Plan reportSubnetIpPlan(this,proxyModel,3, path);
        if(reportSubnetIpPlan.print())
        {
            QDesktopServices::openUrl(path);
        }
        else
        {
            QMessageBox::warning(this, "ERROR", reportSubnetIpPlan.lastError());
        }
    }

}

void Subnet3V4PoolDialog::on_actionRefresh_triggered()
{
    fillIpTV(currentArea);
}

void Subnet3V4PoolDialog::on_addSubnetBtn_clicked()
{
    QSqlQuery *query = dbMan->selectSubnetPool2Record(subnetPool2Id);
//`subnet`, `prefix`, `mask`, `bc`, `hosts`, `area`, `exchange_id`, `application`, `description`
    if(query->next())
    {
        QString subnet = query->value(0).toString();
        int prefix = query->value(1).toInt();
        QString mask = query->value(2).toString();
        QString bc = query->value(3).toString();
        int host = query->value(4).toInt();
        int area = query->value(5).toInt();
        if(area < 2) area = -1;


        if(dbMan->insertSubnetPool3(subnetPool2Id,subnet,prefix,mask,bc,host,area))
        {
            ui->addSubnetBtn->setEnabled(false);
            ui->RemoveSubnetBtn->setEnabled(true);
        }
        else
            QMessageBox::warning(this,"ERROR", "Cannot Add Subnet. Try again later.");

        fillIpTV(Area);
    }
}

void Subnet3V4PoolDialog::on_RemoveSubnetBtn_clicked()
{
    int ipC = dbMan->getIpPoolCount(subnetPool2Id,2);
    if(ipC > 0)
        QMessageBox::warning(this, "ERROR", "You need to remove all related IP POOLs first.");
    else
    {
        bool subnetAssigned = dbMan->isSubnetPoolAssigned(3,subnetPool2Id, 2);
        if(subnetAssigned)
            QMessageBox::warning(this, "ERROR", "Assigned Subnets cannot be removed.");
        else
        {
            if(dbMan->deleteSubnetPool(3,subnetPool2Id,2))
            {
                ui->addSubnetBtn->setEnabled(true);
                ui->RemoveSubnetBtn->setEnabled(false);
            }
            else
                QMessageBox::warning(this, "ERROR", "Cannot remove SUBNET POOLs. Try again later.");

            fillIpTV(Area);
        }
    }

}
