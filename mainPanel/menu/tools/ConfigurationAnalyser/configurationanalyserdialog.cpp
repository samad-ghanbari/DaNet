#include "configurationanalyserdialog.h"
#include "ui_configurationanalyserdialog.h"
#include <QFileDialog>
#include <QTextStream>
#include "lib/mapmodel.h"
#include <QMessageBox>
#include <QStringListModel>
#include <QStandardItemModel>
#include "lib/report/general/Report.h"
#include <QDesktopServices>

ConfigurationAnalyserDialog::ConfigurationAnalyserDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConfigurationAnalyserDialog),
    vlanModel(nullptr),
    etherModel(nullptr),
    vpnModel(new QStandardItemModel(this)),
    interfaceModel(new QStandardItemModel(this)),
    StaticModel(new QStandardItemModel(this)),
    VpnStaticModel(new QStandardItemModel(this)),
    expModel(new QStringListModel(this)),
    impModel(new QStringListModel(this))
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowMinMaxButtonsHint);
    //os
    ui->osCB->addItem("HUAWEI OS", 0);
    ui->osCB->addItem("CISCO IOS", 1);
    ui->osCB->addItem("CISCO IOS-XR", 2);

    ui->etherTV->horizontalHeader()->setStretchLastSection(true);
    ui->etherTV->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->etherTV->verticalHeader()->hide();
    ui->etherTV->setModel(etherModel);

    ui->vlanTV->horizontalHeader()->setStretchLastSection(true);
    ui->vlanTV->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->vlanTV->verticalHeader()->hide();
    ui->vlanTV->setModel(vlanModel);

    ui->vpnTV->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->vpnTV->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->vpnTV->verticalHeader()->hide();
    ui->vpnTV->setModel(vpnModel);

    ui->expLV->setModel(expModel);
    ui->impLV->setModel(impModel);

    ui->interfaceTV->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->interfaceTV->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->interfaceTV->verticalHeader()->hide();
    ui->interfaceTV->setModel(interfaceModel);
    ui->srTV->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->srTV->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->srTV->verticalHeader()->hide();
    ui->srTV->setModel(StaticModel);
    ui->vsrTV->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->vsrTV->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->vsrTV->verticalHeader()->hide();
    ui->vsrTV->setModel(VpnStaticModel);

}

ConfigurationAnalyserDialog::~ConfigurationAnalyserDialog()
{
    delete ui;
    delete vlanModel;
    vlanModel = NULL;
    delete etherModel;
    etherModel = NULL;
    delete vpnModel;
    vpnModel = NULL;
    delete interfaceModel;
    interfaceModel = NULL;
    delete expModel;
    expModel = NULL;
    delete impModel;
    impModel = NULL;
}

void ConfigurationAnalyserDialog::emptyAll()
{
    ui->fileLbl->clear();
    ui->sysNameLbl->clear();

    delete vlanModel;
    vlanModel = NULL;
    delete etherModel;
    etherModel = NULL;
    vpnModel->clear();
    interfaceModel->clear();
    StaticModel->clear();
    VpnStaticModel->clear();
    expModel->setStringList(QStringList());
    impModel->setStringList(QStringList());
    vlanDescMap.clear();
    etherDescMap.clear();
    expMap.clear();
    impMap.clear();
}

void ConfigurationAnalyserDialog::huaweiConfigureAnalyser()
{
    vlanModel = new MapModel(this,QStringList()<<"VLAN"<<"Description");
    etherModel = new MapModel(this,QStringList()<<"Ether-Trunk"<<"Description");
    vpnModel->setColumnCount(4);
    //row,vpn,rd, desc
    vpnModel->setHeaderData(1, Qt::Horizontal,"VPN");
    vpnModel->setHeaderData(2, Qt::Horizontal,"Route Distinguisher");
    vpnModel->setHeaderData(3, Qt::Horizontal,"Description");
    ui->vpnTV->hideColumn(0);

    interfaceModel->setColumnCount(4);
    interfaceModel->setRowCount(0);
    //interface,description,ether,bind
    interfaceModel->setHeaderData(0, Qt::Horizontal,"Interface");
    interfaceModel->setHeaderData(1, Qt::Horizontal,"Description");
    interfaceModel->setHeaderData(2, Qt::Horizontal,"Ether-Trunk");
    interfaceModel->setHeaderData(3, Qt::Horizontal,"Bind");

    StaticModel->setColumnCount(3);
    StaticModel->setRowCount(0);
    StaticModel->setHeaderData(0, Qt::Horizontal,"Network");
    StaticModel->setHeaderData(1, Qt::Horizontal,"Mask");
    StaticModel->setHeaderData(2, Qt::Horizontal,"Next Hop");

    VpnStaticModel->setColumnCount(4);
    VpnStaticModel->setRowCount(0);
    VpnStaticModel->setHeaderData(0, Qt::Horizontal,"VPN-Instance");
    VpnStaticModel->setHeaderData(1, Qt::Horizontal,"Network");
    VpnStaticModel->setHeaderData(2, Qt::Horizontal,"Mask");
    VpnStaticModel->setHeaderData(3, Qt::Horizontal,"Next Hop");


    QModelIndex index;
    int vpnRow = 0, interfaceRow = 0, static1Row = 0, static2Row = 0;


    QTextStream configStream(&configFile);

    QString line, sysname,desc, vpn,net,mask,hop, rd, exp, imp, interface, bind, ethString;
    QStringList list;
    int ind, vlan,ether;
    bool intOK;

    if(configFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        line = configStream.readLine().trimmed();
        bool SYSNAME = true;
        while(!line.isNull())
        {
            //purify remove ---- More ----  and space
            if(line.contains("---- More ----",Qt::CaseInsensitive))
                line = line.remove("---- More ----",Qt::CaseInsensitive);
            line = line.trimmed();

            if(SYSNAME)
            {
                if(line.startsWith("sysname", Qt::CaseInsensitive))
                {
                    line = line.remove("sysname", Qt::CaseInsensitive).trimmed();
                    sysname = line;
                    ui->sysNameLbl->setText(sysname);
                    SYSNAME = false;
                }
            }

            //check vlan desc ...
            if(line.startsWith("vlan", Qt::CaseInsensitive) && !line.contains("batch", Qt::CaseInsensitive))
            {
                line = line.remove("vlan", Qt::CaseInsensitive).trimmed();
                vlan = line.toInt(&intOK);
                line = configStream.readLine().trimmed();
                if(intOK == false)
                {
                    continue;
                }

                //purify remove ---- More ----  and space
                if(line.contains("---- More ----",Qt::CaseInsensitive))
                    line = line.remove("---- More ----",Qt::CaseInsensitive).trimmed();

                while( !(line.startsWith("#") || line.startsWith("interface", Qt::CaseInsensitive) || line.isNull() ))
                {
                    if(line.contains("description"))
                    {
                        ind = line.indexOf("description",0,Qt::CaseInsensitive);
                        if(ind > 0)
                            line = line.remove(0,ind-1).trimmed();
                        line = line.remove("description",Qt::CaseInsensitive);
                        desc = line;
                        if(!vlanDescMap.contains(vlan))
                            vlanDescMap.insert(vlan,desc);
                        else if(!desc.isEmpty())
                            vlanDescMap.insert(vlan,desc);

                        break;
                    }

                    line = configStream.readLine().trimmed();
                    //purify remove ---- More ----  and space
                    if(line.contains("---- More ----",Qt::CaseInsensitive))
                        line = line.remove("---- More ----",Qt::CaseInsensitive).trimmed();
                }

                continue;
            }
            //check vlan batch 10 11 4020 to 4030 > put to array by splitting based on space and ...
            else if(line.startsWith("vlan batch", Qt::CaseInsensitive))
            {
                line = line.remove("vlan batch", Qt::CaseInsensitive).trimmed();
                QStringList list = line.split(" ",QString::SkipEmptyParts,Qt::CaseInsensitive);
                int i,val,nextVal;
                QString Str;
                i = 0;
                while(i < list.length())
                {
                    if(i < (list.length()-1))
                    {
                        Str = list[i+1];
                        if(Str.compare("to", Qt::CaseInsensitive) == 0)
                        { // 10 to 20
                            Str = list[i];
                            val = Str.toInt(&intOK);
                            if(intOK == false)
                            {
                                i++;
                                continue;
                            }

                            Str = list[i+1];
                            nextVal = Str.toInt(&intOK);
                            if(intOK == false)
                            {
                                i++;
                                continue;
                            }

                            for(int j = val; j <= nextVal; j++)
                            {
                                desc = "VLAN-"+QString::number(j);
                                vlanDescMap.insert(j, desc);
                            }
                            i += 2;
                        }else
                        {
                            Str = list[i];
                            vlan = Str.toInt(&intOK);
                            if(intOK == false)
                            {
                                i++;
                                continue;
                            }else
                            {
                                desc = "VLAN-"+Str;
                                vlanDescMap.insert(vlan, desc);
                            }
                        }

                    }
                    else
                    {
                        Str = list[i];
                        vlan = Str.toInt(&intOK);
                        if(intOK == false)
                        {
                            i++;
                            continue;
                        }else
                        {
                            desc = "VLAN-"+Str;
                            vlanDescMap.insert(vlan, desc);
                        }
                    }

                    i++;
                }
            }

            //interfaces
            else if(line.startsWith("interface", Qt::CaseInsensitive))
            {
                line = line.remove("interface",Qt::CaseInsensitive).trimmed();
                interface = line;

                desc = "";
                ethString = "";
                bind = "";
                ether = -1;

                if(line.startsWith("eth-trunk", Qt::CaseInsensitive) && !line.contains("."))
                {
                    ind = line.indexOf("eth-trunk",0,Qt::CaseInsensitive);
                    if(ind > 0)
                        line = line.remove(0,ind-1).trimmed();

                    line = line.remove("eth-trunk",Qt::CaseInsensitive);

                    ether = line.toInt(&intOK);

                    if(!intOK)
                    {
                        ether = -1;
                    }
                }

                line = configStream.readLine().trimmed();
                //purify remove ---- More ----  and space
                if(line.contains("---- More ----",Qt::CaseInsensitive))
                    line = line.remove("---- More ----",Qt::CaseInsensitive);
                line = line.trimmed();


                while( !(line.startsWith("#") || line.isNull() ) )
                {

                    if(line.contains("description"))
                    {
                        line = line.remove("description",Qt::CaseInsensitive).trimmed();
                        desc = line;
                    }
                    else if(line.contains("eth-trunk"))
                    {
                        line = line.remove("eth-trunk",Qt::CaseInsensitive).trimmed();
                        ethString = line;
                    }
                    else if(line.contains("ip binding vpn-instance"))
                    {
                        line = line.remove("ip binding vpn-instance",Qt::CaseInsensitive).trimmed();
                        bind = line;
                    }
                    else if(line.contains("l2 binding"))
                    {
                        line = line.remove("l2 binding",Qt::CaseInsensitive).trimmed();
                        bind = line;
                    }


                    line = configStream.readLine().trimmed();
                    //purify remove ---- More ----  and space
                    if(line.contains("---- More ----",Qt::CaseInsensitive))
                        line = line.remove("---- More ----",Qt::CaseInsensitive);
                    line = line.trimmed();
                }

                if(ether > -1)
                {
                    if(!etherDescMap.contains(ether))
                        etherDescMap.insert(ether, desc);
                    else if(!desc.isEmpty())
                        etherDescMap.insert(ether, desc);
                }

                if(!modelContains(interfaceModel, interface) || !desc.isEmpty())
                {
                    interfaceModel->insertRow(interfaceRow);

                    index = interfaceModel->index(interfaceRow, 0);
                    interfaceModel->setData(index,interface);

                    index = interfaceModel->index(interfaceRow, 1);
                    interfaceModel->setData(index,desc);

                    index = interfaceModel->index(interfaceRow, 2);
                    interfaceModel->setData(index,ethString);

                    index = interfaceModel->index(interfaceRow, 3);
                    interfaceModel->setData(index,bind);

                    interfaceRow++;
                    continue;
                }


            }
            else if(line.startsWith("ip route-static", Qt::CaseInsensitive) && !line.contains("vpn-instance", Qt::CaseInsensitive))
            {
                line = line.remove("ip route-static",Qt::CaseInsensitive).trimmed();
                net = line;
                list = net.split(" ",QString::SkipEmptyParts);
                if(list.count() == 3)
                {
                    net = list.at(0);
                    mask = list.at(1);
                    hop = list.at(2);

                    StaticModel->insertRow(static1Row);
                    index = StaticModel->index(static1Row, 0);
                    StaticModel->setData(index,net);
                    index = index.sibling(static1Row, 1);
                    StaticModel->setData(index,mask);
                    index = index.sibling(static1Row, 2);
                    StaticModel->setData(index,hop);

                    static1Row++;
                }
            }
            else if(line.startsWith("ip route-static vpn-instance", Qt::CaseInsensitive))
            {
                line = line.remove("ip route-static vpn-instance",Qt::CaseInsensitive).trimmed();
                net = line;
                list = net.split(" ",QString::SkipEmptyParts);
                if(list.count() == 4)
                {
                    vpn = list.at(0);
                    net = list.at(1);
                    mask = list.at(2);
                    hop = list.at(3);

                    VpnStaticModel->insertRow(static2Row);
                    index = VpnStaticModel->index(static2Row, 0);
                    VpnStaticModel->setData(index,vpn);
                    index = index.sibling(static2Row, 1);
                    VpnStaticModel->setData(index,net);
                    index = index.sibling(static2Row, 2);
                    VpnStaticModel->setData(index,mask);
                    index = index.sibling(static2Row, 3);
                    VpnStaticModel->setData(index,hop);

                    static2Row++;
                }
            }
            // check VPN
            else if(line.startsWith("ip vpn-instance", Qt::CaseInsensitive))
            {
                ind = line.indexOf("ip vpn-instance",0,Qt::CaseInsensitive);
                if(ind > 0)
                    line = line.remove(0,ind-1).trimmed();
                line = line.remove("ip vpn-instance",Qt::CaseInsensitive).trimmed();
                vpn = line;
                line = configStream.readLine().trimmed();
                //purify remove ---- More ----  and space
                if(line.contains("---- More ----",Qt::CaseInsensitive))
                    line = line.remove("---- More ----",Qt::CaseInsensitive);
                line = line.trimmed();

                desc = "";
                rd = "";
                exp = "";
                imp = "";

                while( !(line.startsWith("#") || line.startsWith("ip vpn-instance", Qt::CaseInsensitive) || line.isNull() ) )
                {

                    if(line.contains("description"))
                    {
                        line = line.remove("description",Qt::CaseInsensitive).trimmed();
                        desc = line;
                    }
                    else if(line.contains("route-distinguisher"))
                    {
                        line = line.remove("route-distinguisher",Qt::CaseInsensitive).trimmed();
                        rd = line;
                    }
                    else if(line.contains("vpn-target"))
                    {
                        line = line.remove("vpn-target",Qt::CaseInsensitive).trimmed();

                        if(line.contains("export-extcommunity"))
                        {
                            line = line.remove("export-extcommunity",Qt::CaseInsensitive).trimmed();
                            exp = line;
                        }
                        else if(line.contains("import-extcommunity"))
                        {
                            line = line.remove("import-extcommunity",Qt::CaseInsensitive).trimmed();
                            imp = line;
                        }
                    }



                    line = configStream.readLine().trimmed();
                    //purify remove ---- More ----  and space
                    if(line.contains("---- More ----",Qt::CaseInsensitive))
                        line = line.remove("---- More ----",Qt::CaseInsensitive);
                    line = line.trimmed();
                }

                vpnModel->insertRow(vpnRow);

                index = vpnModel->index(vpnRow, 0); //row
                vpnModel->setData(index,vpnRow);

                index = vpnModel->index(vpnRow, 1); //vpn
                vpnModel->setData(index,vpn);

                index = vpnModel->index(vpnRow, 2); //rd
                vpnModel->setData(index,rd);

                index = vpnModel->index(vpnRow, 3); //desc
                vpnModel->setData(index,desc);

                expMap.insert(vpnRow, exp);
                impMap.insert(vpnRow, imp);

                vpnRow++;

                continue;
            }
            line = configStream.readLine().trimmed();
        }

        vlanModel->setMap(&vlanDescMap);
        ui->vlanTV->setModel(vlanModel);
        etherModel->setMap(&etherDescMap);
        ui->etherTV->setModel(etherModel);
    }

    configFile.close();

}

void ConfigurationAnalyserDialog::IOSConfigureAnalyser()
{
    vlanModel = new MapModel(this,QStringList()<<"VLAN"<<"Description");
    etherModel = new MapModel(this,QStringList()<<"Port Channel"<<"Description");
    vpnModel->setColumnCount(4);
    //row,vpn,rd, desc
    vpnModel->setHeaderData(1, Qt::Horizontal,"VRF");
    vpnModel->setHeaderData(2, Qt::Horizontal,"Route Distinguisher");
    vpnModel->setHeaderData(3, Qt::Horizontal,"Description");
    ui->vpnTV->hideColumn(0);

    interfaceModel->setColumnCount(4);
    //interface,description,ether,bind
    interfaceModel->setHeaderData(0, Qt::Horizontal,"Interface");
    interfaceModel->setHeaderData(1, Qt::Horizontal,"Description");
    interfaceModel->setHeaderData(2, Qt::Horizontal,"Port Channel");
    interfaceModel->setHeaderData(3, Qt::Horizontal,"VRF Forwarding");


    StaticModel->setColumnCount(3);
    StaticModel->setRowCount(0);
    StaticModel->setHeaderData(0, Qt::Horizontal,"Network");
    StaticModel->setHeaderData(1, Qt::Horizontal,"Mask/Prefix");
    StaticModel->setHeaderData(2, Qt::Horizontal,"Next Hop");

    VpnStaticModel->setColumnCount(4);
    VpnStaticModel->setRowCount(0);
    VpnStaticModel->setHeaderData(0, Qt::Horizontal,"VRF");
    VpnStaticModel->setHeaderData(1, Qt::Horizontal,"Network");
    VpnStaticModel->setHeaderData(2, Qt::Horizontal,"Mask/Prefix");
    VpnStaticModel->setHeaderData(3, Qt::Horizontal,"Next Hop");

    QModelIndex index;
    int vpnRow = 0, interfaceRow = 0, static1Row = 0, static2Row = 0;


    QTextStream configStream(&configFile);

    QString line,sysname,desc, vpn,net,mask,hop, rd, exp, imp, interface, bind, ethString;
    QStringList list;
    int ind, vlan,ether;
    bool intOK;

    if(configFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        line = configStream.readLine().trimmed();
        bool SYSNAME = true;
        while(!line.isNull())
        {
            //purify remove --More--  and space
            if(line.contains("--More--",Qt::CaseInsensitive))
                line = line.remove("--More--",Qt::CaseInsensitive);
            line = line.trimmed();

            if(SYSNAME)
            {
                if(line.startsWith("hostname", Qt::CaseInsensitive))
                {
                    line = line.remove("hostname", Qt::CaseInsensitive).trimmed();
                    sysname = line;
                    ui->sysNameLbl->setText(sysname);
                    SYSNAME = false;
                }
            }

            //check vlan desc ...
            if(line.startsWith("vlan", Qt::CaseInsensitive) && !line.contains(",", Qt::CaseInsensitive))// vlan 10
            {
                line = line.remove("vlan", Qt::CaseInsensitive).trimmed();
                vlan = line.toInt(&intOK);
                line = configStream.readLine().trimmed();
                if(intOK == false)
                {
                    continue;
                }

                //purify remove ---- More ----  and space
                if(line.contains("--More--",Qt::CaseInsensitive))
                    line = line.remove("--More--",Qt::CaseInsensitive).trimmed();

                while( !(line.startsWith("!") || line.startsWith("interface", Qt::CaseInsensitive) || line.startsWith("vlan", Qt::CaseInsensitive) || line.isNull() ))
                {
                    if(line.contains("name"))
                    {
                        ind = line.indexOf("name",0,Qt::CaseInsensitive);
                        if(ind > 0)
                            line = line.remove(0,ind-1).trimmed();
                        line = line.remove("name",Qt::CaseInsensitive);
                        desc = line;

                        if(!vlanDescMap.contains(vlan))
                            vlanDescMap.insert(vlan,desc);
                        else if(!desc.isEmpty())
                            vlanDescMap.insert(vlan,desc);

                        break;
                    }

                    line = configStream.readLine().trimmed();
                    //purify remove ---- More ----  and space
                    if(line.contains("--More--",Qt::CaseInsensitive))
                        line = line.remove("--More--",Qt::CaseInsensitive).trimmed();
                }

                continue;
            }
            //check vlan 10, 11, 4020-4030 > put to array by splitting based on , and ...
            else if( (line.startsWith("vlan", Qt::CaseInsensitive)) && (line.contains(",", Qt::CaseInsensitive)) )
            {
                line = line.remove("vlan", Qt::CaseInsensitive).trimmed();
                QStringList temp;
                QStringList list = line.split(",",QString::SkipEmptyParts,Qt::CaseInsensitive);
                int i,val,nextVal;
                QString Str;
                i = 0;
                while(i < list.length())
                {
                    if(i < (list.length()-1))
                    {
                        Str = list[i];
                        if(Str.contains("-", Qt::CaseInsensitive))
                        { // 10 to 20
                            temp = Str.split("-", QString::SkipEmptyParts, Qt::CaseInsensitive);
                            if(temp.length() != 2)
                                continue;

                            Str = temp[0];
                            val = Str.toInt(&intOK);//start
                            if(intOK == false)
                            {
                                i++;
                                continue;
                            }

                            Str = temp[1];
                            nextVal = Str.toInt(&intOK);
                            if(intOK == false)
                            {
                                i++;
                                continue;
                            }

                            for(int j = val; j <= nextVal; j++)
                            {
                                desc = "VLAN-"+QString::number(j);
                                vlanDescMap.insert(j, desc);
                            }
                            i++;
                        }else
                        {
                            Str = list[i];
                            vlan = Str.toInt(&intOK);
                            if(intOK == false)
                            {
                                i++;
                                continue;
                            }else
                            {
                                desc = "VLAN-"+Str;
                                vlanDescMap.insert(vlan, desc);
                            }
                        }

                    }
                    else
                    {
                        Str = list[i];
                        vlan = Str.toInt(&intOK);
                        if(intOK == false)
                        {
                            i++;
                            continue;
                        }else
                        {
                            desc = "VLAN-"+Str;
                            vlanDescMap.insert(vlan, desc);
                        }
                    }

                    i++;
                }
            }
            //interfaces
            else if(line.startsWith("interface", Qt::CaseInsensitive))
            {
                line = line.remove("interface",Qt::CaseInsensitive).trimmed();
                interface = line;

                desc = "";
                ethString = "";
                bind = "";
                ether = -1;

                if(line.startsWith("Port-channel", Qt::CaseInsensitive) && !line.contains("."))
                {

                    line = line.remove("Port-channel",Qt::CaseInsensitive);

                    ether = line.toInt(&intOK);

                    if(!intOK)
                    {
                        ether = -1;
                    }
                }


                line = configStream.readLine().trimmed();
                //purify remove ---- More ----  and space
                if(line.contains("--More--",Qt::CaseInsensitive))
                    line = line.remove("--More--",Qt::CaseInsensitive);
                line = line.trimmed();

                while( !(line.startsWith("!") || line.isNull() ) )
                {

                    if(line.contains("description"))
                    {
                        line = line.remove("description",Qt::CaseInsensitive).trimmed();
                        desc = line;
                    }
                    else if(line.contains("channel-group"))
                    {
                        line = line.remove("channel-group",Qt::CaseInsensitive).trimmed();
                        if(line.contains("mode", Qt::CaseInsensitive))
                        {
                            ind = line.indexOf("mode",0,Qt::CaseInsensitive);
                            line = line.remove(ind-1, line.length()-1);
                        }
                        ethString = line;
                    }
                    else if(line.contains("ip vrf forwarding"))
                    {
                        line = line.remove("ip vrf forwarding",Qt::CaseInsensitive).trimmed();
                        bind = line;
                    }
                    else if(line.contains("l2 vfi"))
                    {
                        line = line.remove("l2 vfi",Qt::CaseInsensitive).trimmed();
                        bind = line;
                    }


                    line = configStream.readLine().trimmed();
                    //purify remove ---- More ----  and space
                    if(line.contains("--More--",Qt::CaseInsensitive))
                        line = line.remove("--More--",Qt::CaseInsensitive);
                    line = line.trimmed();
                }

                if(ether > -1)
                {
                    if(!etherDescMap.contains(ether))
                        etherDescMap.insert(ether, desc);
                    else if(!desc.isEmpty())
                        etherDescMap.insert(ether, desc);

                }

                if(!modelContains(interfaceModel, interface) || !desc.isEmpty())
                {

                    interfaceModel->insertRow(interfaceRow);

                    index = interfaceModel->index(interfaceRow, 0);
                    interfaceModel->setData(index,interface);

                    index = interfaceModel->index(interfaceRow, 1);
                    interfaceModel->setData(index,desc);

                    index = interfaceModel->index(interfaceRow, 2);
                    interfaceModel->setData(index,ethString);

                    index = interfaceModel->index(interfaceRow, 3);
                    interfaceModel->setData(index,bind);

                    interfaceRow++;
                    continue;
                }

            }
            else if(line.startsWith("ip route", Qt::CaseInsensitive) && !line.contains("vrf", Qt::CaseInsensitive))
            {
                line = line.remove("ip route",Qt::CaseInsensitive).trimmed();
                if(line.contains(" name "))
                {
                    list = line.split(" name ");
                    line = list.at(0);
                    line = line.trimmed();
                }

                list = line.split(" ",QString::SkipEmptyParts);
                if(list.count() == 3)
                {
                    net = list.at(0);
                    mask = list.at(1);
                    hop = list.at(2);

                    StaticModel->insertRow(static1Row);
                    index = StaticModel->index(static1Row, 0);
                    StaticModel->setData(index,net);
                    index = index.sibling(static1Row, 1);
                    StaticModel->setData(index,mask);
                    index = index.sibling(static1Row, 2);
                    StaticModel->setData(index,hop);

                    static1Row++;
                }
                else if(list.count() == 2) //net/prefix
                {
                    net = list.at(0);
                    hop = list.at(1);

                    if(net.contains("/"))
                    {
                        list = net.split("/");
                        net = list.at(0);
                        mask = list.at(1);

                        StaticModel->insertRow(static1Row);
                        index = StaticModel->index(static1Row, 0);
                        StaticModel->setData(index,net);
                        index = index.sibling(static1Row, 1);
                        StaticModel->setData(index,mask);
                        index = index.sibling(static1Row, 2);
                        StaticModel->setData(index,hop);

                        static1Row++;
                    }
                }
            }
            else if(line.startsWith("ip route vrf", Qt::CaseInsensitive))
            {
                line = line.remove("ip route vrf",Qt::CaseInsensitive).trimmed();
                if(line.contains(" name "))
                {
                    list = line.split(" name ");
                    line = list.at(0);
                    line = line.trimmed();
                }

                list = line.split(" ",QString::SkipEmptyParts);
                if(list.count() == 4)
                {
                    vpn = list.at(0);
                    net = list.at(1);
                    mask = list.at(2);
                    hop = list.at(3);

                    VpnStaticModel->insertRow(static2Row);
                    index = VpnStaticModel->index(static2Row, 0);
                    VpnStaticModel->setData(index,vpn);
                    index = index.sibling(static2Row, 1);
                    VpnStaticModel->setData(index,net);
                    index = index.sibling(static2Row, 2);
                    VpnStaticModel->setData(index,mask);
                    index = index.sibling(static2Row, 3);
                    VpnStaticModel->setData(index,hop);

                    static2Row++;
                }
                else if(list.count() == 3) // net/mask
                {
                    vpn = list.at(0);
                    net = list.at(1);
                    hop = list.at(2);

                    if(net.contains("/"))
                    {
                        list = net.split("/");
                        net = list.at(0);
                        mask = list.at(1);

                        VpnStaticModel->insertRow(static2Row);
                        index = VpnStaticModel->index(static2Row, 0);
                        VpnStaticModel->setData(index,vpn);
                        index = index.sibling(static2Row, 1);
                        VpnStaticModel->setData(index,net);
                        index = index.sibling(static2Row, 2);
                        VpnStaticModel->setData(index,mask);
                        index = index.sibling(static2Row, 3);
                        VpnStaticModel->setData(index,hop);

                        static2Row++;
                    }
                }
            }

            // check VPN
            else if(line.startsWith("ip vrf", Qt::CaseInsensitive))
            {
                ind = line.indexOf("ip vrf",0,Qt::CaseInsensitive);
                if(ind > 0)
                    line = line.remove(0,ind-1).trimmed();
                line = line.remove("ip vrf",Qt::CaseInsensitive).trimmed();
                vpn = line;
                line = configStream.readLine().trimmed();
                //purify remove ---- More ----  and space
                if(line.contains("--More--",Qt::CaseInsensitive))
                    line = line.remove("--More--",Qt::CaseInsensitive);
                line = line.trimmed();

                desc = "";
                rd = "";
                exp = "";
                imp = "";

                while( !(line.startsWith("!") || line.startsWith("ip vrf", Qt::CaseInsensitive) || line.startsWith("interface", Qt::CaseInsensitive) || line.isNull() ) )
                {

                    if(line.contains("description"))
                    {
                        line = line.remove("description",Qt::CaseInsensitive).trimmed();
                        desc = line;
                    }
                    else if(line.contains("rd"))
                    {
                        line = line.remove("rd",Qt::CaseInsensitive).trimmed();
                        rd = line;
                    }
                    else if(line.contains("route-target"))
                    {
                        line = line.remove("route-target",Qt::CaseInsensitive).trimmed();

                        if(line.contains("export"))
                        {
                            line = line.remove("export",Qt::CaseInsensitive).trimmed();
                            exp = line;
                        }
                        else if(line.contains("import"))
                        {
                            line = line.remove("import",Qt::CaseInsensitive).trimmed();
                            imp = line;
                        }
                    }



                    line = configStream.readLine().trimmed();
                    //purify remove ---- More ----  and space
                    if(line.contains("--More--",Qt::CaseInsensitive))
                        line = line.remove("--More--",Qt::CaseInsensitive);
                    line = line.trimmed();
                }

                vpnModel->insertRow(vpnRow);

                index = vpnModel->index(vpnRow, 0); //row
                vpnModel->setData(index,vpnRow);

                index = vpnModel->index(vpnRow, 1); //vpn
                vpnModel->setData(index,vpn);

                index = vpnModel->index(vpnRow, 2); //rd
                vpnModel->setData(index,rd);

                index = vpnModel->index(vpnRow, 3); //desc
                vpnModel->setData(index,desc);

                expMap.insert(vpnRow, exp);
                impMap.insert(vpnRow, imp);

                vpnRow++;

                continue;
            }
            line = configStream.readLine().trimmed();
        }

        vlanModel->setMap(&vlanDescMap);
        ui->vlanTV->setModel(vlanModel);
        etherModel->setMap(&etherDescMap);
        ui->etherTV->setModel(etherModel);
    }

    configFile.close();
}

void ConfigurationAnalyserDialog::IOSXRConfigureAnalyser()
{
    etherModel = new MapModel(this,QStringList()<<"Bundle Ether"<<"Description");
    vpnModel->setColumnCount(4);
    //row,vpn,rd, desc
    vpnModel->setHeaderData(1, Qt::Horizontal,"VRF");
    vpnModel->setHeaderData(2, Qt::Horizontal,"Route Distinguisher");
    vpnModel->setHeaderData(3, Qt::Horizontal,"Description");
    ui->vpnTV->hideColumn(0);

    interfaceModel->setColumnCount(4);
    //interface,description,ether,bind
    interfaceModel->setHeaderData(0, Qt::Horizontal,"Interface");
    interfaceModel->setHeaderData(1, Qt::Horizontal,"Description");
    interfaceModel->setHeaderData(2, Qt::Horizontal,"Port Channel");
    interfaceModel->setHeaderData(3, Qt::Horizontal,"VRF Forwarding");


    StaticModel->setColumnCount(3);
    StaticModel->setRowCount(0);
    StaticModel->setHeaderData(0, Qt::Horizontal,"Network");
    StaticModel->setHeaderData(1, Qt::Horizontal,"Mask");
    StaticModel->setHeaderData(2, Qt::Horizontal,"Next Hop");

    VpnStaticModel->setColumnCount(4);
    VpnStaticModel->setRowCount(0);
    VpnStaticModel->setHeaderData(0, Qt::Horizontal,"VRF");
    VpnStaticModel->setHeaderData(1, Qt::Horizontal,"Network");
    VpnStaticModel->setHeaderData(2, Qt::Horizontal,"Mask");
    VpnStaticModel->setHeaderData(3, Qt::Horizontal,"Next Hop");

    QModelIndex index;
    int vpnRow = 0, interfaceRow = 0, static1Row = 0, static2Row = 0;


    QTextStream configStream(&configFile);

    QString line, sysname,desc, vpn,net, mask, hop, rd, exp, imp, interface, bind, ethString;
    QStringList list;
    int ind, ether;
    bool intOk;

    if(configFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        line = configStream.readLine().trimmed();
        bool SYSNAME = true;
        while(!line.isNull())
        {
            //purify remove --More--  and space
            if(line.contains("--More--",Qt::CaseInsensitive))
                line = line.remove("--More--",Qt::CaseInsensitive);
            line = line.trimmed();

            if(SYSNAME)
            {
                if(line.startsWith("hostname", Qt::CaseInsensitive))
                {
                    line = line.remove("hostname", Qt::CaseInsensitive).trimmed();
                    sysname = line;
                    ui->sysNameLbl->setText(sysname);
                    SYSNAME = false;
                }
            }

            //interfaces
            else if(line.startsWith("interface", Qt::CaseInsensitive))
            {
                line = line.remove("interface",Qt::CaseInsensitive).trimmed();
                interface = line;

                desc = "";
                ethString = "";
                bind = "";
                ether = -1;

                if(line.startsWith("Bundle-Ether", Qt::CaseInsensitive) && !line.contains(".") )
                {
                    line = line.remove("Bundle-Ether",Qt::CaseInsensitive);

                    ether = line.toInt(&intOk);
                    if(!intOk) ether = -1;
                }

                line = configStream.readLine().trimmed();
                //purify remove ---- More ----  and space
                if(line.contains("--More--",Qt::CaseInsensitive))
                    line = line.remove("--More--",Qt::CaseInsensitive);
                line = line.trimmed();

                while( !(line.startsWith("!") || line.isNull() ) )
                {

                    if(line.contains("description"))
                    {
                        line = line.remove("description",Qt::CaseInsensitive).trimmed();
                        desc = line;
                    }
                    else if(line.contains("bundle id"))
                    {
                        line = line.remove("bundle id",Qt::CaseInsensitive).trimmed();
                        if(line.contains("mode", Qt::CaseInsensitive))
                        {
                            ind = line.indexOf("mode",0,Qt::CaseInsensitive);
                            line = line.remove(ind-1, line.length()-1);
                        }
                        ethString = line;
                    }
                    else if(line.contains("vrf"))
                    {
                        line = line.remove("vrf",Qt::CaseInsensitive).trimmed();
                        bind = line;
                    }
                    else if(line.contains("l2 vfi"))
                    {
                        line = line.remove("l2 vfi",Qt::CaseInsensitive).trimmed();
                        bind = line;
                    }



                    line = configStream.readLine().trimmed();
                    //purify remove ---- More ----  and space
                    if(line.contains("--More--",Qt::CaseInsensitive))
                        line = line.remove("--More--",Qt::CaseInsensitive);
                    line = line.trimmed();
                }


                if(ether > -1)
                {
                    if( !etherDescMap.contains(ether))
                        etherDescMap.insert(ether, desc);
                    else if(!desc.isEmpty())
                        etherDescMap.insert(ether, desc);
                }

                if(!modelContains(interfaceModel, interface) || !desc.isEmpty())
                {
                    interfaceModel->insertRow(interfaceRow);

                    index = interfaceModel->index(interfaceRow, 0);
                    interfaceModel->setData(index,interface);

                    index = interfaceModel->index(interfaceRow, 1);
                    interfaceModel->setData(index,desc);

                    index = interfaceModel->index(interfaceRow, 2);
                    interfaceModel->setData(index,ethString);

                    index = interfaceModel->index(interfaceRow, 3);
                    interfaceModel->setData(index,bind);

                    interfaceRow++;
                    continue;
                }
            }
            else if(line.startsWith("router static", Qt::CaseInsensitive))
            {
                line = configStream.readLine().trimmed();
                //purify remove --More--  and space
                if(line.contains("--More--",Qt::CaseInsensitive))
                    line = line.remove("--More--",Qt::CaseInsensitive);
                line = line.trimmed();

                if(line.contains("address-family"))
                {
                    line = configStream.readLine().trimmed();
                    //purify remove --More--  and space
                    if(line.contains("--More--",Qt::CaseInsensitive))
                        line = line.remove("--More--",Qt::CaseInsensitive);
                    line = line.trimmed();

                    while(!line.contains("!") && !line.isNull() )
                    {
                        //static route
                        if(line.contains(" name "))
                        {
                            list = line.split(" name ");
                            line = list.at(0);
                            line = line.trimmed();
                        }

                        list = line.split(" ");
                        if(list.count() == 2)
                        { //net/prefix hop
                            net = list.at(0);
                            hop = list.at(1);
                            if(net.contains("/"))
                            {
                                net = list.at(0);
                                mask = list.at(1);
                                StaticModel->insertRow(static1Row);
                                index = StaticModel->index(static1Row, 0);
                                StaticModel->setData(index,net);
                                index = index.sibling(static1Row, 1);
                                StaticModel->setData(index,mask);
                                index = index.sibling(static1Row, 2);
                                StaticModel->setData(index,hop);

                                static1Row++;
                            }
                        }
                        else if(list.count() == 3)
                        { // net mask hop
                            net = list.at(0);
                            mask = list.at(1);
                            hop = list.at(2);

                            StaticModel->insertRow(static1Row);
                            index = StaticModel->index(static1Row, 0);
                            StaticModel->setData(index,net);
                            index = index.sibling(static1Row, 1);
                            StaticModel->setData(index,mask);
                            index = index.sibling(static1Row, 2);
                            StaticModel->setData(index,hop);

                            static1Row++;
                        }

                        line = configStream.readLine().trimmed();
                        //purify remove --More--  and space
                        if(line.contains("--More--",Qt::CaseInsensitive))
                            line = line.remove("--More--",Qt::CaseInsensitive);
                        line = line.trimmed();
                    }

                    line = configStream.readLine().trimmed();
                    //purify remove --More--  and space
                    if(line.contains("--More--",Qt::CaseInsensitive))
                        line = line.remove("--More--",Qt::CaseInsensitive);
                    line = line.trimmed();

                    if(line.startsWith("vrf"))
                    {
                        vpn = line.remove("vrf").trimmed();

                        line = configStream.readLine().trimmed();
                        if(line.contains("address-family"))
                        {
                            line = configStream.readLine().trimmed();
                            //purify remove --More--  and space
                            if(line.contains("--More--",Qt::CaseInsensitive))
                                line = line.remove("--More--",Qt::CaseInsensitive);
                            line = line.trimmed();

                            while(!line.contains("!") && !line.isNull() )
                            {
                                //static route
                                if(line.contains(" name "))
                                {
                                    list = line.split(" name ");
                                    line = list.at(0);
                                    line = line.trimmed();
                                }

                                list = line.split(" ");
                                if(list.count() == 2)
                                { //net/prefix hop
                                    net = list.at(0);
                                    hop = list.at(1);
                                    if(net.contains("/"))
                                    {
                                        net = list.at(0);
                                        mask = list.at(1);
                                        VpnStaticModel->insertRow(static2Row);
                                        index = VpnStaticModel->index(static2Row, 0);
                                        VpnStaticModel->setData(index,vpn);
                                        index = index.sibling(static2Row, 1);
                                        VpnStaticModel->setData(index,net);
                                        index = index.sibling(static2Row, 2);
                                        VpnStaticModel->setData(index,mask);
                                        index = index.sibling(static2Row, 3);
                                        VpnStaticModel->setData(index,hop);

                                        static2Row++;
                                    }
                                }
                                else if(list.count() == 3)
                                { // net mask hop
                                    net = list.at(0);
                                    mask = list.at(1);
                                    hop = list.at(2);

                                    VpnStaticModel->insertRow(static2Row);
                                    index = VpnStaticModel->index(static2Row, 0);
                                    VpnStaticModel->setData(index,vpn);
                                    index = index.sibling(static2Row, 1);
                                    VpnStaticModel->setData(index,net);
                                    index = index.sibling(static2Row, 2);
                                    VpnStaticModel->setData(index,mask);
                                    index = index.sibling(static2Row, 3);
                                    VpnStaticModel->setData(index,hop);

                                    static2Row++;
                                }

                                line = configStream.readLine().trimmed();
                                //purify remove --More--  and space
                                if(line.contains("--More--",Qt::CaseInsensitive))
                                    line = line.remove("--More--",Qt::CaseInsensitive);
                                line = line.trimmed();
                            }
                        }

                    }
                }
                else if(line.startsWith("vrf"))
                {
                    vpn = line.remove("vrf").trimmed();

                    line = configStream.readLine().trimmed();
                    if(line.contains("address-family"))
                    {
                        line = configStream.readLine().trimmed();
                        //purify remove --More--  and space
                        if(line.contains("--More--",Qt::CaseInsensitive))
                            line = line.remove("--More--",Qt::CaseInsensitive);
                        line = line.trimmed();

                        while(!line.contains("!") && !line.isNull() )
                        {
                            //static route
                            if(line.contains(" name "))
                            {
                                list = line.split(" name ");
                                line = list.at(0);
                                line = line.trimmed();
                            }

                            list = line.split(" ");
                            if(list.count() == 2)
                            { //net/prefix hop
                                net = list.at(0);
                                hop = list.at(1);
                                if(net.contains("/"))
                                {
                                    net = list.at(0);
                                    mask = list.at(1);
                                    VpnStaticModel->insertRow(static2Row);
                                    index = VpnStaticModel->index(static2Row, 0);
                                    VpnStaticModel->setData(index,vpn);
                                    index = index.sibling(static2Row, 1);
                                    VpnStaticModel->setData(index,net);
                                    index = index.sibling(static2Row, 2);
                                    VpnStaticModel->setData(index,mask);
                                    index = index.sibling(static2Row, 3);
                                    VpnStaticModel->setData(index,hop);

                                    static2Row++;
                                }
                            }
                            else if(list.count() == 3)
                            { // net mask hop
                                net = list.at(0);
                                mask = list.at(1);
                                hop = list.at(2);

                                VpnStaticModel->insertRow(static2Row);
                                index = VpnStaticModel->index(static2Row, 0);
                                VpnStaticModel->setData(index,vpn);
                                index = index.sibling(static2Row, 1);
                                VpnStaticModel->setData(index,net);
                                index = index.sibling(static2Row, 2);
                                VpnStaticModel->setData(index,mask);
                                index = index.sibling(static2Row, 3);
                                VpnStaticModel->setData(index,hop);

                                static2Row++;
                            }

                            line = configStream.readLine().trimmed();
                            //purify remove --More--  and space
                            if(line.contains("--More--",Qt::CaseInsensitive))
                                line = line.remove("--More--",Qt::CaseInsensitive);
                            line = line.trimmed();
                        }
                    }
                }
            }
            // check VRF
            else if(line.startsWith("vrf", Qt::CaseInsensitive))
            {
                ind = line.indexOf("vrf",0,Qt::CaseInsensitive);
                if(ind > 0)
                    line = line.remove(0,ind-1).trimmed();
                line = line.remove("vrf",Qt::CaseInsensitive).trimmed();
                vpn = line;
                line = configStream.readLine().trimmed();
                //purify remove ---- More ----  and space
                if(line.contains("--More--",Qt::CaseInsensitive))
                    line = line.remove("--More--",Qt::CaseInsensitive);
                line = line.trimmed();

                desc = "";
                rd = "";
                exp = "";
                imp = "";

                while( !(line.startsWith("!") || line.startsWith("vrf", Qt::CaseInsensitive) || line.startsWith("interface", Qt::CaseInsensitive) || line.isNull() ) )
                {

                    if(line.contains("description"))
                    {
                        line = line.remove("description",Qt::CaseInsensitive).trimmed();
                        desc = line;
                    }
                    else if(line.contains("rd"))
                    {
                        line = line.remove("rd",Qt::CaseInsensitive).trimmed();
                        rd = line;
                    }
                    else if(line.contains("route-target"))
                    {
                        line = line.remove("route-target",Qt::CaseInsensitive).trimmed();

                        if(line.contains("export"))
                        {
                            line = line.remove("export",Qt::CaseInsensitive).trimmed();
                            exp = line;
                        }
                        else if(line.contains("import"))
                        {
                            line = line.remove("import",Qt::CaseInsensitive).trimmed();
                            imp = line;
                        }
                    }



                    line = configStream.readLine().trimmed();
                    //purify remove ---- More ----  and space
                    if(line.contains("--More--",Qt::CaseInsensitive))
                        line = line.remove("--More--",Qt::CaseInsensitive);
                    line = line.trimmed();
                }

                vpnModel->insertRow(vpnRow);

                index = vpnModel->index(vpnRow, 0); //row
                vpnModel->setData(index,vpnRow);

                index = vpnModel->index(vpnRow, 1); //vpn
                vpnModel->setData(index,vpn);

                index = vpnModel->index(vpnRow, 2); //rd
                vpnModel->setData(index,rd);

                index = vpnModel->index(vpnRow, 3); //desc
                vpnModel->setData(index,desc);

                expMap.insert(vpnRow, exp);
                impMap.insert(vpnRow, imp);

                vpnRow++;

                continue;
            }
            line = configStream.readLine().trimmed();
            //purify remove --More--  and space
            if(line.contains("--More--",Qt::CaseInsensitive))
                line = line.remove("--More--",Qt::CaseInsensitive);
            line = line.trimmed();
        }


        etherModel->setMap(&etherDescMap);
        ui->etherTV->setModel(etherModel);
    }

    configFile.close();
}

bool ConfigurationAnalyserDialog::modelContains(QStandardItemModel *model, const QString value)
{
    int r = 0;
    bool ok = false;
    QModelIndex index = model->index(0,0);
    QString v;
    while(r < model->rowCount())
    {
        index = index.sibling(r, 0);
        v = model->data(index).toString();
        if(v.compare(value, Qt::CaseInsensitive) == 0)
        {
            ok = true;
            break;
        }

        r++;
    }

    return ok;
}

void ConfigurationAnalyserDialog::on_osCB_currentIndexChanged(int index)
{
    index++;
    emptyAll();
    int os = ui->osCB->currentData().toInt();
    switch (os)
    {
    case 0://huawei
        ui->cmdLbl->setText("display current-configuration (display cu)");
        ui->tab->setTabText(1,"Ether Trunk");
        ui->tab->setTabText(2,"VPN");
        ui->tab->setTabText(5,"VPN Static Route");
        ui->tab->setTabEnabled(0, true);
        break;
    case 1://ios
        ui->cmdLbl->setText("show running-config (show run)");
        ui->tab->setTabText(1,"Port Channel");
        ui->tab->setTabText(2,"VRF");
        ui->tab->setTabText(5,"VRF Static Route");
        ui->tab->setTabEnabled(0, true);
        break;
    case 2://ios xr
        ui->cmdLbl->setText("show running-config (show run)");
        ui->tab->setTabText(1,"Bundle Ether");
        ui->tab->setTabText(2,"VRF");
        ui->tab->setTabText(5,"VRF Static Route");
        ui->tab->setTabEnabled(0, false);
        break;
    }
}

void ConfigurationAnalyserDialog::on_btn_clicked()
{
    emptyAll();
    QString configFileName = QFileDialog::getOpenFileName(this,"Select Configuration");
    if(configFileName.isEmpty())
    {
        return;
    }
    else
    {
        ui->fileLbl->setText(configFileName);

        configFile.setFileName(configFileName);

        int os = ui->osCB->currentData().toInt();
        switch (os)
        {
        case 0:
            huaweiConfigureAnalyser();
            break;
        case 1:
            IOSConfigureAnalyser();
            break;
        case 2:
            IOSXRConfigureAnalyser();
            break;
        }
    }

}

void ConfigurationAnalyserDialog::on_vpnTV_clicked(const QModelIndex &index)
{
    QModelIndex ind = index.sibling(index.row(), 0); // row
    int row = vpnModel->data(ind).toInt();
    QStringList expL,impL;

    expL = expMap.value(row).split(" ",QString::SkipEmptyParts);
    impL = impMap.value(row).split(" ",QString::SkipEmptyParts);

    expModel->setStringList(expL);
    impModel->setStringList(impL);
}

void ConfigurationAnalyserDialog::on_intExportBtn_clicked()
{
    QString path = QFileDialog::getSaveFileName(this, "Save DaNet Report As", "","pdf (*.pdf)");
    if(!path.isEmpty())
    {
        if(!path.endsWith(".pdf",Qt::CaseInsensitive)) path += ".pdf";

        QVector<int> columnStretch = QVector<int>() <<20<<50<<15<<20;
        QVector<QString> headers = QVector<QString>() << "Interface" << "Description"<<"Eth Trunk"<<"Bind";

        Report report(this,interfaceModel,headers ,columnStretch, path, "Interface Report");
        if(report.print())
        {
            QDesktopServices::openUrl(path);
        }
        else
        {
            QMessageBox::warning(this, "ERROR", report.lastError());
        }
    }
}

