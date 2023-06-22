#include "accessdoubleswitchdialog.h"
#include "ui_accessdoubleswitchdialog.h"
#include "lib/database/danetdbman.h"
#include "lib/netTools/nettools.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QFileDialog>
#include <QPrinter>
#include <QDateTime>
#include <QTextEdit>
#include <QFile>
#include <QDir>

AccessDoubleSwitchDialog::AccessDoubleSwitchDialog(QWidget *parent, DanetDbMan *db) :
    QDialog(parent),
    ui(new Ui::AccessDoubleSwitchDialog),
    dbMan(db)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowMinMaxButtonsHint);

    ui->tabWidget->tabBar()->hide();
    ui->tabWidget->setCurrentIndex(0);

    bras_ip_check = false;

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
    int workingArea = dbMan->getLoggedInDefaultArea();
    ui->areaCB->setCurrentIndex(ui->areaCB->findData(workingArea));

    QSqlQuery *query = dbMan->selectModules();
    QString mod;
    while(query->next())
    {
        mod = query->value(1).toString();
        ui->agg1ModCB->addItem(mod);
        ui->agg2ModCB->addItem(mod);
        ui->LmetroModCB->addItem(mod);
        ui->RmetroModCB->addItem(mod);
    }

    ui->agg1ModCB->setCurrentText("SX-500m");
    ui->agg2ModCB->setCurrentText("SX-500m");
    ui->LmetroModCB->setCurrentText("SX-500m");
    ui->RmetroModCB->setCurrentText("LX-10Km");

}

AccessDoubleSwitchDialog::~AccessDoubleSwitchDialog()
{
    delete ui;
}

void AccessDoubleSwitchDialog::checkEntry()
{
    //ip plan
    bool LN = !ui->LNmsUplinkCB->currentText().isEmpty();
    bool LS = !ui->LSigUplinkCB->currentText().isEmpty();
    bool LM = !ui->LMediaUplinkCB->currentText().isEmpty();
    bool LV = !ui->LVoipUplinkCB->currentText().isEmpty();

    bool RN = !ui->RNmsUplinkCB->currentText().isEmpty();
    bool RS = !ui->RSigUplinkCB->currentText().isEmpty();
    bool RM = !ui->RMediaUplinkCB->currentText().isEmpty();
    bool RV = !ui->RVoipUplinkCB->currentText().isEmpty();

    bool IN = !ui->INmsUplinkCB->currentText().isEmpty();
    bool IS = !ui->ISigUplinkCB->currentText().isEmpty();
    bool IM = !ui->IMediaUplinkCB->currentText().isEmpty();
    bool IV = !ui->IVoipUplinkCB->currentText().isEmpty();

    bool AB = (!ui->LabbrCB->currentText().isEmpty()) && (!ui->RabbrCB->currentText().isEmpty());

    if(AB && LN && LS && LM && LV && RN && RS && RM && RV && IN && IS && IM && IV)
    {
        ui->ipNextBtn->setEnabled(true);
        //metro
        bool Lmetro = !ui->LmetroCB->currentText().isEmpty();
        bool LmetroInt = !ui->LmetroIntCB->currentText().isEmpty();
        bool LmetroRInt = !ui->LmetroRemoteIntLE->text().isEmpty();
        bool LmetroMod = !ui->LmetroModCB->currentText().isEmpty();

        bool Rmetro = !ui->RmetroCB->currentText().isEmpty();
        bool RmetroInt = !ui->RmetroIntCB->currentText().isEmpty();
        bool RmetroRInt = !ui->RmetroRemoteIntLE->text().isEmpty();
        bool RmetroMod = !ui->RmetroModCB->currentText().isEmpty();

        if(Lmetro && LmetroInt && LmetroRInt && LmetroMod && Rmetro && RmetroInt && RmetroRInt && RmetroMod )
        {
            ui->metroNextBtn->setEnabled(true);
            //agg
            bool A1 = !ui->agg1CB->currentText().isEmpty();
            bool A1Int = !ui->agg1IntCB->currentText().isEmpty();
            bool A1RInt = !ui->agg1RemoteIntLE->text().isEmpty();
            bool A1M = !ui->agg1ModCB->currentText().isEmpty();

            bool A2 = !ui->agg2CB->currentText().isEmpty();
            bool A2Int = !ui->agg2IntCB->currentText().isEmpty();
            bool A2RInt = !ui->agg2RemoteIntLE->text().isEmpty();
            bool A2M = !ui->agg2ModCB->currentText().isEmpty();

            if(A1 && A1Int && A1RInt && A1M && A2 && A2Int && A2RInt && A2M)
            {
                ui->aggNextBtn->setEnabled(true);
                //bras
                bool B1 = !ui->bras1CB->currentText().isEmpty();
                bool B1A1 = (ui->b1a1SB->value() > -1)? true : false;
                bool B1A2 = (ui->b1a2SB->value() > -1)? true : false;

                bool B2 = !ui->bras1CB->currentText().isEmpty();
                bool B2A1 = (ui->b1a1SB->value() > -1)? true : false;
                bool B2A2 = (ui->b1a2SB->value() > -1)? true : false;

                if(B1 && B1A1 && B1A2 && B2 && B2A1 && B2A2)
                    ui->okBtn->setEnabled(true);
                else
                    ui->okBtn->setEnabled(false);

            }
            else
            {
                ui->aggNextBtn->setEnabled(false);
                ui->okBtn->setEnabled(false);
            }


        }
        else
        {
            ui->metroNextBtn->setEnabled(false);
            ui->okBtn->setEnabled(false);
        }

    }
    else
    {
        ui->ipNextBtn->setEnabled(false);
        ui->okBtn->setEnabled(false);
    }


}

void AccessDoubleSwitchDialog::on_areaCB_currentIndexChanged(int index)
{
    int area = index + 2;
    ui->RabbrCB->clear();
    ui->LabbrCB->clear();
    ui->RabbrCB->setEnabled(false);
    ui->okBtn->setEnabled(false);

    ui->bras1CB->clear();
    ui->bras2CB->clear();
    ui->b1a1Lbl->clear();
    ui->b1a2Lbl->clear();
    ui->b2a1Lbl->clear();
    ui->b2a2Lbl->clear();

    QSqlQuery *query = dbMan->selectAreaAbbreviations(area,true);

    ui->LabbrCB->blockSignals(true);
    ui->RabbrCB->blockSignals(true);

    ui->agg1CB->clear();
    ui->agg2CB->clear();
    ui->LmetroCB->clear();
    ui->RmetroCB->clear();

    ui->LNmsUplinkCB->clear();
    ui->LSigUplinkCB->clear();
    ui->LMediaUplinkCB->clear();
    ui->LVoipUplinkCB->clear();

    ui->RNmsUplinkCB->clear();
    ui->RSigUplinkCB->clear();
    ui->RMediaUplinkCB->clear();
    ui->RVoipUplinkCB->clear();

    ui->INmsUplinkCB->clear();
    ui->ISigUplinkCB->clear();
    ui->IMediaUplinkCB->clear();
    ui->IVoipUplinkCB->clear();


    int exchId;
    QString abbr;
    while(query->next())
    {
        exchId = query->value(0).toInt();
        abbr = query->value(1).toString();

        ui->LabbrCB->addItem(abbr, exchId);
        ui->RabbrCB->addItem(abbr, exchId);
    }
    ui->RabbrCB->setCurrentIndex(-1);
    ui->LabbrCB->setCurrentIndex(-1);
    ui->LabbrCB->blockSignals(false);
    ui->RabbrCB->blockSignals(false);

    ui->tabWidget->setCurrentIndex(0);
    ui->ipNextBtn->setEnabled(false);
}

void AccessDoubleSwitchDialog::on_LabbrCB_currentIndexChanged(int index)
{
    index++;
    ui->tabWidget->setCurrentIndex(0);
    ui->ipNextBtn->setEnabled(false);

    ui->okBtn->setEnabled(false);
    ui->RabbrCB->setEnabled(true);
    ui->RabbrCB->setCurrentIndex(-1);

    ui->agg1CB->clear();
    ui->agg2CB->clear();

    ui->bras1CB->clear();
    ui->bras2CB->clear();
    ui->b1a1Lbl->clear();
    ui->b1a2Lbl->clear();
    ui->b2a1Lbl->clear();
    ui->b2a2Lbl->clear();

    ui->LmetroCB->clear();
    ui->RmetroCB->clear();

    ui->LNmsUplinkCB->clear();
    ui->LSigUplinkCB->clear();
    ui->LMediaUplinkCB->clear();
    ui->LVoipUplinkCB->clear();

    ui->RNmsUplinkCB->clear();
    ui->RSigUplinkCB->clear();
    ui->RMediaUplinkCB->clear();
    ui->RVoipUplinkCB->clear();

    ui->INmsUplinkCB->clear();
    ui->ISigUplinkCB->clear();
    ui->IMediaUplinkCB->clear();
    ui->IVoipUplinkCB->clear();

    if(ui->LabbrCB->currentData().toInt() == ui->RabbrCB->currentData().toInt()) return;

    int exchId = ui->LabbrCB->currentData().toInt();
    // agg
    ui->agg1CB->blockSignals(true);
    ui->agg2CB->blockSignals(true);

    QSqlQuery *query = dbMan->selectExchangeAgg(exchId,true);
    int devexId;
    QString device;

    while(query->next())
    {
        devexId = query->value(0).toInt();
        device = query->value(1).toString();
        ui->agg1CB->addItem(device, devexId);
    }
    ui->agg1CB->setCurrentIndex(-1);
    ui->agg1CB->blockSignals(false);
    if(ui->agg1CB->count() > 0) ui->agg1CB->setCurrentIndex(0);

    query = dbMan->selectExchangeAgg(exchId,false);

    while(query->next())
    {
        devexId = query->value(0).toInt();
        device = query->value(1).toString();
        ui->agg2CB->addItem(device, devexId);
    }
    ui->agg2CB->setCurrentIndex(-1);
    ui->agg2CB->blockSignals(false);
    if(ui->agg2CB->count() > 0) ui->agg2CB->setCurrentIndex(0);

    //metro
    ui->LmetroCB->blockSignals(true);

    query = dbMan->selectExchangeCx(exchId);
    //local
    while(query->next())
    {
        devexId = query->value(0).toInt();
        device = query->value(1).toString();
        ui->LmetroCB->addItem(device, devexId);
    }
    ui->LmetroCB->setCurrentIndex(-1);
    ui->LmetroCB->blockSignals(false);
    if(ui->LmetroCB->count() > 0) ui->LmetroCB->setCurrentIndex(0);

    //ipplan

    int area = ui->areaCB->currentData().toInt();
    QString subnetLike;
    if(area < 9) subnetLike = QString::number(area * 10); else subnetLike = "10";
    subnetLike = "10."+subnetLike+"%";

    query = dbMan->selectEmptySubnetPool3(area,"Uplink", "Access Switch", 30);
    //`id`,`subnet`
    int id;
    QString subnet;
    while(query->next())
    {
        id = query->value(0).toInt();
        subnet = query->value(1).toString() + "/30";

        ui->LNmsUplinkCB->addItem(subnet, id);
        ui->LSigUplinkCB->addItem(subnet, id);
        ui->LMediaUplinkCB->addItem(subnet, id);
        ui->LVoipUplinkCB->addItem(subnet, id);

        ui->RNmsUplinkCB->addItem(subnet, id);
        ui->RSigUplinkCB->addItem(subnet, id);
        ui->RMediaUplinkCB->addItem(subnet, id);
        ui->RVoipUplinkCB->addItem(subnet, id);

        ui->INmsUplinkCB->addItem(subnet, id);
        ui->ISigUplinkCB->addItem(subnet, id);
        ui->IMediaUplinkCB->addItem(subnet, id);
        ui->IVoipUplinkCB->addItem(subnet, id);
    }

    if(query->size() > 11)
    {
        ui->LNmsUplinkCB->setCurrentIndex(0);
        ui->LSigUplinkCB->setCurrentIndex(1);
        ui->LMediaUplinkCB->setCurrentIndex(2);
        ui->LVoipUplinkCB->setCurrentIndex(3);

        ui->RNmsUplinkCB->setCurrentIndex(4);
        ui->RSigUplinkCB->setCurrentIndex(5);
        ui->RMediaUplinkCB->setCurrentIndex(6);
        ui->RVoipUplinkCB->setCurrentIndex(7);

        ui->INmsUplinkCB->setCurrentIndex(8);
        ui->ISigUplinkCB->setCurrentIndex(9);
        ui->IMediaUplinkCB->setCurrentIndex(10);
        ui->IVoipUplinkCB->setCurrentIndex(11);

        bras_ip_check = true;
    }
    else
        QMessageBox::warning(this,"ERROR", "You need at least 12 subnet with /30 prefix.\n Check Plan > IP Plan");


    if(!dbMan->brasAggConnectionExistance(exchId))
    {
        bras_ip_check = false;
        QMessageBox::warning(this,"ERROR", "You need to define BRAS-Aggregation Ether-trunk connection.\n Check Plan > Connection > BRAS TO AGG.");
    }
    else
    {
        //BRAS1
        ui->bras1CB->blockSignals(true);
        query = dbMan->selectBras(exchId,true);
        //id, name
        int ID;
        QString NAME;
        while(query->next())
        {
            ID = query->value(0).toInt();
            NAME = query->value(1).toString();

            ui->bras1CB->addItem(NAME, ID);
        }
        ui->bras1CB->setCurrentIndex(-1);
        ui->bras1CB->blockSignals(false);
        if(ui->bras1CB->count() > 0) ui->bras1CB->setCurrentIndex(0);

        //bras2
        ui->bras2CB->blockSignals(true);
        query = dbMan->selectBras( exchId, false);
        //id, name
        while(query->next())
        {
            ID = query->value(0).toInt();
            NAME = query->value(1).toString();

            ui->bras2CB->addItem(NAME, ID);
        }
        ui->bras2CB->setCurrentIndex(-1);
        ui->bras2CB->blockSignals(false);
        if(ui->bras2CB->count() > 0) ui->bras2CB->setCurrentIndex(0);
    }

    checkEntry();
}

void AccessDoubleSwitchDialog::on_RabbrCB_currentIndexChanged(int index)
{
    index++;
    ui->tabWidget->setCurrentIndex(0);
    ui->ipNextBtn->setEnabled(false);

    ui->okBtn->setEnabled(false);

    ui->RmetroCB->blockSignals(true);
    ui->RmetroCB->clear();

    if(ui->LabbrCB->currentData().toInt() == ui->RabbrCB->currentData().toInt()) return;


    //metro remote
    int remoteExch = ui->RabbrCB->currentData().toInt();
    QSqlQuery *query = dbMan->selectExchangeCx(remoteExch);

    int devexId;
    QString device;
    while(query->next())
    {
        devexId = query->value(0).toInt();
        device = query->value(1).toString();
        ui->RmetroCB->addItem(device, devexId);
    }
    ui->RmetroCB->setCurrentIndex(-1);
    ui->RmetroCB->blockSignals(false);
    if(ui->RmetroCB->count() > 0) ui->RmetroCB->setCurrentIndex(0);
}

void AccessDoubleSwitchDialog::on_LNmsUplinkCB_currentTextChanged(const QString &arg1)
{
    if(arg1.isEmpty())
        ui->okBtn->setEnabled(false);
    else
        checkEntry();
}

void AccessDoubleSwitchDialog::on_LSigUplinkCB_currentTextChanged(const QString &arg1)
{
    if(arg1.isEmpty())
        ui->okBtn->setEnabled(false);
    else
        checkEntry();
}

void AccessDoubleSwitchDialog::on_LMediaUplinkCB_currentTextChanged(const QString &arg1)
{
    if(arg1.isEmpty())
        ui->okBtn->setEnabled(false);
    else
        checkEntry();
}

void AccessDoubleSwitchDialog::on_LVoipUplinkCB_currentTextChanged(const QString &arg1)
{
    if(arg1.isEmpty())
        ui->okBtn->setEnabled(false);
    else
        checkEntry();
}

void AccessDoubleSwitchDialog::on_RNmsUplinkCB_currentTextChanged(const QString &arg1)
{
    if(arg1.isEmpty())
        ui->okBtn->setEnabled(false);
    else
        checkEntry();
}

void AccessDoubleSwitchDialog::on_RSigUplinkCB_currentTextChanged(const QString &arg1)
{
    if(arg1.isEmpty())
        ui->okBtn->setEnabled(false);
    else
        checkEntry();
}

void AccessDoubleSwitchDialog::on_RMediaUplinkCB_currentTextChanged(const QString &arg1)
{
    if(arg1.isEmpty())
        ui->okBtn->setEnabled(false);
    else
        checkEntry();
}

void AccessDoubleSwitchDialog::on_RVoipUplinkCB_currentTextChanged(const QString &arg1)
{
    if(arg1.isEmpty())
        ui->okBtn->setEnabled(false);
    else
        checkEntry();
}

void AccessDoubleSwitchDialog::on_INmsUplinkCB_currentTextChanged(const QString &arg1)
{
    if(arg1.isEmpty())
        ui->okBtn->setEnabled(false);
    else
        checkEntry();
}

void AccessDoubleSwitchDialog::on_ISigUplinkCB_currentTextChanged(const QString &arg1)
{
    if(arg1.isEmpty())
        ui->okBtn->setEnabled(false);
    else
        checkEntry();
}

void AccessDoubleSwitchDialog::on_IMediaUplinkCB_currentTextChanged(const QString &arg1)
{
    if(arg1.isEmpty())
        ui->okBtn->setEnabled(false);
    else
        checkEntry();
}

void AccessDoubleSwitchDialog::on_IVoipUplinkCB_currentTextChanged(const QString &arg1)
{
    if(arg1.isEmpty())
        ui->okBtn->setEnabled(false);
    else
        checkEntry();
}

void AccessDoubleSwitchDialog::on_LmetroCB_currentIndexChanged(int index)
{
    index++;
    ui->LmetroIntCB->clear();
    int cxDevexId = ui->LmetroCB->currentData().toInt();
    QSqlQuery *query = dbMan->selectDeviveEmptyPorts(cxDevexId);
    //`id`, `interface_type`, `interface`
    int id;
    QString type,interface;
    while(query->next())
    {
        id = query->value(0).toInt();
        type = query->value(1).toString();
        type = dbMan->getInterfaceTypeSymble(type);
        interface = query->value(2).toString();
        interface = type + "-" + interface;
        ui->LmetroIntCB->addItem(interface, id);
    }

}

void AccessDoubleSwitchDialog::on_LmetroIntCB_currentTextChanged(const QString &arg1)
{
    if(arg1.isEmpty())
        ui->okBtn->setEnabled(false);
    else
        checkEntry();
}

void AccessDoubleSwitchDialog::on_LmetroRemoteIntLE_textChanged(const QString &arg1)
{
    if(arg1.isEmpty())
        ui->okBtn->setEnabled(false);
    else
        checkEntry();
}

void AccessDoubleSwitchDialog::on_LmetroModCB_currentTextChanged(const QString &arg1)
{
    if(arg1.isEmpty())
        ui->okBtn->setEnabled(false);
    else
        checkEntry();
}

void AccessDoubleSwitchDialog::on_RmetroCB_currentIndexChanged(int index)
{
    index++;
    ui->RmetroIntCB->clear();
    int cxDevexId = ui->RmetroCB->currentData().toInt();
    QSqlQuery *query = dbMan->selectDeviveEmptyPorts(cxDevexId);
    //`id`, `interface_type`, `interface`
    int id;
    QString type,interface;
    while(query->next())
    {
        id = query->value(0).toInt();
        type = query->value(1).toString();
        type = dbMan->getInterfaceTypeSymble(type);
        interface = query->value(2).toString();
        interface = type + "-" + interface;
        ui->RmetroIntCB->addItem(interface, id);
    }
}

void AccessDoubleSwitchDialog::on_RmetroIntCB_currentTextChanged(const QString &arg1)
{
    if(arg1.isEmpty())
        ui->okBtn->setEnabled(false);
    else
        checkEntry();
}

void AccessDoubleSwitchDialog::on_RmetroRemoteIntLE_textChanged(const QString &arg1)
{
    if(arg1.isEmpty())
        ui->okBtn->setEnabled(false);
    else
        checkEntry();
}

void AccessDoubleSwitchDialog::on_RmetroModCB_currentTextChanged(const QString &arg1)
{
    if(arg1.isEmpty())
        ui->okBtn->setEnabled(false);
    else
        checkEntry();
}

void AccessDoubleSwitchDialog::on_agg1CB_currentIndexChanged(int index)
{
    index++;
    ui->agg1IntCB->clear();
    int agg1DevexId = ui->agg1CB->currentData().toInt();
    QSqlQuery *query = dbMan->selectDeviveEmptyPorts(agg1DevexId);
    //`id`, `interface_type`, `interface`
    int id;
    QString type,interface;
    while(query->next())
    {
        id = query->value(0).toInt();
        type = query->value(1).toString();
        type = dbMan->getInterfaceTypeSymble(type);
        interface = query->value(2).toString();
        interface = type + "-" + interface;
        ui->agg1IntCB->addItem(interface, id);
    }
}

void AccessDoubleSwitchDialog::on_agg1IntCB_currentTextChanged(const QString &arg1)
{
    if(arg1.isEmpty())
        ui->okBtn->setEnabled(false);
    else
        checkEntry();
}

void AccessDoubleSwitchDialog::on_agg1RemoteIntLE_textChanged(const QString &arg1)
{
    if(arg1.isEmpty())
        ui->okBtn->setEnabled(false);
    else
        checkEntry();
}

void AccessDoubleSwitchDialog::on_agg1ModCB_currentTextChanged(const QString &arg1)
{
    if(arg1.isEmpty())
        ui->okBtn->setEnabled(false);
    else
        checkEntry();
}

void AccessDoubleSwitchDialog::on_agg2CB_currentIndexChanged(int index)
{
    index++;
    ui->agg2IntCB->clear();
    int agg2DevexId = ui->agg2CB->currentData().toInt();
    QSqlQuery *query = dbMan->selectDeviveEmptyPorts(agg2DevexId);
    //`id`, `interface_type`, `interface`
    int id;
    QString type,interface;
    while(query->next())
    {
        id = query->value(0).toInt();
        type = query->value(1).toString();
        type = dbMan->getInterfaceTypeSymble(type);
        interface = query->value(2).toString();
        interface = type + "-" + interface;
        ui->agg2IntCB->addItem(interface, id);
    }
}

void AccessDoubleSwitchDialog::on_agg2IntCB_currentTextChanged(const QString &arg1)
{
    if(arg1.isEmpty())
        ui->okBtn->setEnabled(false);
    else
        checkEntry();
}

void AccessDoubleSwitchDialog::on_agg2RemoteIntLE_textChanged(const QString &arg1)
{
    if(arg1.isEmpty())
        ui->okBtn->setEnabled(false);
    else
        checkEntry();
}

void AccessDoubleSwitchDialog::on_agg2ModCB_currentTextChanged(const QString &arg1)
{
    if(arg1.isEmpty())
        ui->okBtn->setEnabled(false);
    else
        checkEntry();
}

void AccessDoubleSwitchDialog::on_bras1CB_currentIndexChanged(int index)
{
    index++;
    int brasId = ui->bras1CB->currentData().toInt();
    int exchId = ui->LabbrCB->currentData().toInt();
    QList<int> list = dbMan->getBrasAggIdEth(brasId, exchId);
    //`agg1_id`,`agg1_eth`,`agg2_id`,`agg2_eth`
    if(list.at(0) > -1)
    {
        ui->b1a1SB->setValue(list.at(1));
        ui->b1a2SB->setValue(list.at(3));

        QString lbl = "Eth To "+ ui->LabbrCB->currentText() +" Agg ";
        ui->b1a1Lbl->setText(lbl+"1 :");
        ui->b1a2Lbl->setText(lbl+"2 :");

    }
    else
    {
        ui->b1a1SB->setValue(-1);
        ui->b1a2SB->setValue(-1);
    }
}

void AccessDoubleSwitchDialog::on_bras2CB_currentIndexChanged(int index)
{
    index++;
    int brasId = ui->bras2CB->currentData().toInt();
    int exchId = ui->LabbrCB->currentData().toInt();
    QList<int> list = dbMan->getBrasAggIdEth(brasId, exchId);
    //`agg1_id`,`agg1_eth`,`agg2_id`,`agg2_eth`
    if(list.at(0) > -1)
    {
        ui->b2a1SB->setValue(list.at(1));
        ui->b2a2SB->setValue(list.at(3));

        QString lbl = "Eth To "+ ui->LabbrCB->currentText() +" Agg ";
        ui->b2a1Lbl->setText(lbl+"1 :");
        ui->b2a2Lbl->setText(lbl+"2 :");

    }
    else
    {
        ui->b2a1SB->setValue(-1);
        ui->b2a2SB->setValue(-1);
    }
}

void AccessDoubleSwitchDialog::on_bras2EnCB_toggled(bool checked)
{
    if(checked)
        ui->bras2GB->show();
    else
        ui->bras2GB->hide();
}

void AccessDoubleSwitchDialog::on_okBtn_clicked()
{
    int agg1DevexId = ui->agg1CB->currentData().toInt();
    int agg2DevexId = ui->agg2CB->currentData().toInt();
    int LmetroDevexId = ui->LmetroCB->currentData().toInt();
    int RmetroDevexId = ui->RmetroCB->currentData().toInt();
    int a1e = ui->agg1EthSB->value();
    int a2e = ui->agg2EthSB->value();
    int lce = ui->LmetroEthSB->value();
    int rce = ui->RmetroEthSB->value();
    bool a1 = dbMan->isEtherTrunkUsed(agg1DevexId, a1e);
    bool a2 = dbMan->isEtherTrunkUsed(agg2DevexId, a2e);
    bool lc = dbMan->isEtherTrunkUsed(LmetroDevexId, lce);
    bool rc = dbMan->isEtherTrunkUsed(RmetroDevexId, rce);

    QString hsiVlan = QString::number(ui->hsiVlanSB->value());
    QString sinetVlan = QString::number(ui->sinetVlanSB->value());
    QString voipVlan = QString::number(ui->voipVlanSB->value());

    bool v15 = dbMan->vlanAvailability(agg1DevexId, ui->hsiVlanSB->value());
    bool v17 = dbMan->vlanAvailability(agg1DevexId,ui->sinetVlanSB->value());
    bool v18 = dbMan->vlanAvailability(agg1DevexId, ui->voipVlanSB->value());

    bool v25 = dbMan->vlanAvailability(agg2DevexId, ui->hsiVlanSB->value());
    bool v27 = dbMan->vlanAvailability(agg2DevexId,ui->sinetVlanSB->value());
    bool v28 = dbMan->vlanAvailability(agg2DevexId, ui->voipVlanSB->value());

    if(a1 || a2 || lc || rc || v15 || v17 || v18 || v25 || v27 || v28)
    {
        if(a1)
            QMessageBox::warning(this, "ERROR", "Ether-Trunk "+QString::number(a1e)+" in AGG1 is used.");

        if(a2)
            QMessageBox::warning(this, "ERROR", "Ether-Trunk "+QString::number(a2e)+" in AGG2 is used.");
        if(lc)
            QMessageBox::warning(this, "ERROR", "Ether-Trunk "+QString::number(lce)+" in local metro device is used.");

        if(rc)
            QMessageBox::warning(this, "ERROR", "Ether-Trunk "+QString::number(rce)+" in remote metro device is used.");

        //
        if(v15)
            QMessageBox::warning(this, "ERROR", "VLAN "+hsiVlan+" in AGG1 is used.");

        if(v17)
            QMessageBox::warning(this, "ERROR", "VLAN "+sinetVlan+" in AGG1 is used.");

        if(v18)
            QMessageBox::warning(this, "ERROR", "VLAN "+voipVlan+" in AGG1 is used.");

        //

        if(v25)
            QMessageBox::warning(this, "ERROR", "VLAN "+hsiVlan+" in AGG1 is used.");

        if(v27)
            QMessageBox::warning(this, "ERROR", "VLAN "+sinetVlan+" in AGG1 is used.");

        if(v28)
            QMessageBox::warning(this, "ERROR", "VLAN "+voipVlan+" in AGG1 is used.");

        return;

    }
    else
    {
        QString now = QDateTime::currentDateTime().toString("yyyy/MM/dd  hh:mm");
        QString name = dbMan->getLoggedinLastname();

        QString exchange = QString::number(ui->areaCB->currentData().toInt()) +"-"+ui->LabbrCB->currentText();
        QString remoteExchange = QString::number(ui->areaCB->currentData().toInt()) +"-"+ui->RabbrCB->currentText();
        QString str = exchange +"- Access Double Switch Plan";
        QString path = QFileDialog::getExistingDirectory(this, "Select Output Directory");
        if(!path.isEmpty())
        {

            QString reportPath; //for pdfs
            if(!path.endsWith('/'))
                path = path+"/";
            path += str;

            if(QDir(path).exists())
            {
                int i=2;
                while(QDir(path+QString::number(i)).exists())
                    i++;

                path += QString::number(i);
            }

            if(QDir().mkdir(path))
            {
                reportPath = path + "/"+"Data Plan-"+ exchange +"-Access Double Switch";
                if(QDir().mkdir(reportPath))
                {
                    QTextEdit doc;
                    //######## agg
                    //agg1
                    bool agg1Done = false;
                    agg1DevexId = ui->agg1CB->currentData().toInt();
                    //check if it is huawei device or not
                    int deviceId = dbMan->getDevexDeviceId(agg1DevexId);
                    if(dbMan->isDeviceHuaweiVendor(deviceId))
                    {
                        QString agg1Path = path+"/"+exchange+"-Agg1-Config.txt";
                        QFile agg1Source(":/configurations/HUAWEI/AccessDep/CONFIG-Agg");
                        if(agg1Source.open(QIODevice::ReadOnly | QIODevice::Text))
                        {
                            QString text = QString(agg1Source.readAll());
                            agg1Source.close();
                            QFile agg1(agg1Path);

                            if(agg1.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate))
                            {

                                QString exchParam = ui->LabbrCB->currentText();
                                QString switchParam = dbMan->getDevice(deviceId, false);
                                if(switchParam.endsWith("-1"))
                                {
                                  switchParam = switchParam.replace("-1", "-");
                                }
                                QString interfaceParam = ui->agg1IntCB->currentText();
                                interfaceParam.remove("10G",Qt::CaseInsensitive);
                                interfaceParam.remove("1G",Qt::CaseInsensitive);
                                interfaceParam.remove("-",Qt::CaseInsensitive);
                                QString gigabitEthernetParam;
                                if(interfaceParam.contains("10G",Qt::CaseInsensitive))
                                    gigabitEthernetParam = "XGigabitEthernet";
                                else
                                    gigabitEthernetParam = "GigabitEthernet";

                                QString remoteInterfaceParam = ui->agg1RemoteIntLE->text();
                                QString ethParam = QString::number(ui->agg1EthSB->value());

                                text.replace("exchangeParam", exchParam);
                                text.replace("AggNO", "1");
                                text.replace("SWNo", "1");
                                text.replace("switchParam",switchParam);
                                text.replace("gigabitEthernetParam",gigabitEthernetParam);
                                text.replace("interfaceParam",interfaceParam);
                                text.replace("remoteInterfaceParam",remoteInterfaceParam);
                                text.replace("ethParam",ethParam);

                                text.replace("hsiVlan",hsiVlan);
                                text.replace("sinetVlan",sinetVlan);
                                text.replace("voipVlan",voipVlan);


                                agg1.seek(0); // go to the beginning of the file
                                text = "\t\tDaNet Configuration Report\n\nReported By "  +name + "\n" +now+"\n_______________________________________\n\n" + text;

                                agg1.write(text.toUtf8()); // write the new text back to the file

                                agg1.close(); // close the file handle.
                                agg1Done = true;

                                doc.setText(text);

                            }
                        }

                        if(agg1Done)
                        {
                            //print pdfs
                            //print config
                            QString agg1PdfPath = reportPath+"/"+exchange+"-Agg1-Config.pdf";
                            QPrinter printer(QPrinter::PrinterResolution);
                            printer.setOutputFormat(QPrinter::PdfFormat);
                            printer.setPaperSize(QPrinter::A4);
                            printer.setOutputFileName(agg1PdfPath);
                            doc.print(&printer);

                        }

                    }

                    //agg2
                    bool agg2Done = false;
                    agg2DevexId = ui->agg2CB->currentData().toInt();
                    //check if it is huawei device or not
                    deviceId = dbMan->getDevexDeviceId(agg2DevexId);
                    if(dbMan->isDeviceHuaweiVendor(deviceId))
                    {
                        QString agg2Path = path+"/"+exchange+"-Agg2-Config.txt";
                        QFile agg1Source(":/configurations/HUAWEI/AccessDep/CONFIG-Agg");
                        if(agg1Source.open(QIODevice::ReadOnly | QIODevice::Text))
                        {
                            QString text = QString(agg1Source.readAll());
                            agg1Source.close();
                            QFile agg2(agg2Path);

                            if(agg2.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate))
                            {

                                QString exchParam = ui->LabbrCB->currentText();
                                QString switchParam = dbMan->getDevice(deviceId, false);
                                QString interfaceParam = ui->agg2IntCB->currentText();
                                interfaceParam.remove("10G",Qt::CaseInsensitive);
                                interfaceParam.remove("1G",Qt::CaseInsensitive);
                                interfaceParam.remove("-",Qt::CaseInsensitive);
                                QString gigabitEthernetParam;
                                if(interfaceParam.contains("10G",Qt::CaseInsensitive))
                                    gigabitEthernetParam = "XGigabitEthernet";
                                else
                                    gigabitEthernetParam = "GigabitEthernet";

                                QString remoteInterfaceParam = ui->agg2RemoteIntLE->text();
                                QString ethParam = QString::number(ui->agg2EthSB->value());

                                text.replace("exchangeParam", exchParam);
                                text.replace("AggNO", "2");
                                text.replace("SWNo", "2");
                                text.replace("switchParam",switchParam);
                                text.replace("gigabitEthernetParam",gigabitEthernetParam);
                                text.replace("interfaceParam",interfaceParam);
                                text.replace("remoteInterfaceParam",remoteInterfaceParam);
                                text.replace("ethParam",ethParam);

                                text.replace("hsiVlan",hsiVlan);
                                text.replace("sinetVlan",sinetVlan);
                                text.replace("voipVlan",voipVlan);

                                agg2.seek(0); // go to the beginning of the file
                                text = "\t\tDaNet Configuration Report\n\nReported By "  +name + "\n" +now+"\n_______________________________________\n\n" + text;

                                agg2.write(text.toUtf8()); // write the new text back to the file

                                agg2.close(); // close the file handle.
                                agg2Done = true;

                                doc.setText(text);

                            }
                        }

                        if(agg2Done)
                        {
                            //print pdfs
                            //print config
                            QString agg2PdfPath = reportPath+"/"+exchange+"-Agg2-Config.pdf";
                            QPrinter printer(QPrinter::PrinterResolution);
                            printer.setOutputFormat(QPrinter::PdfFormat);
                            printer.setPaperSize(QPrinter::A4);
                            printer.setOutputFileName(agg2PdfPath);
                            doc.print(&printer);

                        }

                    }

                    //######## Metro
                    // local
                    bool LmetroDone = false;
                    LmetroDevexId = ui->LmetroCB->currentData().toInt();
                    //check metro device vendor
                    deviceId = dbMan->getDevexDeviceId(LmetroDevexId);
                    if(dbMan->isDeviceHuaweiVendor(deviceId))
                    {
                        QString LcxPath = path+"/"+exchange+"-Local CX600-Config.txt";
                        QFile LcxSource(":/configurations/HUAWEI/AccessDep/CONFIG-CX600");
                        if(LcxSource.open(QIODevice::ReadOnly | QIODevice::Text))
                        {
                            QString text = QString(LcxSource.readAll());
                            LcxSource.close();
                            QFile Lcx(LcxPath);

                            if(Lcx.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate))
                            {

                                QString exchangeParam = ui->LabbrCB->currentText();
                                QString switchParam = dbMan->getDevice(deviceId, false);
                                QString interfaceParam = ui->LmetroIntCB->currentText();
                                interfaceParam.remove("10G",Qt::CaseInsensitive);
                                interfaceParam.remove("1G",Qt::CaseInsensitive);
                                interfaceParam.remove("-",Qt::CaseInsensitive);
                                QString gigabitEthernetParam;
                                if(interfaceParam.contains("10G",Qt::CaseInsensitive))
                                    gigabitEthernetParam = "XGigabitEthernet";
                                else
                                    gigabitEthernetParam = "GigabitEthernet";

                                QString remoteInterfaceParam = ui->LmetroRemoteIntLE->text();
                                QString ethParam = QString::number(ui->LmetroEthSB->value());
                                QString nmsOspfIdParam = QString::number(ui->LnmsOspfPidSB->value());
                                QString sigOspfIdParam = QString::number(ui->LsigOspfPidSB->value());
                                QString mediaOspfIdParam = QString::number(ui->LmediaOspfPidSB->value());
                                QString voipOspfIdParam = QString::number(ui->LvoipOspfPidSB->value());
                                QString nmsNetParam = ui->LNmsUplinkCB->currentText().remove("/30");
                                QString sigNetParam = ui->LSigUplinkCB->currentText().remove("/30");
                                QString mediaNetParam = ui->LMediaUplinkCB->currentText().remove("/30");
                                QString voipNetParam = ui->LVoipUplinkCB->currentText().remove("/30");
                                QString nmsIpParam = NetTools::netFirstIp(nmsNetParam, 30);
                                QString sigIpParam = NetTools::netFirstIp(sigNetParam, 30);
                                QString mediaIpParam = NetTools::netFirstIp(mediaNetParam, 30);
                                QString voipIpParam = NetTools::netFirstIp(voipNetParam, 30);


                                text.replace("deviceExchangeParam", exchangeParam);
                                text.replace("exchangeParam", exchangeParam);
                                text.replace("switchParam",switchParam);
                                text.replace("SWNo", "1");
                                text.replace("ethParam",ethParam);
                                text.replace("gigabitEthernetParam",gigabitEthernetParam);
                                text.replace("interfaceParam",interfaceParam);
                                text.replace("remoteInterfaceParam",remoteInterfaceParam);
                                text.replace("nmsOspfIdParam",nmsOspfIdParam);
                                text.replace("sigOspfIdParam",sigOspfIdParam);
                                text.replace("mediaOspfIdParam",mediaOspfIdParam);
                                text.replace("voipOspfIdParam",voipOspfIdParam);
                                text.replace("nmsNetParam",nmsNetParam);
                                text.replace("sigNetParam",sigNetParam);
                                text.replace("mediaNetParam",mediaNetParam);
                                text.replace("voipNetParam",voipNetParam);
                                text.replace("nmsIpParam",nmsIpParam);
                                text.replace("sigIpParam",sigIpParam);
                                text.replace("mediaIpParam",mediaIpParam);
                                text.replace("voipIpParam",voipIpParam);


                                Lcx.seek(0); // go to the beginning of the file
                                text = "\t\tDaNet Configuration Report\n\nReported By "  +name + "\n" +now+"\n_______________________________________\n\n" + text;
                                Lcx.write(text.toUtf8()); // write the new text back to the file

                                Lcx.close(); // close the file handle.
                                LmetroDone = true;
                                doc.setText(text);

                            }
                        }

                        if(LmetroDone)
                        {
                            //print pdfs
                            //print config
                            QString LcxPdfPath = reportPath+"/"+exchange+"-Local CX600-Config.pdf";
                            QPrinter printer(QPrinter::PrinterResolution);
                            printer.setOutputFormat(QPrinter::PdfFormat);
                            printer.setPaperSize(QPrinter::A4);
                            printer.setOutputFileName(LcxPdfPath);
                            doc.print(&printer);

                        }
                    }

                    //remote
                    bool RmetroDone = false;
                    QString remoteExch = QString::number(ui->areaCB->currentData().toInt()) +"-"+ui->RabbrCB->currentText();

                    RmetroDevexId = ui->RmetroCB->currentData().toInt();
                    //check metro device vendor
                    deviceId = dbMan->getDevexDeviceId(RmetroDevexId);
                    if(dbMan->isDeviceHuaweiVendor(deviceId))
                    {
                        QString RcxPath = path+"/"+remoteExch+"-Remote CX600-Config.txt";
                        QFile RcxSource(":/configurations/HUAWEI/AccessDep/CONFIG-CX600");
                        if(RcxSource.open(QIODevice::ReadOnly | QIODevice::Text))
                        {
                            QString text = QString(RcxSource.readAll());
                            RcxSource.close();
                            QFile Rcx(RcxPath);

                            if(Rcx.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate))
                            {
                                QString deviceExchangeParam = ui->RabbrCB->currentText();
                                QString exchangeParam = ui->LabbrCB->currentText();
                                QString switchParam = dbMan->getDevice(deviceId, false);
                                QString interfaceParam = ui->RmetroIntCB->currentText();
                                interfaceParam.remove("10G",Qt::CaseInsensitive);
                                interfaceParam.remove("1G",Qt::CaseInsensitive);
                                interfaceParam.remove("-",Qt::CaseInsensitive);
                                QString gigabitEthernetParam;
                                if(interfaceParam.contains("10G",Qt::CaseInsensitive))
                                    gigabitEthernetParam = "XGigabitEthernet";
                                else
                                    gigabitEthernetParam = "GigabitEthernet";

                                QString remoteInterfaceParam = ui->RmetroRemoteIntLE->text();
                                QString ethParam = QString::number(ui->RmetroEthSB->value());
                                QString nmsOspfIdParam = QString::number(ui->RnmsOspfPidSB->value());
                                QString sigOspfIdParam = QString::number(ui->RsigOspfPidSB->value());
                                QString mediaOspfIdParam = QString::number(ui->RmediaOspfPidSB->value());
                                QString voipOspfIdParam = QString::number(ui->RvoipOspfPidSB->value());
                                QString nmsNetParam = ui->RNmsUplinkCB->currentText().remove("/30");
                                QString sigNetParam = ui->RSigUplinkCB->currentText().remove("/30");
                                QString mediaNetParam = ui->RMediaUplinkCB->currentText().remove("/30");
                                QString voipNetParam = ui->RVoipUplinkCB->currentText().remove("/30");
                                QString nmsIpParam = NetTools::netFirstIp(nmsNetParam, 30);
                                QString sigIpParam = NetTools::netFirstIp(sigNetParam, 30);
                                QString mediaIpParam = NetTools::netFirstIp(mediaNetParam, 30);
                                QString voipIpParam = NetTools::netFirstIp(voipNetParam, 30);

                                text.replace("deviceExchangeParam", deviceExchangeParam);
                                text.replace("exchangeParam", exchangeParam);
                                text.replace("SWNo", "2");
                                text.replace("switchParam",switchParam);
                                text.replace("ethParam",ethParam);
                                text.replace("gigabitEthernetParam",gigabitEthernetParam);
                                text.replace("interfaceParam",interfaceParam);
                                text.replace("remoteInterfaceParam",remoteInterfaceParam);
                                text.replace("nmsOspfIdParam",nmsOspfIdParam);
                                text.replace("sigOspfIdParam",sigOspfIdParam);
                                text.replace("mediaOspfIdParam",mediaOspfIdParam);
                                text.replace("voipOspfIdParam",voipOspfIdParam);
                                text.replace("nmsNetParam",nmsNetParam);
                                text.replace("sigNetParam",sigNetParam);
                                text.replace("mediaNetParam",mediaNetParam);
                                text.replace("voipNetParam",voipNetParam);
                                text.replace("nmsIpParam",nmsIpParam);
                                text.replace("sigIpParam",sigIpParam);
                                text.replace("mediaIpParam",mediaIpParam);
                                text.replace("voipIpParam",voipIpParam);

                                text.replace("4031", "4036");
                                text.replace("4032", "4037");
                                text.replace("4034", "4038");
                                text.replace("4035", "4015");


                                Rcx.seek(0); // go to the beginning of the file
                                text = "\t\tDaNet Configuration Report\n\nReported By "  +name + "\n" +now+"\n_______________________________________\n\n" + text;
                                Rcx.write(text.toUtf8()); // write the new text back to the file

                                Rcx.close(); // close the file handle.
                                RmetroDone = true;
                                doc.setText(text);

                            }
                        }

                        if(RmetroDone)
                        {
                            //print pdfs
                            //print config
                            QString RcxPdfPath = reportPath+"/"+exchange+"-Remote CX600-Config.pdf";
                            QPrinter printer(QPrinter::PrinterResolution);
                            printer.setOutputFormat(QPrinter::PdfFormat);
                            printer.setPaperSize(QPrinter::A4);
                            printer.setOutputFileName(RcxPdfPath);
                            doc.print(&printer);

                        }
                    }

                    //######## BRAS1
                    bool bras1Done = false;
                    int bras1Id = ui->bras1CB->currentData().toInt();
                    //check vendor
                    deviceId = dbMan->getDevexDeviceId(bras1Id);
                    int aggId = ui->agg1CB->currentData().toInt();
                    int aggDeviceId = dbMan->getDevexDeviceId(aggId);
                    if(dbMan->isDeviceHuaweiVendor(deviceId))
                    {
                        QString bras1Path = path+"/"+"BRAS1-Config.txt";
                        QFile bras1Source(":/configurations/HUAWEI/AccessDep/CONFIG-BRAS");
                        if(bras1Source.open(QIODevice::ReadOnly | QIODevice::Text))
                        {
                            QString text = QString(bras1Source.readAll());
                            bras1Source.close();
                            QFile bras1File(bras1Path);

                            if(bras1File.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate))
                            {
                                QString brasExchParam = dbMan->getDevexAreaExchange(bras1Id);
                                QString exchParam = ui->LabbrCB->currentText();
                                QString deviceParam = dbMan->getDevice(deviceId, false);
                                QString switchParam = dbMan->getDevice(aggDeviceId, false);
                                if(switchParam.endsWith("-1"))
                                {
                                  switchParam = switchParam.replace("-1", "-");
                                }
                                QString EthAgg1Param = QString::number(ui->b1a1SB->value());
                                QString EthAgg2Param = QString::number(ui->b1a2SB->value());
                                QString AreaNoParam = QString::number(ui->areaCB->currentData().toInt());
                                QString NASIpParam = dbMan->getBrasNasIp(bras1Id);

                                text.replace("brasExchParam", brasExchParam);
                                text.replace("exchParam", exchParam);
                                text.replace("deviceParam",deviceParam);
                                text.replace("switchParam",switchParam);
                                text.replace("EthAgg1Param",EthAgg1Param);
                                text.replace("EthAgg2Param",EthAgg2Param);
                                text.replace("AreaNoParam",AreaNoParam);
                                text.replace("NASIpParam",NASIpParam);

                                text.replace("hsiVlan",hsiVlan);
                                text.replace("sinetVlan",sinetVlan);
                                text.replace("voipVlan",voipVlan);

                                bras1File.seek(0); // go to the beginning of the file
                                text = "\t\tDaNet Configuration Report\n\nReported By "  +name + "\n" +now+"\n_______________________________________\n\n" + text;
                                bras1File.write(text.toUtf8()); // write the new text back to the file

                                bras1File.close(); // close the file handle.
                                bras1Done = true;
                                doc.setText(text);

                            }
                        }


                        if(bras1Done)
                        {
                            //print pdfs
                            //print config
                            QString bras1PdfPath = reportPath+"/"+"BRAS1-Config.pdf";
                            QPrinter printer(QPrinter::PrinterResolution);
                            printer.setOutputFormat(QPrinter::PdfFormat);
                            printer.setPaperSize(QPrinter::A4);
                            printer.setOutputFileName(bras1PdfPath);
                            doc.print(&printer);

                        }
                    }

                    //######## BRAS2
                    bool bras2Done = false;
                    if(ui->bras2EnCB->isChecked())
                    {
                        int bras2Id = ui->bras2CB->currentData().toInt();
                        //check vendor
                        deviceId = dbMan->getDevexDeviceId(bras2Id);
                        aggId = ui->agg1CB->currentData().toInt();
                        aggDeviceId = dbMan->getDevexDeviceId(aggId);
                        if(dbMan->isDeviceHuaweiVendor(deviceId))
                        {
                            QString bras2Path = path+"/"+"BRAS2-Config.txt";
                            QFile bras2Source(":/configurations/HUAWEI/AccessDep/CONFIG-BRAS");
                            if(bras2Source.open(QIODevice::ReadOnly | QIODevice::Text))
                            {
                                QString text = QString(bras2Source.readAll());
                                bras2Source.close();
                                QFile bras2File(bras2Path);

                                if(bras2File.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate))
                                {

                                    QString brasExchParam = dbMan->getDevexAreaExchange(bras2Id);
                                    QString exchParam = ui->LabbrCB->currentText();
                                    QString deviceParam = dbMan->getDevice(deviceId, false);
                                    QString switchParam = dbMan->getDevice(aggDeviceId, false);
                                    if(switchParam.endsWith("-1"))
                                    {
                                      switchParam = switchParam.replace("-1", "-");
                                    }
                                    QString EthAgg1Param = QString::number(ui->b2a1SB->value());
                                    QString EthAgg2Param = QString::number(ui->b2a2SB->value());
                                    QString AreaNoParam = QString::number(ui->areaCB->currentData().toInt());
                                    QString NASIpParam = dbMan->getBrasNasIp(bras1Id);

                                    text.replace("brasExchParam", brasExchParam);
                                    text.replace("exchParam", exchParam);
                                    text.replace("deviceParam",deviceParam);
                                    text.replace("switchParam",switchParam);
                                    text.replace("EthAgg1Param",EthAgg1Param);
                                    text.replace("EthAgg2Param",EthAgg2Param);
                                    text.replace("AreaNoParam",AreaNoParam);
                                    text.replace("NASIpParam",NASIpParam);

                                    text.replace("hsiVlan",hsiVlan);
                                    text.replace("sinetVlan",sinetVlan);
                                    text.replace("voipVlan",voipVlan);

                                    bras2File.seek(0); // go to the beginning of the file
                                    text = "\t\tDaNet Configuration Report\n\nReported By "  +name + "\n" +now+"\n_______________________________________\n\n" + text;
                                    bras2File.write(text.toUtf8()); // write the new text back to the file

                                    bras2File.close(); // close the file handle.
                                    bras2Done = true;
                                    doc.setText(text);

                                }
                            }


                            if(bras2Done)
                            {
                                //print pdfs
                                //print config
                                QString bras2PdfPath = reportPath+"/"+"BRAS2-Config.pdf";
                                QPrinter printer(QPrinter::PrinterResolution);
                                printer.setOutputFormat(QPrinter::PdfFormat);
                                printer.setPaperSize(QPrinter::A4);
                                printer.setOutputFileName(bras2PdfPath);
                                doc.print(&printer);

                            }
                        }

                    }
                    else
                        bras2Done = true;
                    //##########
                    if(agg1Done && LmetroDone && RmetroDone && bras1Done && bras2Done)
                    {
                        // update subnet pool
                        int area = ui->areaCB->currentData().toInt();
                        int exchId = ui->LabbrCB->currentData().toInt();
                        bool done = true;
                        int id = ui->LNmsUplinkCB->currentData().toInt();
                        if(!dbMan->updateSubnetPool3(id,area,exchId,"Uplink",exchange+" Access Switch Local NMS Uplink"))
                            done = false;
                        id = ui->LSigUplinkCB->currentData().toInt();
                        if(!dbMan->updateSubnetPool3(id,area,exchId,"Uplink",exchange+" Access Switch Local Signaling Uplink"))
                            done = false;
                        id = ui->LMediaUplinkCB->currentData().toInt();
                        if(!dbMan->updateSubnetPool3(id,area,exchId,"Uplink",exchange+" Access Switch Local Media Uplink"))
                            done = false;
                        id = ui->LVoipUplinkCB->currentData().toInt();
                        if(!dbMan->updateSubnetPool3(id,area,exchId,"Uplink",exchange+" Access Switch Local VOIP Uplink"))
                            done = false;
                        //
                        id = ui->RNmsUplinkCB->currentData().toInt();
                        if(!dbMan->updateSubnetPool3(id,area,exchId,"Uplink",exchange+" Access Switch Remote NMS Uplink["+remoteExchange+"]"))
                            done = false;
                        id = ui->RSigUplinkCB->currentData().toInt();
                        if(!dbMan->updateSubnetPool3(id,area,exchId,"Uplink",exchange+" Access Switch Remote Signaling Uplink["+remoteExchange+"]"))
                            done = false;
                        id = ui->RMediaUplinkCB->currentData().toInt();
                        if(!dbMan->updateSubnetPool3(id,area,exchId,"Uplink",exchange+" Access Switch Remote Media Uplink["+remoteExchange+"]"))
                            done = false;
                        id = ui->RVoipUplinkCB->currentData().toInt();
                        if(!dbMan->updateSubnetPool3(id,area,exchId,"Uplink",exchange+" Access Switch Remote VOIP Uplink["+remoteExchange+"]"))
                            done = false;
                        //
                        id = ui->INmsUplinkCB->currentData().toInt();
                        if(!dbMan->updateSubnetPool3(id,area,exchId,"Uplink",exchange+" Access Inter-Switch NMS Uplink"))
                            done = false;
                        id = ui->ISigUplinkCB->currentData().toInt();
                        if(!dbMan->updateSubnetPool3(id,area,exchId,"Uplink",exchange+" Access Inter-Switch Signaling Uplink"))
                            done = false;
                        id = ui->IMediaUplinkCB->currentData().toInt();
                        if(!dbMan->updateSubnetPool3(id,area,exchId,"Uplink",exchange+" Access Inter-Switch Media Uplink"))
                            done = false;
                        id = ui->IVoipUplinkCB->currentData().toInt();
                        if(!dbMan->updateSubnetPool3(id,area,exchId,"Uplink",exchange+" Access Inter-Switch VOIP Uplink"))
                            done = false;



                        if(done)
                        {
                            //LOCAL
                            // update metro interface
                            int portId = ui->LmetroIntCB->currentData().toInt();
                            int cxId = ui->LmetroCB->currentData().toInt();
                            QString remoteInt = ui->LmetroRemoteIntLE->text();
                            QString module = ui->LmetroModCB->currentText();
                            int metroEth = ui->LmetroEthSB->value();

                            if(!dbMan->updateInterface(portId,module, ui->LabbrCB->currentText(),"Access Switch", remoteInt,exchange+"- S9306-1- "+remoteInt,metroEth, ""))
                            {
                                QMessageBox::warning(this, "ERROR", "Cannot assign local metro device interface.\n assign port manually.");
                                done = false;
                            }

                            if(!dbMan->insertEtherTrunk(cxId,metroEth,exchange+"- S9306-1- "+remoteInt))
                                QMessageBox::warning(this, "Error", "Cannot insert local metro device new ether-trunk.\n insert ether-trunk manually.");

                            //update aggregation 1
                            portId = ui->agg1IntCB->currentData().toInt();
                            int agg1Id = ui->agg1CB->currentData().toInt();
                            remoteInt = ui->agg1RemoteIntLE->text();
                            module = ui->agg1ModCB->currentText();
                            int aggEth = ui->agg1EthSB->value();

                            if(!dbMan->updateInterface(portId,module, ui->LabbrCB->currentText(),"Access Switch", remoteInt,exchange+"- S9306-1- "+remoteInt,aggEth, ""))
                            {
                                QMessageBox::warning(this, "ERROR", "Cannot assign Agg1 device interface.\n assign port manually.");
                                done = false;
                            }

                            if(!dbMan->insertEtherTrunk(agg1Id,aggEth,exchange+"- S9306-1- "+remoteInt))
                                QMessageBox::warning(this, "Error", "Cannot insert AGG1 ether-trunk.\n insert ether-trunk manually.");


                            //vlans
                            if(!dbMan->insertVlan(agg1Id,ui->hsiVlanSB->value(),exchange+" HSI Access S9306-1"))
                                QMessageBox::warning(this, "Error", "Cannot insert Agg1 device new VLAN "+hsiVlan+".\n Check if the VLAN is used or not.");

                            if(!dbMan->insertVlan(agg1Id, ui->sinetVlanSB->value(), exchange+" INTRANET Access S9306-1"))
                                QMessageBox::warning(this, "Error", "Cannot insert Agg1 device new VLAN "+sinetVlan+".\n Check if the VLAN is used or not.");

                            if(!dbMan->insertVlan(agg1Id, ui->voipVlanSB->value(), exchange+" VIOP Access S9306-1"))
                                QMessageBox::warning(this, "Error", "Cannot insert Agg1 device new VLAN "+voipVlan+".\n Check if the VLAN is used or not.");


                            //update aggregation 2
                            portId = ui->agg2IntCB->currentData().toInt();
                            int agg2Id = ui->agg2CB->currentData().toInt();
                            remoteInt = ui->agg2RemoteIntLE->text();
                            module = ui->agg2ModCB->currentText();
                            aggEth = ui->agg2EthSB->value();

                            if(!dbMan->updateInterface(portId,module, ui->LabbrCB->currentText(),"Access Switch", remoteInt,exchange+"- S9306-2- "+remoteInt,aggEth, ""))
                            {
                                QMessageBox::warning(this, "ERROR", "Cannot assign AGG2 device interface.\n assign port manually.");
                                done = false;
                            }

                            if(!dbMan->insertEtherTrunk(agg2Id,aggEth,exchange+"- S9306-2- "+remoteInt))
                                QMessageBox::warning(this, "Error", "Cannot insert AGG2 ether-trunk.\n insert ether-trunk manually.");

                            if(!dbMan->insertVlan(agg2Id,ui->hsiVlanSB->value(),exchange+" HSI Access S9306-2"))
                                QMessageBox::warning(this, "Error", "Cannot insert Agg2 device new VLAN "+hsiVlan+".\n Check if the VLAN is used or not.");

                            if(!dbMan->insertVlan(agg2Id, ui->sinetVlanSB->value(), exchange+" INTRANET Access S9306-2"))
                                QMessageBox::warning(this, "Error", "Cannot insert Agg2 device new VLAN "+sinetVlan+".\n Check if the VLAN is used or not.");

                            if(!dbMan->insertVlan(agg2Id, ui->voipVlanSB->value(), exchange+" VOIP Access S9306-2"))
                                QMessageBox::warning(this, "Error", "Cannot insert Agg2 device new VLAN "+voipVlan+".\n Check if the VLAN is used or not.");


                            //REMOTE
                            // update metro interface
                            portId = ui->RmetroIntCB->currentData().toInt();
                            cxId = ui->RmetroCB->currentData().toInt();
                            remoteInt = ui->RmetroRemoteIntLE->text();
                            module = ui->RmetroModCB->currentText();
                            metroEth = ui->RmetroEthSB->value();

                            if(!dbMan->updateInterface(portId,module, ui->LabbrCB->currentText(),"Access Switch", remoteInt,exchange+"- S9306-2- "+remoteInt,metroEth, ""))
                            {
                                QMessageBox::warning(this, "ERROR", "Cannot assign remote metro device interface.\n assign port manually.");
                                done = false;
                            }

                            if(!dbMan->insertEtherTrunk(cxId,metroEth,exchange+"- S9306-2- "+remoteInt))
                                QMessageBox::warning(this, "Error", "Cannot insert remote metro device new ether-trunk.\n insert ether-trunk manually.");

                        }

                        if(done)
                        {
                            QMessageBox::information(this, "Succesfully Done.", "Check the exported files in path:\n"+path);
                            this->close();
                        }
                    }
                    else
                        QMessageBox::warning(this,"error", "Please try again.");

                }
                else
                    QMessageBox::warning(this,"error", "Please try again.");
            }
            else
                QMessageBox::warning(this,"error", "Please try again.");
        }
    }
}

void AccessDoubleSwitchDialog::on_ipNextBtn_clicked()
{
    ui->tabWidget->setCurrentIndex(1);
}

void AccessDoubleSwitchDialog::on_metroBackBtn_clicked()
{
    ui->tabWidget->setCurrentIndex(0);
}

void AccessDoubleSwitchDialog::on_metroNextBtn_clicked()
{
    ui->tabWidget->setCurrentIndex(2);
}

void AccessDoubleSwitchDialog::on_aggBackBtn_clicked()
{
    ui->tabWidget->setCurrentIndex(1);
}

void AccessDoubleSwitchDialog::on_aggNextBtn_clicked()
{
    ui->tabWidget->setCurrentIndex(3);
}

void AccessDoubleSwitchDialog::on_brasBackBtn_clicked()
{
    ui->tabWidget->setCurrentIndex(2);
}
