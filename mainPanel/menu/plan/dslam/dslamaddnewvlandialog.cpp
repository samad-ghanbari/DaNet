#include "dslamaddnewvlandialog.h"
#include "ui_dslamaddnewvlandialog.h"
#include "lib/database/danetdbman.h"
#include <QMessageBox>


DslamAddNewVlanDialog::DslamAddNewVlanDialog(QWidget *parent, DanetDbMan *db, const int DslamId, const QString DslamName) :
    QDialog(parent),
    ui(new Ui::DslamAddNewVlanDialog),
    dbMan(db),
    dslamId(DslamId),
    dslamName(DslamName)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowMinMaxButtonsHint);

    ui->dslamLbl->setText(DslamName);

    exchId = dbMan->getDevexExchangeId(dslamId);
    int motherId = dbMan->getSiteMotherId(exchId);

    QList<int> devIds = dbMan->getExchangeSiteAllDslamIds(motherId);

    motherAgg1Id = dbMan->getExchangeAgg1Id(motherId);
    motherCxId = dbMan->getExchangeCxId(motherId);

    int vlan = dbMan->getNewPppoeVlan(devIds);

    if(vlan > 499)
        ui->hsiSB->setValue(vlan);

    ui->confirmChB->setChecked(false);
    ui->okBtn->setEnabled(false);
}

DslamAddNewVlanDialog::~DslamAddNewVlanDialog()
{
    delete ui;
}

void DslamAddNewVlanDialog::on_confirmChB_toggled(bool checked)
{
    if(checked)
        ui->okBtn->setEnabled(true);
    else
        ui->okBtn->setEnabled(false);
}

void DslamAddNewVlanDialog::on_cancelBtn_clicked()
{
    this->close();
}

void DslamAddNewVlanDialog::on_okBtn_clicked()
{
    QString currentCxAPV, currentAggAPV, temp;
    QList<int> list;
    bool okDone = true;

    //int vas = ui->vasSB->value();
    //int ipmedia = ui->ipMediaSB->value();
    int hsi = ui->hsiSB->value();
    int inet = ui->inetSB->value();
    int sinet = ui->sinetSB->value();
    int voip = ui->voipSB->value();

    QList<int> casNode = dbMan->getSiteCascadeNode(exchId);
    int node = casNode.at(1);
    bool uplinkShelf = dbMan->isDslamUplinkShelf(dslamId);

    //insert new VLANs
    //DSLAM
    //    if(!dbMan->insertVlan(dslamId,vas,dslamName+"TCT-VAS"))
    //        okDone = false;
//    if(!dbMan->insertVlan(dslamId,ipmedia,dslamName+"TCT-IP-Media") && okDone)
//        okDone = false;
    if(!dbMan->insertVlan(dslamId,hsi,dslamName+"TCT-HSI") && okDone)
        okDone = false;
    if(!dbMan->insertVlan(dslamId,inet,dslamName+"TCT-Inet") && okDone)
        okDone = false;
    if(!dbMan->insertVlan(dslamId,sinet,dslamName+"TCT-Sinet") && okDone)
        okDone = false;
    if(!dbMan->insertVlan(dslamId,voip,dslamName+"TCT-Voip") && okDone)
        okDone = false;

    if(!okDone)
        QMessageBox::warning(this, "ERROR", QString("Cannot insert new vlan into DSLAM device."));


    if(okDone && (((node == 0) && uplinkShelf) || (node == 1)) )
    {//local - first node

        //check vlans
        //        if(dbMan->vlanAvailability(motherAgg1Id, vas))
        //        {
        //            QMessageBox::warning(this, "ERROR", QString("Vlan %1 currently exists in aggregation switch.").arg(vas));
        //            okDone = false;
        //        }

//        if(okDone && (dbMan->vlanAvailability(motherAgg1Id, ipmedia)))
//        {
//            QMessageBox::warning(this, "ERROR", QString("Vlan %1 currently exists in aggregation switch.").arg(ipmedia));
//            okDone = false;
//        }

        //        if(okDone && (dbMan->vlanAvailability(motherCxId, vas)))
        //        {
        //            QMessageBox::warning(this, "ERROR", QString("Vlan %1 currently exists in metro switch.").arg(vas));
        //            okDone = false;
        //        }

//        if(okDone && (dbMan->vlanAvailability(motherCxId, ipmedia)))
//        {
//            QMessageBox::warning(this, "ERROR", QString("Vlan %1 currently exists in metro switch.").arg(ipmedia));
//            okDone = false;
//        }

        if(okDone && (dbMan->vlanAvailability(motherAgg1Id, hsi)))
        {
            QMessageBox::warning(this, "ERROR", QString("Vlan %1 currently exists in aggregation switch.").arg(hsi));
            okDone = false;
        }

        if(okDone && (dbMan->vlanAvailability(motherAgg1Id, inet)))
        {
            QMessageBox::warning(this, "ERROR", QString("Vlan %1 currently exists in aggregation switch.").arg(inet));
            okDone = false;
        }

        if(okDone && (dbMan->vlanAvailability(motherAgg1Id, sinet)))
        {
            QMessageBox::warning(this, "ERROR", QString("Vlan %1 currently exists in aggregation switch.").arg(sinet));
            okDone = false;
        }

        if(okDone && (dbMan->vlanAvailability(motherAgg1Id, voip)))
        {
            QMessageBox::warning(this, "ERROR", QString("Vlan %1 currently exists in aggregation switch.").arg(voip));
            okDone = false;
        }

        if(!okDone)
        {
            //remove vlans
            list.clear();
            list  << hsi << inet << sinet << voip;//<< ipmedia
            dbMan->deleteVlan(dslamId, list);
            return;
        }


        bool cxConnection = dbMan->cxDslamExistance(dslamId);
        bool aggConnection = dbMan->dslamAggExistance(dslamId);

        if(aggConnection)
            currentAggAPV = dbMan->getAggDslamAllowPass(dslamId);
        else
            currentAggAPV="";

        if(cxConnection)
            currentCxAPV = dbMan->getCxDslamAllowPass(dslamId);
        else
            currentCxAPV = "";

        if( (currentAggAPV.isEmpty() && aggConnection) || (currentCxAPV.isEmpty() && cxConnection))
        {
            QMessageBox::warning(this, "ERROR", "Cannot fetch current Allow-Pass VLANs.");
            okDone = false;
        }


        //dslam
        if(okDone && cxConnection)
        {
            list = dbMan->rangeToList(currentAggAPV);
            list << hsi << inet << sinet << voip;
            temp = dbMan->listToRange(list);
            temp.replace(",", ", ");
            if(okDone)
                if(!dbMan->updateAggDslamAllowPass(dslamId, temp))
                    okDone = false;

            list = dbMan->rangeToList(currentCxAPV);
            //list  << ipmedia;
            temp = dbMan->listToRange(list);
            temp.replace(",", ", ");
            if(okDone)
                if(!dbMan->updateCxDslamAllowPass(dslamId, temp))
                    okDone = false;

        }
        else if(okDone && aggConnection)
        {

            list = dbMan->rangeToList(currentAggAPV);
            list   << hsi << inet << sinet << voip ; //<< ipmedia
            temp = dbMan->listToRange(list);
            temp.replace(",", ", ");
            if(okDone)
                if(!dbMan->updateAggDslamAllowPass(dslamId, temp))
                    okDone = false;
        }

        //AGG
        if(okDone && aggConnection)
        {
            if(!dbMan->insertVlan(motherAgg1Id,hsi,dslamName+"TCT-HSI") && okDone)
                okDone = false;
            if(!dbMan->insertVlan(motherAgg1Id,inet,dslamName+"TCT-Inet") && okDone)
                okDone = false;
            if(!dbMan->insertVlan(motherAgg1Id,sinet,dslamName+"TCT-Sinet") && okDone)
                okDone = false;
            if(!dbMan->insertVlan(motherAgg1Id,voip,dslamName+"TCT-Voip") && okDone)
                okDone = false;
        }


//        if(okDone && cxConnection)
//        {

//            //cx
//            //            if(!dbMan->insertVlan(motherCxId,vas,dslamName+"TCT-VAS") && okDone)
//            //                okDone = false;
//            if(!dbMan->insertVlan(motherCxId,ipmedia,dslamName+"Reserved") && okDone)
//                okDone = false;

//        }
//        else if(okDone && aggConnection)
//        {
//            //            if(!dbMan->insertVlan(motherAgg1Id,vas,dslamName+"TCT-VAS") && okDone)
//            //                okDone = false;
//            if(!dbMan->insertVlan(motherAgg1Id,ipmedia,dslamName+"Reserved") && okDone)
//                okDone = false;
//        }

        if(!okDone)
        {
            list.clear();
            list   << hsi << inet << sinet << voip; //<< ipmedia
            dbMan->deleteVlan(motherCxId, list);
            dbMan->deleteVlan(motherAgg1Id, list);

            if(cxConnection)
                dbMan->updateCxDslamAllowPass(dslamId, currentCxAPV);

            if(aggConnection)
                dbMan->updateAggDslamAllowPass(dslamId, currentAggAPV);
        }
    }


    if(!okDone)
    {
        //remove vlans
        list.clear();
        list   << hsi << inet << sinet << voip; //<< ipmedia
        dbMan->deleteVlan(dslamId, list);

        QMessageBox::warning(this, "ERROR", "Cannot insert new VLANs. Try again later.");
    }


    this->close();
}

void DslamAddNewVlanDialog::on_hsiSB_valueChanged(int arg1)
{
    int offset = arg1-500;
    ui->inetSB->setValue(600+offset);
    ui->sinetSB->setValue(700+offset);
    ui->voipSB->setValue(800+offset);
}
