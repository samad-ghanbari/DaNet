#include "mxk819singleaggmetroplandialog.h"
#include "ui_mxk819singleaggmetroplandialog.h"
#include <QSqlQuery>
#include "lib/database/danetdbman.h"
#include <QMessageBox>

Mxk819SingleAggMetroPlanDialog::Mxk819SingleAggMetroPlanDialog(QWidget *parent, DanetDbMan *Db, const int ExchId, const int DslamId) :
    QDialog(parent),
    ui(new Ui::Mxk819SingleAggMetroPlanDialog),
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

    //Modules
    query = dbMan->selectModules();
    int i;
    QString m;
    while(query->next())
    {
        i = query->value(0).toInt();
        m = query->value(1).toString();

        ui->a1mCB->addItem(m, i);
        ui->cxmCB->addItem(m, i);
    }

    // DSLAM PORTS
    ui->cxi1diCB->blockSignals(true);
    ui->cxi1diCB->addItem("1G 1-a-4-0/eth", 4);
    ui->cxi1diCB->addItem("1G 1-a-5-0/eth", 5);
    ui->cxi1diCB->addItem("1G 1-a-6-0/eth", 6);
    ui->cxi1diCB->addItem("1G 1-a-7-0/eth", 7);
    ui->cxi1diCB->addItem("1G 1-a-8-0/eth", 8);
    ui->cxi1diCB->addItem("1G 1-a-9-0/eth", 9);
    ui->cxi1diCB->addItem("1G 1-a-10-0/eth", 10);
    ui->cxi1diCB->addItem("1G 1-a-11-0/eth", 11);
    ui->cxi1diCB->blockSignals(false);

    ui->cxi2diCB->blockSignals(true);
    ui->cxi2diCB->addItem("1G 1-b-4-0/eth", 4);
    ui->cxi2diCB->addItem("1G 1-b-5-0/eth", 5);
    ui->cxi2diCB->addItem("1G 1-b-6-0/eth", 6);
    ui->cxi2diCB->addItem("1G 1-b-7-0/eth", 7);
    ui->cxi2diCB->addItem("1G 1-b-8-0/eth", 8);
    ui->cxi2diCB->addItem("1G 1-b-9-0/eth", 9);
    ui->cxi2diCB->addItem("1G 1-b-10-0/eth", 10);
    ui->cxi2diCB->addItem("1G 1-b-11-0/eth", 11);
    ui->cxi2diCB->blockSignals(false);

    ui->a1i1diCB->blockSignals(true);
    ui->a1i1diCB->addItem("1G 1-a-4-0/eth", 4);
    ui->a1i1diCB->addItem("1G 1-a-5-0/eth", 5);
    ui->a1i1diCB->addItem("1G 1-a-6-0/eth", 6);
    ui->a1i1diCB->addItem("1G 1-a-7-0/eth", 7);
    ui->a1i1diCB->addItem("1G 1-a-8-0/eth", 8);
    ui->a1i1diCB->addItem("1G 1-a-9-0/eth", 9);
    ui->a1i1diCB->addItem("1G 1-a-10-0/eth", 10);
    ui->a1i1diCB->addItem("1G 1-a-11-0/eth", 11);
    ui->a1i1diCB->blockSignals(false);

    ui->a1i2diCB->blockSignals(true);
    ui->a1i2diCB->addItem("1G 1-b-4-0/eth", 4);
    ui->a1i2diCB->addItem("1G 1-b-5-0/eth", 5);
    ui->a1i2diCB->addItem("1G 1-b-6-0/eth", 6);
    ui->a1i2diCB->addItem("1G 1-b-7-0/eth", 7);
    ui->a1i2diCB->addItem("1G 1-b-8-0/eth", 8);
    ui->a1i2diCB->addItem("1G 1-b-9-0/eth", 9);
    ui->a1i2diCB->addItem("1G 1-b-10-0/eth", 10);
    ui->a1i2diCB->addItem("1G 1-b-11-0/eth", 11);
    ui->a1i2diCB->blockSignals(false);

    ui->a1i3diCB->blockSignals(true);
    ui->a1i3diCB->addItem("1G 1-a-4-0/eth", 4);
    ui->a1i3diCB->addItem("1G 1-a-5-0/eth", 5);
    ui->a1i3diCB->addItem("1G 1-a-6-0/eth", 6);
    ui->a1i3diCB->addItem("1G 1-a-7-0/eth", 7);
    ui->a1i3diCB->addItem("1G 1-a-8-0/eth", 8);
    ui->a1i3diCB->addItem("1G 1-a-9-0/eth", 9);
    ui->a1i3diCB->addItem("1G 1-a-10-0/eth", 10);
    ui->a1i3diCB->addItem("1G 1-a-11-0/eth", 11);
    ui->a1i3diCB->blockSignals(false);

    ui->a1i4diCB->blockSignals(true);
    ui->a1i4diCB->addItem("1G 1-b-4-0/eth", 4);
    ui->a1i4diCB->addItem("1G 1-b-5-0/eth", 5);
    ui->a1i4diCB->addItem("1G 1-b-6-0/eth", 6);
    ui->a1i4diCB->addItem("1G 1-b-7-0/eth", 7);
    ui->a1i4diCB->addItem("1G 1-b-8-0/eth", 8);
    ui->a1i4diCB->addItem("1G 1-b-9-0/eth", 9);
    ui->a1i4diCB->addItem("1G 1-b-10-0/eth", 10);
    ui->a1i4diCB->addItem("1G 1-b-11-0/eth", 11);
    ui->a1i4diCB->blockSignals(false);

    ui->a1mCB->setCurrentText("SX-500m");
    ui->cxmCB->setCurrentText("SX-500m");

    //desc
    QString dslamName = dbMan->getDslamName(dslamId);
    ui->aggDescLE->setText(dslamName.trimmed());
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

    totalAllowPass = totalAllowPassVlans.join(",");
    totalAllowPass = totalAllowPass.replace(",",", ");

    // dslam interfaces
    // need to be checked with interfaces
    ui->cxi1diCB->setCurrentIndex(2);
    ui->cxi2diCB->setCurrentIndex(2);

    ui->a1i1diCB->setCurrentIndex(0);
    ui->a1i2diCB->setCurrentIndex(0);
    ui->a1i3diCB->setCurrentIndex(1);
    ui->a1i4diCB->setCurrentIndex(1);



    bool a = dbMan->dslamAggExistance(dslamId);

    ui->agg1CB->setCurrentIndex(-1);
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

        query = dbMan->selectAggDslam(dslamId);

        // 0`agg1_id`, 1`agg2_id`, 2`agg1_int1_id`, 3`agg1_int2_id`, 4`agg2_int1_id`, 5`agg2_int2_id`, 6`agg1`,
        //7`agg2`, 8`agg1_eth`, 9`agg2_eth`, 10`agg1_interface1`, 11`agg1_interface2`, 12`agg2_interface1`,
        // 13`agg2_interface2`, 14`allow_pass`, 15`description`, 16`agg1_int3_id`, 17`agg1_int4_id`, 18`agg1_interface3`, 19`agg1_interface4`

        if(query->next())
        {
            int ag1Id;
            if(query->value(0).isNull())
                ag1Id = -1;
            else
                ag1Id = query->value(0).toInt();

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

            int a1p3Id;
            if(query->value(16).isNull())
                a1p3Id = -1;
            else
                a1p3Id = query->value(16).toInt();

            int a1p4Id;
            if(query->value(17).isNull())
                a1p4Id = -1;
            else
                a1p4Id = query->value(17).toInt();

            int a1e = query->value(8).toInt();

            if(ag1Id > -1)
            {
                ui->agg1CB->setCurrentIndex(ui->agg1CB->findData(ag1Id));
                ui->agg1Int1CB->setCurrentIndex(ui->agg1Int1CB->findData(a1p1Id));
                QStringList mi = dbMan->getModulePeerInterface(a1p1Id);
                ui->a1mCB->setCurrentText(mi.at(0));
                ui->a1i1diCB->setCurrentText(mi.at(1));

                if(a1p2Id > -1)
                {
                    ui->agg1Int2CB->setCurrentIndex(ui->agg1Int2CB->findData(a1p2Id));
                    mi = dbMan->getModulePeerInterface(a1p2Id);
                    ui->a1i2diCB->setCurrentText(mi.at(1));
                }

                if(a1p3Id > -1)
                {
                    ui->agg1Int3CB->setCurrentIndex(ui->agg1Int3CB->findData(a1p3Id));
                    mi = dbMan->getModulePeerInterface(a1p3Id);
                    ui->a1i3diCB->setCurrentText(mi.at(1));
                }
                else
                    ui->agg14IntCB->setChecked(false);


                if(a1p4Id > -1)
                {
                    ui->agg1Int4CB->setCurrentIndex(ui->agg1Int4CB->findData(a1p4Id));
                    mi = dbMan->getModulePeerInterface(a1p4Id);
                    ui->a1i4diCB->setCurrentText(mi.at(1));

                }


                ui->agg1EthSB->setValue(a1e);
            }
            else
                ui->agg1ConnCB->setChecked(false);

        }
        ui->okBtn->setEnabled(false);
    }


    AggChanged = false;
    cxChanged = false;
    ui->okBtn->setEnabled(false);

}

Mxk819SingleAggMetroPlanDialog::~Mxk819SingleAggMetroPlanDialog()
{
    delete ui;
}

void Mxk819SingleAggMetroPlanDialog::checkEntry()
{
    bool a1i1  = !ui->agg1Int1CB->currentText().isEmpty() || !ui->agg1ConnCB->isChecked();
    bool a1i2  = !ui->agg1Int2CB->currentText().isEmpty() || !ui->agg1ConnCB->isChecked();

    bool a1i3  = !ui->agg1Int3CB->currentText().isEmpty() || !ui->agg14IntCB->isChecked() || !ui->agg1ConnCB->isChecked();
    bool a1i4  = !ui->agg1Int4CB->currentText().isEmpty() || !ui->agg14IntCB->isChecked() || !ui->agg1ConnCB->isChecked();

    bool a1i1d = !ui->a1i1diCB->currentText().isEmpty()|| !ui->agg1ConnCB->isChecked();
    bool a1i2d = !ui->a1i2diCB->currentText().isEmpty()|| !ui->agg1ConnCB->isChecked();

    bool a1i3d = !ui->a1i3diCB->currentText().isEmpty() || !ui->agg14IntCB->isChecked() || !ui->agg1ConnCB->isChecked();
    bool a1i4d = !ui->a1i4diCB->currentText().isEmpty() || !ui->agg14IntCB->isChecked() || !ui->agg1ConnCB->isChecked();

    if(a1i1 && a1i2 && a1i3 && a1i4 && a1i1d && a1i2d && a1i3d && a1i4d)
        ui->nextBtn->setEnabled(true);
    else
        ui->nextBtn->setEnabled(false);

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
    int a13 = ui->agg1Int3CB->currentData().toInt();
    int a14 = ui->agg1Int4CB->currentData().toInt();

    int ad11 = ui->a1i1diCB->currentData().toInt();
    int ad12 = ui->a1i2diCB->currentData().toInt();
    int ad13 = ui->a1i3diCB->currentData().toInt();
    int ad14 = ui->a1i4diCB->currentData().toInt();

    if(!ui->agg14IntCB->isChecked()){ a13 += 50; ad13 += 100;  a14 += 150; ad14 += 200;} // to make different
    if(!ui->agg1ConnCB->isChecked()){a11 += 50; ad12 += 100;  a12 += 150; ad12 += 200;  a13 += 250; ad13 += 300;  a14 += 350; ad14 += 400;}

    int c1 = ui->cxInt1CB->currentData().toInt();
    int c2 = ui->cxInt2CB->currentData().toInt();
    int cd1 = ui->cxi1diCB->currentData().toInt();
    int cd2 = ui->cxi2diCB->currentData().toInt();
    if(!ui->metroConnCB->isChecked()){c1 += 450; c2 += 500; cd1 += 550; cd2 += 600;}
    if(!ui->cxdIntCB->isChecked()){c2 += 650; cd2 += 700;}


    bool OK = false;
    if(a1i1 && a1i2 && a1i2 && a1i4 && a1i1d && a1i2d && a1i3d && a1i4d && cxi1 && cxi2 && cxi1d && cxi2d && ad && aa && cd  )//&& ca
    {
        if( (a11 == a12) || (a11 == a13) || (a11 == a14) || (a12 == a13) || (a12 == a14) || (a13 == a14) || (c1 == c2) ||
            (ad11 == ad13) || (ad12 == ad14) ||
            (cd1 == ad11) || (cd1 == ad13) ||
            (cd2 == ad12) || (cd2 == ad14) )
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

void Mxk819SingleAggMetroPlanDialog::on_agg1CB_currentIndexChanged(int index)
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

        //0`agg1_id`, `agg2_id`, `agg1_int1_id`, `agg1_int2_id`, `agg2_int1_id`, `agg2_int2_id`,
        //6`agg1`, `agg2`, `agg1_eth`, `agg2_eth`, `agg1_interface1`, `agg1_interface2`,
        //12`agg2_interface1`, `agg2_interface2`, `allow_pass`, `description`, `agg1_int3_id`, `agg1_int4_id`,
        //18`agg1_interface3`, `agg1_interface4`
        if(query->next())
        {
            int agg1Id = query->value(0).toInt();
            int p1 = query->value(2).toInt();
            int p2 = query->value(3).toInt();
            QString int1 = query->value(10).toString();
            QString int2 = query->value(11).toString();

            int p3 = query->value(16).toInt();
            int p4 = query->value(17).toInt();
            QString int3 = query->value(18).toString();
            QString int4 = query->value(19).toString();


            if(aggId == agg1Id)
            {
                ui->agg1Int1CB->addItem(int1,p1);
                ui->agg1Int2CB->addItem(int2,p2);

                ui->agg1Int3CB->addItem(int3,p3);
                ui->agg1Int4CB->addItem(int4,p4);
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

        ui->agg1Int3CB->addItem(port,id);
        ui->agg1Int4CB->addItem(port,id);
    }

    ui->agg1Int1CB->setCurrentIndex(-1);
    ui->agg1Int2CB->setCurrentIndex(-1);
    ui->agg1Int3CB->setCurrentIndex(-1);
    ui->agg1Int4CB->setCurrentIndex(-1);
    checkEntry();
}

void Mxk819SingleAggMetroPlanDialog::on_okBtn_clicked()
{
    int agg1Id = ui->agg1CB->currentData().toInt();
    int agg1port1Id = ui->agg1Int1CB->currentData().toInt();
    QString agg1Int1 = ui->agg1Int1CB->currentText();
    int agg1port2Id = ui->agg1Int2CB->currentData().toInt();
    QString agg1Int2 = ui->agg1Int2CB->currentText();
    int agg1port3Id = ui->agg1Int3CB->currentData().toInt();
    QString agg1Int3 = ui->agg1Int3CB->currentText();
    int agg1port4Id = ui->agg1Int4CB->currentData().toInt();
    QString agg1Int4 = ui->agg1Int4CB->currentText();

    int agg1Eth = ui->agg1EthSB->value();

    QString desc = ui->aggDescLE->text(); //dslamName
    QString agg1 = ui->agg1CB->currentText();
    agg1 = agg1.replace(" ","");

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
    QString a1i3d = ui->a1i3diCB->currentText();
    QString a1i4d = ui->a1i4diCB->currentText();
    QString cxi1d = ui->cxi1diCB->currentText();
    QString cxi2d = ui->cxi2diCB->currentText();

    bool agg1p4 = ui->agg14IntCB->isChecked();
    bool a1 = ui->agg1ConnCB->isChecked();
    bool c = ui->metroConnCB->isChecked();
    bool cxp2 = ui->cxdIntCB->isChecked();

    if(!cxp2)
    {
        cxp2Id = -1;
        cxInt2 = "";
    }

    if(!agg1p4)
    {
        agg1port3Id = -1;
        agg1Int3 = "";
        agg1port4Id = -1;
        agg1Int4 = "";
    }


    QString a1m = ui->a1mCB->currentText();
    QString cxm = ui->cxmCB->currentText();

    QString exch = dbMan->getExchangeAreaName(exchId);

    if(!a1) agg1Id = -1;
    if(!c) cxId = -1;

    bool a1Cond;
    if(a1 && agg1p4)
        a1Cond = (agg1port1Id == agg1port2Id) || (agg1port1Id == agg1port3Id) || (agg1port1Id == agg1port4Id) || (agg1port2Id == agg1port3Id) || (agg1port3Id == agg1port4Id) ;
    else if(a1)
        a1Cond = (agg1port1Id == agg1port2Id);
    else
        a1Cond = false;

    bool cxCond = (c && cxp2) && (cxp1Id == cxp2Id);

    if( a1Cond || cxCond )
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
            //10`agg1_interface1`, 11`agg1_interface2`, 12`agg2_interface1`, 13`agg2_interface2`, 14`allow_pass`, 15`description`, 16`agg1_int3_id`, 17`agg1_int4_id`, 18`agg1_interface3`, 19`agg1_interface4`
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

                int a1p3;
                if(query->value(16).isNull())
                    a1p3 = -1;
                else
                    a1p3 = query->value(16).toInt();

                int a1p4;
                if(query->value(17).isNull())
                    a1p4 = -1;
                else
                    a1p4 = query->value(17).toInt();


                int a2p1;
                if(query->value(4).isNull())
                    a2p1 =  -1;
                else
                    a2p1 = query->value(4).toInt();

                int a2p2;
                if(query->value(5).isNull())
                    a2p2 =  -1;
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
                    if(a1p1 > -1)
                        ok = ok && dbMan->depleteInterface(a1p1);
                    if(a1p2 > -1)
                        ok = ok && dbMan->depleteInterface(a1p2);
                    if(a1p3 > -1)
                        ok = ok && dbMan->depleteInterface(a1p3);
                    if(a1p4 > -1)
                        ok = ok && dbMan->depleteInterface(a1p4);
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
                    Desc = agg1 +"- "+agg1Int2;
                    ok = dbMan->insertUpdateDslamInterface(dslamId, a1i2d, Desc);

                    if(agg1p4)
                    {
                        Desc = agg1 +"- "+agg1Int3;
                        ok = dbMan->insertUpdateDslamInterface(dslamId, a1i3d, Desc);

                        Desc = agg1 +"- "+agg1Int4;
                        ok = dbMan->insertUpdateDslamInterface(dslamId, a1i4d, Desc);
                    }

                }


//                if(a2)
//                {
//                    Desc = agg2 +"- "+agg2Int1;
//                    ok = ok && dbMan->insertUpdateDslamInterface(dslamId, a2i1d, Desc);
//                    if(agg2p2)
//                    {
//                        Desc = agg2 +"- "+agg2Int2;
//                        ok = ok && dbMan->insertUpdateDslamInterface(dslamId, a2i2d, Desc);
//                    }
//                }

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
                    agg1port3Id = -1;
                    agg1port4Id = -1;
                    agg1 = "";
                    agg1Eth = -1;
                    agg1Int1 = "";
                    agg1Int2 ="";
                    agg1Int3 ="";
                    agg1Int4 ="";

                }



                if(dbMan->insertAggDslam(dslamId, agg1Id,-1,agg1port1Id, agg1port2Id,-1,-1,agg1 ,"" , agg1Eth,-1,agg1Int1,agg1Int2,"","",ap,desc, agg1port3Id, agg1port4Id, agg1Int3, agg1Int4  ))
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

                        if(!dbMan->updateInterface(agg1port2Id,a1m,exch,"DSLAM MA5600",a1i2d,desc+"-"+a1i1d,agg1Eth,""))
                        {
                            ok = false;
                            QMessageBox::warning(this,"ERROR", "Cannot assign agregation 1 interface port 2.\n"+dbMan->lastError());
                        }

                        if(agg1p4)
                        {
                            if(!dbMan->updateInterface(agg1port3Id,a1m,exch,"DSLAM MA5600",a1i3d,desc+"-"+a1i3d,agg1Eth, ""))
                            {
                                ok = false;
                                QMessageBox::warning(this,"ERROR", "Cannot assign agregation 1 interface port 3.\n"+dbMan->lastError());
                            }


                            if(!dbMan->updateInterface(agg1port4Id,a1m,exch,"DSLAM MA5600",a1i4d,desc+"-"+a1i4d,agg1Eth, ""))
                            {
                                ok = false;
                                QMessageBox::warning(this,"ERROR", "Cannot assign agregation 1 interface port 4.\n"+dbMan->lastError());
                            }
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
                    QMessageBox::warning(this,"Warning", "Cannot insert CX600 Ether-Trunk "+QString::number(cxEth)+".\n"+dbMan->lastError());


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

                }
            }
        }

        if(a1)
            ok = ok && dbMan->deleteEtherTrunk(agg1Id, agg1Eth);
        if(c)
            ok = ok && dbMan->deleteEtherTrunk(cxId, cxEth);

        if(a1)
        {
            ok = ok && dbMan->depleteInterface(agg1port1Id);
            ok = ok && dbMan->depleteInterface(agg1port2Id);
            ok = ok && dbMan->depleteInterface(agg1port3Id);
            ok = ok && dbMan->depleteInterface(agg1port4Id);

        }

        ok = ok && dbMan->deleteAggDslam(dslamId);

        ok = ok && dbMan->insertUpdateDslamInterface(dslamId, a1i1d, "");
        ok = ok && dbMan->insertUpdateDslamInterface(dslamId, a1i2d, "");
        ok = ok && dbMan->insertUpdateDslamInterface(dslamId, a1i3d, "");
        ok = ok && dbMan->insertUpdateDslamInterface(dslamId, a1i4d, "");
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

void Mxk819SingleAggMetroPlanDialog::on_agg1EthSB_valueChanged(int arg1)
{
    arg1++;
    AggChanged = true;
    checkEntry();
}

void Mxk819SingleAggMetroPlanDialog::on_agg1Int1CB_currentIndexChanged(int index)
{
    index++;
    AggChanged = true;
    QString temp = ui->agg1Int1CB->currentText();
    checkEntry();
}

void Mxk819SingleAggMetroPlanDialog::on_agg1Int2CB_currentIndexChanged(int index)
{
    index++;
    AggChanged = true;
    QString temp = ui->agg1Int2CB->currentText();
    checkEntry();
}

void Mxk819SingleAggMetroPlanDialog::on_agg1Int3CB_currentIndexChanged(int index)
{
    index++;
    AggChanged = true;
    checkEntry();
}

void Mxk819SingleAggMetroPlanDialog::on_agg1Int4CB_currentIndexChanged(int index)
{
    index++;
    AggChanged = true;
    checkEntry();
}

void Mxk819SingleAggMetroPlanDialog::on_agg14IntCB_toggled(bool checked)
{
    AggChanged = true;
    if(checked)
    {
        ui->agg1Int2Frame->setEnabled(true);
        ui->agg1Int2Frame->setVisible(true);
        ui->agg1Int3Frame->setVisible(true);
        ui->agg1Int4Frame->setVisible(true);
    }
    else
    {
        ui->agg1Int2Frame->setEnabled(true);
        ui->agg1Int2Frame->setVisible(true);
        ui->agg1Int3Frame->setVisible(false);
        ui->agg1Int4Frame->setVisible(false);
    }

    checkEntry();
}

void Mxk819SingleAggMetroPlanDialog::on_agg1ConnCB_toggled(bool checked)
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

void Mxk819SingleAggMetroPlanDialog::on_metroConnCB_toggled(bool checked)
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

void Mxk819SingleAggMetroPlanDialog::on_cxdIntCB_toggled(bool checked)
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

void Mxk819SingleAggMetroPlanDialog::on_cxCB_currentIndexChanged(int index)
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

void Mxk819SingleAggMetroPlanDialog::on_a1i1diCB_currentIndexChanged(int index)
{
    index++;
    AggChanged = true;
    checkEntry();
}

void Mxk819SingleAggMetroPlanDialog::on_a1i2diCB_currentIndexChanged(int index)
{
    index++;
    AggChanged = true;
    checkEntry();
}

void Mxk819SingleAggMetroPlanDialog::on_a1i3diCB_currentIndexChanged(int index)
{
    index++;
    AggChanged = true;
    checkEntry();
}

void Mxk819SingleAggMetroPlanDialog::on_a1i4diCB_currentIndexChanged(int index)
{
    index++;
    AggChanged = true;
    checkEntry();
}

void Mxk819SingleAggMetroPlanDialog::on_cxi1diCB_currentIndexChanged(int index)
{
    index++;
    cxChanged = true;
    checkEntry();
}

void Mxk819SingleAggMetroPlanDialog::on_cxi2diCB_currentIndexChanged(int index)
{
    index++;
    cxChanged = true;
    checkEntry();
}

void Mxk819SingleAggMetroPlanDialog::on_cxInt1CB_currentIndexChanged(int index)
{
    index++;
    cxChanged = true;
    checkEntry();
}

void Mxk819SingleAggMetroPlanDialog::on_cxInt2CB_currentIndexChanged(int index)
{
    index++;
    cxChanged = true;
    checkEntry();
}

void Mxk819SingleAggMetroPlanDialog::on_a1mCB_currentTextChanged(const QString &arg1)
{
    if(arg1.isEmpty())
        AggChanged = true;
    else
        AggChanged = true;
    checkEntry();
}

void Mxk819SingleAggMetroPlanDialog::on_cxmCB_currentIndexChanged(int index)
{
    index++;
    cxChanged = true;
    checkEntry();
}

void Mxk819SingleAggMetroPlanDialog::on_cxEthSB_valueChanged(int arg1)
{
    arg1++;
    cxChanged = true;
    checkEntry();
}

void Mxk819SingleAggMetroPlanDialog::on_nextBtn_clicked()
{
    ui->tabWidget->setCurrentIndex(1);
}

void Mxk819SingleAggMetroPlanDialog::on_metroBackBtn_clicked()
{
    ui->tabWidget->setCurrentIndex(0);
}


