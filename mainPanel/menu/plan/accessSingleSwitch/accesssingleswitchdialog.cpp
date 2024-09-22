#include "accesssingleswitchdialog.h"
#include "ui_accesssingleswitchdialog.h"
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

AccessSingleSwitchDialog::AccessSingleSwitchDialog(QWidget *parent, DanetDbMan *db) :
    QDialog(parent),
    ui(new Ui::AccessSingleSwitchDialog),
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
        ui->metroModCB->addItem(mod);
    }
    ui->agg1ModCB->setCurrentText("SX-500m");
    ui->metroModCB->setCurrentText("SX-500m");

    ui->metroEthSB->setValue(40);
    ui->agg1EthSB->setValue(94);
    ui->nmsOspfPidSB->setValue(10);
    ui->sigOspfPidSB->setValue(20);
    ui->mediaOspfPidSB->setValue(40);
    ui->voipOspfPidSB->setValue(50);

}

AccessSingleSwitchDialog::~AccessSingleSwitchDialog()
{
    delete ui;
}

void AccessSingleSwitchDialog::checkEntry()
{
    bool agg1 = !ui->agg1CB->currentText().isEmpty();
    bool cx = !ui->metroCB->currentText().isEmpty();
    bool agg1Int = !ui->agg1IntCB->currentText().isEmpty();
    bool agg1Remote = !ui->agg1RemoteIntLE->text().isEmpty();
    bool cxInt = !ui->metroIntCB->currentText().isEmpty();
    bool cxRemote = !ui->metroRemoteIntLE->text().isEmpty();
    bool agg1Mod = !ui->agg1ModCB->currentText().isEmpty();
    bool cxMod = !ui->metroModCB->currentText().isEmpty();
    bool nms = !ui->nmsUplinkCB->currentText().isEmpty();
    bool sig = !ui->sigUplinkCB->currentText().isEmpty();
    bool media = !ui->mediaUplinkCB->currentText().isEmpty();
    bool voip = !ui->voipUplinkCB->currentText().isEmpty();

    if(nms && sig && media && voip)
    {
        ui->ipNextBtn->setEnabled(true);
    }
    else
    {
        ui->ipNextBtn->setEnabled(false);
        ui->tabWidget->setCurrentIndex(0);
        return;
    }

    if(cx && cxInt && cxMod)
    {
        ui->metroNextBtn->setEnabled(true);
    }
    else
    {
        ui->metroNextBtn->setEnabled(false);
        ui->tabWidget->setCurrentIndex(1);
        return;
    }

    if(agg1 && agg1Int && agg1Mod)
    {
        ui->aggNextBtn->setEnabled(true);
    }
    else
    {
        ui->aggNextBtn->setEnabled(false);
        ui->tabWidget->setCurrentIndex(2);
        return;
    }

    bool b1 = !ui->bras1CB->currentText().isEmpty();
    bool b2 = ! (ui->bras2CB->currentText().isEmpty() && ui->bras2EnCB->isChecked() );

    int n = ui->nmsUplinkCB->currentData().toInt();
    int s = ui->sigUplinkCB->currentData().toInt();
    int m = ui->mediaUplinkCB->currentData().toInt();
    int v = ui->voipUplinkCB->currentData().toInt();

    bool e1 = (n == s || n == m || n == v)? false : true;
    bool e2 = ( s == m || s == v)? false : true;
    bool e3 = (m == v)? false: true;


    if(bras_ip_check && b1 && b2 && agg1 && cx && agg1Int && agg1Remote && cxInt && cxRemote && agg1Mod && cxMod && nms && sig && media && voip && e1 && e2 && e3)
        ui->okBtn->setEnabled(true);
    else
        ui->okBtn->setEnabled(false);

}

void AccessSingleSwitchDialog::on_areaCB_currentIndexChanged(int index)
{
    int area = index + 2;
    ui->tabWidget->setCurrentIndex(0);
    ui->ipNextBtn->setEnabled(false);

    ui->okBtn->setEnabled(false);
    ui->abbrCB->blockSignals(true);
    ui->abbrCB->clear();
    ui->agg1CB->clear();
    ui->metroCB->clear();
    ui->nmsUplinkCB->clear();
    ui->sigUplinkCB->clear();
    ui->mediaUplinkCB->clear();
    ui->voipUplinkCB->clear();

    ui->bras1CB->clear();
    ui->bras2CB->clear();
    ui->b1a1Lbl->clear();
    ui->b1a2Lbl->clear();
    ui->b2a1Lbl->clear();
    ui->b2a2Lbl->clear();

    int exchId;
    QString abbr;
    QSqlQuery *query = dbMan->selectAreaAbbreviations(area,true);
    while(query->next())
    {
        exchId = query->value(0).toInt();
        abbr = query->value(1).toString();

        ui->abbrCB->addItem(abbr, exchId);
    }
    ui->abbrCB->setCurrentIndex(-1);
    ui->abbrCB->blockSignals(false);


}

void AccessSingleSwitchDialog::on_abbrCB_currentIndexChanged(int index)
{
    index++;
    ui->okBtn->setEnabled(false);

    ui->bras1CB->clear();
    ui->bras2CB->clear();
    ui->b1a1Lbl->clear();
    ui->b1a2Lbl->clear();
    ui->b2a1Lbl->clear();
    ui->b2a2Lbl->clear();

    ui->agg1CB->clear();
    ui->metroCB->clear();
    ui->nmsUplinkCB->clear();
    ui->sigUplinkCB->clear();
    ui->mediaUplinkCB->clear();
    ui->voipUplinkCB->clear();

    int exchId = ui->abbrCB->currentData().toInt();
    ui->agg1CB->blockSignals(true);
    QSqlQuery *query = dbMan->selectExchangeAgg(exchId,true);
    int shelfId;
    QString device;

    while(query->next())
    {
        shelfId = query->value(0).toInt();
        device = query->value(1).toString();
        ui->agg1CB->addItem(device, shelfId);
    }
    ui->agg1CB->setCurrentIndex(-1);
    ui->agg1CB->blockSignals(false);
    if(ui->agg1CB->count() > 0) ui->agg1CB->setCurrentIndex(0);

    ui->metroCB->blockSignals(true);
    query = dbMan->selectExchangeCx(exchId);
    while(query->next())
    {
        shelfId = query->value(0).toInt();
        device = query->value(1).toString();
        ui->metroCB->addItem(device, shelfId);
    }
    ui->metroCB->setCurrentIndex(-1);
    ui->metroCB->blockSignals(false);
    if(ui->metroCB->count() > 0) ui->metroCB->setCurrentIndex(0);

    int area = ui->areaCB->currentData().toInt();

    query = dbMan->selectEmptySubnetPool3(area,"Uplink","Access Switch", 30);
    //`id`,`subnet`
    int id;
    QString subnet;
    while(query->next())
    {
        id = query->value(0).toInt();
        subnet = query->value(1).toString() + "/30";

        ui->nmsUplinkCB->addItem(subnet, id);
        ui->sigUplinkCB->addItem(subnet, id);
        ui->mediaUplinkCB->addItem(subnet, id);
        ui->voipUplinkCB->addItem(subnet, id);
    }
    if(query->size() > 3)
    {
        ui->nmsUplinkCB->setCurrentIndex(0);
        ui->sigUplinkCB->setCurrentIndex(1);
        ui->mediaUplinkCB->setCurrentIndex(2);
        ui->voipUplinkCB->setCurrentIndex(3);

        bras_ip_check = true;
    }
    else
        QMessageBox::warning(this,"ERROR", "You need at least 4 subnet with /30 prefix.\n Check Plan > IP Plan");


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

    ui->tabWidget->setCurrentIndex(0);
    checkEntry();
}

void AccessSingleSwitchDialog::on_agg1CB_currentIndexChanged(int index)
{
    index++;
    ui->agg1IntCB->clear();
    int agg1ShelfId = ui->agg1CB->currentData().toInt();
    QSqlQuery *query = dbMan->selectDeviveEmptyPorts(agg1ShelfId);
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

void AccessSingleSwitchDialog::on_metroCB_currentIndexChanged(int index)
{
    index++;
    ui->metroIntCB->clear();
    int cxShelfId = ui->metroCB->currentData().toInt();
    QSqlQuery *query = dbMan->selectDeviveEmptyPorts(cxShelfId);
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
        ui->metroIntCB->addItem(interface, id);
    }

}

void AccessSingleSwitchDialog::on_nmsUplinkCB_currentTextChanged(const QString &arg1)
{
    if(arg1.isEmpty())
        ui->okBtn->setEnabled(false);
    else
        checkEntry();
}

void AccessSingleSwitchDialog::on_sigUplinkCB_currentTextChanged(const QString &arg1)
{
    if(arg1.isEmpty())
        ui->okBtn->setEnabled(false);
    else
        checkEntry();
}

void AccessSingleSwitchDialog::on_mediaUplinkCB_currentTextChanged(const QString &arg1)
{
    if(arg1.isEmpty())
        ui->okBtn->setEnabled(false);
    else
        checkEntry();
}

void AccessSingleSwitchDialog::on_voipUplinkCB_currentTextChanged(const QString &arg1)
{
    if(arg1.isEmpty())
        ui->okBtn->setEnabled(false);
    else
        checkEntry();
}

void AccessSingleSwitchDialog::on_agg1IntCB_currentTextChanged(const QString &arg1)
{
    if(arg1.isEmpty())
        ui->okBtn->setEnabled(false);
    else
        checkEntry();
}

void AccessSingleSwitchDialog::on_agg1ModCB_currentTextChanged(const QString &arg1)
{
    if(arg1.isEmpty())
        ui->okBtn->setEnabled(false);
    else
        checkEntry();
}

void AccessSingleSwitchDialog::on_metroIntCB_currentTextChanged(const QString &arg1)
{
    if(arg1.isEmpty())
        ui->okBtn->setEnabled(false);
    else
        checkEntry();
}

void AccessSingleSwitchDialog::on_metroModCB_currentTextChanged(const QString &arg1)
{
    if(arg1.isEmpty())
        ui->okBtn->setEnabled(false);
    else
        checkEntry();
}

void AccessSingleSwitchDialog::on_metroRemoteIntLE_textChanged(const QString &arg1)
{
    if(arg1.isEmpty())
        ui->okBtn->setEnabled(false);
    else
        checkEntry();
}

void AccessSingleSwitchDialog::on_agg1RemoteIntLE_textChanged(const QString &arg1)
{
    if(arg1.isEmpty())
        ui->okBtn->setEnabled(false);
    else
        checkEntry();
}

void AccessSingleSwitchDialog::on_okBtn_clicked()
{
    int aggId = ui->agg1CB->currentData().toInt();
    int metroId = ui->metroCB->currentData().toInt();
    int aggEth = ui->agg1EthSB->value();
    int metroEth = ui->metroEthSB->value();
    bool e1 = dbMan->isEtherTrunkUsed(aggId, aggEth);
    bool e2 = dbMan->isEtherTrunkUsed(metroId, metroEth);
    QString hsiVlan = QString::number(ui->hsiVlanSB->value());
    QString sinetVlan = QString::number(ui->sinetVlanSB->value());
    QString voipVlan = QString::number(ui->voipVlanSB->value());

    bool v5 = dbMan->vlanAvailability(aggId, ui->hsiVlanSB->value());
    bool v7 = dbMan->vlanAvailability(aggId,ui->sinetVlanSB->value());
    bool v8 = dbMan->vlanAvailability(aggId, ui->voipVlanSB->value());

    if(e1 || e2 || v5 || v7 || v8)
    {
        if(e1)
            QMessageBox::warning(this, "ERROR", "Ether-Trunk "+QString::number(aggEth)+" in AGG1 is used.");

        if(e2)
            QMessageBox::warning(this, "ERROR", "Ether-Trunk "+QString::number(metroEth)+" in metro device is used.");

        if(v5)
            QMessageBox::warning(this, "ERROR",QString("VLAN ")+hsiVlan+QString(" in AGG1 is used."));

        if(v7)
            QMessageBox::warning(this, "ERROR", QString("VLAN ")+sinetVlan+QString(" in AGG1 is used."));

        if(v8)
            QMessageBox::warning(this, "ERROR", QString("VLAN ")+voipVlan+QString(" in AGG1 is used."));



        return;
    }
    else
    {
        QString now = QDateTime::currentDateTime().toString("yyyy/MM/dd  hh:mm");
        QString name = dbMan->getLoggedinLastname();

        QString exchange = QString::number(ui->areaCB->currentData().toInt()) +"-"+ui->abbrCB->currentText();
        QString str = exchange +"-Access Switch Plan";
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
                reportPath = path + "/"+"Data Plan-"+ exchange +"-Access Switch";
                if(QDir().mkdir(reportPath))
                {
                    QTextEdit doc;
                    //######## agg
                    bool agg1Done = false;
                    aggId = ui->agg1CB->currentData().toInt();
                    //check if it is huawei device or not
                    int deviceId = dbMan->getDevexDeviceId(aggId);
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

                                QString exchParam = ui->abbrCB->currentText();
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

                    //######## Metro
                    bool metroDone = false;
                    metroId = ui->metroCB->currentData().toInt();
                    //check metro device vendor
                    deviceId = dbMan->getDevexDeviceId(metroId);
                    if(dbMan->isDeviceHuaweiVendor(deviceId))
                    {
                        QString cxPath = path+"/"+exchange+"-CX600-Config.txt";
                        QFile cxSource(":/configurations/HUAWEI/AccessDep/CONFIG-CX600");
                        if(cxSource.open(QIODevice::ReadOnly | QIODevice::Text))
                        {
                            QString text = QString(cxSource.readAll());
                            cxSource.close();
                            QFile cx(cxPath);

                            if(cx.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate))
                            {

                                QString exchangeParam = ui->abbrCB->currentText();
                                QString switchParam = dbMan->getDevice(deviceId, false);
                                QString interfaceParam = ui->metroIntCB->currentText();
                                interfaceParam.remove("10G",Qt::CaseInsensitive);
                                interfaceParam.remove("1G",Qt::CaseInsensitive);
                                interfaceParam.remove("-",Qt::CaseInsensitive);
                                QString gigabitEthernetParam;
                                if(interfaceParam.contains("10G",Qt::CaseInsensitive))
                                    gigabitEthernetParam = "XGigabitEthernet";
                                else
                                    gigabitEthernetParam = "GigabitEthernet";

                                QString remoteInterfaceParam = ui->metroRemoteIntLE->text();
                                QString ethParam = QString::number(ui->metroEthSB->value());
                                QString nmsOspfIdParam = QString::number(ui->nmsOspfPidSB->value());
                                QString sigOspfIdParam = QString::number(ui->sigOspfPidSB->value());
                                QString mediaOspfIdParam = QString::number(ui->mediaOspfPidSB->value());
                                QString voipOspfIdParam = QString::number(ui->voipOspfPidSB->value());
                                QString nmsNetParam = ui->nmsUplinkCB->currentText().remove("/30");
                                QString sigNetParam = ui->sigUplinkCB->currentText().remove("/30");
                                QString mediaNetParam = ui->mediaUplinkCB->currentText().remove("/30");
                                QString voipNetParam = ui->voipUplinkCB->currentText().remove("/30");
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


                                cx.seek(0); // go to the beginning of the file
                                text = "\t\tDaNet Configuration Report\n\nReported By "  +name + "\n" +now+"\n_______________________________________\n\n" + text;
                                cx.write(text.toUtf8()); // write the new text back to the file

                                cx.close(); // close the file handle.
                                metroDone = true;
                                doc.setText(text);

                            }
                        }

                        if(metroDone)
                        {
                            //print pdfs
                            //print config
                            QString cxPdfPath = reportPath+"/"+exchange+"-CX600-Config.pdf";
                            QPrinter printer(QPrinter::PrinterResolution);
                            printer.setOutputFormat(QPrinter::PdfFormat);
                            printer.setPaperSize(QPrinter::A4);
                            printer.setOutputFileName(cxPdfPath);
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
                                QString exchParam = ui->abbrCB->currentText();
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
                                    QString exchParam = ui->abbrCB->currentText();
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
                    if(agg1Done && metroDone && bras1Done && bras2Done)
                    {
                        // update subnet pool
                        int exchId = ui->abbrCB->currentData().toInt();
                        int area = ui->areaCB->currentData().toInt();

                        bool done = true;
                        int id = ui->nmsUplinkCB->currentData().toInt();
                        if(!dbMan->updateSubnetPool3(id,area,exchId,"Uplink",exchange+" Access Switch Local NMS Uplink"))
                            done = false;
                        id = ui->sigUplinkCB->currentData().toInt();
                        if(!dbMan->updateSubnetPool3(id,area,exchId,"Uplink",exchange+" Access Switch Local Signaling Uplink"))
                            done = false;
                        id = ui->mediaUplinkCB->currentData().toInt();
                        if(!dbMan->updateSubnetPool3(id,area,exchId,"Uplink",exchange+" Access Switch Local Media Uplink"))
                            done = false;
                        id = ui->voipUplinkCB->currentData().toInt();
                        if(!dbMan->updateSubnetPool3(id,area,exchId,"Uplink",exchange+" Access Switch Local VOIP Uplink"))
                            done = false;


                        if(done)
                        {
                            // update metro interface
                            int portId = ui->metroIntCB->currentData().toInt();
                            int cxId = ui->metroCB->currentData().toInt();
                            QString remoteInt = ui->metroRemoteIntLE->text();
                            QString module = ui->metroModCB->currentText();
                            int metroEth = ui->metroEthSB->value();

                            if(!dbMan->updateInterface(portId,module, ui->abbrCB->currentText(),"Access Switch", remoteInt,exchange+"- S9306-1- "+remoteInt,metroEth, ""))
                            {
                                QMessageBox::warning(this, "ERROR", "Cannot assign metro device interface.\n assign port manually.");
                                done = false;
                            }

                            if(!dbMan->insertEtherTrunk(cxId,metroEth,exchange+"- S9306-1- "+remoteInt))
                                QMessageBox::warning(this, "Error", "Cannot insert metro device new ether-trunk.\n insert ether-trunk manually.");


                            //update aggregation 1
                            portId = ui->agg1IntCB->currentData().toInt();
                            int agg1Id = ui->agg1CB->currentData().toInt();
                            remoteInt = ui->agg1RemoteIntLE->text();
                            module = ui->agg1ModCB->currentText();
                            int aggEth = ui->agg1EthSB->value();

                            if(!dbMan->updateInterface(portId,module, ui->abbrCB->currentText(),"Access Switch", remoteInt,exchange+"- S9306-1- "+remoteInt,aggEth, ""))
                            {
                                QMessageBox::warning(this, "ERROR", "Cannot assign metro device interface.\n assign port manually.");
                                done = false;
                            }

                            if(!dbMan->insertEtherTrunk(agg1Id,aggEth,exchange+"- S9306-1- "+remoteInt))
                                QMessageBox::warning(this, "Error", "Cannot insert Agg device new ether-trunk.\n insert ether-trunk manually.");


                            if(!dbMan->insertVlan(agg1Id,ui->hsiVlanSB->value(),exchange+" HSI Access S9306-1"))
                                QMessageBox::warning(this, "Error", "Cannot insert Agg device new VLAN "+hsiVlan+".\n Check if the VLAN is used or not.");

                            if(!dbMan->insertVlan(agg1Id, ui->sinetVlanSB->value(), exchange+" SINET Access S9306-1"))
                                QMessageBox::warning(this, "Error", "Cannot insert Agg device new VLAN "+sinetVlan+".\n Check if the VLAN is used or not.");

                            if(!dbMan->insertVlan(agg1Id, ui->voipVlanSB->value(), exchange+" VOIP Access S9306-1"))
                                QMessageBox::warning(this, "Error", "Cannot insert Agg device new VLAN "+voipVlan+".\n Check if the VLAN is used or not.");

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

void AccessSingleSwitchDialog::on_bras2EnCB_toggled(bool checked)
{
    if(checked)
        ui->bras2GB->show();
    else
        ui->bras2GB->hide();
    checkEntry();
}

void AccessSingleSwitchDialog::on_bras1CB_currentIndexChanged(int index)
{
    index++;
    int brasId = ui->bras1CB->currentData().toInt();
    int exchId = ui->abbrCB->currentData().toInt();
    QList<int> list = dbMan->getBrasAggIdEth(brasId, exchId);
    //`agg1_shelf_id`,`agg1_eth`,`agg2_shelf_id`,`agg2_eth`
    if(list.at(0) > -1)
    {
        ui->b1a1SB->setValue(list.at(1));
        ui->b1a2SB->setValue(list.at(3));

        QString lbl = "Eth To "+ ui->abbrCB->currentText() +" Agg ";
        ui->b1a1Lbl->setText(lbl+"1 :");
        ui->b1a2Lbl->setText(lbl+"2 :");

    }
    else
    {
        ui->b1a1SB->setValue(-1);
        ui->b1a2SB->setValue(-1);
    }

}

void AccessSingleSwitchDialog::on_bras2CB_currentIndexChanged(int index)
{
    index++;
    int brasId = ui->bras2CB->currentData().toInt();
    int exchId = ui->abbrCB->currentData().toInt();
    QList<int> list = dbMan->getBrasAggIdEth(brasId, exchId);
    //`agg1_shelf_id`,`agg1_eth`,`agg2_shelf_id`,`agg2_eth`
    if(list.at(0) > -1)
    {
        ui->b2a1SB->setValue(list.at(1));
        ui->b2a2SB->setValue(list.at(3));

        QString lbl = "Eth To "+ ui->abbrCB->currentText() +" Agg ";
        ui->b2a1Lbl->setText(lbl+"1 :");
        ui->b2a2Lbl->setText(lbl+"2 :");

    }
    else
    {
        ui->b2a1SB->setValue(-1);
        ui->b2a2SB->setValue(-1);
    }
}

void AccessSingleSwitchDialog::on_ipNextBtn_clicked()
{
    ui->tabWidget->setCurrentIndex(1);
}

void AccessSingleSwitchDialog::on_metroBackBtn_clicked()
{
    ui->tabWidget->setCurrentIndex(0);
}

void AccessSingleSwitchDialog::on_metroNextBtn_clicked()
{
    ui->tabWidget->setCurrentIndex(2);
}

void AccessSingleSwitchDialog::on_aggBackBtn_clicked()
{
    ui->tabWidget->setCurrentIndex(1);
}

void AccessSingleSwitchDialog::on_aggNextBtn_clicked()
{
    ui->tabWidget->setCurrentIndex(3);
}

void AccessSingleSwitchDialog::on_brasBackBtn_clicked()
{
    ui->tabWidget->setCurrentIndex(2);
}
