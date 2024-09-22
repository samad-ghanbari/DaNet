#include "ma5600aggmetroplandialog.h"
#include "ui_ma5600aggmetroplandialog.h"
#include <QSqlQuery>
#include "lib/database/danetdbman.h"
#include <QMessageBox>

MA5600AggMetroPlanDialog::MA5600AggMetroPlanDialog(QWidget *parent, DanetDbMan *Db, const int ExchId, const int DslamId) :
    QDialog(parent),
    ui(new Ui::MA5600AggMetroPlanDialog),
    dbMan(Db),
    exchId(ExchId),
    dslamId(DslamId)

{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowMinMaxButtonsHint);

    ui->tabWidget->tabBar()->hide();

    int motherId = dbMan->getSiteMotherId(ExchId);

    QSqlQuery *query;
    ui->agg1CB->blockSignals(true);
    //Agg1
    if(motherId > -1)
    {
        if(motherId == 37)
            query = dbMan->selectExchangeAgg(ExchId, 38, true);
        else
            query = dbMan->selectExchangeAgg(ExchId, motherId, true);
    }
    else
    {
        query = dbMan->selectExchangeAgg(ExchId, true);
    }

    //`id`,`device_name`
    int ID;
    QString NAME;

    while(query->next())
    {
        ID = query->value(0).toInt();
        NAME = query->value(1).toString();
        ui->agg1CB->addItem(NAME, ID);
    }
    ui->agg1CB->setCurrentIndex(-1);
    ui->agg1CB->blockSignals(false);


    //Agg2
    ui->agg2CB->blockSignals(true);
    if(motherId > -1)
    {
        if(motherId == 37)
            query = dbMan->selectExchangeAgg(ExchId, 38, false);
        else

            query = dbMan->selectExchangeAgg(ExchId, motherId, false);
    }
    else
        query = dbMan->selectExchangeAgg(ExchId,false);

    //`id`,`device_name`
    while(query->next())
    {
        ID = query->value(0).toInt();
        NAME = query->value(1).toString();
        ui->agg2CB->addItem(NAME, ID);
    }
    ui->agg2CB->setCurrentIndex(-1);
    ui->agg2CB->blockSignals(false);

    //CX600
    ui->cxCB->blockSignals(true);

    if(motherId > -1) // site
    {
        //if GR2 GR3 set GR1
        int m = dbMan->exchangeGR23Except(motherId);
        query = dbMan->selectExchangeCx(ExchId, m);

        //if h-kv set ms
        m = dbMan->exchangeKhvExcept(motherId);
        query = dbMan->selectExchangeCx(m);

    }
    else
    { // exchange
        //if GR2 GR3 set GR1
        int e = dbMan->exchangeGR23Except(ExchId);
        query = dbMan->selectExchangeCx(e);

        //if h-kv set ms
        int m = dbMan->exchangeKhvExcept(motherId);
        query = dbMan->selectExchangeCx(m);

    }



    //id , deviceName
    while(query->next())
    {
        ID = query->value(0).toInt();
        NAME = query->value(1).toString();
        ui->cxCB->addItem(NAME, ID);
    }
    ui->cxCB->setCurrentIndex(-1);
    ui->cxCB->blockSignals(false);


    ui->metroConnCB->setChecked(true);
    ui->agg2ConnCB->setChecked(true);

    //Modules
    query = dbMan->selectModules();
    int i;
    QString m;
    while(query->next())
    {
        i = query->value(0).toInt();
        m = query->value(1).toString();

        ui->a1mCB->addItem(m, i);
        ui->a2mCB->addItem(m, i);
        ui->cxmCB->addItem(m, i);
    }

    // DSLAM PORTS
    ui->cxi1diCB->blockSignals(true);
    ui->cxi1diCB->addItem("1G-0/7/0", 0);
    ui->cxi1diCB->addItem("1G-0/7/1", 1);
    ui->cxi1diCB->addItem("1G-0/7/2", 2);
    ui->cxi1diCB->addItem("1G-0/7/3", 3);
    ui->cxi1diCB->addItem("1G-0/7/4", 4);
    ui->cxi1diCB->addItem("1G-0/7/5", 5);
    ui->cxi1diCB->blockSignals(false);

    ui->cxi2diCB->blockSignals(true);
    ui->cxi2diCB->addItem("1G-0/8/0", 0);
    ui->cxi2diCB->addItem("1G-0/8/1", 1);
    ui->cxi2diCB->addItem("1G-0/8/2", 2);
    ui->cxi2diCB->addItem("1G-0/8/3", 3);
    ui->cxi2diCB->addItem("1G-0/8/4", 4);
    ui->cxi2diCB->addItem("1G-0/8/5", 5);
    ui->cxi2diCB->blockSignals(false);

    ui->a1i1diCB->blockSignals(true);
    ui->a1i1diCB->addItem("1G-0/7/0", 0);
    ui->a1i1diCB->addItem("1G-0/7/1", 1);
    ui->a1i1diCB->addItem("1G-0/7/2", 2);
    ui->a1i1diCB->addItem("1G-0/7/3", 3);
    ui->a1i1diCB->addItem("1G-0/7/4", 4);
    ui->a1i1diCB->addItem("1G-0/7/5", 5);
    ui->a1i1diCB->blockSignals(false);

    ui->a1i2diCB->blockSignals(true);
    ui->a1i2diCB->addItem("1G-0/7/0", 0);
    ui->a1i2diCB->addItem("1G-0/7/1", 1);
    ui->a1i2diCB->addItem("1G-0/7/2", 2);
    ui->a1i2diCB->addItem("1G-0/7/3", 3);
    ui->a1i2diCB->addItem("1G-0/7/4", 4);
    ui->a1i2diCB->addItem("1G-0/7/5", 5);
    ui->a1i2diCB->blockSignals(false);

    ui->a2i1diCB->blockSignals(true);
    ui->a2i1diCB->addItem("1G-0/8/0", 0);
    ui->a2i1diCB->addItem("1G-0/8/1", 1);
    ui->a2i1diCB->addItem("1G-0/8/2", 2);
    ui->a2i1diCB->addItem("1G-0/8/3", 3);
    ui->a2i1diCB->addItem("1G-0/8/4", 4);
    ui->a2i1diCB->addItem("1G-0/8/5", 5);
    ui->a2i1diCB->blockSignals(false);

    ui->a2i2diCB->blockSignals(true);
    ui->a2i2diCB->addItem("1G-0/8/0", 0);
    ui->a2i2diCB->addItem("1G-0/8/1", 1);
    ui->a2i2diCB->addItem("1G-0/8/2", 2);
    ui->a2i2diCB->addItem("1G-0/8/3", 3);
    ui->a2i2diCB->addItem("1G-0/8/4", 4);
    ui->a2i2diCB->addItem("1G-0/8/5", 5);
    ui->a2i2diCB->blockSignals(false);

    ui->a1mCB->setCurrentText("SX-500m");
    ui->a2mCB->setCurrentText("SX-500m");
    ui->cxmCB->setCurrentText("SX-500m");

    //desc
    QString dslamName = dbMan->getDslamName(dslamId);
    ui->aggDescLE->setText(dslamName.trimmed());
    ui->agg2DescLE->setText(dslamName.trimmed());
    ui->cxDescLE->setText(dslamName.trimmed());
    ui->dslamLbl->setText(dslamName);

    // VLANS
    //int min;
    QStringList  aggAllowPassVlans, totalAllowPassVlans; //cxAllowPassVlans,
    totalVlanList << 10 << 11;
    totalAllowPassVlans << "10" << "11";
    QList<int> vlanList;
    QString temp;
    //    vlanList = dbMan->getDslamVasVlanList(DslamId);
    //    temp = dbMan->listToRange(vlanList);
    //    cxAllowPassVlans << temp;
    //    totalAllowPassVlans << temp;
    //    foreach(int i,vlanList)
    //    {
    //        cxVlanList << i;
    //        totalVlanList << i;
    //    }

    //    vlanList = dbMan->getDslamIpMediaVlanList(DslamId);
    //    temp = dbMan->listToRange(vlanList);
    //    cxAllowPassVlans << temp;
    //    totalAllowPassVlans << temp;
    //    foreach(int i, vlanList)
    //    {
    //        cxVlanList << i;
    //        totalVlanList << i;
    //    }


    //    min = dbMan->getDslamBtvVlan(DslamId);
    //    if(min > 399)
    //    {
    //        cxAllowPassVlans << QString::number(min);
    //        totalAllowPassVlans << QString::number(min);
    //        cxVlanList << min;
    //        totalVlanList << min;
    //    }

    //Agg VLANs 10 11 5xx 6xx 7xx 8xx
    aggAllowPassVlans << "10" <<"11";

    aggVlanList << 10 <<11;

    vlanList = dbMan->getDslamPppoeVlanList(DslamId);
    temp = dbMan->listToRange(vlanList);
    aggAllowPassVlans << temp;
    totalAllowPassVlans << temp;
    foreach(int i,vlanList)
    {
        aggVlanList << i;
        totalVlanList << i;
    }

    vlanList = dbMan->getDslamInetVlanList(DslamId);
    temp = dbMan->listToRange(vlanList);

    aggAllowPassVlans << temp;
    totalAllowPassVlans << temp;
    foreach(int i,vlanList)
    {
        aggVlanList << i;
        totalVlanList << i;
    }


    vlanList = dbMan->getDslamSinetVlanList(DslamId);
    temp = dbMan->listToRange(vlanList);
    aggAllowPassVlans << temp;
    totalAllowPassVlans << temp;
    foreach(int i, vlanList)
    {
        aggVlanList << i;
        totalVlanList << i;
    }


    vlanList = dbMan->getDslamVoipVlanList(DslamId);
    temp = dbMan->listToRange(vlanList);
    aggAllowPassVlans << temp;
    totalAllowPassVlans << temp;
    foreach(int i, vlanList)
    {
        aggVlanList << i;
        totalVlanList << i;
    }


    //    cxAllowPass = cxAllowPassVlans.join(",");
    //    cxAllowPass = cxAllowPass.replace(",",", ");
    //    ui->cxAllowPassLE->setText(cxAllowPass);

    aggAllowPass = aggAllowPassVlans.join(",");
    aggAllowPass = aggAllowPass.replace(",",", ");
    ui->aggAllowPassLE->setText(aggAllowPass);
    ui->agg2AllowPassLE->setText(aggAllowPass);

    totalAllowPass = totalAllowPassVlans.join(",");
    totalAllowPass = totalAllowPass.replace(",",", ");


    ui->cxi1diCB->setCurrentIndex(0);
    ui->cxi2diCB->setCurrentIndex(0);

    ui->a1i1diCB->setCurrentIndex(2);
    ui->a1i2diCB->setCurrentIndex(3);

    ui->a2i1diCB->setCurrentIndex(2);
    ui->a2i2diCB->setCurrentIndex(3);



    bool a = dbMan->dslamAggExistance(dslamId);

    ui->agg1CB->setCurrentIndex(-1);
    ui->agg2CB->setCurrentIndex(-1);
    ui->cxCB->setCurrentIndex(-1);


    if(a)
    {
        if(dbMan->cxDslamExistance(dslamId))
        {
            ui->metroConnCB->setChecked(true);
            query = dbMan->selectCxDslam(dslamId);
            //`dslam_id`, `cx_id`, `port1_id`, `port2_id`, `cx`, `ether_trunk`, `interface1`, `interface2`, `allow_pass`, `description`
            if(query->next())
            {
                int cxId = query->value(1).toInt();
                int p1 = query->value(2).toInt();
                int p2;
                if(query->value(3).isNull())
                    p2 = -1;
                else
                    p2 = query->value(3).toInt();
                int e = query->value(5).toInt();

                ui->cxCB->setCurrentIndex(ui->cxCB->findData(cxId));
                ui->cxInt1CB->setCurrentIndex(ui->cxInt1CB->findData(p1));
                if(p2 > -1)
                    ui->cxInt2CB->setCurrentIndex(ui->cxInt2CB->findData(p2));
                else
                    ui->cxdIntCB->setChecked(false);
                ui->cxEthSB->setValue(e);

                QStringList modPeerPort = dbMan->getModulePeerInterface(p1);
                ui->cxmCB->setCurrentText(modPeerPort.at(0));
                ui->cxi1diCB->setCurrentText(modPeerPort.at(1));

                if(p2 > -1)
                {
                    modPeerPort = dbMan->getModulePeerInterface(p2);
                    ui->cxi2diCB->setCurrentText(modPeerPort.at(1));
                }

            }

            ui->okBtn->setEnabled(false);
        }
        else
            ui->metroConnCB->setChecked(false);



        ui->agg2ConnCB->setChecked(true);
        query = dbMan->selectAggDslam(dslamId);

        //0`agg1_id`, 1`agg2_id`, 2`agg1_int1_id`, 3`agg1_int2_id`, 4`agg2_int1_id`, 5`agg2_int2_id`, 6`agg1`,
        //7`agg2`, 8`agg1_eth`, 9`agg2_eth`, 10`agg1_interface1`, 11`agg1_interface2`, 12`agg2_interface1`, 13`agg2_interface2`, 14`allow_pass`, 15`description`
        if(query->next())
        {
            int ag1Id;
            if(query->value(0).isNull())
                ag1Id = -1;
            else
                ag1Id = query->value(0).toInt();

            int ag2Id;
            if(query->value(1).isNull())
                ag2Id = -1;
            else
                ag2Id = query->value(1).toInt();

            int a1p1Id;
            if(query->value(2).isNull())
                a1p1Id = -1;
            else
                a1p1Id = query->value(2).toInt();


            int a1p2Id;
            if(query->value(3).isNull())
                a1p2Id = -1;
            else
                a1p2Id = query->value(3).toInt();

            int a2p1Id;
            if(query->value(4).isNull())
                a2p1Id = -1;
            else
                a2p1Id = query->value(4).toInt();

            int a2p2Id;
            if(query->value(5).isNull())
                a2p2Id = -1;
            else
                a2p2Id = query->value(5).toInt();

            int a1e = query->value(8).toInt();

            int a2e;
            if(query->value(9).isNull())
                a2e = -1;
            else
                a2e = query->value(9).toInt();

            if(ag1Id > -1)
            {
                ui->agg1CB->setCurrentIndex(ui->agg1CB->findData(ag1Id));
                ui->agg1Int1CB->setCurrentIndex(ui->agg1Int1CB->findData(a1p1Id));
                ui->agg1EthSB->setValue(a1e);
                QStringList mi = dbMan->getModulePeerInterface(a1p1Id);
                ui->a1mCB->setCurrentText(mi.at(0));
                ui->a1i1diCB->setCurrentText(mi.at(1));
                if(a1p2Id > -1)
                {
                    ui->agg1Int2CB->setCurrentIndex(ui->agg1Int2CB->findData(a1p2Id));
                    mi = dbMan->getModulePeerInterface(a1p2Id);
                    ui->a1i2diCB->setCurrentText(mi.at(1));
                }
                else
                    ui->agg1dIntCB->setChecked(false);
            }
            else
                ui->agg1ConnCB->setChecked(false);


            if(ag2Id > -1)
            {
                ui->agg2CB->setCurrentIndex(ui->agg2CB->findData(ag2Id));
                ui->agg2Int1CB->setCurrentIndex(ui->agg2Int1CB->findData(a2p1Id));
                ui->agg2EthSB->setValue(a2e);
                QStringList mi = dbMan->getModulePeerInterface(a2p1Id);
                ui->a2mCB->setCurrentText(mi.at(0));
                ui->a2i1diCB->setCurrentText(mi.at(1));

                if(a2p2Id > -1)
                {
                    ui->agg2Int2CB->setCurrentIndex(ui->agg2Int2CB->findData(a2p2Id));
                    mi = dbMan->getModulePeerInterface(a2p2Id);
                    ui->a2i2diCB->setCurrentText(mi.at(1));
                }
                else
                    ui->agg2dIntCB->setChecked(false);

            }
            else
                ui->agg2ConnCB->setChecked(false);

        }
        ui->okBtn->setEnabled(false);
    }


    AggChanged = false;
    cxChanged = false;
    ui->okBtn->setEnabled(false);

}

MA5600AggMetroPlanDialog::~MA5600AggMetroPlanDialog()
{
    delete ui;
}

void MA5600AggMetroPlanDialog::checkEntry()
{
    bool a1i1  = !ui->agg1Int1CB->currentText().isEmpty() || !ui->agg1ConnCB->isChecked();
    bool a1i2  = !ui->agg1Int2CB->currentText().isEmpty() || !ui->agg1dIntCB->isChecked() || !ui->agg1ConnCB->isChecked();
    bool a1i1d = !ui->a1i1diCB->currentText().isEmpty()|| !ui->agg1ConnCB->isChecked();
    bool a1i2d = !ui->a1i2diCB->currentText().isEmpty() || !ui->agg1dIntCB->isChecked() || !ui->agg1ConnCB->isChecked();

    if(a1i1 && a1i2 && a1i1d && a1i2d)
        ui->nextBtn->setEnabled(true);
    else
        ui->nextBtn->setEnabled(false);

    bool a2i1  = !ui->agg2Int1CB->currentText().isEmpty() || !ui->agg2ConnCB->isChecked();
    bool a2i2  = !ui->agg2Int2CB->currentText().isEmpty() || !ui->agg2ConnCB->isChecked() || !ui->agg2dIntCB->isChecked();
    bool a2i1d = !ui->a2i1diCB->currentText().isEmpty()   || !ui->agg2ConnCB->isChecked();
    bool a2i2d = !ui->a2i2diCB->currentText().isEmpty()   || !ui->agg2ConnCB->isChecked() || !ui->agg2dIntCB->isChecked();

    if(a2i1 && a2i2 && a2i1d && a2i2d)
        ui->agg2NextBtn->setEnabled(true);
    else
        ui->agg2NextBtn->setEnabled(false);

    bool cxi1  = !ui->cxInt1CB->currentText().isEmpty() || !ui->metroConnCB->isChecked();
    bool cxi2  = !ui->cxInt2CB->currentText().isEmpty() || !ui->metroConnCB->isChecked() || !ui->cxdIntCB->isChecked();
    bool cxi1d = !ui->cxi1diCB->currentText().isEmpty() || !ui->metroConnCB->isChecked();
    bool cxi2d = !ui->cxi2diCB->currentText().isEmpty() || !ui->metroConnCB->isChecked() || !ui->cxdIntCB->isChecked();

    bool ad = !ui->aggDescLE->text().isEmpty();
    bool aa = !ui->aggAllowPassLE->text().isEmpty();

    bool cd = !ui->cxDescLE->text().isEmpty() || !ui->metroConnCB->isChecked();
    //bool ca = !ui->cxAllowPassLE->text().isEmpty() || !ui->metroConnCB->isChecked();

    int a11 = ui->agg1Int1CB->currentData().toInt();
    int a12 = ui->agg1Int2CB->currentData().toInt();
    int ad11 = ui->a1i1diCB->currentData().toInt();
    int ad12 = ui->a1i2diCB->currentData().toInt();
    if(!ui->agg1dIntCB->isChecked()){ a12 += 50; ad12 += 100;}
    if(!ui->agg1ConnCB->isChecked()){a11 += 150; a12 += 200; ad11 += 250; ad12 += 300;}

    int a21 = ui->agg2Int1CB->currentData().toInt();
    int a22 = ui->agg2Int2CB->currentData().toInt();
    int ad21 = ui->a2i1diCB->currentData().toInt();
    int ad22 = ui->a2i2diCB->currentData().toInt();
    if(!ui->agg2ConnCB->isChecked()){a21 += 150; a22 += 200; ad21 += 250; ad22 += 300;}
    if(!ui->agg2dIntCB->isChecked()){a22 += 350; ad22 += 400;}

    int c1 = ui->cxInt1CB->currentData().toInt();
    int c2 = ui->cxInt2CB->currentData().toInt();
    int cd1 = ui->cxi1diCB->currentData().toInt();
    int cd2 = ui->cxi2diCB->currentData().toInt();
    if(!ui->metroConnCB->isChecked()){c1 += 450; c2 += 500; cd1 += 550; cd2 += 600;}
    if(!ui->cxdIntCB->isChecked()){c2 += 650; cd2 += 700;}

    bool a1a2c =true;
    if( (!ui->agg1ConnCB->isChecked()) && (!ui->agg2ConnCB->isChecked()) && (!ui->metroConnCB->isChecked()) )
        a1a2c = false;

    bool OK = false;
    if(a1a2c && a1i1 && a1i2 && a1i1d && a1i2d && a2i1 && a2i2 && a2i1d && a2i2d && cxi1 && cxi2 && cxi1d && cxi2d && ad && aa && cd  )//&& ca
    {
        if( (a11 == a12) || (a21 == a22) || (c1 == c2) || (ad11 == ad12) || (ad11 == cd1) || (ad12 == cd1) || (ad21 == ad22) || (ad21 == cd2) || (ad22 == cd2) )
            OK = false;
        else
            OK = true;

    }
    else
        OK = false;


    if(OK)
        ui->okBtn->setEnabled(true);
    else
        ui->okBtn->setEnabled(false);
}

void MA5600AggMetroPlanDialog::on_agg1CB_currentIndexChanged(int index)
{
    index++;
    AggChanged = true;
    int aggId = ui->agg1CB->currentData().toInt();
    ui->agg1Int1CB->clear();
    ui->agg1Int2CB->clear();

    QSqlQuery *query;

    if(dbMan->dslamAggExistance(dslamId))
    {
        query = dbMan->selectAggDslam(dslamId);
        //0`agg1_id`, 1`agg2_id`, 2`agg1_int1_id`, 3`agg1_int2_id`, 4`agg2_int1_id`, 5`agg2_int2_id`, 6`agg1`, 7`agg2`,
        //8`agg1_eth`, 9`agg2_eth`, 10`agg1_interface1`, 11`agg1_interface2`, 12`agg2_interface1`, 13`agg2_interface2`, 14`allow_pass`, 15`description`
        if(query->next())
        {
            int agg1Id = query->value(0).toInt();
            int p1 = query->value(2).toInt();
            int p2 = query->value(3).toInt();
            QString int1 = query->value(10).toString();
            QString int2 = query->value(11).toString();

            if(aggId == agg1Id)
            {
                ui->agg1Int1CB->addItem(int1,p1);
                ui->agg1Int2CB->addItem(int2,p2);
            }
        }
    }

    //eth
    int eth = dbMan->getNewEtherTrunk(aggId, 3); // 3 eth empty consequently
    ui->agg1EthSB->setValue(eth);
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
        ui->agg1Int1CB->addItem(port, id);
        ui->agg1Int2CB->addItem(port,id);
    }

    ui->agg1Int1CB->setCurrentIndex(0);
    ui->agg1Int2CB->setCurrentIndex(1);
    checkEntry();
}

void MA5600AggMetroPlanDialog::on_agg2CB_currentIndexChanged(int index)
{
    index++;
    AggChanged = true;
    int aggId = ui->agg2CB->currentData().toInt();
    ui->agg2Int1CB->clear();
    ui->agg2Int2CB->clear();

    QSqlQuery *query;

    if(dbMan->dslamAggExistance(dslamId))
    {
        query = dbMan->selectAggDslam(dslamId);
        //0`agg1_id`, 1`agg2_id`, 2`agg1_int1_id`, 3`agg1_int2_id`, 4`agg2_int1_id`, 5`agg2_int2_id`, 6`agg1`, 7`agg2`, 8`agg1_eth`, 9`agg2_eth`
        //10`agg1_interface1`, 11`agg1_interface2`, 12`agg2_interface1`, 13`agg2_interface2`, 14`allow_pass`, 15`description`
        if(query->next())
        {
            int agg2ID = query->value(1).toInt();
            int p1 = query->value(4).toInt();
            int p2 = query->value(5).toInt();
            QString int1 = query->value(12).toString();
            QString int2 = query->value(13).toString();
            if(agg2ID == aggId)
            {
                ui->agg2Int1CB->addItem(int1,p1);
                ui->agg2Int2CB->addItem(int2,p2);
            }
        }

    }

    //eth
    int eth = dbMan->getNewEtherTrunk(aggId, 3); // 3 eth empty consequently
    ui->agg2EthSB->setValue(eth);
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
        ui->agg2Int1CB->addItem(port, id);
        ui->agg2Int2CB->addItem(port,id);
    }

    ui->agg2Int1CB->setCurrentIndex(0);
    ui->agg2Int2CB->setCurrentIndex(1);
    checkEntry();
}

void MA5600AggMetroPlanDialog::on_okBtn_clicked()
{
    int agg1Id = ui->agg1CB->currentData().toInt();
    int agg1port1Id = ui->agg1Int1CB->currentData().toInt();
    QString agg1Int1 = ui->agg1Int1CB->currentText();
    int agg1port2Id = ui->agg1Int2CB->currentData().toInt();
    QString agg1Int2 = ui->agg1Int2CB->currentText();
    int agg2ID = ui->agg2CB->currentData().toInt();
    int agg2port1Id = ui->agg2Int1CB->currentData().toInt();
    QString agg2Int1 = ui->agg2Int1CB->currentText();
    int agg2port2Id = ui->agg2Int2CB->currentData().toInt();
    QString agg2Int2 = ui->agg2Int2CB->currentText();
    int agg1Eth = ui->agg1EthSB->value();
    int agg2Eth = ui->agg2EthSB->value();
    QString desc = ui->aggDescLE->text(); //dslamName
    QString agg1 = ui->agg1CB->currentText();
    agg1 = agg1.replace(" ","");
    QString agg2 = ui->agg2CB->currentText();
    agg2 = agg2.replace(" ","");
    int cxId = ui->cxCB->currentData().toInt();
    QString cx = ui->cxCB->currentText();
    cx = cx.replace(" ","");
    int cxEth = ui->cxEthSB->value();
    int cxp1Id = ui->cxInt1CB->currentData().toInt();
    int cxp2Id = ui->cxInt2CB->currentData().toInt();
    QString cxInt1 = ui->cxInt1CB->currentText();
    QString cxInt2 = ui->cxInt2CB->currentText();

    //dslam ports
    QString a1i1d = ui->a1i1diCB->currentText();
    QString a1i2d = ui->a1i2diCB->currentText();
    QString a2i1d = ui->a2i1diCB->currentText();
    QString a2i2d = ui->a2i2diCB->currentText();
    QString cxi1d = ui->cxi1diCB->currentText();
    QString cxi2d = ui->cxi2diCB->currentText();

    bool agg1p2 = ui->agg1dIntCB->isChecked();
    bool a1 = ui->agg1ConnCB->isChecked();
    bool a2 = ui->agg2ConnCB->isChecked();
    bool agg2p2 = ui->agg2dIntCB->isChecked();
    bool c = ui->metroConnCB->isChecked();
    bool cxp2 = ui->cxdIntCB->isChecked();

    if(!cxp2)
    {
        cxp2Id = -1;
        cxInt2 = "";
    }

    if(!agg1p2)
    {
        agg1port2Id = -1;
        agg1Int2 = "";
    }

    if(!agg2p2)
    {
        agg2port2Id = -1;
        agg2Int2 = "";
    }

    QString a1m = ui->a1mCB->currentText();
    QString a2m = ui->a2mCB->currentText();
    QString cxm = ui->cxmCB->currentText();

    QString exch = dbMan->getExchangeAreaName(exchId);

    if(!a1) agg1Id = -1;
    if(!a2) agg2ID = -1;
    if(!c) cxId = -1;

    bool a1Cond = (a1 && agg1p2) && (agg1port1Id == agg1port2Id);
    bool a2Cond = (a2 && agg2p2) && (agg2port1Id == agg2port2Id);
    bool cxCond = (c && cxp2) && (cxp1Id == cxp2Id);

    if( a1Cond || a2Cond || cxCond )
    {
        QMessageBox::warning(this,"ERROR", "Interfaces cannot be the same. ");
        return;
    }

    QSqlQuery *query;

    //.................................................. plan
    //Agg
    bool ok = true;
    if(AggChanged)
    {
        //remove if exists
        if(dbMan->dslamAggExistance(dslamId))
        {

            query = dbMan->selectAggDslam(dslamId);
            //0`agg1_id`, 1`agg2_id`, 2`agg1_int1_id`, 3`agg1_int2_id`, 4`agg2_int1_id`, 5`agg2_int2_id`, 6`agg1`, 7`agg2`, 8`agg1_eth`, 9`agg2_eth`,
            //10`agg1_interface1`, 11`agg1_interface2`, 12`agg2_interface1`, 13`agg2_interface2`, 14`allow_pass`, 15`description`
            if(query->next())
            {
                int a1;
                if(query->value(0).isNull())
                    a1 = -1;
                else
                    a1 = query->value(0).toInt();

                int a2;
                if(query->value(1).isNull())
                    a2 = -1;
                else
                    a2 = query->value(1).toInt();

                int a1p1;
                if(query->value(2).isNull())
                    a1p1 =  -1;
                else
                    a1p1 = query->value(2).toInt();

                int a1p2;
                if(query->value(3).isNull())
                    a1p2 =  -1;
                else
                    a1p2 = query->value(3).toInt();

                int a2p1;
                if(query->value(4).isNull())
                    a2p1 = -1;
                else
                    a2p1 = query->value(4).toInt();

                int a2p2;
                if(query->value(5).isNull())
                    a2p2 = -1;
                else
                    a2p2 = query->value(5).toInt();

                int e1;
                if(query->value(8).isNull())
                    e1 = -1;
                else
                    e1= query->value(8).toInt();

                int e2;
                if(query->value(9).isNull())
                    e2 = -1;
                else
                    e2= query->value(9).toInt();

                QString v = query->value(14).toString();

                if(a1 > -1)
                {
                    ok = ok && dbMan->deleteEtherTrunk(a1,e1);
                    ok = ok && dbMan->depleteInterface(a1p1);
                    if(a1p2 > -1)
                        ok = ok && dbMan->depleteInterface(a1p2);
                }

                if(a2 > -1)
                {
                    ok = ok && dbMan->deleteEtherTrunk(a2,e2);
                    if(a2p1 > -1)
                        ok = ok && dbMan->depleteInterface(a2p1);
                    if(a2p2 > -1)
                        ok = ok && dbMan->depleteInterface(a2p2);
                }

                QList<int> VL = dbMan->rangeToList(v);
                foreach (int V, VL)
                {
                    if(a1 > -1)
                        ok = ok && dbMan->deleteVlan(a1, V);
                    if(a2 > -1)
                        ok = ok && dbMan->deleteVlan(a2, V);
                }
            }
            else ok = false;

            ok = ok && dbMan->deleteAggDslamPlan(dslamId);

            if(!ok)
            {
                QMessageBox::warning(this,"warning", "Problem removing old aggregation plan.");
                ok = false;
                return;
            }

        }

        //check ether trunk
        if(a1)
        {
            if(dbMan->isEtherTrunkUsed(agg1Id, agg1Eth) )
            {
                QString msg = QString("Aggregation 1 ether-Trunk is used for %1. Change your ether-trunk.").arg(dbMan->getEtherTrunkDescription(agg1Id, agg1Eth));
                QMessageBox::warning(this,"ERROR", msg);
                ok = false;
                return;
            }
        }

        if(a2)
        {
            if(dbMan->isEtherTrunkUsed(agg2ID, agg2Eth))
            {
                QString msg = QString("Aggregation 2 ether-Trunk is used for %1. Change your ether-trunk.").arg(dbMan->getEtherTrunkDescription(agg2ID, agg2Eth));
                QMessageBox::warning(this,"ERROR", msg);
                ok = false;
                return;
            }
        }


        if(ok)
        {
            //vlan
            QString Desc;
            if(c)
            {//with CX
                if(a1)
                {
                    //with agg1
                    foreach (int vlan, aggVlanList)
                    {
                        if(vlan == 10)
                            Desc = "Global-Connection";
                        else if(vlan == 11)
                            Desc = "MPLS-VPN_nms";
                        else
                            Desc = desc;

                        if(!dbMan->insertVlan(agg1Id,vlan,Desc))
                        {
                            ok = false;
                            QMessageBox::warning(this,"ERROR", "Cannot insert Aggregation 1 VLAN "+QString::number(vlan)+".\n"+dbMan->lastError());
                            break;
                        }
                    }
                }

                if(a2)
                {//with agg2
                    foreach (int vlan, aggVlanList)
                    {
                        if(vlan == 10)
                            Desc = "Global-Connection";
                        else if(vlan == 11)
                            Desc = "MPLS-VPN_nms";
                        else
                            Desc = desc;

                        if(!dbMan->insertVlan(agg2ID,vlan,Desc))
                        {
                            ok = false;
                            QMessageBox::warning(this,"ERROR", "Cannot insert Aggregation 2 VLAN "+QString::number(vlan)+".\n"+dbMan->lastError());
                            break;
                        }
                    }
                }
            }
            else
            {
                // no metro connection
                if(a1)
                { //with agg1
                    foreach (int vlan, totalVlanList)
                    {
                        if(vlan == 10)
                            Desc = "Global-Connection";
                        else if(vlan == 11)
                            Desc = "MPLS-VPN_nms";
                        else
                            Desc = desc;

                        if(!dbMan->insertVlan(agg1Id,vlan,Desc))
                        {
                            ok = false;
                            QMessageBox::warning(this,"ERROR", "Cannot insert Aggregation 1 VLAN "+QString::number(vlan)+".\n"+dbMan->lastError());
                            break;
                        }
                    }
                }

                if(a2)
                {//with agg2
                    foreach (int vlan, totalVlanList)
                    {
                        if(vlan == 10)
                            Desc = "Global-Connection";
                        else if(vlan == 11)
                            Desc = "MPLS-VPN_nms";
                        else
                            Desc = desc;

                        if(!dbMan->insertVlan(agg2ID,vlan,Desc))
                        {
                            ok = false;
                            QMessageBox::warning(this,"ERROR", "Cannot insert Aggregation 2 VLAN "+QString::number(vlan)+".\n"+dbMan->lastError());
                            break;
                        }
                    }
                }
            }

            if(!ok)
            {
                QMessageBox::warning(this,"Error", "Error in Agg VLAN insertion.");
            }

            if(ok)
            {
                //etherTrunk
                if(a1)
                {
                    if(!dbMan->insertEtherTrunk(agg1Id,agg1Eth,desc))
                        QMessageBox::warning(this,"Warning", "Cannot insert Aggregation 1 Ether-Trunk "+QString::number(agg1Eth)+".\n"+dbMan->lastError());
                    //update ports

                    Desc = agg1 +"- "+agg1Int1;
                    ok = dbMan->insertUpdateDslamInterface(dslamId, a1i1d, Desc);
                    if(agg1p2)
                    {
                        Desc = agg1 +"- "+agg1Int2;
                        ok = ok && dbMan->insertUpdateDslamInterface(dslamId, a1i2d, Desc);
                    }
                }

                if(a2)
                {
                    if(!dbMan->insertEtherTrunk(agg2ID,agg2Eth,desc))
                        QMessageBox::warning(this,"Warning", "Cannot insert Aggregation 2 Ether-Trunk "+QString::number(agg2Eth)+".\n"+dbMan->lastError());

                    //update ports

                    Desc = agg2 +"- "+agg2Int1;
                    ok = dbMan->insertUpdateDslamInterface(dslamId, a2i1d, Desc);
                    if(agg2p2)
                    {
                        Desc = agg2 +"- "+agg2Int2;
                        ok = ok && dbMan->insertUpdateDslamInterface(dslamId, a2i2d, Desc);
                    }

                }



                if(a2)
                {
                    Desc = agg2 +"- "+agg2Int1;
                    ok = ok && dbMan->insertUpdateDslamInterface(dslamId, a2i1d, Desc);
                    if(agg2p2)
                    {
                        Desc = agg2 +"- "+agg2Int2;
                        ok = ok && dbMan->insertUpdateDslamInterface(dslamId, a2i2d, Desc);
                    }
                }

                if(!ok)
                    QMessageBox::warning(this,"Warning", "Cannot update DSLAM port description.\n"+dbMan->lastError());

                QString ap;
                ok = true;
                if(c) ap = aggAllowPass; else ap = totalAllowPass;

                if(!a1)
                {
                    agg1Id = -1;
                    agg1port1Id = -1;
                    agg1port2Id = -1;
                    agg1 = "";
                    agg1Eth = -1;
                    agg1Int1 = "";
                    agg1Int2 ="";
                }
                if(!a2)
                {
                    agg2ID = -1;
                    agg2port1Id = -1;
                    agg2port2Id = -1;
                    agg2 = "";
                    agg2Eth = -1;
                    agg2Int1 = "";
                    agg2Int2 = "";
                }


                if(dbMan->insertAggDslam(dslamId, agg1Id,agg2ID,agg1port1Id, agg1port2Id,agg2port1Id,agg2port2Id,agg1 ,agg2 , agg1Eth,agg2Eth,agg1Int1,agg1Int2, agg2Int1, agg2Int2,ap,desc))
                {
                    // insert in port and ODF
                    //int portId, QString module, int peerExchId, QString peerExch, QString dev, QString peerInt, QString peerLabel, int eth

                    if(a1)
                    {
                        if(!dbMan->updateInterface(agg1port1Id,a1m,exch,"DSLAM MA5600",a1i1d,desc+"-"+a1i1d,agg1Eth,""))
                        {
                            ok = false;
                            QMessageBox::warning(this,"ERROR", "Cannot assign agregation 1 interface port 1.\n"+dbMan->lastError());
                        }

                        if(agg1p2)
                            if(!dbMan->updateInterface(agg1port2Id,a1m,exch,"DSLAM MA5600",a1i2d,desc+"-"+a1i2d,agg1Eth, ""))
                            {
                                ok = false;
                                QMessageBox::warning(this,"ERROR", "Cannot assign agregation 1 interface port 2.\n"+dbMan->lastError());
                            }

                    }

                    if(a2)
                    {
                        if(!dbMan->updateInterface(agg2port1Id,a2m,exch,"DSLAM MA5600",a2i1d,desc+"-"+a2i1d,agg2Eth, ""))
                        {
                            ok = false;
                            QMessageBox::warning(this,"ERROR", "Cannot assign agregation 2 port 1.\n"+dbMan->lastError());
                        }

                        if(agg2p2)
                            if(!dbMan->updateInterface(agg2port2Id,a2m,exch,"DSLAM MA5600",a2i2d,desc+"-"+a2i2d,agg2Eth, ""))
                            {
                                ok = false;
                                QMessageBox::warning(this,"ERROR", "Cannot assign agregation 2 port 2.\n"+dbMan->lastError());
                            }
                    }

                }
                else
                {
                    ok = false;
                    QMessageBox::warning(this,"ERROR", "Cannot plan agregation connection.\n"+dbMan->lastError());
                }
            }
        }
    }

    //Metro
    if(ok && cxChanged)
    {
        if(dbMan->cxDslamExistance(dslamId))
        {
            /*
            QSqlQuery *query = dbMan->selectCxDslam(dslamId);
            //0`dslam_id`, 1`cx_id`, 2`int1_id`, 3`int2_id`, 4`cx`, 5`ether_trunk`, 6`interface1`, 7`interface2`, 8`allow_pass`, 9`description`
            if(query->next())
            {
                int cxId = query->value(1).toInt();
                int p1Id = query->value(2).toInt();
                int p2Id;
                if(query->value(3).isNull())
                    p2Id = -1;
                else
                    p2Id = query->value(3).toInt();
                int e = query->value(5).toInt();
                QString v = query->value(8).toString();


                ok = ok && dbMan->deleteEtherTrunk(cxId, e);
                QList<int> VL = dbMan->rangeToList(v);
                foreach (int V, VL)
                    ok = ok && dbMan->deleteVlan(cxId, V);

                //empty port and ODF
                ok = ok && dbMan->depleteInterface(p1Id); //deplete both
                if(p2Id > -1)
                    ok = ok && dbMan->depleteInterface(p2Id);
            } else ok = false;

            ok = ok && dbMan->deleteCxDslamPlan(dslamId);
*/
            if(!dbMan->deleteCxDslamPlan(dslamId))
                QMessageBox::warning(this,"warning", "Problem removing old metro plan.");

        }

        if(ok && c && dbMan->isEtherTrunkUsed(cxId, cxEth))
        {
            QString msg = QString("CX600 ether-Trunk is used for %1. Change your ether-trunk.").arg(dbMan->getEtherTrunkDescription(cxId, cxEth));
            QMessageBox::warning(this,"ERROR", msg);
            return;
        }


        if(ok && c)
        {
            // VLAN
            //            foreach (int vlan , cxVlanList)
            //            {

            //                if(!dbMan->insertVlan(cxId,vlan,desc))
            //                {
            //                    ok = false;
            //                    QMessageBox::warning(this,"ERROR", "Cannot insert metro device VLAN "+QString::number(vlan)+".\n"+dbMan->lastError());
            //                    break;
            //                }
            //            }

            if(!ok)
                QMessageBox::warning(this,"Error", "CX VLAN insertion error.");
            else
            {
                //ether truenk
                if(!dbMan->insertEtherTrunk(cxId,cxEth,desc))
                    QMessageBox::warning(this,"Warning", "Cannot insert CX600 Ether-Trunk "+QString::number(agg2Eth)+".\n"+dbMan->lastError());


                //`dslam_id`, `cx_id`, `port1_id`, `port2_id`, `cx`, `ether_trunk`, `interface1`, `interface2`, `allow_pass`, `description`
                if(dbMan->insertCxDslam(cxp2, dslamId,cxId,cxp1Id, cxp2Id,cx,cxEth,cxInt1,cxInt2, "",desc)) //cxAllowPass
                {
                    //int portId, QString module, int peerExchId, QString peerExch, QString dev, QString peerInt, QString peerLabel, int eth

                    if(!dbMan->updateInterface(cxp1Id,cxm,exch,"DSLAM MA5600",cxi1d,desc+"-"+cxi1d,cxEth, ""))
                    {
                        ok = false;
                        QMessageBox::warning(this,"ERROR", "Cannot assign Metro device interface port 1.\n"+dbMan->lastError());
                    }

                    if(ok && cxp2)
                        if(!dbMan->updateInterface(cxp2Id,cxm,exch,"DSLAM MA5600",cxi2d,desc+"-"+cxi2d,cxEth, ""))
                        {
                            ok = false;
                            QMessageBox::warning(this,"ERROR", "Cannot assign Metro device interface port 2.\n"+dbMan->lastError());
                        }
                }
                else
                {
                    ok = false;
                    QMessageBox::warning(this,"ERROR", "Cannot plan DSLAM metro connection.\n"+dbMan->lastError());
                }

                QString Desc = cx +"- "+cxInt1;
                ok = ok && dbMan->insertUpdateDslamInterface(dslamId, cxi1d, Desc);
                if(ok && cxp2)
                {
                    Desc = cx +"- "+cxInt2;
                    ok = ok && dbMan->insertUpdateDslamInterface(dslamId, cxi2d, Desc);
                }
            }
        }
    }

    if(!ok)
    {// remove plan ---- error
        if(c)
        {
            foreach (int vlan, aggVlanList)
            {
                if(vlan < 4000)
                {
                    if(a1)
                        ok = dbMan->deleteVlan(agg1Id,vlan);
                    if(a2)
                        ok = ok && dbMan->deleteVlan(agg2ID,vlan);
                }
            }

            //            foreach (int vlan, cxVlanList)
            //            {
            //                ok = ok && dbMan->deleteVlan(cxId, vlan);
            //            }
        }
        else
        {
            foreach (int vlan, totalVlanList)
            {
                if(vlan < 4000)
                {
                    if(a1)
                        ok = ok && dbMan->deleteVlan(agg1Id,vlan);
                    if(a2)
                        ok = ok && dbMan->deleteVlan(agg2ID,vlan);
                }
            }
        }

        if(a1)
            ok = ok && dbMan->deleteEtherTrunk(agg1Id, agg1Eth);
        if(a2)
            ok = ok && dbMan->deleteEtherTrunk(agg2ID, agg2Eth);
        if(c)
            ok = ok && dbMan->deleteEtherTrunk(cxId, cxEth);

        if(a1)
        {
            ok = ok && dbMan->depleteInterface(agg1port1Id);
            if(agg1p2)
                ok = ok && dbMan->depleteInterface(agg1port2Id);
        }

        if(a2)
        {
            ok = ok && dbMan->depleteInterface(agg2port1Id);
            if(agg2p2)
                ok = ok && dbMan->depleteInterface(agg2port2Id);
        }

        ok = ok && dbMan->deleteAggDslam(dslamId);

        ok = ok && dbMan->insertUpdateDslamInterface(dslamId, a1i1d, "");
        ok = ok && dbMan->insertUpdateDslamInterface(dslamId, a1i2d, "");
        ok = ok && dbMan->insertUpdateDslamInterface(dslamId, a2i1d, "" );
        ok = ok && dbMan->insertUpdateDslamInterface(dslamId, a2i2d, "" );
        ok = ok && dbMan->insertUpdateDslamInterface(dslamId, cxi1d, "");
        ok = ok && dbMan->insertUpdateDslamInterface(dslamId, cxi2d, "");


        if(c)
        {
            ok = ok && dbMan->depleteInterface(cxp1Id);
            if(cxp2)
                ok = ok && dbMan->depleteInterface(cxp2Id);

            ok = ok && dbMan->deleteCxDslam(dslamId);
        }

        QMessageBox::warning(this,"ERROR", "Remove the existing Agg/Metro plan and try again. ");
    }


    this->close();
}

void MA5600AggMetroPlanDialog::on_agg1EthSB_valueChanged(int arg1)
{
    ui->agg2EthSB->setValue(arg1);
    AggChanged = true;
    checkEntry();
}

void MA5600AggMetroPlanDialog::on_agg2EthSB_valueChanged(int arg1)
{
    arg1++;
    AggChanged = true;
    checkEntry();
}

void MA5600AggMetroPlanDialog::on_agg1Int1CB_currentIndexChanged(int index)
{
    index++;
    AggChanged = true;
    QString temp = ui->agg1Int1CB->currentText();
    ui->agg2Int1CB->setCurrentText(temp);
    checkEntry();
}

void MA5600AggMetroPlanDialog::on_agg1Int2CB_currentIndexChanged(int index)
{
    index++;
    AggChanged = true;
    QString temp = ui->agg1Int2CB->currentText();
    ui->agg2Int2CB->setCurrentText(temp);
    checkEntry();
}

void MA5600AggMetroPlanDialog::on_agg2Int1CB_currentIndexChanged(int index)
{
    index++;
    AggChanged = true;
    checkEntry();
}

void MA5600AggMetroPlanDialog::on_agg2Int2CB_currentIndexChanged(int index)
{
    index++;
    AggChanged = true;
    checkEntry();
}

void MA5600AggMetroPlanDialog::on_agg1dIntCB_toggled(bool checked)
{
    AggChanged = true;
    if(checked)
    {
        ui->agg1Int2Frame->setEnabled(true);
        ui->agg1Int2Frame->setVisible(true);

        ui->agg2dIntCB->setChecked(true);
    }
    else
    {
        ui->agg1Int2Frame->setEnabled(false);
        ui->agg1Int2Frame->setVisible(false);

        ui->agg2dIntCB->setChecked(false);
    }

    checkEntry();
}

void MA5600AggMetroPlanDialog::on_agg1ConnCB_toggled(bool checked)
{
    AggChanged = true;
    if(checked)
    {
        ui->agg1GB->setEnabled(true);
        ui->agg1GB->setVisible(true);
        //ui->aggGB->setMinimumHeight(650);
    }
    else
    {
        ui->agg1GB->setEnabled(false);
        ui->agg1GB->setVisible(false);
        //ui->aggGB->setMinimumHeight(450);
        ui->agg1CB->setCurrentIndex(-1);
    }

    checkEntry();
}

void MA5600AggMetroPlanDialog::on_agg2ConnCB_toggled(bool checked)
{
    AggChanged = true;
    if(checked)
    {
        ui->agg2GB->setEnabled(true);
        ui->agg2GB->setVisible(true);
        //ui->aggGB->setMinimumHeight(650);
    }
    else
    {
        ui->agg2GB->setEnabled(false);
        ui->agg2GB->setVisible(false);
        //ui->aggGB->setMinimumHeight(450);
        ui->agg2CB->setCurrentIndex(-1);
    }

    checkEntry();
}

void MA5600AggMetroPlanDialog::on_agg2dIntCB_toggled(bool checked)
{
    AggChanged = true;
    if(checked)
    {
        ui->agg2Int2Frame->setEnabled(true);
        ui->agg2Int2Frame->setVisible(true);
    }
    else
    {
        ui->agg2Int2Frame->setEnabled(false);
        ui->agg2Int2Frame->setVisible(false);
    }

    checkEntry();
}

void MA5600AggMetroPlanDialog::on_metroConnCB_toggled(bool checked)
{
    cxChanged = true;
    AggChanged = true;

    if(checked)
    {
        ui->metroGB->setVisible(true);
        ui->metroGB->setEnabled(true);
        ui->aggAllowPassLE->setText(aggAllowPass);
    }
    else
    {
        ui->metroGB->setVisible(false);
        ui->metroGB->setEnabled(false);
        ui->aggAllowPassLE->setText(totalAllowPass);
        ui->cxCB->setCurrentIndex(-1);
    }

    checkEntry();
}

void MA5600AggMetroPlanDialog::on_cxdIntCB_toggled(bool checked)
{
    cxChanged = true;
    if(checked)
    {
        ui->cxInt2Frame->setEnabled(true);
        ui->cxInt2Frame->setVisible(true);
    }
    else
    {
        ui->cxInt2Frame->setEnabled(false);
        ui->cxInt2Frame->setVisible(false);
    }

    checkEntry();
}

void MA5600AggMetroPlanDialog::on_cxCB_currentIndexChanged(int index)
{
    index++;
    cxChanged = true;
    ui->cxInt1CB->clear();
    ui->cxInt2CB->clear();

    int cxId = ui->cxCB->currentData().toInt();

    if(dbMan->cxDslamExistance(dslamId))
    {
        QSqlQuery *query = dbMan->selectCxDslam(dslamId);

        //`dslam_id`, `cx_id`, `port1_id`, `port2_id`, `cx`, `ether_trunk`, `interface1`, `interface2`, `allow_pass`, `description`
        if(query->next())
        {
            int cxID = query->value(1).toInt();
            int p1 = query->value(2).toInt();
            int p2 = query->value(3).toInt();
            QString int1 = query->value(6).toString();
            QString int2 = query->value(7).toString();

            if(cxId == cxID)
            {
                ui->cxInt1CB->addItem(int1,p1);
                ui->cxInt2CB->addItem(int2,p2);
            }

        }

    }


    //eth
    int eth = dbMan->getNewEtherTrunk(cxId, 3); // 3 eth empty consequently
    ui->cxEthSB->setValue(eth);
    //int
    QSqlQuery *query = dbMan->selectDeviveEmptyPorts(cxId);
    //`id`, `interface_type`, `interface`
    int id;
    QString type,port;

    while(query->next())
    {
        id = query->value(0).toInt();
        type = query->value(1).toString();
        port = query->value(2).toString();
        type = dbMan->getInterfaceTypeSymble(type);
        port = type + "-"+port;
        ui->cxInt1CB->addItem(port, id);
        ui->cxInt2CB->addItem(port,id);
    }

    ui->cxInt1CB->setCurrentIndex(0);
    ui->cxInt2CB->setCurrentIndex(1);
    checkEntry();
}

void MA5600AggMetroPlanDialog::on_a1i1diCB_currentIndexChanged(int index)
{
    AggChanged = true;
    ui->a2i1diCB->setCurrentIndex(index);
    checkEntry();
}

void MA5600AggMetroPlanDialog::on_a1i2diCB_currentIndexChanged(int index)
{
    AggChanged = true;
    ui->a2i2diCB->setCurrentIndex(index);
    checkEntry();
}

void MA5600AggMetroPlanDialog::on_a2i1diCB_currentIndexChanged(int index)
{
    index++;
    AggChanged = true;
    checkEntry();
}

void MA5600AggMetroPlanDialog::on_a2i2diCB_currentIndexChanged(int index)
{
    index++;
    AggChanged = true;
    checkEntry();
}

void MA5600AggMetroPlanDialog::on_cxi1diCB_currentIndexChanged(int index)
{
    index++;
    cxChanged = true;
    checkEntry();
}

void MA5600AggMetroPlanDialog::on_cxi2diCB_currentIndexChanged(int index)
{
    index++;
    cxChanged = true;
    checkEntry();
}

void MA5600AggMetroPlanDialog::on_cxInt1CB_currentIndexChanged(int index)
{
    index++;
    cxChanged = true;
    checkEntry();
}

void MA5600AggMetroPlanDialog::on_cxInt2CB_currentIndexChanged(int index)
{
    index++;
    cxChanged = true;
    checkEntry();
}

void MA5600AggMetroPlanDialog::on_a1mCB_currentTextChanged(const QString &arg1)
{
    ui->a2mCB->setCurrentText(arg1);
    AggChanged = true;
    checkEntry();
}

void MA5600AggMetroPlanDialog::on_a2mCB_currentIndexChanged(int index)
{
    index++;
    AggChanged = true;
    checkEntry();
}

void MA5600AggMetroPlanDialog::on_cxmCB_currentIndexChanged(int index)
{
    index++;
    cxChanged = true;
    checkEntry();
}

void MA5600AggMetroPlanDialog::on_cxEthSB_valueChanged(int arg1)
{
    arg1++;
    cxChanged = true;
    checkEntry();
}

void MA5600AggMetroPlanDialog::on_nextBtn_clicked()
{
    ui->tabWidget->setCurrentIndex(1);
}

void MA5600AggMetroPlanDialog::on_metroBackBtn_clicked()
{
    ui->tabWidget->setCurrentIndex(1);
}

void MA5600AggMetroPlanDialog::on_agg2NextBtn_clicked()
{
    ui->tabWidget->setCurrentIndex(2);
}

void MA5600AggMetroPlanDialog::on_agg2BackBtn_clicked()
{
    ui->tabWidget->setCurrentIndex(0);
}


