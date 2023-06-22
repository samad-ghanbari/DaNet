#include "dslamma5616widget.h"
#include "ui_dslamma5616widget.h"
#include "lib/database/danetdbman.h"
#include <QSqlQuery>
#include <QStringListModel>
#include <QSqlQueryModel>
#include "mainPanel/menu/plan/dslam/dslam5616/ma5616plandialog.h"
#include "mainPanel/menu/plan/dslam/dslam5616/ma5616aggmetroplandialog.h"
#include "mainPanel/menu/plan/dslam/dslam5616/ma5616brasplandialog.h"
#include "mainPanel/menu/plan/dslam/dslam5616/ma5616removeaggmetroplandialog.h"
#include "mainPanel/menu/plan/dslam/dslam5616/ma5616removebrasplandialog.h"
#include "mainPanel/menu/plan/dslam/dslam5616/ma5616removeplandialog.h"
#include "mainPanel/menu/plan/dslam/dslam5616/dslamma5616portdescdialog.h"
#include "mainPanel/menu/plan/dslam/dslamaddnewvlandialog.h"


DslamMa5616Widget::DslamMa5616Widget(QWidget *parent, DanetDbMan *Db, const int Area, const int ExchId, const int DslamId) :
    QWidget(parent),
    ui(new Ui::DslamMa5616Widget),
    dbMan(Db),
    portDescModel(new QSqlQueryModel(this)),
    brasModel(new QSqlQueryModel(this)),
    agg1AllowPassModel(new QStringListModel(this)),
    //agg1CxAllowPassModel(new QStringListModel(this)),
    agg1IntModel(new QStringListModel(this)),
    area(Area),
    exchId(ExchId),
    dslamId(DslamId)
{
    ui->setupUi(this);

    contextMenu.addAction(ui->actionEdit_Port_Description);

    dslamName = dbMan->getDevexDeviceName(dslamId);

    ui->portDescTV->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->portDescTV, SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(contextMenuSlot(QPoint)));

    fillForm(1);
}

DslamMa5616Widget::~DslamMa5616Widget()
{
    delete ui;
}

void DslamMa5616Widget::emptyValues()
{
    ui->typeLbl->clear();
    ui->purchaseLbl->clear();
    ui->snLbl->clear();

    //ui->vasLbl->clear();
    //ui->btvLbl->clear();
    //ui->ipMediaLbl->clear();
    ui->pppoeLbl->clear();
    ui->inetLbl->clear();
    ui->sinetLbl->clear();
    ui->voipLbl->clear();

    ui->gVlanLbl->clear();
    ui->gIpLbl->clear();
    ui->gBindLbl->clear();
    ui->gGwLbl->clear();

    ui->mVlanLbl->clear();
    ui->mIpLbl->clear();
    ui->mBindLbl->clear();
    ui->mGwLbl->clear();

    portDescModel->clear();

    ui->agg1Lbl->clear();
    ui->agg1EtherLbl->clear();
    agg1AllowPassModel->setStringList(QStringList());
    ui->agg1DescLbl->clear();


    ui->bras1Lbl->clear();
    ui->bras1Agg1EthLbl->clear();
    ui->bras1Agg2EthLbl->clear();
    brasModel->clear();

    ui->bras2Lbl->clear();
    ui->bras2Agg1EthLbl->clear();
    ui->bras2Agg2EthLbl->clear();


    ui->dslamPlanBtn->setEnabled(false);
    ui->aggMetroPlanBtn->setEnabled(false);
    ui->aggMetroPlanBtn->setVisible(false);
    ui->brasPlanBtn->setEnabled(false);
    ui->brasPlanBtn->setVisible(false);
    ui->removePlanBtn->setEnabled(false);

}

void DslamMa5616Widget::hideGB()
{
    //dslam
    ui->servicesGB->setEnabled(false);
    ui->servicesGB->setVisible(false);
    ui->dslamFrame->setVisible(false);
    ui->nmsGB->setEnabled(false);
    ui->nmsGB->setVisible(false);
    ui->portDescGB->setEnabled(false);
    ui->portDescGB->setVisible(false);

    //agg/metro
    ui->aggGB->setEnabled(false);
    ui->aggGB->setVisible(false);
    ui->aggMetroGB->setVisible(false);
    ui->tabWidget->setTabEnabled(1, false);

    //bras1
    ui->bras1GB->setEnabled(false);
    ui->bras1GB->setVisible(false);
    ui->bras2GB->setEnabled(false);
    ui->bras2GB->setVisible(false);
    ui->brasTV->setVisible(false);
    ui->brasGB->setVisible(false);


    ui->removePlanBtn->setVisible(false);
}

void DslamMa5616Widget::fillForm(int i)
{
    i++;
    emptyValues();
    hideGB();
    if(dslamId == -1)
        return;


    QList<int> cascadeNode = dbMan->getSiteCascadeNode(exchId);
    int node = cascadeNode.at(1);

    if(dbMan->dslamExistance(dslamId))
    {
        // fill
        fillFormDslamPlan(1);
        ui->tabWidget->setTabEnabled(1, true);
        if(node <= 1)
        {
            ui->aggMetroGB->setEnabled(true);
            ui->aggMetroGB->setVisible(true);
            fillFormAggMetroPlan(1);
            ui->tabWidget->setTabEnabled(1, true);
        }
        else
        {
            ui->aggMetroGB->setEnabled(false);
            ui->aggMetroGB->setVisible(false);
            ui->tabWidget->setTabEnabled(1, false);
        }
        fillFormBrasPlan(1);

        ui->removePlanBtn->setEnabled(true);
        ui->removePlanBtn->setVisible(true);
        ui->dslamAddVlanBtn->setEnabled(true);
        ui->dslamAddVlanBtn->setVisible(true);
    }
    else
    {
        ui->dslamPlanBtn->setEnabled(true);
        ui->removePlanBtn->setEnabled(false);
        ui->removePlanBtn->setVisible(false);
        ui->RemoveAggMetroPlanBtn->setEnabled(false);
        ui->RemoveAggMetroPlanBtn->setVisible(false);
        ui->removeBrasBtn->setEnabled(false);
        ui->removeBrasBtn->setVisible(false);
        ui->dslamAddVlanBtn->setEnabled(false);
        ui->dslamAddVlanBtn->setVisible(false);
    }
}

void DslamMa5616Widget::fillFormDslamPlan(int i)
{
    i++;
    ui->typeLbl->clear();
    ui->purchaseLbl->clear();
    ui->snLbl->clear();
    //ui->vasLbl->clear();
    //ui->btvLbl->clear();
    //ui->ipMediaLbl->clear();
    ui->pppoeLbl->clear();
    ui->inetLbl->clear();
    ui->sinetLbl->clear();
    ui->voipLbl->clear();
    ui->gVlanLbl->clear();
    ui->gIpLbl->clear();
    ui->gBindLbl->clear();
    ui->gGwLbl->clear();
    ui->mVlanLbl->clear();
    ui->mIpLbl->clear();
    ui->mBindLbl->clear();
    ui->mGwLbl->clear();
    portDescModel->clear();

    ui->servicesGB->setEnabled(false);
    ui->servicesGB->setVisible(false);
    ui->dslamFrame->setVisible(false);
    ui->nmsGB->setEnabled(false);
    ui->nmsGB->setVisible(false);
    ui->portDescGB->setEnabled(false);
    ui->portDescGB->setVisible(false);

    int SN = 1;
    QString PUR_ABBR = "", TYPE = "";

    if(dslamId == -1) return;

    QSqlQuery *query = dbMan->selectDslam(dslamId);
    //`type`, `purchase`, `sn`
    if(query->next())
    {
        TYPE = query->value(0).toString();
        if(TYPE.compare("L", Qt::CaseInsensitive) == 0) TYPE = "Local";
        else if(TYPE.compare("I", Qt::CaseInsensitive) == 0) TYPE = "Indoor";
        else if(TYPE.compare("O", Qt::CaseInsensitive) == 0) TYPE = "Outdoor";
        PUR_ABBR = query->value(1).toString();
        SN = query->value(2).toInt();
    }


    ui->typeLbl->setText(TYPE);
    ui->purchaseLbl->setText(PUR_ABBR);
    ui->snLbl->setText(QString::number(SN));

    if(dslamId > -1)
        ui->dslamPlanBtn->setEnabled(true);
    else
        ui->dslamPlanBtn->setEnabled(false);


    //VLAN
    query = dbMan->selectVlans(dslamId);
    //`id`,`vlan`,`description`
    if(query->size() > 0)
    {
        ui->servicesGB->setEnabled(true);
        ui->servicesGB->setVisible(true);
        ui->dslamFrame->setVisible(true);

        ui->aggMetroGB->setEnabled(true);
        ui->aggMetroGB->setVisible(true);
        ui->tabWidget->setTabEnabled(1, true);
        ui->aggMetroPlanBtn->setEnabled(true);
        ui->aggMetroPlanBtn->setVisible(true);

        ui->brasGB->setEnabled(true);
        ui->brasGB->setVisible(true);
        ui->brasPlanBtn->setEnabled(true);
        ui->brasPlanBtn->setVisible(true);

        ui->removePlanBtn->setEnabled(true);
        ui->removePlanBtn->setVisible(true);
    }
    else
    {
        ui->servicesGB->setEnabled(false);
        ui->servicesGB->setVisible(false);
        ui->dslamFrame->setVisible(false);

        ui->aggMetroGB->setEnabled(false);
        ui->aggMetroGB->setVisible(false);
        ui->tabWidget->setTabEnabled(1, false);

        ui->brasGB->setEnabled(false);
        ui->brasGB->setVisible(false);

        ui->removePlanBtn->setEnabled(false);
        ui->removePlanBtn->setVisible(false);
    }

    int value;

    QList<int>  btvList,ipMediaList, hsiList,inetList, sinetList, voipList;//vasList,


    while(query->next())
    {
        value = query->value(1).toInt();

//        if(value > 99 && value < 200)
//        {
//            //VAS
//            vasList << value;
//        }
/*         if(value > 299 && value < 400)
        {
            //IP MEDIA
            ipMediaList << value;
        }
        else if(value > 399 && value < 500)
        {
            //BTV
            btvList << value;
        }
        else */if(value > 499 && value < 600)
        {
            //PPPOE
            hsiList << value;
        }
        else if(value > 599 && value < 700)
        {
            //INET
            inetList << value;
        }
        else if(value > 699 && value < 800)
        {
            //SINET
            sinetList << value;
        }
        else if(value > 799 && value < 900)
        {
            //VOIP
            voipList << value;
        }
    }

    QString temp;
//    temp = dbMan->listToRange(vasList);
//    ui->vasLbl->setText(temp);

//    temp = dbMan->listToRange(btvList);
//    ui->btvLbl->setText(temp);

//    temp = dbMan->listToRange(ipMediaList);
//    ui->ipMediaLbl->setText(temp);

    temp = dbMan->listToRange(hsiList);
    ui->pppoeLbl->setText(temp);

    temp = dbMan->listToRange(inetList);
    ui->inetLbl->setText(temp);

    temp = dbMan->listToRange(sinetList);
    ui->sinetLbl->setText(temp);

    temp = dbMan->listToRange(voipList);
    ui->voipLbl->setText(temp);


    //NMS
    query = dbMan->selectDslamNMS(dslamId);
    if(query->size() > 0)
    {
        ui->nmsGB->setEnabled(true);
        ui->nmsGB->setVisible(true);
    }
    //`global_vlan`, `global_ip_pool_id`, `global_ipp`, `global_bind`, `global_gw`, `mpls_vlan`, `mpls_ip_pool_id`, `mpls_ipp`, `mpls_bind`, `mpls_gw`
    if(query->next())
    {
        int gVlan,mVlan;
        QString gIpp, gBind,gGw,mIpp, mBind,mGw;

        gVlan = query->value(0).toInt();
        gIpp = query->value(2).toString();
        gBind = query->value(3).toString();
        gGw = query->value(4).toString();
        mVlan = query->value(5).toInt();
        mIpp = query->value(7).toString();
        mBind = query->value(8).toString();
        mGw = query->value(9).toString();

        ui->gVlanLbl->setText(QString::number(gVlan));
        ui->gIpLbl->setText(gIpp);
        ui->gBindLbl->setText(gBind);
        ui->gGwLbl->setText(gGw);

        ui->mVlanLbl->setText(QString::number(mVlan));
        ui->mIpLbl->setText(mIpp);
        ui->mBindLbl->setText(mBind);
        ui->mGwLbl->setText(mGw);
    }

    //PORT
    query = dbMan->selectDslamInterfaceDesc(dslamId);
    if(query->size() > 0)
    {
        ui->portDescGB->setEnabled(true);
        ui->portDescGB->setVisible(true);
    }
    // `id`, `port`, `description`
    portDescModel->setQuery(*query);
    portDescModel->setHeaderData(1, Qt::Horizontal,"Port");
    portDescModel->setHeaderData(2, Qt::Horizontal,"Description");

    ui->portDescTV->setModel(portDescModel);
    ui->portDescTV->hideColumn(0);
    ui->portDescTV->verticalHeader()->hide();
    ui->portDescTV->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    /////////////////////////// ???????//////
    if(dbMan->getSiteNode(exchId) > 1)
        ui->aggMetroGB->hide();

}

void DslamMa5616Widget::fillFormAggMetroPlan(int i)
{
    i++;
    ui->agg1Lbl->clear();
    ui->agg1EtherLbl->clear();
    agg1IntModel->setStringList(QStringList());
    agg1AllowPassModel->setStringList(QStringList());
    ui->agg1DescLbl->clear();

    if(dslamId == -1) return;

    QSqlQuery *query = dbMan->selectAggDslam(dslamId);
    if(query->size() > 0)
    {
        ui->aggGB->setEnabled(true);
        ui->aggGB->setVisible(true);

        ui->RemoveAggMetroPlanBtn->setEnabled(true);
        ui->RemoveAggMetroPlanBtn->setVisible(true);
    }
    else
    {
        ui->aggGB->setEnabled(false);
        ui->aggGB->setVisible(false);

        ui->RemoveAggMetroPlanBtn->setEnabled(false);
        ui->RemoveAggMetroPlanBtn->setVisible(false);
    }
    //`agg1_id`, `agg2_id`, `agg1_port1_id`, `agg1_port2_id`, `agg2_port1_id`, `agg2_port2_id`,
    //`agg1`, `agg2`, `agg1_eth`, `agg2_eth`, `agg1_interface1`, `agg1_interface2`, `agg2_interface1`, `agg2_interface2`, `allow_pass`, `description`
    if(query->next())
    {
        QString agg, aggInt1,aggInt2;
        int eth;

        if(query->value(0).isNull())
        {
            //agg2
            agg = query->value(7).toString();

            eth = query->value(9).toInt();
            aggInt1 = query->value(12).toString();
            if(query->value(13).isNull())
                aggInt2 = "";
            else
                aggInt2 = query->value(13).toString();
        }
        else
        {
            //agg1
            agg = query->value(6).toString();

            eth = query->value(8).toInt();
            aggInt1 = query->value(10).toString();
            if(query->value(11).isNull())
                aggInt2 = "";
            else
                aggInt2 = query->value(11).toString();
        }


        QString ap = query->value(14).toString();
        QString desc = query->value(15).toString();
        QStringList list = ap.split(",", QString::SkipEmptyParts);
        QStringList aggIntList;
        if(aggInt2.isEmpty())
            aggIntList<< aggInt1;
        else
            aggIntList<< aggInt1 << aggInt2;

        ui->agg1Lbl->setText(agg);
        ui->agg1EtherLbl->setText(QString::number(eth));
        agg1IntModel->setStringList(aggIntList);
        ui->aggIntLV->setModel(agg1IntModel);
        agg1AllowPassModel->setStringList(list);
        ui->agg1AllowPassLV->setModel(agg1AllowPassModel);
        ui->agg1DescLbl->setText(desc);
    }

    /////////

    if(dbMan->dslamAggExistance(dslamId)) //cascade top node
    {
        //ui->agg1CxGB->setEnabled(true);
        //ui->agg1CxGB->setVisible(true);

        //int mother = dbMan->getSiteMotherId(exchId);
        //int agg1ShelfId = dbMan->getExchangeAgg1Id(mother);
        //int cxShelfId = dbMan->getExchangeCxId(mother);
        //QList<int> cxAggEth = dbMan->getCxAggEtherTrunks(cxShelfId, agg1ShelfId);
        //int cxEth, aggEth;
//        if(cxAggEth.count() > 1)
//        {
//            cxEth = cxAggEth.at(0);
//            aggEth = cxAggEth.at(1);
//        }
//        else
//        {
//            cxEth = 4;
//            aggEth = 19;
//        }

        //ui->cxAgg1EthLbl->setText(QString::number(cxEth));
        //ui->agg1CxEthLbl->setText(QString::number(aggEth));

//        QStringList metroAp = dbMan->getDslamMetroVlans(dslamId);
//        agg1CxAllowPassModel->setStringList(metroAp);
//        ui->agg1CxApLV->setModel(agg1CxAllowPassModel);

    }
    else
    {
        //ui->agg1CxGB->setEnabled(false);
        //ui->agg1CxGB->setVisible(false);
    }
}

void DslamMa5616Widget::fillFormBrasPlan(int i)
{
    i++;
    ui->bras1Lbl->clear();
    ui->bras1Agg1EthLbl->clear();
    ui->bras1Agg2EthLbl->clear();
    brasModel->clear();

    ui->bras2Lbl->clear();
    ui->bras2Agg1EthLbl->clear();
    ui->bras2Agg2EthLbl->clear();

    int motherId = dbMan->getSiteMotherId(exchId);
    if(motherId == 37) motherId =38;
    QString mother = dbMan->getExchangeAbbr(motherId);

    QString str1 = "Ether-Trunk to "+mother +" Agg1 :";
    QString str2 = "Ether-Trunk to "+mother +" Agg2 :";
    ui->b1a1Lbl->setText(str1);
    ui->b1a2Lbl->setText(str2);
    ui->b2a1Lbl->setText(str1);
    ui->b2a2Lbl->setText(str2);

    if(dbMan->dslamPppoeExistance(dslamId))
    {
        ui->brasGB->setVisible(true);
        ui->bras1GB->setEnabled(true);
        ui->bras1GB->setVisible(true);
        ui->brasTV->setVisible(true);
        ui->removeBrasBtn->setEnabled(true);
        ui->removeBrasBtn->setVisible(true);

        QList<int> brasList = dbMan->getDslamPppoeBrasId(dslamId);
        int bras1 = -1, bras2 = -1;
        if(brasList.count() > 1)
        {
            bras1 = brasList.at(0);
            bras2 = brasList.at(1);
        }

        QString name = dbMan->getDevexDeviceName(bras1);
        ui->bras1Lbl->setText(name);
        if(bras2 > -1)
        {
            ui->bras2GB->setEnabled(true);
            ui->bras2GB->setVisible(true);
            name = dbMan->getDevexDeviceName(bras2);
            ui->bras2Lbl->setText(name);
        }
        else
        {
            ui->bras2GB->setEnabled(false);
            ui->bras2GB->setVisible(false);

        }


        QList<int> list = dbMan->getBrasAggIdEth(bras1, motherId);
        //`agg1_id`,`agg1_eth`,`agg2_id`,`agg2_eth`
        if(list.count() > 3)
        {
            if(list.at(0) > -1)
            {
                ui->bras1Agg1EthLbl->setText(QString::number(list.at(1)));
                ui->bras1Agg2EthLbl->setText(QString::number(list.at(3)));
            }
        }

        list = dbMan->getBrasAggIdEth(bras2, motherId);
        //`agg1_id`,`agg1_eth`,`agg2_id`,`agg2_eth`
        if(list.count() > 3)
        {
            if(list.at(0) > -1)
            {
                ui->bras2Agg1EthLbl->setText(QString::number(list.at(1)));
                ui->bras2Agg2EthLbl->setText(QString::number(list.at(3)));
            }
        }


        QSqlQuery *query = dbMan->selectDslamPppoe(dslamId);
        // `vlan`, `description`
        brasModel->setQuery(*query);
        brasModel->setHeaderData(0, Qt::Horizontal, "Sub-Interface");
        brasModel->setHeaderData(1, Qt::Horizontal, "Description");
        ui->brasTV->setModel(brasModel);
        ui->brasTV->verticalHeader()->hide();
        ui->brasTV->horizontalHeader()->setStretchLastSection(true);


    }
    else
    {
        brasModel->clear();
        ui->brasGB->setVisible(true);
        ui->bras1GB->setEnabled(false);
        ui->bras1GB->setVisible(false);
        ui->bras2GB->setEnabled(false);
        ui->bras2GB->setVisible(false);
        ui->brasTV->setVisible(false);
        ui->removeBrasBtn->setEnabled(false);
        ui->removeBrasBtn->setVisible(false);
    }
}

void DslamMa5616Widget::refreshAggMetroSlot(int i)
{
    i++;
    fillFormAggMetroPlan(1);
    fillFormDslamPlan(1);
    emit finished(1);
}

void DslamMa5616Widget::refreshPortDescSlot(int i)
{
    i++;
    fillForm(1);
    ui->portDescTV->selectRow(clickedRow);
}

bool DslamMa5616Widget::cxAllowance(int val)
{
    if( (val >99 && val < 500) || (val > 999 && val < 4079) )
        return true;
    else
        return false;
}

void DslamMa5616Widget::on_dslamPlanBtn_clicked()
{
    MA5616PlanDialog *ma5616PlanDialog = new MA5616PlanDialog(this, dbMan,exchId, dslamId, dslamName);
    ma5616PlanDialog->setAttribute(Qt::WA_DeleteOnClose);
    connect(ma5616PlanDialog, SIGNAL(finished(int)), this, SLOT(fillForm(int)));
    connect(ma5616PlanDialog, SIGNAL(finished(int)), this, SIGNAL(dslamNameSignal(int)));
    ma5616PlanDialog->show();
}

void DslamMa5616Widget::on_aggMetroPlanBtn_clicked()
{
    MA5616AggMetroPlanDialog *ma5616AggMetroPlanDialog = new MA5616AggMetroPlanDialog(this, dbMan, exchId, dslamId);
    ma5616AggMetroPlanDialog->setAttribute(Qt::WA_DeleteOnClose);
    connect(ma5616AggMetroPlanDialog, SIGNAL(finished(int)), this, SLOT(refreshAggMetroSlot(int)));
    ma5616AggMetroPlanDialog->show();
}

void DslamMa5616Widget::on_RemoveAggMetroPlanBtn_clicked()
{
    MA5616RemoveAggMetroPlanDialog *ma5616RemoveAggMetroPlanDialog = new MA5616RemoveAggMetroPlanDialog(this,dbMan,dslamId,dslamName);
    ma5616RemoveAggMetroPlanDialog->setAttribute(Qt::WA_DeleteOnClose);
    connect(ma5616RemoveAggMetroPlanDialog, SIGNAL(finished(int)), this, SLOT(fillFormAggMetroPlan(int)));
    ma5616RemoveAggMetroPlanDialog->show();
}

void DslamMa5616Widget::on_brasPlanBtn_clicked()
{
    MA5616BrasPlanDialog *ma5616BrasPlanDialog = new MA5616BrasPlanDialog(this, dbMan, area, exchId, dslamId, dslamName);
    ma5616BrasPlanDialog->setAttribute(Qt::WA_DeleteOnClose);
    connect(ma5616BrasPlanDialog, SIGNAL(finished(int)), this, SLOT(fillFormBrasPlan(int)));
    ma5616BrasPlanDialog->show();
}

void DslamMa5616Widget::on_removeBrasBtn_clicked()
{
    MA5616RemoveBrasPlanDialog *ma5616RemoveBrasPlanDialog = new MA5616RemoveBrasPlanDialog(this,dbMan,dslamId,dslamName);
    ma5616RemoveBrasPlanDialog->setAttribute(Qt::WA_DeleteOnClose);
    connect(ma5616RemoveBrasPlanDialog, SIGNAL(finished(int)), this, SLOT(fillFormBrasPlan(int)));
    ma5616RemoveBrasPlanDialog->show();
}

void DslamMa5616Widget::on_removePlanBtn_clicked()
{
    MA5616RemovePlanDialog *ma5616RemovePlanDialog = new MA5616RemovePlanDialog(this,dbMan,dslamId,dslamName);
    ma5616RemovePlanDialog->setAttribute(Qt::WA_DeleteOnClose);
    connect(ma5616RemovePlanDialog, SIGNAL(finished(int)), this, SLOT(fillForm(int)));
    connect(ma5616RemovePlanDialog, SIGNAL(finished(int)), this, SIGNAL(dslamNameSignal(int)));
    ma5616RemovePlanDialog->show();
}

void DslamMa5616Widget::contextMenuSlot(QPoint ptr)
{
    //`id`, `interface`, `description`

    QModelIndex clickedIndex = ui->portDescTV->indexAt(ptr);
    clickedRow = clickedIndex.row();

    clickedIndex = clickedIndex.sibling(clickedIndex.row(), 0);
    clickedId = portDescModel->data(clickedIndex).toInt();
    clickedIndex = clickedIndex.sibling(clickedIndex.row(), 1);
    clickedInterface = portDescModel->data(clickedIndex).toString();
    clickedIndex = clickedIndex.sibling(clickedIndex.row(), 2);
    clickedDesc = portDescModel->data(clickedIndex).toString();


    if( (clickedId > 0) && (!clickedInterface.isEmpty()))
    {
        contextMenu.popup(ui->portDescTV->mapToGlobal(ptr));
    }
}

void DslamMa5616Widget::on_actionEdit_Port_Description_triggered()
{
    DslamMa5616PortDescDialog *dslamMa5616PortDescDialog = new DslamMa5616PortDescDialog(this, dbMan,dslamName, clickedId, clickedInterface, clickedDesc);
    dslamMa5616PortDescDialog->setAttribute(Qt::WA_DeleteOnClose);
    connect(dslamMa5616PortDescDialog, SIGNAL(finished(int)), this, SLOT(refreshPortDescSlot(int)));
    dslamMa5616PortDescDialog->show();
}

void DslamMa5616Widget::on_dslamAddVlanBtn_clicked()
{
    DslamAddNewVlanDialog *dslamAddNewVlanDialog = new DslamAddNewVlanDialog(this,dbMan,dslamId, dslamName);
    dslamAddNewVlanDialog->setAttribute(Qt::WA_DeleteOnClose);
    connect(dslamAddNewVlanDialog, SIGNAL(finished(int)), this, SLOT(fillForm(int)));
    dslamAddNewVlanDialog->show();
}
