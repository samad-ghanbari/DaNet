#include "ma5600plandialog.h"
#include "ui_ma5600plandialog.h"
#include "lib/database/danetdbman.h"
#include <QSqlQuery>
#include <QMessageBox>
#include <QRegExpValidator>

MA5600PlanDialog::MA5600PlanDialog(QWidget *parent, DanetDbMan *Db, const int ExchId, const int DevexId, const QString Name) :
    QDialog(parent),
    ui(new Ui::MA5600PlanDialog),
    dbMan(Db),
    exchId(ExchId),
    devexId(DevexId),
    name(Name),
    //vasValidator(nullptr),
    //ipMediaValidator(nullptr),
    //btvValidator(nullptr),
    hsiValidator(nullptr),
    inetValidator(nullptr),
    sinetValidator(nullptr),
    voipValidator(nullptr)//,
  //    sipTValidator(nullptr),
  //    vpnValidator(nullptr),
  //ngnValidator(nullptr)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowMinMaxButtonsHint);

    ui->tabWidget->tabBar()->hide();

    //    vasRegex.setPattern("((1\\d{2}){1}|(1\\d{2}\\-1\\d{2}){1})((\\,1\\d{2})|(\\,1\\d{2}\\-1\\d{2}))*");
    //    vasValidator = new QRegExpValidator(vasRegex, this);
    //    ui->vasLE->setValidator(vasValidator);

    //ipMediaRegex.setPattern("((3\\d{2}){1}|(3\\d{2}\\-3\\d{2}){1})((\\, 3\\d{2})|(\\, 3\\d{2}\\-3\\d{2}))*");
    //ipMediaValidator = new QRegExpValidator(ipMediaRegex, this);
    //ui->ipMediaLE->setValidator(ipMediaValidator);

    //btvRegex.setPattern("(4\\d{2}){1}");
    //btvValidator = new QRegExpValidator(btvRegex, this);
    //ui->btvLE->setValidator(btvValidator);


    hsiRegex.setPattern("((5\\d{2}){1}|(5\\d{2}\\-5\\d{2}){1})((\\, 5\\d{2})|(\\, 5\\d{2}\\-5\\d{2}))*");
    hsiValidator = new QRegExpValidator(hsiRegex, this);
    ui->hsiLE->setValidator(hsiValidator);

    inetRegex.setPattern("((6\\d{2}){1}|(6\\d{2}\\-6\\d{2}){1})((\\, 6\\d{2})|(\\, 6\\d{2}\\-6\\d{2}))*");
    inetValidator = new QRegExpValidator(inetRegex, this);
    ui->inetLE->setValidator(inetValidator);

    sinetRegex.setPattern("((7\\d{2}){1}|(7\\d{2}\\-7\\d{2}){1})((\\, 7\\d{2})|(\\, 7\\d{2}\\-7\\d{2}))*");
    sinetValidator = new QRegExpValidator(sinetRegex, this);
    ui->sinetLE->setValidator(sinetValidator);

    voipRegex.setPattern("((8\\d{2}){1}|(8\\d{2}\\-8\\d{2}){1})((\\, 8\\d{2})|(\\, 8\\d{2}\\-8\\d{2}))*");
    voipValidator = new QRegExpValidator(voipRegex, this);
    ui->voipLE->setValidator(voipValidator);


    //    sipTRegex.setPattern("((9\\d{2}){1}|(9\\d{2}\\-9\\d{2}){1})((\\,9\\d{2})|(\\,9\\d{2}\\-9\\d{2}))*");
    //    sipTValidator = new QRegExpValidator(sipTRegex, this);
    //    ui->sipTLE->setValidator(sipTValidator);
    //    vpnRegex.setPattern("([1-3]\\d{3}\\-[1-3]\\d{3}){1}");
    //    vpnValidator = new QRegExpValidator(vpnRegex, this);
    //    ui->vpnLE->setValidator(vpnValidator);

    //    ngnRegex.setPattern("4000\\-4078");
    //    ngnValidator = new QRegExpValidator(ngnRegex, this);
    //    ui->ngnLE->setValidator(ngnValidator);

    ui->dslamNameLbl->setText(name);

    QSqlQuery *query;

    bool dslamExist = dbMan->dslamExistance(devexId);

    QString purchase = dbMan->getDevexPurchase(devexId);
    ui->purchaseLbl->setText(purchase);

    int s = dbMan->getDslamMaxSN(exchId);
    ui->snSB->setValue(s+1);

    ui->uplinkShelfCB->setChecked(true);

    motherId = dbMan->getSiteMotherId(ExchId);
    motherAgg1Id = dbMan->getExchangeAgg1Id(motherId);
    motherCxId = dbMan->getExchangeCxId(motherId);
    QList<int> casNode = dbMan->getSiteCascadeNode(ExchId);
    cascade = casNode.at(0);
    node = casNode.at(1);

    int subnetPool3Id = dbMan->getSubnetPool3NmsId(motherId, "172.30.%");
    int Prefix = dbMan->getSubnetPool3Prefix(subnetPool3Id);

    int ID;
    QString IP;

    QList<int> ipIds = dbMan->getDslamNmsIpIds(devexId);
    int gIpId = -1, mIpId = -1;
    if(ipIds.count() > 1)
    {
        gIpId = ipIds.at(0);
        mIpId = ipIds.at(1);
    }

    if( (subnetPool3Id > -1) && (Prefix > 0) )
    {
        ui->gIppCB->blockSignals(true);
        query = dbMan->selectEmptyIpPool(subnetPool3Id, gIpId, mIpId);
        //`id`, `ip`
        while(query->next())
        {
            ID = query->value(0).toInt();
            IP = query->value(1).toString()+"/"+QString::number(Prefix);
            ui->gIppCB->addItem(IP,ID);
        }
        ui->gIppCB->setCurrentIndex(-1);

        //gateway
        QString gw = dbMan->getIpPoolGateway(subnetPool3Id);
        ui->gGwLbl->setText(gw);
    }

    ui->mIppCB->blockSignals(true);
    subnetPool3Id = dbMan->getSubnetPool3NmsId(motherId, "172.31.%");
    Prefix = dbMan->getSubnetPool3Prefix(subnetPool3Id);

    if((subnetPool3Id > -1) && (Prefix > 0) )
    {
        query = dbMan->selectEmptyIpPool(subnetPool3Id ,gIpId, mIpId);
        //`id`, `ip`
        while(query->next())
        {
            ID = query->value(0).toInt();
            IP = query->value(1).toString()+"/"+QString::number(Prefix);
            ui->mIppCB->addItem(IP,ID);
        }
        ui->mIppCB->setCurrentIndex(-1);

        //gateway
        QString gw = dbMan->getIpPoolGateway(subnetPool3Id);
        ui->mGwLbl->setText(gw);

    }


    //.....................................
    //vlan query based on existing values

    uplinkDevexId = -1;
    currentShelf = -1;
    QList<int> XSRRS = dbMan->getDevexXSRRS(devexId);
    shelf = XSRRS.at(4);

    if(node == 0)
    {
        //local
        ui->typeCB->blockSignals(true);
        ui->typeCB->addItem("Local", 0);
        ui->typeCB->setCurrentIndex(-1);
        //get rack and find other shelf which have dslam >> is that uplink or not
        uplinkDevexId = dbMan->getDslamUplinkDevexId(XSRRS.at(0), XSRRS.at(1),XSRRS.at(2), XSRRS.at(3));// exch- saloon- row- rack

        uplinkDevexId = dbMan->getDslamUplinkDevexId(XSRRS.at(0), XSRRS.at(1),XSRRS.at(2), XSRRS.at(3));// exch- saloon- row- rack
        if(uplinkDevexId > -1)
        {
            //getVlans
            ui->uplinkShelfCB->setChecked(false);
            QList<int> vlanLs = dbMan->getDslamPppoeVlanList(uplinkDevexId);
            int shelfNo = dbMan->getDevexShelfNo(devexId);
            int vlan = vlanLs[shelfNo-1];

            //int bVlan = dbMan->getDslamBtvVlan(uplinkDevexId);

            //ui->btvLE->setText(QString::number(bVlan));
            //ui->ngnLE->setText("4000-4078");
            if(vlan > 499)
            {
                int offset = vlan - 500;
                //                ui->vasLE->setText(QString::number(100+offset));
                //ui->ipMediaLE->setText(QString::number(300+offset));
                ui->hsiLE->setText(QString::number(500+offset));
                ui->inetLE->setText(QString::number(600+offset));
                ui->sinetLE->setText(QString::number(700+offset));
                ui->voipLE->setText(QString::number(800+offset));
            }


        }
        else
        {
            //AS new uplink shelf
            ui->uplinkShelfCB->setChecked(true);

            QList<int> devIds = dbMan->getExchangeSiteAllDslamIds(motherId);
            //            devIds << motherAgg1Id << motherCxId;

            int vlan = dbMan->getNewPppoeVlan(devIds);
            //int bVlan = dbMan->getNewBtvVlan(devIds);

            /* include Gap vlans

            int vlan = dbMan->getNewPppoeVlan(devIds,5);
            if(vlan == -1)
                vlan = dbMan->getNewPppoeVlan(devIds,2);
            if(vlan == -1)
                vlan = dbMan->getNewPppoeVlan(devIds,1);

            int bVlan = dbMan->getNewBtvVlan(devIds, 1);

*/

            //ui->btvLE->setText(QString::number(bVlan));
            //ui->ngnLE->setText("4000-4078");

            if(vlan > 499)
            {
                int offset = vlan - 500;
                //                ui->vasLE->setText(QString::number(100+offset)+"-"+QString::number(102+offset));
                //ui->ipMediaLE->setText(QString::number(300+offset)+"-"+QString::number(302+offset));
                ui->hsiLE->setText(QString::number(500+offset)+"-"+QString::number(502+offset));
                ui->inetLE->setText(QString::number(600+offset)+"-"+QString::number(602+offset));
                ui->sinetLE->setText(QString::number(700+offset)+"-"+QString::number(702+offset));
                ui->voipLE->setText(QString::number(800+offset)+"-"+QString::number(802+offset));
            }
        }

    }
    else if( (node == 1) && (shelf == 1) )
    {
        ui->typeCB->addItem("Indoor", 1);
        ui->typeCB->addItem("Outdoor", 2);
        ui->typeCB->setCurrentIndex(-1);

        //cascade head >> find new vlans
        QList<int> devIds = dbMan->getExchangeSiteAllDslamIds(motherId);
        //        devIds << motherAgg1Id << motherCxId;

        int vlan = dbMan->getNewPppoeVlan(devIds);
        //int bVlan = dbMan->getNewBtvVlan(devIds);


        /* include gap
        int vlan = dbMan->getNewPppoeVlan(devIds,5);
        if(vlan == -1)
            vlan = dbMan->getNewPppoeVlan(devIds,2);
        if(vlan == -1)
            vlan = dbMan->getNewPppoeVlan(devIds,1);

        int bVlan = dbMan->getNewBtvVlan(devIds, 1);
*/



        //ui->btvLE->setText(QString::number(bVlan));
        //ui->ngnLE->setText("4000-4078");

        if(vlan > 499)
        {
            int offset = vlan - 500;
            //            ui->vasLE->setText(QString::number(100+offset)+"-"+QString::number(105+offset));
            //ui->ipMediaLE->setText(QString::number(300+offset)+"-"+QString::number(305+offset));
            ui->hsiLE->setText(QString::number(500+offset)+"-"+QString::number(505+offset));
            ui->inetLE->setText(QString::number(600+offset)+"-"+QString::number(605+offset));
            ui->sinetLE->setText(QString::number(700+offset)+"-"+QString::number(705+offset));
            ui->voipLE->setText(QString::number(800+offset)+"-"+QString::number(805+offset));
        }
    }
    else if( (node > 1) || (shelf > 1))
    {
        ui->typeCB->addItem("Indoor", 1);
        ui->typeCB->addItem("Outdoor", 2);
        ui->typeCB->setCurrentIndex(-1);


        QList<int> dslamDevexIds;
        int siteId;
        if(shelf == 1)
        {
            //cascade first MA values
            siteId = dbMan->getExchangeSiteId(motherId, cascade, node-1);
            dslamDevexIds = dbMan->getExchangeSiteDslamIds(siteId); //with shelf_no 1

        }
        else
        {
            //site next shelf
            ui->uplinkShelfCB->setChecked(false);
            uplinkDevexId = dbMan->getDslamUplinkDevexId(XSRRS.at(0), XSRRS.at(1),XSRRS.at(2), XSRRS.at(3));// exch- saloon- row- rack
            siteId = dbMan->getExchangeSiteId(motherId, cascade, node);
            dslamDevexIds = dbMan->getExchangeSiteDslamIds(siteId);
        }


        if(dslamDevexIds.count() >= 1)
        {
            int node1devexId;
            if(dslamDevexIds.count() == 1)
                node1devexId = dslamDevexIds.at(0);
            else
                node1devexId = dbMan->getDslamIdWithMaxPppoeVlan(dslamDevexIds);


            QList<int> offsetList = dbMan->getDslamPassedPppoeVlanList(node1devexId);

            if(offsetList.at(0) > 499)
            {
                QList<int> temp;
                QString str;
                str = dbMan->listToRange(offsetList);//hsi
                ui->hsiLE->setText(str);

                for(int i = 0; i<offsetList.length();i++)
                    offsetList[i] -=500;//offset

                //                temp = offsetList;
                //                for(int i = 0; i<temp.length();i++)
                //                    temp[i] +=300;//300
                //                str = dbMan->listToRange(temp);
                //                ui->ipMediaLE->setText(str);

                str = dbMan->listToRange(offsetList);

                temp = offsetList;
                for(int i = 0; i<temp.length();i++)
                    temp[i] +=600;//600
                str = dbMan->listToRange(temp);
                ui->inetLE->setText(str);

                temp = offsetList;
                for(int i = 0; i<temp.length();i++)
                    temp[i] +=700;//700
                str = dbMan->listToRange(temp);
                ui->sinetLE->setText(str);

                temp = offsetList;
                for(int i = 0; i<temp.length();i++)
                    temp[i] +=800;//800
                str = dbMan->listToRange(temp);
                ui->voipLE->setText(str);

            }

            //int bVlan = dbMan->getDslamBtvVlan(node1devexId);
            //ui->btvLE->setText(QString::number(bVlan));
            //ui->ngnLE->setText("4000-4078");

        }
    }


    //.....................................
    if(dslamExist)
    {
        ExistPlan = true;
        fillDslamParam();
        //        if(dbMan->cxDslamExistance(devexId) || dbMan->dslamAggExistance(devexId) || dbMan->dslamPppoeExistance(devexId))
        //        {
        ui->servicesGB->setEnabled(false);
        //ui->portDescGB->setEnabled(false);
        //ui->typeCB->setEnabled(false);
        //ui->snSB->setEnabled(false);
        //        }
    }
    else
    {
        ExistPlan = false;
        //NMS
        ui->gVlanLbl->setText("10");
        ui->mVlanLbl->setText("11");
        ui->gBindLbl->setText("Global Connection");
        ui->mBindLbl->setText("MPLS-VPN_nms");
        if((uplinkDevexId != devexId) && (uplinkDevexId > -1))
        {
            if(XSRRS.count() == 5)
                currentShelf = XSRRS.at(4);
            QString uplinkShelfName = dbMan->getDslamName(uplinkDevexId);
            uplinkShelfName = uplinkShelfName.replace(" ","");
            if(currentShelf == 2)
            {
                uplinkShelfName += "- 1G-0/7/4";
                ui->desc070LE->setText(uplinkShelfName);
            }
            else if(currentShelf == 3)
            {
                uplinkShelfName += "- 1G-0/7/5";
                ui->desc070LE->setText(uplinkShelfName);
            }
        }
    }

    ui->typeCB->blockSignals(false);
    ui->gIppCB->blockSignals(false);
    ui->mIppCB->blockSignals(false);

    entryCheck();
    dslamNameChanged = false;
    oldSN = ui->snSB->value();
    descChanged = false;
    ipChanged = false;
    ui->okBtn->setEnabled(false);
}

MA5600PlanDialog::~MA5600PlanDialog()
{
    delete ui;
}

void MA5600PlanDialog::entryCheck()
{
    if(entriesCheck())
        ui->okBtn->setEnabled(true);
    else
        ui->okBtn->setEnabled(false);
}

bool MA5600PlanDialog::entriesCheck()
{
    bool t = ui->typeCB->currentText().isEmpty();
    bool gIp = ui->gIppCB->currentText().isEmpty();
    bool gGw = ui->gGwLbl->text().isEmpty();
    bool mIp = ui->mIppCB->currentText().isEmpty();
    bool mGw = ui->mGwLbl->text().isEmpty();
    bool mBind = ui->mBindLbl->text().isEmpty();
    bool gBind = ui->gBindLbl->text().isEmpty();
    //bool vas = !ui->vasLE->hasAcceptableInput();
    //bool btv = !ui->btvLE->hasAcceptableInput();
    //bool ipmedia = !ui->ipMediaLE->hasAcceptableInput();
    bool hsi = !ui->hsiLE->hasAcceptableInput();
    bool inet = !ui->inetLE->hasAcceptableInput();
    bool sinet = !ui->sinetLE->hasAcceptableInput();
    bool voip = !ui->voipLE->hasAcceptableInput();
    //bool ngn = !ui->ngnLE->hasAcceptableInput();

    if( hsi || inet || sinet || voip)//btv || ipmedia ||
        ui->serviceNextBtn->setEnabled(false);
    else
        ui->serviceNextBtn->setEnabled(true);

    if(gIp || mIp)
        ui->nmsNextBtn->setEnabled(false);
    else
        ui->nmsNextBtn->setEnabled(true);

    if(t || gIp || gGw || gBind || mBind || mIp || mGw || hsi || inet || sinet || voip ) //|| vas || btv || ipmedia
        return false;
    else
        return true;
}

bool MA5600PlanDialog::insertVlan(QString vlanRanges, QString desc)
{
    //dslam
    QList<int> list = dbMan->rangeToList(vlanRanges);

    foreach(int vlan, list)
    {
        if(!dbMan->insertVlan(devexId,vlan, desc))
            return false;
    }

    return true;
}

bool MA5600PlanDialog::insertVlan(int vlan, QString desc)
{
    if(dbMan->insertVlan(devexId,vlan, desc))
        return true;
    else
        return false;
}

void MA5600PlanDialog::fillDslamParam()
{
    int SN = 1, TYPE_NO = 0;
    QSqlQuery *query = dbMan->selectDslam(devexId);
    //`type`, `purchase`, `sn`
    if(query->next())
    {
        QString t = query->value(0).toString();
        if(t.compare("L", Qt::CaseInsensitive) == 0) TYPE_NO = 0;
        else if(t.compare("I", Qt::CaseInsensitive) == 0) TYPE_NO = 1;
        else if(t.compare("O", Qt::CaseInsensitive) == 0) TYPE_NO = 2;
        SN = query->value(2).toInt();
    }

    ui->typeCB->setCurrentIndex(ui->typeCB->findData(TYPE_NO));
    ui->snSB->setValue(SN);

    //SHELF UPLINK CB

    if(dbMan->isDslamUplinkShelf(devexId))
    {
        ui->uplinkShelfCB->setChecked(true);
    }
    else
        ui->uplinkShelfCB->setChecked(false);

    //VLAN
    query = dbMan->selectVlans(devexId);
    //`id`,`vlan`,`description`
    int value;
    QString temp;

    QList<int>  hsiList, inetList, sinetList, voipList;//, ngnList;vasList; btvList, ipMediaList,

    while(query->next())
    {
        value = query->value(1).toInt();

        /*        if(value > 99 && value < 200)
        {
            //VAS
            vasList << value;
        }
        else */
        //        if(value > 299 && value < 400)
        //        {
        //            //IP MEDIA
        //            ipMediaList << value;
        //        }
        //        if(value > 399 && value < 500)
        //        {
        //            //BTV
        //            btvList << value;
        //        }
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

    //    temp = dbMan->listToRange(vasList);
    //    ui->vasLE->setText(temp);

    //temp = dbMan->listToRange(btvList);
    //ui->btvLE->setText(temp);

    //temp = dbMan->listToRange(ipMediaList);
    //ui->ipMediaLE->setText(temp);

    temp = dbMan->listToRange(hsiList);
    ui->hsiLE->setText(temp);

    temp = dbMan->listToRange(inetList);
    ui->inetLE->setText(temp);

    temp = dbMan->listToRange(sinetList);
    ui->sinetLE->setText(temp);

    temp = dbMan->listToRange(voipList);
    ui->voipLE->setText(temp);

    //    temp = dbMan->listToRange(ngnList);
    //    temp = temp.replace(",", "-");
    //    temp = temp.replace(" ", "");
    //    ui->ngnLE->setText(temp);


    //NMS
    query = dbMan->selectDslamNMS(devexId);
    //`global_vlan`, `global_ip_pool_id`, `global_ipp`, `global_bind`, `global_gw`, `mpls_vlan`, `mpls_ip_pool_id`, `mpls_ipp`, `mpls_bind`, `mpls_gw`
    if(query->next())
    {
        int gVlan,gIpPoolId,mVlan,mIpPoolId;
        QString gBind,gGw, mBind,mGw;
        gVlan = query->value(0).toInt();
        gIpPoolId = query->value(1).toInt();
        //        QString gIp = query->value(2).toString();
        gBind = query->value(3).toString();
        gGw = query->value(4).toString();
        mVlan = query->value(5).toInt();
        mIpPoolId = query->value(6).toInt();
        //        QString mIp = query->value(7).toString();
        mBind = query->value(8).toString();
        mGw = query->value(9).toString();

        //        ui->gIppCB->addItem(gIp,gIpPoolId);
        //        ui->mIppCB->addItem(mIp,mIpPoolId);

        ui->gVlanLbl->setText(QString::number(gVlan));
        ui->gIppCB->setCurrentIndex(ui->gIppCB->findData(gIpPoolId));
        ui->gBindLbl->setText(gBind);
        ui->gGwLbl->setText(gGw);

        ui->mVlanLbl->setText(QString::number(mVlan));
        ui->mIppCB->setCurrentIndex(ui->mIppCB->findData(mIpPoolId));
        ui->mBindLbl->setText(mBind);
        ui->mGwLbl->setText(mGw);
    }

    //PORT
    query = dbMan->selectDslamInterfaceDesc(devexId);
    // `id`, `port`, `description`
    QString port, desc;
    ui->desc070LE->blockSignals(true);
    ui->desc071LE->blockSignals(true);
    ui->desc072LE->blockSignals(true);
    ui->desc073LE->blockSignals(true);
    ui->desc074LE->blockSignals(true);
    ui->desc075LE->blockSignals(true);
    while(query->next())
    {
        port = query->value(1).toString();
        desc = query->value(2).toString();

        if(port.compare("1G-0/7/0", Qt::CaseInsensitive) == 0)
            ui->desc070LE->setText(desc);
        else
            if(port.compare("1G-0/7/1", Qt::CaseInsensitive) == 0)
                ui->desc071LE->setText(desc);
            else
                if(port.compare("1G-0/7/2", Qt::CaseInsensitive) == 0)
                    ui->desc072LE->setText(desc);
                else
                    if(port.compare("1G-0/7/3", Qt::CaseInsensitive) == 0)
                        ui->desc073LE->setText(desc);
                    else
                        if(port.compare("1G-0/7/4", Qt::CaseInsensitive) == 0)
                            ui->desc074LE->setText(desc);
                        else
                            if(port.compare("1G-0/7/5", Qt::CaseInsensitive) == 0)
                                ui->desc075LE->setText(desc);
    }

    ui->desc070LE->blockSignals(false);
    ui->desc071LE->blockSignals(false);
    ui->desc072LE->blockSignals(false);
    ui->desc073LE->blockSignals(false);
    ui->desc074LE->blockSignals(false);
    ui->desc075LE->blockSignals(false);

}

void MA5600PlanDialog::on_okBtn_clicked()
{
    if(ExistPlan)
    {
        if(dslamNameChanged)
        {
            int TYPE = ui->typeCB->currentData().toInt();
            QString type;
            if(TYPE == 0)
                type = "L";
            else if(TYPE == 1)
                type = "I";
            else
                type = "O";

            int SN = ui->snSB->value();
            //check SN
            if(oldSN != SN)
                if(dbMan->dslamSNExistence(exchId, SN))
                {
                    QMessageBox::warning(this, "ERROR", "DSLAM last shelf No currently exists.\n"+dbMan->lastError());
                    return;
                }

            if(dbMan->insertUpdateDslam(devexId, type, SN))
            {
                QString dslamName = dbMan->getDslamName(devexId);
                //update agg desc
                if(!dbMan->updateAggDslamDesc(devexId, dslamName))
                    QMessageBox::warning(this, "ERROR", "Cannot update Agg-DSLAM description.\n"+dbMan->lastError());

                //update cx desc
                if(!dbMan->updateCxDslamDesc(devexId, dslamName))
                    QMessageBox::warning(this, "ERROR", "Cannot update CX-DSLAM description.\n"+dbMan->lastError());

                // update interface remote label
                if(!dbMan->updateAggCxRemoteLabel(devexId, dslamName))
                    QMessageBox::warning(this, "ERROR", "Cannot update interfaces of CX600 & Agg remote Label.\n"+dbMan->lastError());

                //update bras
                QString device = dbMan->getDslamDevice(devexId); // EQ-T-P-SN
                int agg1Id = dbMan->getDslamAgg1Id(devexId);
                QString agg = dbMan->getAggNameBrief(agg1Id, true);// NN-S9303-AGG#
                if(!dbMan->updateDslamPppoe(devexId,agg,device))
                    QMessageBox::warning(this, "ERROR", "Cannot update DSLAM-BRAS PPPOE description.\n"+dbMan->lastError());
            }
            else
                QMessageBox::warning(this, "ERROR", "Cannot insert DSLAM Type and SN.\n"+dbMan->lastError());
        }

        if(ipChanged)
        {
            //release  NMS ip
            QList<int> ipPoolId = dbMan->getDslamIpPoolId(devexId);
            int gIpPId, mIpPId;
            if(ipPoolId.count() > 1)
            {
                gIpPId = ipPoolId.at(0);
                mIpPId = ipPoolId.at(1);
                if( !dbMan->updateIp(gIpPId) || !dbMan->updateIp(mIpPId) )
                {
                    QMessageBox::warning(this, "ERROR", "Cannot release DSLAM NMS parameters.\n"+dbMan->lastError());
                    return;
                }
            }


            // change only ip and description
            int TYPE = ui->typeCB->currentData().toInt();
            QString purchase = ui->purchaseLbl->text();
            QString type;
            if(TYPE == 0)
                type = "L";
            else if(TYPE == 1)
                type = "I";
            else
                type = "O";

            int SN = ui->snSB->value();

            //.........NMS
            int gVlan = ui->gVlanLbl->text().toInt();
            QString gIpp = ui->gIppCB->currentText();
            QString gBind = ui->gBindLbl->text();
            QString gGw = ui->gGwLbl->text();
            int gIpPoolId = ui->gIppCB->currentData().toInt();

            int mVlan = ui->mVlanLbl->text().toInt();
            QString mIpp = ui->mIppCB->currentText();
            QString mBind = ui->mBindLbl->text();
            QString mGw = ui->mGwLbl->text();
            int mIpPoolId = ui->mIppCB->currentData().toInt();

            QString assignDesc = name+"- "+type+"- "+purchase+"- "+QString::number(SN);
            if(! dbMan->updateDslamNms(devexId,gVlan,gIpPoolId,gIpp,gBind,gGw,mVlan,mIpPoolId, mIpp,mBind,mGw, assignDesc))
            {
                QMessageBox::warning(this, "ERROR", "Cannot update DSLAM NMS parameters.\n"+dbMan->lastError());
                return;
            }

        }

        if(descChanged)
        {
            //......... DESC

            //release all related desc
            if(!dbMan->deleteDslamInterface(devexId))
            {
                QMessageBox::warning(this, "ERROR", "Cannot release DSLAM Interface Descriptions.\n"+dbMan->lastError());
                return;
            }

            QString port = "1G-0/7/0";
            QString desc = ui->desc070LE->text();
            if(!desc.isEmpty())
                if(!dbMan->insertDslamInterface(devexId, port, desc))
                    QMessageBox::warning(this, "ERROR", "Cannot insert DSLAM port description.\n"+dbMan->lastError());


            port = "1G-0/7/1";
            desc = ui->desc071LE->text();
            if(!desc.isEmpty())
                if(!dbMan->insertDslamInterface(devexId, port, desc))
                    QMessageBox::warning(this, "ERROR", "Cannot insert DSLAM port description.\n"+dbMan->lastError());


            port = "1G-0/7/2";
            desc = ui->desc072LE->text();
            if(!desc.isEmpty())
                if(!dbMan->insertDslamInterface(devexId, port, desc))
                    QMessageBox::warning(this, "ERROR", "Cannot insert DSLAM port description.\n"+dbMan->lastError());


            port = "1G-0/7/3";
            desc = ui->desc073LE->text();
            if(!desc.isEmpty())
                if(!dbMan->insertDslamInterface(devexId, port, desc))
                    QMessageBox::warning(this, "ERROR", "Cannot insert DSLAM port description.\n"+dbMan->lastError());


            port = "1G-0/7/4";
            desc = ui->desc074LE->text();
            if(!desc.isEmpty())
                if(!dbMan->insertDslamInterface(devexId, port, desc))
                    QMessageBox::warning(this, "ERROR", "Cannot insert DSLAM port description.\n"+dbMan->lastError());


            port = "1G-0/7/5";
            desc = ui->desc075LE->text();
            if(!desc.isEmpty())
                if(!dbMan->insertDslamInterface(devexId, port, desc))
                    QMessageBox::warning(this, "ERROR", "Cannot insert DSLAM port description.\n"+dbMan->lastError());

        }

        this->close();
    }
    else
    {
        // change all
        //int vlan;//,min,max;
        QList<int> list;
        QString range, desc;

        //*****check vlan entries
        //VAS
        //        range = ui->vasLE->text();
        //        list = dbMan->rangeToList(range);
        //        range = dbMan->listToRange(list);
        //        ui->vasLE->setText(range);
        //IP MEDIA
        //            range = ui->ipMediaLE->text();
        //            list = dbMan->rangeToList(range);
        //            range = dbMan->listToRange(list);
        //            ui->ipMediaLE->setText(range);
        //            //BTV
        //            range = ui->btvLE->text();
        //            list = dbMan->rangeToList(range);
        //            range = dbMan->listToRange(list);
        //            ui->btvLE->setText(range);
        //HSI
        range = ui->hsiLE->text();
        list = dbMan->rangeToList(range);
        range = dbMan->listToRange(list);
        ui->hsiLE->setText(range);
        //INET
        range = ui->inetLE->text();
        list = dbMan->rangeToList(range);
        range = dbMan->listToRange(list);
        ui->inetLE->setText(range);
        //SINET
        range = ui->sinetLE->text();
        list = dbMan->rangeToList(range);
        range = dbMan->listToRange(list);
        ui->sinetLE->setText(range);
        //VOIP
        range = ui->voipLE->text();
        list = dbMan->rangeToList(range);
        range = dbMan->listToRange(list);
        ui->voipLE->setText(range);
        //NGN
        //        range = ui->ngnLE->text();
        //        list = dbMan->rangeToList(range);
        //        range = dbMan->listToRange(list);
        //        ui->ngnLE->setText(range);
        //*****
        if(!entriesCheck())
            return;
        //________________________check vlan availability
        if( (ui->uplinkShelfCB->isChecked() && node == 0) || ((node == 1) && (shelf == 1) ) )
        {
            range = ui->hsiLE->text();
            list = dbMan->rangeToList(range);
            QList<int> devIds = dbMan->getExchangeSiteAllDslamIds(motherId);
            devIds << motherAgg1Id;
            if(dbMan->vlanAvailability(devIds, list))
            {
                QStringList devices = dbMan->getDeviceNameWithVlan(motherId, list);
                QString S = devices.join(',');

                QMessageBox::warning(this, "ERROR", "HSI Vlan currently exists in "+S);
                return;
            }
            //#####
            //            range = ui->vasLE->text();
            //            list = dbMan->rangeToList(range);
            //            if(dbMan->vlanAvailability(motherAgg1Id, list))
            //            {
            //                QMessageBox::warning(this, "ERROR", "VAS-Vlan currently exists in aggregation 1.");
            //                return;
            //            }
            //            if(dbMan->vlanAvailability(motherCxId, list))
            //            {
            //                QMessageBox::warning(this, "ERROR", "VAS-Vlan currently exists in CX600.");
            //                return;
            //            }

            //                range = ui->ipMediaLE->text();
            //                list = dbMan->rangeToList(range);
            //                devIds = dbMan->getExchangeSiteAllDslamIds(motherId);
            //                devIds << motherAgg1Id;
            //                devIds << motherCxId;
            //                if(dbMan->vlanAvailability(devIds, list))
            //                {
            //                    QStringList devices = dbMan->getDeviceNameWithVlan(motherId, list);
            //                    QString S = devices.join(',');
            //                    QMessageBox::warning(this, "ERROR", "Reserved IP-Media Vlan currently exists in "+S);
            //                    return;
            //                }


            range = ui->inetLE->text();
            list = dbMan->rangeToList(range);
            devIds = dbMan->getExchangeSiteAllDslamIds(motherId);
            devIds << motherAgg1Id;
            if(dbMan->vlanAvailability(devIds, list))
            {
                QStringList devices = dbMan->getDeviceNameWithVlan(motherId, list);
                QString S = devices.join(',');
                QMessageBox::warning(this, "ERROR", "INET-Vlan currently exists in "+S);
                return;
            }

            range = ui->sinetLE->text();
            list = dbMan->rangeToList(range);
            devIds = dbMan->getExchangeSiteAllDslamIds(motherId);
            devIds << motherAgg1Id;
            if(dbMan->vlanAvailability(devIds, list))
            {
                QStringList devices = dbMan->getDeviceNameWithVlan(motherId, list);
                QString S = devices.join(',');
                QMessageBox::warning(this, "ERROR", "SINET-Vlan currently exists in "+S);
                return;
            }

            range = ui->voipLE->text();
            list = dbMan->rangeToList(range);
            devIds = dbMan->getExchangeSiteAllDslamIds(motherId);
            devIds << motherAgg1Id;
            if(dbMan->vlanAvailability(devIds, list))
            {
                QStringList devices = dbMan->getDeviceNameWithVlan(motherId, list);
                QString S = devices.join(',');
                QMessageBox::warning(this, "ERROR", "VOIP-Vlan currently exists in "+S);
                return;
            }

            //#####
            //                vlan = ui->btvLE->text().toInt();
            //                list.clear();
            //                list <<vlan;
            //                devIds = dbMan->getExchangeSiteAllDslamIds(motherId);
            //                devIds << motherAgg1Id;
            //                devIds << motherCxId;
            //                if(dbMan->vlanAvailability(devIds, list))
            //                {
            //                    QStringList devices = dbMan->getDeviceNameWithVlan(motherId, list);
            //                    QString S = devices.join(',');

            //                    QMessageBox::warning(this, "ERROR", "BTV-Vlan currently exists in "+S);
            //                    return;
            //                }

            //        min = ui->vpnMinSB->value();
            //        max = ui->vpnMaxSB->value();
            //        if(dbMan->vlanAvailability(motherCxId, min, max))
            //        {
            //            QMessageBox::warning(this, "ERROR", "VPN-Vlan currently exists in CX600.");
            //            return;
            //        }
            //        if(dbMan->vlanAvailability(motherAgg1Id, min, max))
            //        {
            //            QMessageBox::warning(this, "ERROR", "VPN-Vlan currently exists in aggregation1.");
            //            return;
            //        }
        }
        //________________________


        int TYPE = ui->typeCB->currentData().toInt();
        QString purchase = ui->purchaseLbl->text();
        QString type;
        if(TYPE == 0)
            type = "L";
        else if(TYPE == 1)
            type = "I";
        else
            type = "O";

        int SN = ui->snSB->value();
        //check SN
        if(dbMan->dslamSNExistence(exchId, SN))
        {
            QMessageBox::warning(this, "ERROR", "DSLAM last shelf No currently exists.\n"+dbMan->lastError());
            return;
        }
        //

        if(dbMan->insertUpdateDslam(devexId, type, SN))
        {
            //........SERVICES, NMS, DESC
            // remove existing vlans, nms and description
            if(!dbMan->deleteDslamParams(devexId))
            {
                QMessageBox::warning(this, "ERROR", "Cannot remove existing DSLAM parameters.\n"+dbMan->lastError());
                return;
            }


            //VAS
            //            range = ui->vasLE->text();
            //            desc = name + " -"+"TCT-VAS";
            //            if(!insertVlan(range, desc))
            //            {
            //                QMessageBox::warning(this, "ERROR", "Cannot insert VAS VLAN parameters.\n"+dbMan->lastError());
            //                return;
            //            }
            //IP MEDIA
            //                range = ui->ipMediaLE->text();
            //                desc = name + " -"+"TCT-IP-Media";
            //                if(!insertVlan(range, desc))
            //                {
            //                    QMessageBox::warning(this, "ERROR", "Cannot insert IP-Media VLAN parameters.\n"+dbMan->lastError());
            //                    return;
            //                }
            //BTV
            //                range = ui->btvLE->text();
            //                desc = name + " -"+"TCT-BTV";
            //                if(!insertVlan(range, desc))
            //                {
            //                    QMessageBox::warning(this, "ERROR", "Cannot insert BTV VLAN parameters.\n"+dbMan->lastError());
            //                    return;
            //                }
            //PPPOE
            range = ui->hsiLE->text();
            desc = name + " -"+"TCT-PPPOE";
            if(!insertVlan(range, desc))
            {
                QMessageBox::warning(this, "ERROR", "Cannot insert PPPOE VLAN parameters.\n"+dbMan->lastError());
                return;
            }
            //INET
            range = ui->inetLE->text();
            desc = name + " -"+"INET-PPPOE";
            if(!insertVlan(range, desc))
            {
                QMessageBox::warning(this, "ERROR", "Cannot insert INET PPPOE VLAN parameters.\n"+dbMan->lastError());
                return;
            }
            //SINET
            range = ui->sinetLE->text();
            desc = name + " -"+"SINET-PPPOE";
            if(!insertVlan(range, desc))
            {
                QMessageBox::warning(this, "ERROR", "Cannot insert SINET PPPOE VLAN parameters.\n"+dbMan->lastError());
                return;
            }
            //VOIP
            range = ui->voipLE->text();
            desc = name + " -"+"VOIP-PPPOE";
            if(!insertVlan(range, desc))
            {
                QMessageBox::warning(this, "ERROR", "Cannot insert VOIP PPPOE VLAN parameters.\n"+dbMan->lastError());
                return;
            }


            //NGN
            //only start or end insert
            //            range = ui->ngnLE->text();
            //            QStringList listStr = range.split('-', QString::SkipEmptyParts);
            //            if(listStr.length() == 2)
            //            {
            //                min = listStr[0].toInt();
            //                max = listStr[1].toInt();
            //                desc = name + " -"+"NGN";
            //                if(!insertVlan(min, desc))
            //                {
            //                    QMessageBox::warning(this, "Warning", "Cannot insert NGN VLAN parameters.\n"+dbMan->lastError());
            //                    return;
            //                }
            //                if(!insertVlan(max, desc))
            //                {
            //                    QMessageBox::warning(this, "Warning", "Cannot insert NGN VLAN parameters.\n"+dbMan->lastError());
            //                    return;
            //                }
            //            }
            //            else
            //            {
            //                QMessageBox::warning(this, "Warning", "Cannot insert NGN VLAN parameters.");
            //                return;
            //            }

            //.........NMS
            int gVlan = ui->gVlanLbl->text().toInt();
            QString gIpp = ui->gIppCB->currentText();
            QString gBind = ui->gBindLbl->text();
            QString gGw = ui->gGwLbl->text();
            int gIpPoolId = ui->gIppCB->currentData().toInt();

            int mVlan = ui->mVlanLbl->text().toInt();
            QString mIpp = ui->mIppCB->currentText();
            QString mBind = ui->mBindLbl->text();
            QString mGw = ui->mGwLbl->text();
            int mIpPoolId = ui->mIppCB->currentData().toInt();

            QString assignDesc = name+"- "+type+"- "+purchase+"- "+QString::number(SN);
            if(! dbMan->insertDslamNms(devexId,gVlan,gIpPoolId,gIpp,gBind,gGw,mVlan,mIpPoolId, mIpp,mBind,mGw, assignDesc))
            {
                QMessageBox::warning(this, "ERROR", "Cannot insert DSLAM NMS parameters.\n"+dbMan->lastError());
                return;
            }


            //......... DESC

            QString port = "1G-0/7/0";
            desc = ui->desc070LE->text();
            if(!desc.isEmpty())
                if(!dbMan->insertDslamInterface(devexId, port, desc))
                    QMessageBox::warning(this, "ERROR", "Cannot insert DSLAM port description.\n"+dbMan->lastError());


            port = "1G-0/7/1";
            desc = ui->desc071LE->text();
            if(!desc.isEmpty())
                if(!dbMan->insertDslamInterface(devexId, port, desc))
                    QMessageBox::warning(this, "ERROR", "Cannot insert DSLAM port description.\n"+dbMan->lastError());


            port = "1G-0/7/2";
            desc = ui->desc072LE->text();
            if(!desc.isEmpty())
                if(!dbMan->insertDslamInterface(devexId, port, desc))
                    QMessageBox::warning(this, "ERROR", "Cannot insert DSLAM port description.\n"+dbMan->lastError());


            port = "1G-0/7/3";
            desc = ui->desc073LE->text();
            if(!desc.isEmpty())
                if(!dbMan->insertDslamInterface(devexId, port, desc))
                    QMessageBox::warning(this, "ERROR", "Cannot insert DSLAM port description.\n"+dbMan->lastError());


            port = "1G-0/7/4";
            desc = ui->desc074LE->text();
            if(!desc.isEmpty())
                if(!dbMan->insertDslamInterface(devexId, port, desc))
                    QMessageBox::warning(this, "ERROR", "Cannot insert DSLAM port description.\n"+dbMan->lastError());


            port = "1G-0/7/5";
            desc = ui->desc075LE->text();
            if(!desc.isEmpty())
                if(!dbMan->insertDslamInterface(devexId, port, desc))
                    QMessageBox::warning(this, "ERROR", "Cannot insert DSLAM port description.\n"+dbMan->lastError());



            if((devexId != uplinkDevexId) && (uplinkDevexId > -1))
            {
                QString dn = name;
                dn = dn.replace(" ","");
                dn += "-1G 0/7/0";
                if(currentShelf == 2)
                {
                    dbMan->insertUpdateDslamInterface(uplinkDevexId,"1G-0/7/4", dn);
                }
                else if(currentShelf == 3)
                {
                    dbMan->insertUpdateDslamInterface(uplinkDevexId,"1G-0/7/5", dn);
                }

            }


            this->close();
        }
        else
            QMessageBox::warning(this, "ERROR", "Cannot insert DSLAM parameters.\n"+dbMan->lastError());

    }

    //update dslam port desc
    if(node > 1)
        if(ui->desc070LE->text().isEmpty())
        {
            int uplinkSiteId = dbMan->getSiteUplinkId(exchId);
            if(uplinkSiteId > -1)
            {
                bool ok = true;
                QString uplinkDevexName = dbMan->getSiteDevexName(uplinkSiteId);
                int uplinkDevexId = dbMan->getSiteDevexId(uplinkSiteId);
                if(uplinkDevexName.contains("5616"))
                {
                    uplinkDevexName +="- 1G 0/0/1";
                    ok = ok && dbMan->insertUpdateDslamInterface(devexId,"1G-0/7/0", uplinkDevexName);
                    ok = ok && dbMan->insertUpdateDslamInterface(uplinkDevexId,"1G-0/0/1", name+" 1G-0/7/0");

                    if(ok)
                        QMessageBox::information(this,"Update", "Uplink and current DSLAM interface descriptions are updated.");

                }

                if(uplinkDevexName.contains("5600"))
                {
                    ok = ok && dbMan->insertUpdateDslamInterface(devexId,"1G-0/7/0", uplinkDevexName);

                    if(dbMan->dslamInterfaceExistence(devexId, "1G-0/7/4"))
                    {
                        uplinkDevexName +=" 1G-0/7/4";
                        ok = ok && dbMan->insertUpdateDslamInterface(uplinkDevexId,"1G-0/7/4", name+" 1G-0/7/0");

                    }
                    else
                    {
                        uplinkDevexName +=" 1G-0/7/5";
                        ok = ok && dbMan->insertUpdateDslamInterface(uplinkDevexId,"1G-0/7/5", name+" 1G-0/7/0");
                    }


                    if(ok)
                        QMessageBox::information(this,"Update", "Uplink and current DSLAM interface descriptions are updated.");
                }
            }
        }
}

void MA5600PlanDialog::on_uplinkShelfCB_toggled(bool checked)
{
    if(checked)
    {
        //        ui->vasLE->setPlaceholderText("100-102, 105, 125");

        //ui->ipMediaLE->setPlaceholderText("300-302, 305, 325");

        ui->hsiLE->setPlaceholderText("500-502, 505, 525");

        ui->inetLE->setPlaceholderText("600-602, 605, 625");

        ui->sinetLE->setPlaceholderText("700-702, 705, 725");

        ui->voipLE->setPlaceholderText("800-802, 805, 825");


    }else
    {
        //        ui->vasLE->setPlaceholderText("101");

        //ui->ipMediaLE->setPlaceholderText("301");

        ui->hsiLE->setPlaceholderText("501");

        ui->inetLE->setPlaceholderText("601");

        ui->sinetLE->setPlaceholderText("701");

        ui->voipLE->setPlaceholderText("801");
    }

    entryCheck();
}

void MA5600PlanDialog::on_typeCB_currentIndexChanged(int index)
{
    dslamNameChanged = true;
    index++;
    entryCheck();
}

void MA5600PlanDialog::on_gIppCB_currentTextChanged(const QString &arg1)
{
    ipChanged =true;
    if(arg1.isEmpty())
    {
        ui->okBtn->setEnabled(false);
        return ;
    }
    else
        entryCheck();
}

void MA5600PlanDialog::on_mIppCB_currentTextChanged(const QString &arg1)
{
    ipChanged =true;
    if(arg1.isEmpty())
    {
        ui->okBtn->setEnabled(false);
        return ;
    }
    else
        entryCheck();
}

void MA5600PlanDialog::on_desc070LE_textChanged(const QString &arg1)
{
    descChanged = true;
    if(arg1.isEmpty())
        entryCheck();
    else
        entryCheck();
}

void MA5600PlanDialog::on_desc072LE_textChanged(const QString &arg1)
{
    descChanged = true;
    if(arg1.isEmpty())
        entryCheck();
    else
        entryCheck();
}

void MA5600PlanDialog::on_desc073LE_textChanged(const QString &arg1)
{
    descChanged = true;
    if(arg1.isEmpty())
        entryCheck();
    else
        entryCheck();
}

void MA5600PlanDialog::on_desc074LE_textChanged(const QString &arg1)
{
    descChanged = true;
    if(arg1.isEmpty())
        entryCheck();
    else
        entryCheck();
}

void MA5600PlanDialog::on_desc075LE_textChanged(const QString &arg1)
{
    descChanged = true;
    if(arg1.isEmpty())
        entryCheck();
    else
        entryCheck();}

void MA5600PlanDialog::on_snSB_valueChanged(int arg1)
{
    arg1++;
    dslamNameChanged = true;
    entryCheck();
}

void MA5600PlanDialog::on_gIppCB_currentIndexChanged(int index)
{
    ui->mIppCB->setCurrentIndex(index);
}

//void MA5600PlanDialog::on_btvLE_textChanged(const QString &arg1)
//{
//    ui->okBtn->setEnabled(false);
//    ui->serviceNextBtn->setEnabled(false);
//    if(arg1.isEmpty())
//    {
//        ui->okBtn->setEnabled(false);
//    }
//    else
//    {
//        if(ui->btvLE->hasAcceptableInput())
//        {
//            ui->btvLE->setStyleSheet("QLineEdit{color: rgb(85,0,127)}");
//            entryCheck();
//        }
//        else
//            ui->btvLE->setStyleSheet("QLineEdit{color: darkred}");
//    }
//}

//void MA5600PlanDialog::on_ipMediaLE_textChanged(const QString &arg1)
//{
//    ui->okBtn->setEnabled(false);
//    ui->serviceNextBtn->setEnabled(false);

//    if(arg1.isEmpty())
//    {
//        ui->okBtn->setEnabled(false);
//        ui->serviceNextBtn->setEnabled(false);
//    }
//    else
//    {
//        QString arg2 = arg1;
//        arg2 = arg2.replace(" ", "");
//        arg2 = arg2.replace(",", ", ");
//        ui->ipMediaLE->blockSignals(true);
//        ui->ipMediaLE->setText(arg2);
//        ui->ipMediaLE->blockSignals(false);

//        if(ui->ipMediaLE->hasAcceptableInput())
//        {
//            ui->ipMediaLE->setStyleSheet("QLineEdit{color: rgb(85,0,127)}");
//            //fill others value
//            vlanList = dbMan->rangeToList(arg2);
//            //offset
//            QList<int> offset;
//            QString temp;
//            foreach(int i,vlanList)
//                offset << (i-300);

//            //hsi
//            vlanList.clear();
//            foreach(int i,offset)
//                vlanList << i+500;
//            temp = dbMan->listToRange(vlanList);
//            ui->hsiLE->setText(temp);
//            //inet
//            vlanList.clear();
//            foreach(int i,offset)
//                vlanList << i+600;
//            temp = dbMan->listToRange(vlanList);
//            ui->inetLE->setText(temp);
//            //sinet
//            vlanList.clear();
//            foreach(int i,offset)
//                vlanList << i+700;
//            temp = dbMan->listToRange(vlanList);
//            ui->sinetLE->setText(temp);
//            //voip
//            vlanList.clear();
//            foreach(int i,offset)
//                vlanList << i+800;
//            temp = dbMan->listToRange(vlanList);
//            ui->voipLE->setText(temp);

//            entryCheck();
//        }
//        else
//            ui->ipMediaLE->setStyleSheet("QLineEdit{color: darkred}");
//    }
//}

void MA5600PlanDialog::on_hsiLE_textChanged(const QString &arg1)
{
    ui->okBtn->setEnabled(false);
    ui->serviceNextBtn->setEnabled(false);

    if(arg1.isEmpty())
    {
        ui->serviceNextBtn->setEnabled(false);
        ui->okBtn->setEnabled(false);
    }
    else
    {
        if(ui->hsiLE->hasAcceptableInput())
        {
            ui->hsiLE->setStyleSheet("QLineEdit{color: rgb(85,0,127)}");

            //fill others value
            vlanList = dbMan->rangeToList(arg1);
            //offset
            QList<int> offset;
            QString temp;
            foreach(int i,vlanList)
                offset << (i-500);

            //inet
            vlanList.clear();
            foreach(int i,offset)
                vlanList << i+600;
            temp = dbMan->listToRange(vlanList);
            ui->inetLE->setText(temp);
            //sinet
            vlanList.clear();
            foreach(int i,offset)
                vlanList << i+700;
            temp = dbMan->listToRange(vlanList);
            ui->sinetLE->setText(temp);
            //voip
            vlanList.clear();
            foreach(int i,offset)
                vlanList << i+800;
            temp = dbMan->listToRange(vlanList);
            ui->voipLE->setText(temp);

            entryCheck();
        }
        else
            ui->hsiLE->setStyleSheet("QLineEdit{color: darkred}");
    }
}

void MA5600PlanDialog::on_inetLE_textChanged(const QString &arg1)
{
    ui->okBtn->setEnabled(false);
    ui->serviceNextBtn->setEnabled(false);

    if(arg1.isEmpty())
    {
        ui->serviceNextBtn->setEnabled(false);
        ui->okBtn->setEnabled(false);
    }
    else
    {
        if(ui->inetLE->hasAcceptableInput())
        {
            ui->inetLE->setStyleSheet("QLineEdit{color: rgb(85,0,127)}");
            entryCheck();
        }
        else
            ui->inetLE->setStyleSheet("QLineEdit{color: darkred}");
    }
}

void MA5600PlanDialog::on_sinetLE_textChanged(const QString &arg1)
{
    ui->okBtn->setEnabled(false);
    ui->serviceNextBtn->setEnabled(false);

    if(arg1.isEmpty())
    {
        ui->serviceNextBtn->setEnabled(false);
        ui->okBtn->setEnabled(false);
    }
    else
    {
        if(ui->sinetLE->hasAcceptableInput())
        {
            ui->sinetLE->setStyleSheet("QLineEdit{color: rgb(85,0,127)}");
            entryCheck();
        }
        else
            ui->sinetLE->setStyleSheet("QLineEdit{color: darkred}");
    }
}

void MA5600PlanDialog::on_voipLE_textChanged(const QString &arg1)
{
    ui->okBtn->setEnabled(false);
    ui->serviceNextBtn->setEnabled(false);

    if(arg1.isEmpty())
    {
        ui->serviceNextBtn->setEnabled(false);
        ui->okBtn->setEnabled(false);
    }
    else
    {
        if(ui->voipLE->hasAcceptableInput())
        {
            ui->voipLE->setStyleSheet("QLineEdit{color: rgb(85,0,127)}");
            entryCheck();
        }
        else
            ui->voipLE->setStyleSheet("QLineEdit{color: darkred}");
    }
}

//void MA5600PlanDialog::on_ngnLE_textChanged(const QString &arg1)
//{
//    ui->okBtn->setEnabled(false);

//    if(arg1.isEmpty())
//        ui->okBtn->setEnabled(false);
//    else
//    {
//        if(ui->ngnLE->hasAcceptableInput())
//        {
//            ui->ngnLE->setStyleSheet("QLineEdit{color: rgb(85,0,127)}");
//            entryCheck();
//        }
//        else
//            ui->ngnLE->setStyleSheet("QLineEdit{color: darkred}");
//    }
//}

void MA5600PlanDialog::on_serviceNextBtn_clicked()
{
    ui->tabWidget->setCurrentIndex(1);
}

void MA5600PlanDialog::on_nmsNextBtn_clicked()
{
    ui->tabWidget->setCurrentIndex(2);
}

void MA5600PlanDialog::on_nmsBackBtn_clicked()
{
    ui->tabWidget->setCurrentIndex(0);
}

void MA5600PlanDialog::on_descBackBtn_clicked()
{
    ui->tabWidget->setCurrentIndex(1);
}
