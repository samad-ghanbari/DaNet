#include "reportdslamdialog.h"
#include "ui_reportdslamdialog.h"
#include "lib/database/danetdbman.h"
#include "lib/report/DSLAM/ReportDSLAM.h"
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QStringListModel>
#include <QFileDialog>
#include <QFile>
#include <QDir>
#include <QMessageBox>
#include <QDesktopServices>

ReportDSLAMDialog::ReportDSLAMDialog(QWidget *parent, DanetDbMan *db) :
    QDialog(parent),
    ui(new Ui::ReportDSLAMDialog),
    dbMan(db),
    portDescModel(new QSqlQueryModel(this)),
    brasModel(new QSqlQueryModel(this)),
    cxIntModel(new QStringListModel(this)),
    //cxAllowPassModel(new QStringListModel(this)),
    agg1IntModel(new QStringListModel(this)),
    agg1AllowPassModel(new QStringListModel(this)),
    agg2IntModel(new QStringListModel(this)),
    agg2AllowPassModel(new QStringListModel(this))
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowMinMaxButtonsHint);

    singleAgg = false;

    ui->toolBox->setCurrentIndex(0);

    //area
    ui->areaCB->blockSignals(true);
    ui->areaCB->addItem("2", 2);
    ui->areaCB->addItem("3", 3);
    ui->areaCB->addItem("4", 4);
    ui->areaCB->addItem("5", 5);
    ui->areaCB->addItem("6", 6);
    ui->areaCB->addItem("7", 7);
    ui->areaCB->addItem("8", 8);
    ui->areaCB->addItem("Karaj", 9);
    ui->areaCB->setCurrentIndex(-1);
    ui->areaCB->blockSignals(false);

    //type
    ui->typeCB->blockSignals(true);
    ui->typeCB->addItem("Exchange", 2);
    ui->typeCB->addItem("Site", 3);
    ui->typeCB->setCurrentIndex(-1);
    ui->typeCB->blockSignals(false);

    int workingArea = dbMan->getLoggedInDefaultArea();
    ui->areaCB->setCurrentIndex(ui->areaCB->findData(workingArea));

    dslamId = -1;
    ui->okBtn->setEnabled(false);
    ui->tabWidget->setEnabled(false);

}

ReportDSLAMDialog::~ReportDSLAMDialog()
{
    delete ui;
}

void ReportDSLAMDialog::emptyValues()
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


    serviceMap.clear();
    portMap.clear();
    nmsMap.clear();
    brasInfo.clear();
    agg1List.clear();
    agg2List.clear();
    aggInfo.clear();
    bras1List.clear();
    bras2List.clear();
    cxList.clear();
    //agg1CxList.clear();

    ui->okBtn->setEnabled(false);
}

void ReportDSLAMDialog::fillForm()
{
    emptyValues();
    ui->toolBox->setCurrentIndex(0);
    dslamId = ui->dslamCB->currentData().toInt();
    hideGB();
    if(dbMan->dslamExistance(dslamId))
    {
        int exchId = ui->abbrCB->currentData().toInt();
        singleAgg = dbMan->isSingleAggExchange(exchId);
        if(ui->typeCB->currentData().toInt() == 3)
            exchId = ui->siteCB->currentData().toInt();

        // fill
        fillFormDslamPlan();
        fillFormAggPlan();
        fillFormMetroPlan();
        fillFormBrasPlan();

        ui->tabWidget->setTabEnabled(1, true);
        if( (dbMan->getSiteNode(exchId) > 1) || (!dbMan->isDslamUplinkShelf(dslamId)) )
        {
            ui->tabWidget->setTabEnabled(1, false);
            ui->aggMetroGB->hide();
        }

        ui->okBtn->setEnabled(true);

    }
    else
    {
        ui->okBtn->setEnabled(false);
    }
}

void ReportDSLAMDialog::fillFormDslamPlan()
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

    ui->dslamGB->setVisible(false);
    ui->servicesGB->setEnabled(false);
    ui->servicesGB->setVisible(false);
    ui->nmsGB->setEnabled(false);
    ui->nmsGB->setVisible(false);
    ui->portDescGB->setEnabled(false);
    ui->portDescGB->setVisible(false);

    int SN = 1;
    QString PUR_ABBR = "", TYPE = "";
    dslamId = ui->dslamCB->currentData().toInt();
    dslamName = dbMan->getDevexDeviceName(dslamId);


    dslamName = dslamName.replace("MA5600", "5600");
    dslamName = dslamName.replace("MA5616", "5616");

    QSqlQuery *query = dbMan->selectDslam(dslamId);
    //`type`, `purchase`, `sn`
    if(query->next())
    {
        TYPE = query->value(0).toString();
        dslamName += "-"+TYPE;
        if(TYPE.compare("L", Qt::CaseInsensitive) == 0) TYPE = "Local";
        else if(TYPE.compare("I", Qt::CaseInsensitive) == 0) TYPE = "Indoor";
        else if(TYPE.compare("O", Qt::CaseInsensitive) == 0) TYPE = "Outdoor";
        PUR_ABBR = query->value(1).toString();
        dslamName += "-"+PUR_ABBR;
        SN = query->value(2).toInt();
        if(SN < 10)
            dslamName += "-0"+QString::number(SN);
        else
            dslamName += "-"+QString::number(SN);

        dslamName = dslamName.replace(" ", "");
    }


    ui->typeLbl->setText(TYPE);
    ui->purchaseLbl->setText(PUR_ABBR);
    ui->snLbl->setText(QString::number(SN));

    //VLAN
    query = dbMan->selectVlans(dslamId);
    //`id`,`vlan`,`description`
    if(query->size() > 0)
    {
        ui->dslamGB->setVisible(true);
        ui->servicesGB->setEnabled(true);
        ui->servicesGB->setVisible(true);

        ui->aggMetroGB->setEnabled(true);
        ui->aggMetroGB->setVisible(true);

        ui->brasGB->setEnabled(true);
        ui->brasGB->setVisible(true);
    }
    else
    {
        ui->dslamGB->setVisible(false);
        ui->servicesGB->setEnabled(false);
        ui->servicesGB->setVisible(false);

        ui->aggMetroGB->setEnabled(false);
        ui->aggMetroGB->setVisible(false);

        ui->brasGB->setEnabled(false);
        ui->brasGB->setVisible(false);
    }

    int value;

    QList<int> btvList,ipMediaList, hsiList,inetList, sinetList, voipList;//, ngnList;

    while(query->next())
    {
        value = query->value(1).toInt();

//        if(value > 99 && value < 200)
//        {
//            //VAS
//            vasList << value;
//        }
/*        if(value > 299 && value < 400)
        {
            //IP MEDIA
            ipMediaList << value;
        }
        else if(value > 399 && value < 500)
        {
            //BTV
            btvList << value;
        }
        else */
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
//        else if(value > 3999 && value < 4079)
//        {
//            //NGN
//            ngnList << value;
//        }
    }

    QString temp;
//    temp = dbMan->listToRange(vasList);
//    ui->vasLbl->setText(temp);
//    serviceMap.insert(100,temp);

//    temp = dbMan->listToRange(ipMediaList);
//    ui->ipMediaLbl->setText(temp);
//    serviceMap.insert(300,temp);

//    temp = dbMan->listToRange(btvList);
//    ui->btvLbl->setText(temp);
//    serviceMap.insert(400,temp);

    temp = dbMan->listToRange(hsiList);
    ui->pppoeLbl->setText(temp);
    serviceMap.insert(500,temp);

    temp = dbMan->listToRange(inetList);
    ui->inetLbl->setText(temp);
    serviceMap.insert(600,temp);

    temp = dbMan->listToRange(sinetList);
    ui->sinetLbl->setText(temp);
    serviceMap.insert(700,temp);

    temp = dbMan->listToRange(voipList);
    ui->voipLbl->setText(temp);
    serviceMap.insert(800,temp);


//    temp = QString::number(ngnList.at(0))+"-"+QString::number(ngnList.at(1));
//    ui->ngnLbl->setText(temp);
//    serviceMap.insert(4000,temp);


    //NMS
    query = dbMan->selectDslamNMS(dslamId);

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

        nmsMap.insert(gVlan, QStringList() << gIpp << gGw);

        ui->mVlanLbl->setText(QString::number(mVlan));
        ui->mIpLbl->setText(mIpp);
        ui->mBindLbl->setText(mBind);
        ui->mGwLbl->setText(mGw);

        nmsMap.insert(mVlan, QStringList() << mIpp << mGw);
    }

    if(query->size() > 0)
    {
        ui->nmsGB->setEnabled(true);
        ui->nmsGB->setVisible(true);
    }

    bool HUAWEIDSLAM = dbMan->isDevexHuaweiVendor(ui->dslamCB->currentData().toInt());

    if(HUAWEIDSLAM)
    {
        ZhoneUplink = false;

        ui->nmsGB->setTitle("NMS");
        ui->globalVlanLbl->setText("Standard VLAN:");
        ui->mplsVlanLbl->setText("Standard VLAN:");

        //ui->vasVlanLbl->setText("Smart VLAN");
        //ui->btvVlanLbl->setText("Smart VLAN");
        //ui->ipMVlanLbl->setText("Smart VLAN");
        ui->hsiVlanLbl->setText("Smart VLAN");
        ui->inetVlanLbl->setText("Smart VLAN");
        ui->sinetVlanLbl->setText("Smart VLAN");
        ui->voipVlanLbl->setText("Smart VLAN");
        //ui->ngnVlanLbl->setText("Smart VLAN");
    }
    else
    {
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

    QString p, d;
    while(query->next())
    {
        p = query->value(1).toString();
        d = query->value(2).toString();
        portMap.insert(p,d);
    }

    int exchId = ui->abbrCB->currentData().toInt();
    if(ui->typeCB->currentData().toInt() == 3)
        exchId = ui->siteCB->currentData().toInt();

    ui->tabWidget->setTabEnabled(1, true);
    if( (dbMan->getSiteNode(exchId) > 1) || (!dbMan->isDslamUplinkShelf(dslamId)) )
    {
        ui->tabWidget->setTabEnabled(1, false);
        ui->aggMetroGB->hide();
    }
}

void ReportDSLAMDialog::fillFormAggPlan()
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

    dslamId = ui->dslamCB->currentData().toInt();
    QSqlQuery *query = dbMan->selectAggDslam(dslamId);
    if(query->size() > 0)
    {
        ui->aggGB->setEnabled(true);
        ui->aggGB->setVisible(true);
        ui->agg1GB->setEnabled(true);
        ui->agg1GB->setVisible(true);

    }
    else
    {
        ui->aggGB->setEnabled(false);
        ui->aggGB->setVisible(false);
        ui->agg1GB->setEnabled(false);
        ui->agg1GB->setVisible(false);
    }
    // `agg1_id`, `agg2_id`, `agg1_int1_id`, 3`agg1_int2_id`, 4`agg2_int1_id`, 5`agg2_int2_id`, 6`agg1`, 7`agg2`, 8`agg1_eth`, 9`agg2_eth`, 10`agg1_interface1`, 11`agg1_interface2`,
    // 12`agg2_interface1`, 13`agg2_interface2`, 14`allow_pass`, 15`description`, 16`agg1_int3_id`, 17`agg1_int4_id`, 18`agg1_interface3`, 19`agg1_interface4`
    if(query->next())
    {
        bool a1 = !query->value(0).isNull();
        bool a2 = !query->value(1).isNull();
        bool a1p2 = !query->value(3).isNull();
        bool a1p3 = !query->value(16).isNull();
        bool a1p4 = !query->value(17).isNull();
        bool a2p2 = !query->value(5).isNull();

        QString agg1 = query->value(6).toString();
        agg1 = agg1.replace(" ", "");
        QString agg2 = (a2) ? query->value(7).toString() : "";
        agg2 = agg2.replace(" ", "");

        int eth1 = query->value(8).toInt();
        int eth2 = (a2) ? query->value(9).toInt() : -1;
        QStringList agg1Int,agg2Int;
        agg1Int << query->value(10).toString();
        if(a1p2)
            agg1Int << query->value(11).toString();
        if(a1p3)
            agg1Int << query->value(18).toString();
        if(a1p4)
            agg1Int << query->value(19).toString();

        if(a2)
        {
            agg2Int << query->value(12).toString();
            if(a2p2)
                agg2Int << query->value(13).toString();
        }

        agg1List << agg1 << QString::number(eth1)<<query->value(10).toString()<<query->value(11).toString()<<query->value(18).toString()<<query->value(19).toString();
        agg2List << agg2 <<QString::number(eth2) <<query->value(12).toString() <<query->value(13).toString();

        QString ap = query->value(14).toString();
        QString desc = query->value(15).toString();

        aggInfo << ap << desc;

        QStringList list = ap.split(" ", QString::SkipEmptyParts);

        if(a1)
        {
            ui->agg1GB->setEnabled(true);
            ui->agg1GB->setVisible(true);

            ui->agg1Lbl->setText(agg1);
            ui->agg1EtherLbl->setText(QString::number(eth1));
            agg1IntModel->setStringList(agg1Int);
            ui->agg1IntLV->setModel(agg1IntModel);
            agg1AllowPassModel->setStringList(agg1Int);
            agg1AllowPassModel->setStringList(list);
            ui->agg1AllowPassLV->setModel(agg1AllowPassModel);
            ui->agg1DescLbl->setText(desc);
        }
        else
        {
            ui->agg1GB->setEnabled(false);
            ui->agg1GB->setVisible(false);
        }

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

void ReportDSLAMDialog::fillFormMetroPlan()
{
    ui->cxLbl->clear();
    ui->cxEtherLbl->clear();
    cxIntModel->setStringList(QStringList());
    //cxAllowPassModel->setStringList(QStringList());
    ui->cxDescLbl->clear();

//    int exchId;
//    if(ui->typeCB->currentData().toInt() == 2)//exch
//        exchId = ui->abbrCB->currentData().toInt();
//    else
//        exchId = ui->siteCB->currentData().toInt();

    dslamId = ui->dslamCB->currentData().toInt();
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
            cx = cx.replace(" ", "");
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
            QStringList apList = ap.split(" ", QString::SkipEmptyParts);
            //cxAllowPassModel->setStringList(apList);
            //ui->cxAllowPassLV->setModel(cxAllowPassModel);
            QString desc = query->value(9).toString();
            ui->cxDescLbl->setText(desc);

            cxList << cx << QString::number(eth) << query->value(6).toString() << query->value(7).toString() << ap << desc;
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

//        int agg1dslamId = dbMan->getExchangeAgg1Id(exchId);
//        int cxdslamId = dbMan->getExchangeCxId(exchId);
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

//        QStringList metroAp = dbMan->getDslamMetroVlans(dslamId);
//        cxAllowPassModel->setStringList(metroAp);
//        ui->agg1CxApLV->setModel(cxAllowPassModel);

//        agg1CxList << QString::number(aggEth) << QString::number(cxEth) << metroAp.join(' ');
    }
}

void ReportDSLAMDialog::fillFormBrasPlan()
{
    ui->bras1Lbl->clear();
    ui->bras1Agg1EthLbl->clear();
    ui->bras1Agg2EthLbl->clear();
    brasModel->clear();

    ui->bras2Lbl->clear();
    ui->bras2Agg1EthLbl->clear();
    ui->bras2Agg2EthLbl->clear();

    int exchId = ui->abbrCB->currentData().toInt();
    if(ui->typeCB->currentData().toInt() == 3)
        exchId = ui->siteCB->currentData().toInt();

    int motherId = dbMan->getSiteMotherId(exchId);
    if(motherId == 37) motherId =38;
    QString mother = dbMan->getExchangeAbbr(motherId);

    QString str1 = "Ether-Trunk to "+mother +" Agg1 :";
    ui->b1a1Lbl->setText(str1);
    ui->b2a1Lbl->setText(str1);
    QString str2 = "Ether-Trunk to "+mother +" Agg2 :";
    ui->b1a2Lbl->setText(str2);
    ui->b2a2Lbl->setText(str2);


    if(dbMan->dslamPppoeExistance(dslamId))
    {
        ui->brasGB->setVisible(true);
        ui->bras1GB->setEnabled(true);
        ui->bras1GB->setVisible(true);
        ui->brasTV->setVisible(true);

        QList<int> brasList = dbMan->getDslamPppoeBrasId(dslamId);
        int bras1 = brasList.at(0);
        int bras2 = brasList.at(1);

        QString name = dbMan->getDevexDeviceName(bras1);
        bras1List << name;
        ui->bras1Lbl->setText(name);
        if(bras2 > -1)
        {
            ui->bras2GB->setEnabled(true);
            ui->bras2GB->setVisible(true);
            name = dbMan->getDevexDeviceName(bras2);
            bras2List << name;
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
            bras1List << QString::number(list.at(1));
        }
        if(list.at(2) > -1)
        {
            ui->bras1Agg2EthLbl->setText(QString::number(list.at(3)));
            bras1List << QString::number(list.at(3));
        }


        list = dbMan->getBrasAggIdEth(bras2, motherId);
        //`agg1_id`,`agg1_eth`,`agg2_id`,`agg2_eth`
        if(list.at(0) > -1)
        {
            ui->bras2Agg1EthLbl->setText(QString::number(list.at(1)));
            bras2List << QString::number(list.at(1));
        }
        if(list.at(2) > -1)
        {
            ui->bras2Agg2EthLbl->setText(QString::number(list.at(3)));
            bras2List << QString::number(list.at(3));
        }

        QSqlQuery *query = dbMan->selectDslamPppoe(dslamId);
        // `vlan`, `description`
        brasModel->setQuery(*query);
        brasModel->setHeaderData(0, Qt::Horizontal, "Sub-Interface");
        brasModel->setHeaderData(1, Qt::Horizontal, "Description");
        ui->brasTV->setModel(brasModel);
        ui->brasTV->verticalHeader()->hide();
        ui->brasTV->horizontalHeader()->setStretchLastSection(true);

        int v, vlanOffset=0;
        QString d;
        while(query->next())
        {
            v = query->value(0).toInt();
            if(v>499 && v < 600) vlanOffset = v - 500;
            d = query->value(1).toString();
            brasInfo.insert(v, d);
        }

        if(ZhoneUplink)
        {
            QString tmp;
//            tmp = uplinkIntraVlan(ui->vasLbl->text(),100+vlanOffset);
//            ui->vasVlanLbl->setText(tmp);
//            ui->btvVlanLbl->setText("Uplink Bridge & Intralink Bridge");
//            tmp = uplinkIntraVlan(ui->ipMediaLbl->text(),300+vlanOffset);
//            ui->ipMVlanLbl->setText(tmp);
            tmp = uplinkIntraVlan(ui->pppoeLbl->text(),500+vlanOffset);
            ui->hsiVlanLbl->setText(tmp);
            tmp = uplinkIntraVlan(ui->inetLbl->text(),600+vlanOffset);
            ui->inetVlanLbl->setText(tmp);
            tmp = uplinkIntraVlan(ui->sinetLbl->text(),700+vlanOffset);
            ui->sinetVlanLbl->setText(tmp);
            tmp = uplinkIntraVlan(ui->voipLbl->text(),800+vlanOffset);
            ui->voipVlanLbl->setText(tmp);
            //ui->ngnVlanLbl->setText("Uplink Bridge & Intralink Bridge");
        }

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
    }


    if(singleAgg)
    {
        ui->bras1Agg2EthLbl->setText("");
        ui->b1a2Lbl->setText("");

        ui->bras2Agg2EthLbl->setText("");
        ui->b2a2Lbl->setText("");
    }
}

void ReportDSLAMDialog::hideGB()
{
    //dslam
    ui->dslamGB->setVisible(false);
    ui->servicesGB->setEnabled(false);
    ui->servicesGB->setVisible(false);
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
}

bool ReportDSLAMDialog::cxAllowance(int val)
{
    if( (val >99 && val < 500) || (val > 999 && val < 4079) )
        return true;
    else
        return false;
}

QString ReportDSLAMDialog::uplinkIntraVlan(QString vlanRange, int vlan)
{
    QList<int> rangeList = dbMan->rangeToList(vlanRange);
    QString temp;

    if(rangeList.count() == 0) return "";
    if(rangeList.count() == 1)
        return " "+vlanRange+": Uplink Bridge";
    else
    {
        temp = " "+vlanRange + ": Uplink Bridge & ";
        int ind = rangeList.indexOf(vlan);
        rangeList.removeAt(ind);
        temp += dbMan->listToRange(rangeList)+": Intralink Bridge";

        return temp;
    }
}

void ReportDSLAMDialog::on_okBtn_clicked()
{

    QString customInfo = ui->customInfoLE->text().trimmed();
    QString path = QFileDialog::getSaveFileName(this, "Save DaNet DSLAM Report As", "","pdf (*.pdf)");
    if(!path.isEmpty())
    {
        if(!path.endsWith(".pdf")) path += ".pdf";


    /*
            QString dslamName;
            QMap<QString, QString> serviceMap, portMap;// "VAS", "100-102"     "0/7/0","To..."
            QMap<int, QStringList> nmsMap;// 10, ip<<gw

            QStringList agg1List, agg2List, aggInfo;// agg1 << ether <<port1<<port2 / vlan << desc
            QStringList bras1List, bras2List;// bras << sw1 <<sw2
            QMap<int, QString> brasInfo;// 500, "To..."
            QStringList cxList;// cx << port1 << port2 <<vlan list << desc
            QStringList agg1CxList // <<ether<<eth << vlan
    */

            //check inputs
            bool d = dslamName.isEmpty();
            bool s = (serviceMap.count() > 0)? false : true; // now is 4
            //bool p = (portMap.count() > 0)? false : true;
            bool n = (nmsMap.count() == 2)? false : true;
            if(agg1List.count() < 6)
                agg1List <<" "<<" "<<" "<<" " << "" <<"";

            if(agg2List.count() < 4)
                agg2List <<""<<""<<""<<"";

            if(aggInfo.count() < 2)
                aggInfo<<""<<"";

            bool b1;
            b1 = (bras1List.count() == 3)? false : true;
            if(bras2List.count() < 3)
                bras2List<<""<<""<<"";

            bool b = (brasInfo.count() == 4) ? false : true;

            if(cxList.count() < 6)
                cxList<<""<<""<<""<<""<<""<<"";


//            if(agg1CxList.count() < 3)
//                agg1CxList<<""<<""<<"";

            if(d || s  || n || b1 || b) //|| p
            {
                QMessageBox::warning(this, "ERROR", "DSLAM Parameters is not available.");
            }
            else
            {
                bool HUAWEIDSLAM = dbMan->isDevexHuaweiVendor(ui->dslamCB->currentData().toInt());

                int exchId = ui->abbrCB->currentData().toInt();
                if(ui->typeCB->currentData().toInt() == 3)
                    exchId = ui->siteCB->currentData().toInt();

                int node = dbMan->getSiteNode(exchId);
                bool interSwitch = false;


                if(node <= 1)
                {
                    bool C = QString(cxList.at(0)).trimmed().isEmpty();
                    bool A1 = QString(agg1List.at(0)).trimmed().isEmpty();
                    bool A2 = QString(agg2List.at(0)).trimmed().isEmpty();
                    if(C)
                    {
                        if(A1 || A2)
                            interSwitch = false;
                        else
                            interSwitch = true;
                    }

                }


                if(customInfo.isEmpty())
                {
                    ReportDSLAM reportDSLAM(this, path,HUAWEIDSLAM, singleAgg, dslamName,interSwitch, serviceMap,nmsMap,portMap,agg1List, agg2List,aggInfo,bras1List, bras2List, brasInfo,cxList);//agg1CxList
                    if(reportDSLAM.print())
                    {
                        QDesktopServices::openUrl(path);
                    }
                    else
                    {
                        QMessageBox::warning(this, "ERROR", reportDSLAM.lastError());
                    }
                }
                else
                {
                    ReportDSLAM reportDSLAM(this, path,HUAWEIDSLAM, singleAgg, dslamName,interSwitch, serviceMap,nmsMap,portMap,agg1List, agg2List,aggInfo,bras1List, bras2List, brasInfo,cxList,  customInfo);//agg1CxList,
                    if(reportDSLAM.print())
                    {
                        QDesktopServices::openUrl(path);
                    }
                    else
                    {
                        QMessageBox::warning(this, "ERROR", reportDSLAM.lastError());
                    }
                }

                this->close();

            }
    }

}

void ReportDSLAMDialog::on_areaCB_currentIndexChanged(int index)
{
    int area = index + 2;
    ui->tabWidget->setEnabled(false);
    emptyValues();

    ui->abbrCB->blockSignals(true);
    ui->abbrCB->clear();

    ui->dslamCB->blockSignals(true);
    ui->dslamCB->clear();
    ui->dslamCB->blockSignals(false);

    QString abbr;
    int id;
    QSqlQuery *query = dbMan->selectExchanges(area);
    //`id`,`abbr`

    while(query->next())
    {
        id = query->value(0).toInt();
        abbr = query->value(1).toString();
        ui->abbrCB->addItem(abbr, id);
    }

    ui->abbrCB->setCurrentIndex(-1);
    ui->abbrCB->blockSignals(false);
    ui->typeCB->setCurrentIndex(0);

    hideGB();

}

void ReportDSLAMDialog::on_abbrCB_currentIndexChanged(int index)
{
    index++;
    emptyValues();
    ui->tabWidget->setEnabled(false);
    ui->typeCB->blockSignals(true);
    ui->typeCB->setCurrentIndex(0);
    ui->typeCB->blockSignals(false);

    ui->siteCB->blockSignals(true);
    ui->siteCB->clear();
    ui->siteCB->blockSignals(false);
    ui->frame->setEnabled(false);
    ui->frame->setVisible(false);

    ui->dslamCB->blockSignals(true);
    ui->dslamCB->clear();

    int exchId = ui->abbrCB->currentData().toInt();

    QSqlQuery *query = dbMan->selectExchangeDslams(exchId);
    //id,dslam
    int id;
    QString dslam,pur,t,sn;
    while(query->next())
    {
        id = query->value(0).toInt();
        dslam = query->value(1).toString();
        pur = query->value(2).toString();
        if(query->value(3).isNull() || query->value(4).isNull())
        {
            ui->dslamCB->addItem(dslam,id);
        }
        else
        {
            t = query->value(3).toString();
            sn = query->value(4).toString();
            dslam += "-"+t+"-"+pur+"-"+sn;
            ui->dslamCB->addItem(dslam,id);
        }
    }

    ui->dslamCB->setCurrentIndex(-1);
    ui->dslamCB->blockSignals(false);

    hideGB();
}

void ReportDSLAMDialog::on_typeCB_currentIndexChanged(int index)
{
    index++;
    emptyValues();
    ui->tabWidget->setEnabled(false);
    hideGB();
    ui->siteCB->blockSignals(true);
    ui->siteCB->clear();

    int type = ui->typeCB->currentData().toInt();

    if(type == 2) //exch
    {
        ui->frame->setEnabled(false);
        ui->frame->setVisible(false);
        ui->siteCB->blockSignals(false);

        ui->dslamCB->blockSignals(true);
        ui->dslamCB->clear();

        int exchId = ui->abbrCB->currentData().toInt();

        QSqlQuery *query = dbMan->selectExchangeDslams(exchId);
        //id,dslam
        int id;
        QString dslam,pur,t,sn;
        while(query->next())
        {
            id = query->value(0).toInt();
            dslam = query->value(1).toString();
            pur = query->value(2).toString();
            if(query->value(3).isNull() || query->value(4).isNull())
            {
                ui->dslamCB->addItem(dslam,id);
            }
            else
            {
                t = query->value(3).toString();
                sn = query->value(4).toString();
                dslam += "-"+t+"-"+pur+"-"+sn;
                ui->dslamCB->addItem(dslam,id);
            }
        }

        ui->dslamCB->setCurrentIndex(-1);
        ui->dslamCB->blockSignals(false);

    }
    else //site
    {
        ui->frame->setEnabled(true);
        ui->frame->setVisible(true);
        ui->dslamCB->blockSignals(true);
        ui->dslamCB->clear();
        ui->dslamCB->blockSignals(false);

        int area = ui->areaCB->currentData().toInt();
        QString mother = ui->abbrCB->currentText();

        QSqlQuery *query = dbMan->selectSites(area,mother);
        //`id`,`abbr`

        int i;
        QString a, s,c,n;
        while(query->next())
        {
            i = query->value(0).toInt();
            a = query->value(1).toString();
            s = query->value(2).toString();
            c = query->value(3).toString();
            n = query->value(4).toString();
            ui->siteCB->addItem(c+"-"+n+"*"+a+"*"+s, i);
        }

        ui->siteCB->setCurrentIndex(-1);
        ui->siteCB->blockSignals(false);

    }


}

void ReportDSLAMDialog::on_siteCB_currentIndexChanged(int index)
{
    index++;
    emptyValues();
    ui->tabWidget->setEnabled(false);
    ui->dslamCB->blockSignals(true);
    ui->dslamCB->clear();

    if(ui->typeCB->currentData().toInt() == 3) //site
    {
        int exchId = ui->siteCB->currentData().toInt();

        QSqlQuery *query = dbMan->selectExchangeDslams(exchId);
        //id,dslam
        int id;
        QString dslam,pur,t,sn;
        while(query->next())
        {
            id = query->value(0).toInt();
            dslam = query->value(1).toString();
            pur = query->value(2).toString();
            if(query->value(3).isNull() || query->value(4).isNull())
            {
                ui->dslamCB->addItem(dslam,id);
            }
            else
            {
                t = query->value(3).toString();
                sn = query->value(4).toString();
                dslam += "-"+t+"-"+pur+"-"+sn;
                ui->dslamCB->addItem(dslam,id);
            }
        }
    }

    ui->dslamCB->setCurrentIndex(-1);
    ui->dslamCB->blockSignals(false);
    hideGB();

    if(ui->dslamCB->count() > 0)
        ui->dslamCB->setCurrentIndex(0);
}

void ReportDSLAMDialog::on_dslamCB_currentIndexChanged(int index)
{
    index++;
    ui->tabWidget->setEnabled(true);
    ui->tabWidget->setCurrentIndex(0);
    fillForm();
}

