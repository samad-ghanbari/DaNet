#include "subnet1v4pooldialog.h"
#include "ui_subnet1v4pooldialog.h"
#include "lib/headerview.h"
#include "lib/database/danetdbman.h"
#include <QSqlQuery>
#include <QSqlQueryModel>
#include "lib/sortfilterproxymodel.h"
#include "mainPanel/menu/plan/ipv4/subnet1/subnet1v4assignmentdialog.h"
#include "mainPanel/menu/plan/ipv4/subnet1/subnet1v4releasedialog.h"
#include "mainPanel/menu/plan/ipv4/subnet1/splitsubnet1v4dialog.h"
#include "mainPanel/menu/plan/ipv4/subnet1/mergesubnet1v4dialog.h"
#include "mainPanel/menu/plan/ipv4/ip/ipv4pooldialog.h"
#include "mainPanel/menu/plan/ipv4/subnet2/subnet2v4pooldialog.h"
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
#define DEP_NO 11
#define DEP 12
#define APPLICATION 13
#define DESC 14


Subnet1V4PoolDialog::Subnet1V4PoolDialog(QWidget *parent, DanetDbMan *db, const int NetPoolId, const QString net, const QString NetDesc) :
    QDialog(parent),
    ui(new Ui::Subnet1V4PoolDialog),
    dbMan(db),
    model(new QSqlQueryModel(this)),
    proxyModel(nullptr),
    header(nullptr),
    netPoolId(NetPoolId)
{
    ui->setupUi(this);
    resize(parent->width()-50, parent->height()-50);
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowMinMaxButtonsHint);

    proxyModel = new SortFilterProxyModel(this);
    header = new HeaderView(15,MASK, ui->ipTV);
    ui->ipTV->setHorizontalHeader(header);
    ui->ipTV->setModel(proxyModel);

    ui->netLbl->setText(net);
    ui->descLbl->setText(NetDesc);


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

    header->hideWidgets(DEP_NO);

    header->hideLineEdit(DEP);
    header->setLableText(DEP, "Department");
    header->addComboBoxItem(DEP,"Data", 1);
    header->addComboBoxItem(DEP,"Switch", 2);
    header->addComboBoxItem(DEP,"Access", 3);
    header->addComboBoxItem(DEP,"Transmission", 4);
    header->addComboBoxItem(DEP,"All", 0);
    header->setComboBoxCurrentData(DEP, 0);


    header->hideComboBox(APPLICATION);
    header->setLableText(APPLICATION, "Application");
    header->setLineEditPlaceHolder(APPLICATION, "Search");

    header->hideComboBox(DESC);
    header->setLableText(DESC, "Description");
    header->setLineEditPlaceHolder(DESC, "Search");

    connect(header, SIGNAL(sectionsWidgetsSignal()), this, SLOT(headerWidgetsChagedSlot()));


//    int workingArea = dbMan->getLoggedInDefaultArea();
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
    contextMenu.addAction(ui->actionView_Subnet_Pool);
    contextMenu.addAction(ui->actionView_Ip_Pool);
    contextMenu.addSeparator();
    contextMenu.addAction(ui->actionExport_PDF);
}

Subnet1V4PoolDialog::~Subnet1V4PoolDialog()
{
    delete ui;
}

void Subnet1V4PoolDialog::fillIpTV(int area)
{
    //`id`, `subnet`, `prefix`, `mask`, `bc`, `hosts`, `area_no`, `area`, `exchange_id`,
    //`abbr`, `exchange`, dep_no, dep, `application`, `description`

    QString subnet,mask, bc, abb, exch, ap,desc;
    int prefix, host, depNo;
    currentArea = area;

    subnet = header->sections[SUBNET]->lineEdit->text();
    prefix = header->sections[PREFIX]->lineEdit->text().toInt();
    mask = header->sections[MASK]->lineEdit->text();
    bc = header->sections[BC]->lineEdit->text();
    host = header->sections[HOST]->lineEdit->text().toInt();
    abb = header->sections[ABBR]->lineEdit->text();
    exch = header->sections[EXCHANGE]->lineEdit->text();
    depNo  = header->sections[DEP]->comboBox->currentData().toInt();
    ap = header->sections[APPLICATION]->lineEdit->text();
    desc = header->sections[DESC]->lineEdit->text();

    QSqlQuery *query;
    if(area > 1)
        query = dbMan->selectSubnetPool1View(area,netPoolId);
    else
        query = dbMan->selectSubnetPool1View(netPoolId);

    header->blockSignals(true);
    model->setQuery(*query);
    proxyModel->setSourceModel(model);
    proxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    ui->ipTV->hideColumn(ID);
    ui->ipTV->hideColumn(AREA_NO);
    ui->ipTV->hideColumn(EXCHANGE_ID);
    ui->ipTV->hideColumn(DEP_NO);
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

    header->setComboBoxCurrentData(DEP,depNo);

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

void Subnet1V4PoolDialog::refreshCurrentAreaSlot(int i)
{
    i++;
    fillIpTV(currentArea);
    ui->ipTV->selectRow(Row);
}


void Subnet1V4PoolDialog::headerWidgetsChagedSlot()
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
        Dep = header->sections[DEP]->comboBox->currentText();
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
        if(!Dep.isEmpty())
        {
            if(Dep.compare("All", Qt::CaseInsensitive) != 0)
            {
                reg.setPattern(Dep);
                mapRegExp.insert(DEP, reg);
            }
        }
        else
            header->setComboBoxCurrentData(DEP,0);
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
        ui->ipTV->hideColumn(DEP_NO);
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

void Subnet1V4PoolDialog::contextMenuSlot(QPoint ptr)
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
    clickedIndex = clickedIndex.sibling(clickedIndex.row(), DEP_NO);
    DepNo = proxyModel->data(clickedIndex).toInt();
    clickedIndex = clickedIndex.sibling(clickedIndex.row(), DEP);
    Dep = proxyModel->data(clickedIndex).toString();
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

        int subnetPool2Count = dbMan->getSubnetPoolCount(2,Id,1);
        int ipPoolCount = dbMan->getIpPoolCount(Id,1);

        contextMenu.popup(ui->ipTV->mapToGlobal(ptr));

        if(assigned)
        {
            ui->actionNetwork_Release->setEnabled(true);
            ui->actionView_Ip_Pool->setEnabled(true);
        }

        ////
        if(ipPoolCount == 0)
            ui->actionView_Subnet_Pool->setEnabled(true);
        else
            ui->actionView_Subnet_Pool->setEnabled(false);

        if(subnetPool2Count == 0)
            ui->actionView_Ip_Pool->setEnabled(true);
        else
            ui->actionView_Ip_Pool->setEnabled(false);

        if((ipPoolCount == 0) && (subnetPool2Count == 0))
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

        ///
        if(!assigned)
        {
            ui->actionNetwork_Release->setEnabled(false);
            ui->actionView_Ip_Pool->setEnabled(false);
            ui->actionView_Subnet_Pool->setEnabled(false);
        }
    }
}

void Subnet1V4PoolDialog::doubleClickedSlot(QModelIndex clickedIndex)
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
    clickedIndex = clickedIndex.sibling(clickedIndex.row(), DEP);
    Dep = proxyModel->data(clickedIndex).toString();
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
        int subC = dbMan->getSubnetPoolCount(2,Id,1);

        if(subC == 0)
        {
            int ipC = dbMan->getIpPoolCount(Id, 1);
            if(ipC > 0)
            {
                IpV4PoolDialog *ipV4PoolDialog = new IpV4PoolDialog(this,dbMan,1, Id,Subnet,Prefix,Mask,Bc,Host,Area,Abbr,Ap,Desc);
                ipV4PoolDialog->setAttribute(Qt::WA_DeleteOnClose);
                ipV4PoolDialog->showMaximized();
            }
        }
        else
        {
            QString subnet = Subnet +" / "+ QString::number(Prefix);
            QString temp;
            if((Area>1) && (Area<9))
                temp = "Area:"+QString::number(Area);
            else if(Area == 9)
                temp = "Karaj";

            if(!Abbr.isEmpty())
            {
                if(!temp.isEmpty())
                    temp = temp +"-";
                temp = temp + Abbr;
            }
            if(!Dep.isEmpty())
            {
                if(!temp.isEmpty())
                    temp = temp + "-";
                temp = temp + "Dep:"+Dep;
            }

            if(!Ap.isEmpty())
            {
                if(!temp.isEmpty())
                    temp = temp + "-";
                temp =  temp + Ap;
            }


            if(!Desc.isEmpty())
            {
                if(!temp.isEmpty())
                    temp = temp +"-";
                temp = temp + Desc;
            }

            Subnet2V4PoolDialog *subnet2V4PoolDialog = new Subnet2V4PoolDialog(this,dbMan,Id,subnet,temp);
            subnet2V4PoolDialog->setAttribute(Qt::WA_DeleteOnClose);
            subnet2V4PoolDialog->showMaximized();
        }
    }

}

void Subnet1V4PoolDialog::on_actionNetwork_Assignment_triggered()
{

    Subnet1V4AssignmentDialog *subnet1V4AssignmentDialog = new Subnet1V4AssignmentDialog(this,dbMan,Id,Subnet,Prefix, Area,ExchId,DepNo, Ap, Desc);
    subnet1V4AssignmentDialog->setAttribute(Qt::WA_DeleteOnClose);
    connect(subnet1V4AssignmentDialog, SIGNAL(finished(int)), this, SLOT(refreshCurrentAreaSlot(int)));
    subnet1V4AssignmentDialog->show();
}

void Subnet1V4PoolDialog::on_actionNetwork_Release_triggered()
{
    Subnet1V4ReleaseDialog *subnet1V4ReleaseDialog = new Subnet1V4ReleaseDialog(this,dbMan,Id,Subnet,Prefix,Mask,Bc,Host, Area,Abbr, Ap,Desc);
    subnet1V4ReleaseDialog->setAttribute(Qt::WA_DeleteOnClose);
    connect(subnet1V4ReleaseDialog, SIGNAL(finished(int)), this, SLOT(refreshCurrentAreaSlot(int)));
    subnet1V4ReleaseDialog->show();
}

void Subnet1V4PoolDialog::on_actionSplitNetwork_triggered()
{
    QString abbr;
    if( (Area > 2) && (Area < 9) )
        abbr = QString::number(Area)+"-"+Abbr;
    else if(Area == 9)
        abbr = QString("Karaj")+"-"+Abbr;
    else if(Area == 1)
        abbr = "General";


    SplitSubnet1V4Dialog *splitSubnet1V4Dialog = new SplitSubnet1V4Dialog(this,dbMan,netPoolId, Id,abbr,Subnet,Prefix, Area,DepNo, Ap, Desc);
    splitSubnet1V4Dialog->setAttribute(Qt::WA_DeleteOnClose);
    connect(splitSubnet1V4Dialog, SIGNAL(finished(int)), this, SLOT(refreshCurrentAreaSlot(int)));
    splitSubnet1V4Dialog->show();
}

void Subnet1V4PoolDialog::on_actionMergenets_triggered()
{
    QString abbr;
    if( (Area > 2) && (Area < 9) )
        abbr = QString::number(Area)+"-"+Abbr;
    else if(Area == 9)
        abbr = QString("Karaj")+"-"+Abbr;
    else if(Area == 1)
        abbr = "General";


    MergeSubnet1V4Dialog *mergeSubnet1V4Dialog = new MergeSubnet1V4Dialog(this, dbMan,Id,abbr,Ap,Subnet,Prefix);
    mergeSubnet1V4Dialog->setAttribute(Qt::WA_DeleteOnClose);
    connect(mergeSubnet1V4Dialog, SIGNAL(finished(int)), this, SLOT(refreshCurrentAreaSlot(int)));
    mergeSubnet1V4Dialog->show();
}

void Subnet1V4PoolDialog::on_actionView_Ip_Pool_triggered()
{
    IpV4PoolDialog *ipV4PoolDialog = new IpV4PoolDialog(this,dbMan,1, Id,Subnet,Prefix,Mask,Bc,Host,Area,Abbr,Ap,Desc);
    ipV4PoolDialog->setAttribute(Qt::WA_DeleteOnClose);
    ipV4PoolDialog->showMaximized();
}

void Subnet1V4PoolDialog::on_actionExport_PDF_triggered()
{
    QString path = QFileDialog::getSaveFileName(this, "Save DaNet Subnet Report As", "","pdf (*.pdf)");
    if(!path.isEmpty())
    {
        if(!path.endsWith(".pdf", Qt::CaseInsensitive)) path += ".pdf";


        ReportSubnetIpV4Plan reportSubnetIpPlan(this,proxyModel,1, path);
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

void Subnet1V4PoolDialog::on_actionRefresh_triggered()
{
    fillIpTV(currentArea);
}

void Subnet1V4PoolDialog::on_actionView_Subnet_Pool_triggered()
{
    QString subnet = Subnet +" / "+ QString::number(Prefix);
    QString temp;
    if((Area>1) && (Area<9))
        temp = "Area:"+QString::number(Area);
    else if(Area == 9)
        temp = "Karaj";

    if(!Abbr.isEmpty())
    {
        if(!temp.isEmpty())
            temp = temp +"-";
        temp = temp + Abbr;
    }
    if(!Dep.isEmpty())
    {
        if(!temp.isEmpty())
            temp = temp + "-";
        temp = temp + "Dep:"+Dep;
    }

    if(!Ap.isEmpty())
    {
        if(!temp.isEmpty())
            temp = temp + "-";
        temp =  temp + Ap;
    }


    if(!Desc.isEmpty())
    {
        if(!temp.isEmpty())
            temp = temp +"-";
        temp = temp + Desc;
    }


    Subnet2V4PoolDialog *subnet2V4PoolDialog = new Subnet2V4PoolDialog(this,dbMan,Id,subnet,temp);
    subnet2V4PoolDialog->setAttribute(Qt::WA_DeleteOnClose);
    subnet2V4PoolDialog->showMaximized();
}
