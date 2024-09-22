#include "dslammxk819widget.h"
#include "ui_dslammxk819widget.h"
#include "lib/database/danetdbman.h"
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QStringListModel>
#include "mainPanel/menu/plan/dslam/dslammxk819/mxk819plandialog.h"
#include "mainPanel/menu/plan/dslam/dslammxk819/mxk819aggmetroplandialog.h"
#include "mainPanel/menu/plan/dslam/dslammxk819/mxk819singleaggmetroplandialog.h"
#include "mainPanel/menu/plan/dslam/dslammxk819/mxk819brasplandialog.h"
#include "mainPanel/menu/plan/dslam/dslammxk819/mxk819singleaggbrasplandialog.h"
#include "mainPanel/menu/plan/dslam/dslammxk819/mxk819removeplandialog.h"
#include "mainPanel/menu/plan/dslam/dslammxk819/mxk819removeaggmetroplandialog.h"
#include "mainPanel/menu/plan/dslam/dslammxk819/mxk819removebrasplandialog.h"
#include "mainPanel/menu/plan/dslam/dslammxk819/dslammxk819portdescdialog.h"
#include "mainPanel/menu/plan/dslam/dslamaddnewvlandialog.h"


DslamMxk819Widget::DslamMxk819Widget(QWidget *parent, DanetDbMan *Db, const int Area, const int ExchId, const int DslamId) :
    QWidget(parent),
    ui(new Ui::DslamMxk819Widget),
    dbMan(Db),
    portDescModel(new QSqlQueryModel(this)),
    brasModel(new QSqlQueryModel(this)),
    cxIntModel(new QStringListModel(this)),
    //cxAllowPassModel(new QStringListModel(this)),
    agg1IntModel(new QStringListModel(this)),
    agg1AllowPassModel(new QStringListModel(this)),
    agg2IntModel(new QStringListModel(this)),
    agg2AllowPassModel(new QStringListModel(this)),
    area(Area),
    exchId(ExchId),
    dslamId(DslamId)
{
    ui->setupUi(this);
    dslamName = dbMan->getDevexDeviceName(dslamId);

    int motherId = dbMan->getSiteMotherId(exchId);
    if(motherId == 37) motherId =38;
    singleAgg = dbMan->isSingleAggExchange(motherId);

    contextMenu.addAction(ui->actionEdit_Port_Description);

    ui->portDescTV->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->portDescTV, SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(contextMenuSlot(QPoint)));

    ui->dslamPlanBtn->setEnabled(false);
    ui->aggMetroPlanBtn->setEnabled(false);
    ui->aggMetroPlanBtn->setVisible(false);
    ui->brasPlanBtn->setEnabled(false);
    ui->brasPlanBtn->setVisible(false);
    ui->removePlanBtn->setEnabled(false);

    fillForm(1);
}

DslamMxk819Widget::~DslamMxk819Widget()
{
    delete ui;
}

void DslamMxk819Widget::emptyValues()
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
    //ui->ngnLbl->clear();

    ui->gVlanLbl->clear();
    ui->gIpLbl->clear();
    ui->gBindLbl->clear();
    ui->gGwLbl->clear();

    ui->mVlanLbl->clear();
    ui->mIpLbl->clear();
    ui->mBindLbl->clear();
    ui->mGwLbl->clear();

    portDescModel->clear();

    ui->cxLbl->clear();
    ui->cxEtherLbl->clear();
    cxIntModel->setStringList(QStringList());
    //cxAllowPassModel->setStringList(QStringList());
    ui->cxDescLbl->clear();

    ui->agg1Lbl->clear();
    ui->agg1EtherLbl->clear();
    agg1IntModel->setStringList(QStringList());
    agg1AllowPassModel->setStringList(QStringList());
    ui->agg1DescLbl->clear();

    ui->agg2Lbl->clear();
    ui->agg2EtherLbl->clear();
    agg2IntModel->setStringList(QStringList());
    agg2AllowPassModel->setStringList(QStringList());
    ui->agg2DescLbl->clear();

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

void DslamMxk819Widget::fillForm(int i)
{
    i++;
    emptyValues();
    hideGB();
    if(dbMan->dslamExistance(dslamId))
    {
        // fill
        fillFormDslamPlan(1);
        fillFormAggPlan(1);
        fillFormMetroPlan(1);
        fillFormBrasPlan(1);

        ui->removePlanBtn->setEnabled(true);
        ui->removePlanBtn->setVisible(true);
        ui->dslamAddVlanBtn->setEnabled(true);
        ui->dslamAddVlanBtn->setVisible(true);

        ui->tabWidget->setTabEnabled(1, true);
        QList<int> XSRRS = dbMan->getDevexXSRRS(dslamId);
        if( (dbMan->getSiteNode(exchId) > 1) || (XSRRS.at(4) >1) )
        {
            ui->tabWidget->setTabEnabled(1, false);
            ui->aggMetroGB->hide();
        }

        if( (dbMan->getSiteNode(exchId) == 0) && (!dbMan->isDslamUplinkShelf(dslamId)))
        {
            ui->tabWidget->setTabEnabled(1, false);
            ui->aggMetroGB->hide();
        }

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

void DslamMxk819Widget::fillFormDslamPlan(int i)
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
    //ui->ngnLbl->clear();
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

        ui->brasGB->setEnabled(false);
        ui->brasGB->setVisible(false);

        ui->removePlanBtn->setEnabled(false);
        ui->removePlanBtn->setVisible(false);
    }

    int value;
//vasList,
    QList<int>  btvList,ipMediaList, hsiList,inetList, sinetList, voipList;//, ngnList;

    while(query->next())
    {
        value = query->value(1).toInt();

//        if(value > 99 && value < 200)
//        {
//            //VAS
//            vasList << value;
//        }
//        if(value > 299 && value < 400)
//        {
//            //IP MEDIA
//            ipMediaList << value;
//        }
//        else if(value > 399 && value < 500)
//        {
//            //BTV
//            btvList << value;
//        }

        //else
        if(value > 499 && value < 600)
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

    //ui->ngnLbl->setText(QString::number(ngnList.at(0))+"-"+QString::number(ngnList.at(1)));


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

    if(hsiList.count() > 1) ZhoneUplink = true;else ZhoneUplink = false;

    ui->nmsGB->setTitle("ZMS");
    ui->globalVlanLbl->setText("TLS Bridge VLAN:");
    ui->mplsVlanLbl->setText("TLS Bridge VLAN:");

    //ui->vasVlanLbl->setText("Uplink Bridge");
    //ui->btvVlanLbl->setText("Uplink Bridge");
    //ui->ipMVlanLbl->setText("Uplink Bridge");
    ui->hsiVlanLbl->setText("Uplink Bridge");
    ui->inetVlanLbl->setText("Uplink Bridge");
    ui->sinetVlanLbl->setText("Uplink Bridge");
    ui->voipVlanLbl->setText("Uplink Bridge");
    //ui->ngnVlanLbl->setText("Uplink Bridge");



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

    ui->tabWidget->setTabEnabled(1, true);
    if( (dbMan->getSiteNode(exchId) > 1) )
    {
        ui->tabWidget->setTabEnabled(1, false);
        ui->aggMetroGB->hide();
    }
    if( (dbMan->getSiteNode(exchId) == 0) && (!dbMan->isDslamUplinkShelf(dslamId)))
    {
        ui->tabWidget->setTabEnabled(1, false);
        ui->aggMetroGB->hide();
    }
}

void DslamMxk819Widget::fillFormAggPlan(int i)
{
    i++;

    if(singleAgg)
    {
        ui->agg2GB->setEnabled(false);
        ui->agg2GB->setVisible(false);

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
            ui->agg1GB->setEnabled(true);
            ui->agg1GB->setVisible(true);
            ui->RemoveAggMetroPlanBtn->setEnabled(true);
            ui->RemoveAggMetroPlanBtn->setVisible(true);
        }
        else
        {
            ui->aggGB->setEnabled(false);
            ui->aggGB->setVisible(false);
            ui->agg1GB->setEnabled(false);
            ui->agg1GB->setVisible(false);
            ui->RemoveAggMetroPlanBtn->setEnabled(false);
            ui->RemoveAggMetroPlanBtn->setVisible(false);
        }

        //0 `agg1_id`, `agg2_id`, `agg1_int1_id`, `agg1_int2_id`, `agg2_int1_id`, `agg2_int2_id`,
        //6`agg1`, `agg2`, `agg1_eth`, `agg2_eth`, `agg1_interface1`, `agg1_interface2`,
        //12`agg2_interface1`, `agg2_interface2`, `allow_pass`, `description`, `agg1_int3_id`, `agg1_int4_id`,
        //18 `agg1_interface3`, `agg1_interface4`

        if(query->next())
        {
            bool a1 = !query->value(0).isNull();
            //bool a1p3 = !query->value(16).isNull();
            //bool a1p4 = !query->value(17).isNull();

            QString agg1 = (a1) ? query->value(6).toString() : "";

            int eth1 = (a1) ?  query->value(8).toInt() : -1;

            QStringList agg1Int;
            if(a1)
            {
                agg1Int << query->value(10).toString();//  a1p1
                agg1Int << query->value(11).toString(); // a1p2
                agg1Int << query->value(18).toString(); // a1p3
                agg1Int << query->value(19).toString(); // a1p4
            }



            QString ap = query->value(14).toString();
            QString desc = query->value(15).toString();
            QStringList list = ap.split(",", QString::SkipEmptyParts);

            if(a1)
            {
                ui->agg1Lbl->setText(agg1);
                ui->agg1EtherLbl->setText(QString::number(eth1));
                agg1IntModel->setStringList(agg1Int);
                ui->agg1IntLV->setModel(agg1IntModel);
                agg1AllowPassModel->setStringList(list);
                ui->agg1AllowPassLV->setModel(agg1AllowPassModel);
                ui->agg1DescLbl->setText(desc);
            }
            else
            {
                ui->agg1GB->setEnabled(false);
                ui->agg1GB->setVisible(false);
            }

        }
    }
    else
    {
        ui->agg1Lbl->clear();
        ui->agg1EtherLbl->clear();
        agg1IntModel->setStringList(QStringList());
        agg1AllowPassModel->setStringList(QStringList());
        ui->agg1DescLbl->clear();

        ui->agg2Lbl->clear();
        ui->agg2EtherLbl->clear();
        agg2IntModel->setStringList(QStringList());
        agg2AllowPassModel->setStringList(QStringList());
        ui->agg2DescLbl->clear();

        QSqlQuery *query = dbMan->selectAggDslam(dslamId);
        if(query->size() > 0)
        {
            ui->aggGB->setEnabled(true);
            ui->aggGB->setVisible(true);
            ui->agg1GB->setEnabled(true);
            ui->agg1GB->setVisible(true);
            ui->RemoveAggMetroPlanBtn->setEnabled(true);
            ui->RemoveAggMetroPlanBtn->setVisible(true);
        }
        else
        {
            ui->aggGB->setEnabled(false);
            ui->aggGB->setVisible(false);
            ui->agg1GB->setEnabled(false);
            ui->agg1GB->setVisible(false);
            ui->RemoveAggMetroPlanBtn->setEnabled(false);
            ui->RemoveAggMetroPlanBtn->setVisible(false);
        }
        //`agg1_id`, `agg2_id`, `agg1_port1_id`, `agg1_port2_id`, `agg2_port1_id`, `agg2_port2_id`,
        //`agg1`, `agg2`, `agg1_eth`, `agg2_eth`, `agg1_interface1`, `agg1_interface2`, `agg2_interface1`, `agg2_interface2`, `allow_pass`, `description`
        if(query->next())
        {
            bool a2 = !query->value(1).isNull();
            bool a1p2 = !query->value(3).isNull();
            bool a2p2 = !query->value(5).isNull();

            QString agg1 = query->value(6).toString();
            QString agg2 = (a2) ? query->value(7).toString() : "";
            int eth1 = query->value(8).toInt();
            int eth2 = (a2) ? query->value(9).toInt() : -1;
            QStringList agg1Int,agg2Int;
            agg1Int << query->value(10).toString();
            if(a1p2)
                agg1Int << query->value(11).toString();
            if(a2)
            {
                agg2Int << query->value(12).toString();
                if(a2p2)
                    agg2Int << query->value(13).toString();
            }


            QString ap = query->value(14).toString();
            QString desc = query->value(15).toString();
            QStringList list = ap.split(",", QString::SkipEmptyParts);

            ui->agg1Lbl->setText(agg1);
            ui->agg1EtherLbl->setText(QString::number(eth1));
            agg1IntModel->setStringList(agg1Int);
            ui->agg1IntLV->setModel(agg1IntModel);
            agg1AllowPassModel->setStringList(list);
            ui->agg1AllowPassLV->setModel(agg1AllowPassModel);
            ui->agg1DescLbl->setText(desc);

            if(a2)
            {
                ui->agg2GB->setEnabled(true);
                ui->agg2GB->setVisible(true);

                ui->agg2Lbl->setText(agg2);
                ui->agg2EtherLbl->setText(QString::number(eth2));
                agg2IntModel->setStringList(agg2Int);
                ui->agg2IntLV->setModel(agg2IntModel);
                agg2AllowPassModel->setStringList(agg2Int);
                agg2AllowPassModel->setStringList(list);
                ui->agg2AllowPassLV->setModel(agg2AllowPassModel);
                ui->agg2DescLbl->setText(desc);
            }
            else
            {
                ui->agg2GB->setEnabled(false);
                ui->agg2GB->setVisible(false);
            }
        }
    }


}

void DslamMxk819Widget::fillFormMetroPlan(int i)
{
    i++;
    ui->cxLbl->clear();
    ui->cxEtherLbl->clear();
    cxIntModel->setStringList(QStringList());
    //cxAllowPassModel->setStringList(QStringList());
    ui->cxDescLbl->clear();

    QSqlQuery *query = dbMan->selectCxDslam(dslamId);
    if(query->size() > 0)
    {
        ui->metroGB->setEnabled(true);
        ui->metroGB->setVisible(true);
        //ui->agg1CxGB->setEnabled(false);
        //ui->agg1CxGB->setVisible(false);

        //`dslam_id`, `cx_id`, `port1_id`, `port2_id`, `cx`, `ether_trunk`, `interface1`, `interface2`, `allow_pass`, `description`
        if(query->next())
        {
            bool cxp2 = !query->value(3).isNull();

            QString cx = query->value(4).toString();
            ui->cxLbl->setText(cx);
            int eth = query->value(5).toInt();
            ui->cxEtherLbl->setText(QString::number(eth));
            QStringList intList;
            intList << query->value(6).toString();
            if(cxp2)
                intList << query->value(7).toString();
            cxIntModel->setStringList(intList);
            ui->cxIntLV->setModel(cxIntModel);
            QString ap = query->value(8).toString();
            QStringList apList = ap.split(",", QString::SkipEmptyParts);
            //cxAllowPassModel->setStringList(apList);
            //ui->cxAllowPassLV->setModel(cxAllowPassModel);
            QString desc = query->value(9).toString();
            ui->cxDescLbl->setText(desc);
        }
    }
    else
    {
        //no CX connection
        ui->metroGB->setEnabled(false);
        ui->metroGB->setVisible(false);
//        if(dbMan->dslamAggExistance(dslamId)) //cascade top node
//        {
//            ui->agg1CxGB->setEnabled(true);
//            ui->agg1CxGB->setVisible(true);
//        }
//        else
//        {
//            ui->agg1CxGB->setEnabled(false);
//            ui->agg1CxGB->setVisible(false);
//        }

        //int agg1dslamId = dbMan->getExchangeAgg1Id(exchId);
        //int cxdslamId = dbMan->getExchangeCxId(exchId);
//        QList<int> cxAggEth = dbMan->getCxAggEtherTrunks(cxdslamId, agg1dslamId);
//        int cxEth, aggEth;
//        if(cxAggEth.at(0) > -1)
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

        //QStringList metroAp = dbMan->getDslamMetroVlans(dslamId);
        //cxAllowPassModel->setStringList(metroAp);
        //ui->agg1CxApLV->setModel(cxAllowPassModel);
    }
}

void DslamMxk819Widget::fillFormBrasPlan(int i)
{
    i++;
    if(singleAgg)
    {
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
        //QString str2 = "Ether-Trunk to "+mother +" Agg2 :";
        ui->b1a1Lbl->setText(str1);
        //ui->b1a2Lbl->setText(str2);
        ui->b2a1Lbl->setText(str1);
        //ui->b2a2Lbl->setText(str2);

        if(dbMan->dslamPppoeExistance(dslamId))
        {
            ui->brasGB->setVisible(true);
            ui->bras1GB->setEnabled(true);
            ui->bras1GB->setVisible(true);
            ui->brasTV->setVisible(true);
            ui->removeBrasBtn->setEnabled(true);
            ui->removeBrasBtn->setVisible(true);

            QList<int> brasList = dbMan->getDslamPppoeBrasId(dslamId);
            int bras1 = brasList.at(0);
            int bras2 = brasList.at(1);

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
            if(list.at(0) > -1)
            {
                ui->bras1Agg1EthLbl->setText(QString::number(list.at(1)));
                //ui->bras1Agg2EthLbl->setText(QString::number(list.at(3)));
            }

            list = dbMan->getBrasAggIdEth(bras2, motherId);
            //`agg1_id`,`agg1_eth`,`agg2_id`,`agg2_eth`
            if(list.at(0) > -1)
            {
                ui->bras2Agg1EthLbl->setText(QString::number(list.at(1)));
                //ui->bras2Agg2EthLbl->setText(QString::number(list.at(3)));
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
    else
    {
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
            int bras1 = brasList.at(0);
            int bras2 = brasList.at(1);

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
            if(list.at(0) > -1)
            {
                ui->bras1Agg1EthLbl->setText(QString::number(list.at(1)));
                ui->bras1Agg2EthLbl->setText(QString::number(list.at(3)));
            }

            list = dbMan->getBrasAggIdEth(bras2, motherId);
            //`agg1_id`,`agg1_eth`,`agg2_id`,`agg2_eth`
            if(list.at(0) > -1)
            {
                ui->bras2Agg1EthLbl->setText(QString::number(list.at(1)));
                ui->bras2Agg2EthLbl->setText(QString::number(list.at(3)));
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

}

void DslamMxk819Widget::hideGB()
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
    ui->metroGB->setEnabled(false);
    ui->metroGB->setVisible(false);

    //bras1
    ui->bras1GB->setEnabled(false);
    ui->bras1GB->setVisible(false);
    ui->bras2GB->setEnabled(false);
    ui->bras2GB->setVisible(false);
    ui->brasTV->setVisible(false);
    ui->brasGB->setVisible(false);

    ui->aggMetroGB->setVisible(false);

    ui->removePlanBtn->setVisible(false);

}

bool DslamMxk819Widget::cxAllowance(int val)
{
    if( (val >99 && val < 500) || (val > 999 && val < 4079) )
        return true;
    else
        return false;
}

void DslamMxk819Widget::refreshAggMetroSlot(int i)
{
    i++;
    fillFormAggPlan(1);
    fillFormMetroPlan(1);
    fillFormDslamPlan(1);
    emit finished(1);
}

void DslamMxk819Widget::refreshPortDescSlot(int i)
{
    fillForm(i);
    ui->portDescTV->selectRow(clickedRow);
}

void DslamMxk819Widget::on_dslamPlanBtn_clicked()
{
    Mxk819PlanDialog *mxk819PlanDialog = new Mxk819PlanDialog(this, dbMan,exchId, dslamId, dslamName);
    mxk819PlanDialog->setAttribute(Qt::WA_DeleteOnClose);
    connect(mxk819PlanDialog, SIGNAL(finished(int)), this, SLOT(fillForm(int)));
    connect(mxk819PlanDialog, SIGNAL(finished(int)), this, SIGNAL(dslamNameSignal(int)));
    mxk819PlanDialog->show();
}

void DslamMxk819Widget::on_aggMetroPlanBtn_clicked()
{
    if(singleAgg)
    {
        Mxk819SingleAggMetroPlanDialog *mxk819AggMetroPlanDialog = new Mxk819SingleAggMetroPlanDialog(this, dbMan, exchId, dslamId);
        mxk819AggMetroPlanDialog->setAttribute(Qt::WA_DeleteOnClose);
        connect(mxk819AggMetroPlanDialog, SIGNAL(finished(int)), this, SLOT(refreshAggMetroSlot(int)));
        mxk819AggMetroPlanDialog->show();
    }
    else
    {
        Mxk819AggMetroPlanDialog *mxk819AggMetroPlanDialog = new Mxk819AggMetroPlanDialog(this, dbMan, exchId, dslamId);
        mxk819AggMetroPlanDialog->setAttribute(Qt::WA_DeleteOnClose);
        connect(mxk819AggMetroPlanDialog, SIGNAL(finished(int)), this, SLOT(refreshAggMetroSlot(int)));
        mxk819AggMetroPlanDialog->show();
    }

}

void DslamMxk819Widget::on_brasPlanBtn_clicked()
{
    if(singleAgg)
    {

        Mxk819SingleAggBrasPlanDialog *mxk819BrasPlanDialog = new Mxk819SingleAggBrasPlanDialog(this, dbMan, area, exchId, dslamId, dslamName);
        mxk819BrasPlanDialog->setAttribute(Qt::WA_DeleteOnClose);
        connect(mxk819BrasPlanDialog, SIGNAL(finished(int)), this, SLOT(fillFormBrasPlan(int)));
        mxk819BrasPlanDialog->show();
    }
    else
    {

        Mxk819BrasPlanDialog *mxk819BrasPlanDialog = new Mxk819BrasPlanDialog(this, dbMan, area, exchId, dslamId, dslamName);
        mxk819BrasPlanDialog->setAttribute(Qt::WA_DeleteOnClose);
        connect(mxk819BrasPlanDialog, SIGNAL(finished(int)), this, SLOT(fillFormBrasPlan(int)));
        mxk819BrasPlanDialog->show();
    }
}

void DslamMxk819Widget::on_removePlanBtn_clicked()
{
    Mxk819RemovePlanDialog *mxk819RemovePlanDialog = new Mxk819RemovePlanDialog(this,dbMan,dslamId,dslamName);
    connect(mxk819RemovePlanDialog, SIGNAL(finished(int)), this, SLOT(fillForm(int)));
    connect(mxk819RemovePlanDialog, SIGNAL(finished(int)), this, SIGNAL(dslamNameSignal(int)));
    mxk819RemovePlanDialog->show();
}

void DslamMxk819Widget::on_RemoveAggMetroPlanBtn_clicked()
{
    Mxk819RemoveAggMetroPlanDialog *mxk819RemoveAggMetroPlanDialog = new Mxk819RemoveAggMetroPlanDialog(this,dbMan,dslamId,dslamName);
    mxk819RemoveAggMetroPlanDialog->setAttribute(Qt::WA_DeleteOnClose);
    connect(mxk819RemoveAggMetroPlanDialog, SIGNAL(finished(int)), this, SLOT(refreshAggMetroSlot(int)));
    mxk819RemoveAggMetroPlanDialog->show();
}

void DslamMxk819Widget::on_removeBrasBtn_clicked()
{
    Mxk819RemoveBrasPlanDialog *mxk819RemoveBrasPlanDialog = new Mxk819RemoveBrasPlanDialog(this,dbMan,dslamId,dslamName);
    mxk819RemoveBrasPlanDialog->setAttribute(Qt::WA_DeleteOnClose);
    connect(mxk819RemoveBrasPlanDialog, SIGNAL(finished(int)), this, SLOT(fillFormBrasPlan(int)));
    mxk819RemoveBrasPlanDialog->show();
}

void DslamMxk819Widget::contextMenuSlot(QPoint ptr)
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

void DslamMxk819Widget::on_actionEdit_Port_Description_triggered()
{
     DslamMxk819PortDescDialog *dslamMxk819PortDescDialog = new DslamMxk819PortDescDialog(this, dbMan,dslamName, clickedId, clickedInterface, clickedDesc);
     dslamMxk819PortDescDialog->setAttribute(Qt::WA_DeleteOnClose);
    connect(dslamMxk819PortDescDialog, SIGNAL(finished(int)), this, SLOT(refreshPortDescSlot(int)));
     dslamMxk819PortDescDialog->show();
}

void DslamMxk819Widget::on_dslamAddVlanBtn_clicked()
{
    DslamAddNewVlanDialog *dslamAddNewVlanDialog = new DslamAddNewVlanDialog(this,dbMan,dslamId, dslamName);
    dslamAddNewVlanDialog->setAttribute(Qt::WA_DeleteOnClose);
    connect(dslamAddNewVlanDialog, SIGNAL(finished(int)), this, SLOT(fillForm(int)));
    dslamAddNewVlanDialog->show();
}
