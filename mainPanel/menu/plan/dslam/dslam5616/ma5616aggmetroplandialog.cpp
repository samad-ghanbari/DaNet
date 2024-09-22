#include "ma5616aggmetroplandialog.h"
#include "ui_ma5616aggmetroplandialog.h"
#include "lib/database/danetdbman.h"
#include <QSqlQuery>
#include <QMessageBox>

MA5616AggMetroPlanDialog::MA5616AggMetroPlanDialog(QWidget *parent, DanetDbMan *Db, const int ExchId, const int DslamId) :
    QDialog(parent),
    ui(new Ui::MA5616AggMetroPlanDialog),
    dbMan(Db),
    exchId(ExchId),
    dslamId(DslamId)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowMinMaxButtonsHint);

    motherId = dbMan->getSiteMotherId(exchId);
    QSqlQuery *query;
    //Agg1
    ui->aggCB->blockSignals(true);
    if(motherId > -1)
    {
        if(motherId == 37)
            query = dbMan->selectExchangeAgg(ExchId, 38);
        else
            query = dbMan->selectExchangeAgg(ExchId, motherId);
    }
    else
        query = dbMan->selectExchangeAgg(ExchId);

    //`id`,`device_name`
    int ID;
    QString NAME;
    while(query->next())
    {
        ID = query->value(0).toInt();
        NAME = query->value(1).toString();
        ui->aggCB->addItem(NAME, ID);
    }
    ui->aggCB->setCurrentIndex(-1);
    ui->aggCB->blockSignals(false);

    //Modules
    query = dbMan->selectModules();
    int i;
    QString m;
    while(query->next())
    {
        i = query->value(0).toInt();
        m = query->value(1).toString();

        ui->amCB->addItem(m, i);
    }

    ui->amCB->setCurrentText("LX-10Km");

    // DSLAM PORTS
    ui->ai1diCB->blockSignals(true);
    ui->ai1diCB->addItem("1G-0/0/0", 0);
    ui->ai1diCB->addItem("1G-0/0/1", 1);
    ui->ai1diCB->setCurrentIndex(0);
    ui->ai1diCB->blockSignals(false);

    ui->ai2diCB->blockSignals(true);
    ui->ai2diCB->addItem("1G-0/0/0", 0);
    ui->ai2diCB->addItem("1G-0/0/1", 1);
    ui->ai2diCB->setCurrentIndex(1);
    ui->ai2diCB->blockSignals(false);

    //desc
    QString dslamName = dbMan->getDslamName(dslamId);
    ui->aggDescLE->setText(dslamName.trimmed());
    ui->dslamLbl->setText(dslamName);

    // VLANS
    //int min;
    totalVlanList << 10 << 11;
    totalAllowPassSL << "10" << "11";
    QList<int> vlanList;// = dbMan->getDslamVasVlanList(DslamId);
    QString temp;// = dbMan->listToRange(vlanList);
//    totalAllowPassSL << temp;
//    foreach(int i,vlanList)
//    {
//        totalVlanList << i;
//    }

//    vlanList = dbMan->getDslamIpMediaVlanList(DslamId);
//    temp = dbMan->listToRange(vlanList);
//    totalAllowPassSL << temp;
//    foreach(int i, vlanList)
//    {
//        totalVlanList << i;
//    }

//    min = dbMan->getDslamBtvVlan(DslamId);
//    if(min > 399)
//    {
//        totalAllowPassSL << QString::number(min);
//        totalVlanList << min;
//    }


    vlanList = dbMan->getDslamPppoeVlanList(DslamId);
    temp = dbMan->listToRange(vlanList);
    totalAllowPassSL << temp;
    foreach(int i,vlanList)
    {
        totalVlanList << i;
    }

    vlanList = dbMan->getDslamInetVlanList(DslamId);
    temp = dbMan->listToRange(vlanList);
    totalAllowPassSL << temp;
    foreach(int i,vlanList)
    {
        totalVlanList << i;
    }

    vlanList = dbMan->getDslamSinetVlanList(DslamId);
    temp = dbMan->listToRange(vlanList);
    totalAllowPassSL << temp;
    foreach(int i, vlanList)
        totalVlanList << i;


    vlanList = dbMan->getDslamVoipVlanList(DslamId);
    temp = dbMan->listToRange(vlanList);
    totalAllowPassSL << temp;
    foreach(int i, vlanList)
        totalVlanList << i;


    QString aggAllowPass = totalAllowPassSL.join(",");
    aggAllowPass = aggAllowPass.replace(",",", ");
    ui->aggAllowPassLE->setText(aggAllowPass);
    ui->ai1diCB->setCurrentIndex(-1);
    ui->ai1diCB->setCurrentIndex(0);

    ui->twoIntChB->setChecked(false);
    ui->agg1Int2Frame->setVisible(false);

    bool a = dbMan->dslamAggExistance(dslamId);

    if(a)
    {
        query = dbMan->selectAggDslam(dslamId);
        //`agg1_id`, `agg2_id`, `agg1_port1_id`, `agg1_port2_id`, `agg2_port1_id`,
        //`agg2_port2_id`, `agg1`, `agg2`, `agg1_eth`, `agg2_eth`, `agg1_port1`, `agg1_port2`,
        //`agg2_port1`, `agg2_port2`, `allow_pass`, `description`
        if(query->next())
        {
            int ag1Id = query->value(0).toInt();
            int ag2Id = query->value(1).toInt();
            if(query->value(0).isNull())
                ag1Id = -1;
            if(query->value(1).isNull())
                ag2Id = -1;

            int a1p1Id = query->value(2).toInt();
            int a1p2Id = query->value(3).toInt();
            if(query->value(3).isNull())
                a1p2Id = -1;
            int a2p1Id = query->value(4).toInt();
            int a2p2Id = query->value(5).toInt();
            if(query->value(5).isNull())
                a2p2Id = -1;

            int a1e = query->value(8).toInt();
            int a2e = query->value(9).toInt();

            bool twoInt = false;
            if((a1p2Id > -1) || (a2p2Id > -1))
            {
                twoInt = true;
                ui->twoIntChB->setChecked(true);
            }

            if(ag1Id > -1)
            {
                ui->aggCB->setCurrentIndex(ui->aggCB->findData(ag1Id));
                ui->aggInt1CB->setCurrentIndex(ui->aggInt1CB->findData(a1p1Id));
                ui->aggEthSB->setValue(a1e);
                QStringList mi = dbMan->getModulePeerInterface(a1p1Id);
                ui->amCB->setCurrentText(mi.at(0));
                ui->ai1diCB->setCurrentText(mi.at(1));
                mi = dbMan->getModulePeerInterface(a1p2Id);
                if(twoInt)
                {
                    ui->aggInt2CB->setCurrentIndex(ui->aggInt2CB->findData(a1p2Id));
                    ui->ai2diCB->setCurrentText(mi.at(1));
                }
            }
            else
            {
                ui->aggCB->setCurrentIndex(ui->aggCB->findData(ag2Id));
                ui->aggEthSB->setValue(a2e);
                ui->aggInt1CB->setCurrentIndex(ui->aggInt1CB->findData(a2p1Id));
                QStringList mi = dbMan->getModulePeerInterface(a2p1Id);
                ui->amCB->setCurrentText(mi.at(0));
                ui->ai1diCB->setCurrentText(mi.at(1));
                mi = dbMan->getModulePeerInterface(a2p2Id);
                if(twoInt)
                {
                    ui->aggInt2CB->setCurrentIndex(ui->aggInt2CB->findData(a2p2Id));
                    ui->ai2diCB->setCurrentText(mi.at(1));
                }
            }

        }

        ui->okBtn->setEnabled(false);
    }

    AggChanged = false;
    ui->okBtn->setEnabled(false);
}

MA5616AggMetroPlanDialog::~MA5616AggMetroPlanDialog()
{
    delete ui;
}

void MA5616AggMetroPlanDialog::checkEntry()
{
    bool twoInt = ui->twoIntChB->isChecked();

    QString ai1,ai2, di1, di2;
    ai1 = ui->aggInt1CB->currentText();
    ai2 = ui->aggInt2CB->currentText();
    di1 = ui->ai1diCB->currentText();
    di2 = ui->ai2diCB->currentText();

    bool a1i1  = !ai1.isEmpty();
    bool a1i1d = !di1.isEmpty();
    bool a1i2 = !ai2.isEmpty();
    bool a1i2d = !di2.isEmpty();




    bool ad = !ui->aggDescLE->text().isEmpty();
    bool aa = !ui->aggAllowPassLE->text().isEmpty();

    bool int2 = (twoInt && a1i2 && a1i2d)? true : false;
    if(!twoInt)
        int2 = true;

    if(twoInt)
    {
        if(ai1.compare(ai2) == 0)
            int2 = false;

        if(di1.compare(di2) == 0)
            int2 = false;
    }

    if(a1i1 && a1i1d && ad && aa && int2 )
        ui->okBtn->setEnabled(true);
    else
        ui->okBtn->setEnabled(false);

}

void MA5616AggMetroPlanDialog::on_aggCB_currentIndexChanged(int index)
{
    index++;
    AggChanged = true;
    int aggId = ui->aggCB->currentData().toInt();
    ui->aggInt1CB->clear();
    ui->aggInt2CB->clear();

    ui->aggInt1CB->blockSignals(true);
    ui->aggInt2CB->blockSignals(true);

    QSqlQuery *query;
    if(dbMan->dslamAggExistance(dslamId))
    {
        query = dbMan->selectAggDslam(dslamId);
        //`agg1_id`, `agg2_id`, `agg1_port1_id`, `agg1_port2_id`, `agg2_port1_id`,
        //`agg2_port2_id`, `agg1`, `agg2`, `agg1_eth`, `agg2_eth`, `agg1_port1`,
        //`agg1_port2`, `agg2_port1`, `agg2_port2`, `allow_pass`, `description`
        if(query->next())
        {
            int agg1Id = query->value(0).toInt();
            int p11 = query->value(2).toInt();
            int p12 = query->value(3).toInt();
            if(query->value(3).isNull())
                p12 = -1;
            QString int11 = query->value(10).toString();
            QString int12 = query->value(11).toString();

            int agg2Id = query->value(1).toInt();
            int p21 = query->value(4).toInt();
            int p22 = query->value(5).toInt();
            if(query->value(5).isNull())
                p22 = -1;
            QString int21 = query->value(12).toString();
            QString int22 = query->value(13).toString();

            if(aggId == agg1Id)
            {
                ui->aggInt1CB->addItem(int11,p11);
                if(p12 > -1)
                    ui->aggInt2CB->addItem(int12,p12);
            }

            if(aggId == agg2Id)
            {
                ui->aggInt1CB->addItem(int21,p21);
                if(p22 > -1)
                    ui->aggInt2CB->addItem(int22,p22);
            }
        }
    }

    //eth
    int eth = dbMan->getNewEtherTrunk(aggId, 3); // 3 eth empty consequently
    ui->aggEthSB->setValue(eth);
    //int
    query = dbMan->selectDeviveEmptyPorts(aggId);
    //`id`, `interface_type`, `interface`
    int id;
    QString type,port;
    while(query->next())
    {
        id = query->value(0).toInt();
        type = query->value(1).toString();
        type = dbMan->getInterfaceTypeSymble(type);
        port = query->value(2).toString();
        port = type + "-"+port;
        ui->aggInt1CB->addItem(port, id);
        ui->aggInt2CB->addItem(port, id);
    }

    ui->aggInt1CB->setCurrentIndex(0);
    ui->aggInt2CB->setCurrentIndex(-1);
    ui->aggInt1CB->blockSignals(false);
    ui->aggInt2CB->blockSignals(false);
    checkEntry();
}

void MA5616AggMetroPlanDialog::on_aggEthSB_valueChanged(int arg1)
{
    arg1++;
    AggChanged = true;
    checkEntry();
}

void MA5616AggMetroPlanDialog::on_amCB_currentIndexChanged(int index)
{
    index++;
    AggChanged = true;
    checkEntry();
}

void MA5616AggMetroPlanDialog::on_aggInt1CB_currentIndexChanged(int index)
{
    index++;
    AggChanged = true;
    checkEntry();
}

void MA5616AggMetroPlanDialog::on_ai1diCB_currentIndexChanged(int index)
{
    index++;
    AggChanged = true;
    checkEntry();
}

void MA5616AggMetroPlanDialog::on_okBtn_clicked()
{
    if(AggChanged)
    {
        int aggId = ui->aggCB->currentData().toInt();
        bool IsAgg1 = dbMan->isDevexAgg1(aggId);
        bool ok = true;

        if(IsAgg1)
        {
            int agg1Id = aggId;
            int agg1port1Id = ui->aggInt1CB->currentData().toInt();
            int agg1port2Id = ui->aggInt2CB->currentData().toInt();
            QString agg1Int1 = ui->aggInt1CB->currentText();
            QString agg1Int2 = ui->aggInt2CB->currentText();

            bool twoInt = ui->twoIntChB->isChecked();
            if(!twoInt)
            {
                agg1port2Id = -1;
                agg1Int2 = "";
            }

            int agg1Eth = ui->aggEthSB->value();

            QString desc = ui->aggDescLE->text();
            QString agg1 = ui->aggCB->currentText();
            agg1 = agg1.replace(" ","");
            //dslam ports
            QString a1i1d = ui->ai1diCB->currentText();
            QString a1i2d = ui->ai2diCB->currentText();

            QString a1m = ui->amCB->currentText();

            QString exch = dbMan->getExchangeAreaName(exchId);

            //QSqlQuery *query;

            //........................................plan

            if(dbMan->dslamAggExistance(dslamId))
            {
/*
                query = dbMan->selectAggDslam(dslamId);
                //`agg1_id`, `agg2_id`, `agg1_port1_id`, `agg1_port2_id`, `agg2_port1_id`,
                //`agg2_port2_id`, `agg1`, `agg2`, `agg1_eth`, `agg2_eth`, `agg1_port1`,
                //`agg1_port2`, `agg2_port1`, `agg2_port2`, `allow_pass`, `description`
                if(query->next())
                {
                    int a1 = query->value(0).toInt();
                    int a2 = query->value(1).toInt();
                    if(query->value(0).isNull())
                        a1 = -1;
                    if(query->value(1).isNull())
                        a2 = -1;

                    int a1p1 = query->value(2).toInt();
                    int a1p2 = query->value(3).toInt();
                    if(query->value(3).isNull())
                        a1p2 = -1;
                    int a2p1 = query->value(4).toInt();
                    int a2p2 = query->value(5).toInt();
                    if(query->value(5).isNull())
                        a2p2 = -1;

                    int e1 = query->value(8).toInt();
                    int e2 = query->value(9).toInt();
                    QString v = query->value(14).toString();

                    if(a1 > -1)
                    {
                        ok = ok && dbMan->deleteEtherTrunk(a1,e1);
                        ok = ok && dbMan->depleteInterface(a1p1);
                        ok = ok && dbMan->depleteInterface(a1p2);

                        QList<int> VL = dbMan->rangeToList(v);
                        foreach (int V, VL)
                            ok = ok && dbMan->deleteVlan(a1, V);

                    }
                    if(a2 > -1)
                    {
                        ok = ok && dbMan->deleteEtherTrunk(a2,e2);
                        ok = ok && dbMan->depleteInterface(a2p1);
                        ok = ok && dbMan->depleteInterface(a2p2);

                        QList<int> VL = dbMan->rangeToList(v);
                        foreach (int V, VL)
                           ok = ok && dbMan->deleteVlan(a2, V);

                    }

                } else ok = false;

                if(ok)
                    ok = ok && dbMan->deleteAggDslam(dslamId);

*/
                if(!dbMan->deleteAggDslamPlan(dslamId))
                {
                    QMessageBox::warning(this,"warning", "Problem removing old aggregation plan.");
                    return ;
                }
            }

            if(dbMan->isEtherTrunkUsed(agg1Id, agg1Eth) )
            {
                ok = false;
                QString msg = QString("Aggregation 1 ether-Trunk is used for %1. Change your ether-trunk.").arg(dbMan->getEtherTrunkDescription(agg1Id, agg1Eth));
                QMessageBox::warning(this,"ERROR", msg);
                return;
            }

            //...............................................
            // VLAN
            QString Desc;

            foreach (int vlan, totalVlanList)
            {
                if(!dbMan->insertVlan(agg1Id,vlan,desc))
                {
                    ok = false;
                    QMessageBox::warning(this,"ERROR", "Cannot insert Aggregation 1 VLAN "+QString::number(vlan)+".\n"+dbMan->lastError());
                }
            }
            //etherTrunk
            if(ok)
                if(!dbMan->insertEtherTrunk(agg1Id,agg1Eth,desc))
                {
                    ok = false;
                    QMessageBox::warning(this,"ERROR", "Cannot insert Aggregation 1 Ether-Trunk "+QString::number(agg1Eth)+".\n"+dbMan->lastError());
                }

            //AGG DSLAM
            //`dslam_id`, `agg1_id`, `agg2_id`, `agg1_port1_id`, `agg1_port2_id`, `agg2_port1_id`, `agg2_port2_id`, `agg1`, `agg2`, `agg1_eth`, `agg2_eth`, `agg1_interface1`, `agg1_interface2`, `agg2_interface1`, `agg2_interface2`, `allow_pass`, `description`

            QString ap = totalAllowPassSL.join(", ");
            if(ok)
            {
                if(dbMan->insertAggDslam(dslamId, agg1Id, -1 ,agg1port1Id, agg1port2Id , -1 , -1,agg1 , "" , agg1Eth, -1,agg1Int1,agg1Int2, "", "",ap,desc))
                {
                    // insert in port and ODF
                    //int portId, QString module, int peerExchId, QString peerExch, QString dev, QString peerInt, QString peerLabel, int eth

                    if(!dbMan->updateInterface(agg1port1Id,a1m, exch,"DSLAM MA5616",a1i1d,desc+"-"+a1i1d,agg1Eth, ""))
                    {
                        ok = false;
                        QMessageBox::warning(this,"ERROR", "Cannot assign agregation 1 interface port 1.\n"+dbMan->lastError());
                    }

                    if(twoInt)
                        if(!dbMan->updateInterface(agg1port2Id,a1m, exch,"DSLAM MA5616",a1i2d,desc+"-"+a1i2d,agg1Eth, ""))
                        {
                            ok = false;
                            QMessageBox::warning(this,"ERROR", "Cannot assign agregation 1 interface port 2.\n"+dbMan->lastError());
                        }

                }
                else
                {
                    ok = false;
                    QMessageBox::warning(this,"ERROR", "Cannot assign agregation plan.\n"+dbMan->lastError());
                }
            }

            Desc = agg1 +"- "+agg1Int1;
            ok = ok && dbMan->insertUpdateDslamInterface(dslamId, a1i1d, Desc);
            Desc = agg1 +"- "+agg1Int2;
            if(twoInt)
                ok = ok && dbMan->insertUpdateDslamInterface(dslamId, a1i2d, Desc);

            if(!ok)
            {
                ok = true;

                foreach (int vlan, totalVlanList)
                {
                    if(vlan < 4000)
                        ok = ok && dbMan->deleteVlan(agg1Id,vlan);

                }

                ok = ok && dbMan->deleteEtherTrunk(agg1Id, agg1Eth);
                ok = ok && dbMan->depleteInterface(agg1port1Id);
                if(twoInt)
                    ok = ok && dbMan->depleteInterface(agg1port2Id);
                ok = ok && dbMan->deleteAggDslam(dslamId);

                ok = ok && dbMan->insertUpdateDslamInterface(dslamId, a1i1d, "");
                if(twoInt)
                    ok = ok && dbMan->insertUpdateDslamInterface(dslamId, a1i2d, "");


                QMessageBox::warning(this,"ERROR", "Remove the existing plan and try again. ");

            }
        }
        else
        { //MA5616 is connected to agg2

            int agg2Id = ui->aggCB->currentData().toInt();
            int agg2port1Id = ui->aggInt1CB->currentData().toInt();
            int agg2port2Id = ui->aggInt2CB->currentData().toInt();
            QString agg2Int1 = ui->aggInt1CB->currentText();
            QString agg2Int2 = ui->aggInt2CB->currentText();

            bool twoInt = ui->twoIntChB->isChecked();


            int agg2Eth = ui->aggEthSB->value();

            QString desc = ui->aggDescLE->text();
            QString agg2 = ui->aggCB->currentText();
            agg2 = agg2.replace(" ","");
            //dslam ports
            QString ai1d = ui->ai1diCB->currentText();
            QString ai2d = ui->ai2diCB->currentText();

            if(!twoInt)
            {
                agg2port2Id = -1;
                agg2Int2 = "";
                ai2d = "";
            }

            QString am = ui->amCB->currentText();

            QString exch = dbMan->getExchangeAreaName(exchId);

            //QSqlQuery *query;

            if(dbMan->dslamAggExistance(dslamId))
            {
                /*
                query = dbMan->selectAggDslam(dslamId);
                //`agg1_id`, `agg2_id`, `agg1_port1_id`, `agg1_port2_id`, `agg2_port1_id`,
                //`agg2_port2_id`, `agg1`, `agg2`, `agg1_eth`, `agg2_eth`, `agg1_port1`,
                //`agg1_port2`, `agg2_port1`, `agg2_port2`, `allow_pass`, `description`
                if(query->next())
                {
                    int a1 = query->value(0).toInt();
                    int a2 = query->value(1).toInt();
                    if(query->value(0).isNull())
                        a1 = -1;
                    if(query->value(1).isNull())
                        a2 = -1;

                    int a1p1 = query->value(2).toInt();
                    int a2p1 = query->value(4).toInt();

                    int e1 = query->value(8).toInt();
                    int e2 = query->value(9).toInt();
                    QString v = query->value(14).toString();

                    if(a1 > -1)
                    {
                        ok = ok && dbMan->deleteEtherTrunk(a1,e1);
                        ok = ok && dbMan->depleteInterface(a1p1);

                        QList<int> VL = dbMan->rangeToList(v);
                        foreach (int V, VL)
                            ok = ok && dbMan->deleteVlan(a1, V);

                    }
                    if(a2 > -1)
                    {
                        ok = ok && dbMan->deleteEtherTrunk(a2,e2);
                        ok = ok && dbMan->depleteInterface(a2p1);

                        QList<int> VL = dbMan->rangeToList(v);
                        foreach (int V, VL)
                           ok = ok && dbMan->deleteVlan(a2, V);

                    }

                } else ok = false;

                if(ok)
                    ok = ok && dbMan->deleteAggDslam(dslamId);

*/
                if(!dbMan->deleteAggDslamPlan(dslamId))
                {
                    QMessageBox::warning(this,"warning", "Problem removing old aggregation plan.");
                    return ;
                }
            }

            if(dbMan->isEtherTrunkUsed(agg2Id, agg2Eth) )
            {
                ok = false;
                QString msg = QString("Aggregation 2 ether-Trunk is used for %1. Change your ether-trunk.").arg(dbMan->getEtherTrunkDescription(agg2Id, agg2Eth));
                QMessageBox::warning(this,"ERROR", msg);
                return;
            }


            //...............................................
            // VLAN
            QString Desc;

            foreach (int vlan, totalVlanList)
            {
                if(!dbMan->insertVlan(agg2Id,vlan,desc))
                {
                    ok = false;
                    QMessageBox::warning(this,"ERROR", "Cannot insert Aggregation 2 VLAN "+QString::number(vlan)+".\n"+dbMan->lastError());
                }
            }

            //...............................................
            //etherTrunk
            if(ok)
                if(!dbMan->insertEtherTrunk(agg2Id,agg2Eth,desc))
                {
                    ok = false;
                    QMessageBox::warning(this,"ERROR", "Cannot insert Aggregation 2 Ether-Trunk "+QString::number(agg2Eth)+".\n"+dbMan->lastError());
                }
            //...............................................
            //AGG DSLAM
            //`dslam_id`, `agg1_id`, `agg2_id`, `agg1_port1_id`, `agg1_port2_id`, `agg2_port1_id`, `agg2_port2_id`, `agg1`, `agg2`, `agg1_eth`, `agg2_eth`, `agg1_interface1`, `agg1_interface2`, `agg2_interface1`, `agg2_interface2`, `allow_pass`, `description`

            QString ap = totalAllowPassSL.join(", ");

            if(ok)
            {
                if(dbMan->insertAggDslam( dslamId, -1, agg2Id ,-1, -1 , agg2port1Id, agg2port2Id,"" , agg2, -1, agg2Eth ,"","", agg2Int1, agg2Int2,ap,desc))
                {
                    // insert in port and ODF
                    //int portId, QString module, int peerExchId, QString peerExch, QString dev, QString peerInt, QString peerLabel, int eth

                    if(!dbMan->updateInterface(agg2port1Id,am, exch,"DSLAM MA5616",ai1d,desc+"-"+ai1d,agg2Eth, ""))
                    {
                        ok = false;
                        QMessageBox::warning(this,"ERROR", "Cannot assign agregation 2 interface port 1.\n"+dbMan->lastError());
                    }

                    if(twoInt)
                        if(!dbMan->updateInterface(agg2port2Id,am, exch,"DSLAM MA5616",ai2d,desc+"-"+ai2d,agg2Eth, ""))
                        {
                            ok = false;
                            QMessageBox::warning(this,"ERROR", "Cannot assign agregation 2 interface port 2.\n"+dbMan->lastError());
                        }

                }
                else
                {
                    ok = false;
                    QMessageBox::warning(this,"ERROR", "Cannot assign agregation plan.\n"+dbMan->lastError());
                }
            }

            Desc = agg2 +"- "+agg2Int1;
            ok = ok && dbMan->insertUpdateDslamInterface(dslamId, ai1d, Desc);


            Desc = agg2 +"- "+agg2Int2;
            if(twoInt)
                ok = ok && dbMan->insertUpdateDslamInterface(dslamId, ai2d, Desc);

            if(!ok)
            {
                ok = true;

                foreach (int vlan, totalVlanList)
                {
                    if(vlan < 4000)
                        ok = ok && dbMan->deleteVlan(agg2Id,vlan);

                }

                ok = ok && dbMan->deleteEtherTrunk(agg2Id, agg2Eth);
                ok = ok && dbMan->depleteInterface(agg2port1Id);
                if(twoInt)
                    ok = ok && dbMan->depleteInterface(agg2port2Id);
                ok = ok && dbMan->deleteAggDslam(dslamId);

                ok = ok && dbMan->insertUpdateDslamInterface(dslamId, ai1d, "");
                if(twoInt)
                    ok = ok && dbMan->insertUpdateDslamInterface(dslamId, ai2d, "");

                QMessageBox::warning(this,"ERROR", "Remove the existing plan and try again. ");
            }
        }

        if(ok)
        {
            this->close();
        }
    }
    else
    {
        this->close();
    }
}

void MA5616AggMetroPlanDialog::on_twoIntChB_toggled(bool checked)
{
    AggChanged = true;

    if(checked)
        ui->agg1Int2Frame->setVisible(true);
    else
        ui->agg1Int2Frame->setVisible(false);

    checkEntry();
}

void MA5616AggMetroPlanDialog::on_aggInt2CB_currentIndexChanged(int index)
{
    index++;
    AggChanged = true;
    checkEntry();
}

void MA5616AggMetroPlanDialog::on_ai2diCB_currentIndexChanged(int index)
{
    index++;
    AggChanged = true;
    checkEntry();
}
