#include "networkassistantdialog.h"
#include "ui_networkassistantdialog.h"
#include <QHBoxLayout>
#include <QTextStream>
#include <QFileDialog>
#include <QFile>
#include <QDesktopServices>
#include "mainPanel/menu/tools/NetworkAssistant/configWidgets/allowpasswidget.h"
#include "mainPanel/menu/tools/NetworkAssistant/configWidgets/batchvlanwidget.h"
#include "mainPanel/menu/tools/NetworkAssistant/configWidgets/bindvpnwidget.h"
#include "mainPanel/menu/tools/NetworkAssistant/configWidgets/defaultvlanwidget.h"
#include "mainPanel/menu/tools/NetworkAssistant/configWidgets/dot1qtagwidget.h"
#include "mainPanel/menu/tools/NetworkAssistant/configWidgets/hybridportwidget.h"
#include "mainPanel/menu/tools/NetworkAssistant/configWidgets/interfacewidget.h"
#include "mainPanel/menu/tools/NetworkAssistant/configWidgets/ipassignwidget.h"
#include "mainPanel/menu/tools/NetworkAssistant/configWidgets/linktypewidget.h"
#include "mainPanel/menu/tools/NetworkAssistant/configWidgets/loadbalacewidget.h"
#include "mainPanel/menu/tools/NetworkAssistant/configWidgets/mapvlanwidget.h"
#include "mainPanel/menu/tools/NetworkAssistant/configWidgets/portswitchwidget.h"
#include "mainPanel/menu/tools/NetworkAssistant/configWidgets/qinqwidget.h"
#include "mainPanel/menu/tools/NetworkAssistant/configWidgets/routewidget.h"
#include "mainPanel/menu/tools/NetworkAssistant/configWidgets/sysnamewidget.h"
#include "mainPanel/menu/tools/NetworkAssistant/configWidgets/trunkwidget.h"
#include "mainPanel/menu/tools/NetworkAssistant/configWidgets/vlanwidget.h"
#include "mainPanel/menu/tools/NetworkAssistant/configWidgets/vpninstancewidget.h"


#define NONE 0
#define NONESEP 1

#define SYSNAME 2
#define SYSNAMESEP 3

#define BATCHVLAN 4
#define VLAN 5
#define VLANSEP 6

#define INTERFACE 7
#define PORTSWITCH 8
#define LINKTYPE 9
#define ETHTRUNK 10
#define LOADBALANCE 11
#define ALLOWPASS 12
#define DEFAULTVLAN 13
#define HYBRIDPORT 14
#define MAPVLAN 15
#define IP 16
#define BINDVPN 17
#define DOT1QTAG 18
#define QINQ 19
#define INTSEP 20

#define DEFAULTROUTE 21
#define STATIC1 22
#define STATIC2 23
#define ROUTESEP 24
#define VPN 25

NetworkAssistantDialog::NetworkAssistantDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NetworkAssistantDialog)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowMinMaxButtonsHint);

    //os
    ui->osCB->addItem("HUAWEI OS", 0);
    ui->osCB->addItem("CISCO IOS", 1);
    ui->osCB->addItem("CISCO IOS-XR", 2);

}

NetworkAssistantDialog::~NetworkAssistantDialog()
{
    delete ui;
}

void NetworkAssistantDialog::on_osCB_currentIndexChanged(int index)
{
    index++;
    ui->configTE->clear();
    ui->configCB->blockSignals(true);
    ui->configCB->clear();
    ui->configCB->addItem("None", NONE);

    int os = ui->osCB->currentData().toInt();
    switch (os)
    {
    case 0: //HUAWEI
        ui->vendorLbl->setPixmap(QPixmap(":/Image/Huawei.png"));
        //init
        ui->configCB->insertSeparator(NONESEP);
        ui->configCB->addItem("Sysname", SYSNAME);
        ui->configCB->insertSeparator(SYSNAMESEP);

        ui->configCB->addItem("Vlan Batch", BATCHVLAN);
        ui->configCB->addItem("Vlan", VLAN);
        ui->configCB->insertSeparator(VLANSEP);

        ui->configCB->addItem("Interface", INTERFACE);
        ui->configCB->addItem("PortSwitch", PORTSWITCH);
        ui->configCB->addItem("Port Link-Type", LINKTYPE);
        ui->configCB->addItem("Bind To Eth-Trunk", ETHTRUNK);
        ui->configCB->addItem("Trunk Load Balance", LOADBALANCE);
        ui->configCB->addItem("Trunk Allow-Pass", ALLOWPASS);
        ui->configCB->addItem("Access Port Default VLAN", DEFAULTVLAN);
        ui->configCB->addItem("Tag/Untag Hybrid Port", HYBRIDPORT);
        ui->configCB->addItem("VLAN MAP", MAPVLAN);
        ui->configCB->addItem("SET IP", IP);
        ui->configCB->addItem("Bind To VPN", BINDVPN);
        ui->configCB->addItem("Sub-Interface dot1q Tag", DOT1QTAG);
        ui->configCB->addItem("qinq", QINQ);
        ui->configCB->insertSeparator(INTSEP);

        ui->configCB->addItem("Default Route", DEFAULTROUTE);
        ui->configCB->addItem("Static Route", STATIC1);
        ui->configCB->addItem("VPN Static Route", STATIC2);
        ui->configCB->insertSeparator(ROUTESEP);

        ui->configCB->addItem("VPN", VPN);

        break;
    case 1:// IOS
        ui->vendorLbl->setPixmap(QPixmap(":/Image/cisco.png"));
        //init
        ui->configCB->insertSeparator(NONESEP);
        ui->configCB->addItem("HostName", SYSNAME);
        ui->configCB->insertSeparator(SYSNAMESEP);

        ui->configCB->addItem("Vlan Group", BATCHVLAN);
        ui->configCB->addItem("Vlan", VLAN);
        ui->configCB->insertSeparator(VLANSEP);

        ui->configCB->addItem("Interface", INTERFACE);
        ui->configCB->addItem("SwitchPort", PORTSWITCH);
        ui->configCB->addItem("SwitchPort Mode", LINKTYPE);
        ui->configCB->addItem("Bind To Channel-Group", ETHTRUNK);
        ui->configCB->addItem("Port-Channel Load Balance", LOADBALANCE);
        ui->configCB->addItem("Port-Channel Allowed VLAN", ALLOWPASS);
        ui->configCB->addItem("Access Port Default VLAN", DEFAULTVLAN);
        ui->configCB->addItem("VLAN MAP", MAPVLAN);
        ui->configCB->addItem("SET IP", IP);
        ui->configCB->addItem("Bind To VRF", BINDVPN);
        ui->configCB->addItem("Sub-Interface dot1q Encap.", DOT1QTAG);
        ui->configCB->addItem("QinQ Encapsulation", QINQ);
        ui->configCB->insertSeparator(INTSEP);

        ui->configCB->addItem("Default Route", DEFAULTROUTE);
        ui->configCB->addItem("Static Route", STATIC1);
        ui->configCB->addItem("VRF Static Route", STATIC2);
        ui->configCB->insertSeparator(ROUTESEP);

        ui->configCB->addItem("VRF", VPN);

        break;
    case 2: //IOS XR
        ui->vendorLbl->setPixmap(QPixmap(":/Image/cisco.png"));
        //init
        ui->configCB->insertSeparator(NONESEP);
        ui->configCB->addItem("HostName", SYSNAME);
        ui->configCB->insertSeparator(SYSNAMESEP);

        ui->configCB->addItem("Interface", INTERFACE);
        ui->configCB->addItem("Bind To Bundle", ETHTRUNK);
        ui->configCB->addItem("SET IP", IP);
        ui->configCB->addItem("Bind To VRF", BINDVPN);
        ui->configCB->addItem("Sub-Interface dot1q Encap.", DOT1QTAG);
        ui->configCB->addItem("QinQ Encapsulation", QINQ);
        ui->configCB->insertSeparator(INTSEP);

        ui->configCB->addItem("Default Route", DEFAULTROUTE);
        ui->configCB->addItem("Static Route", STATIC1);
        ui->configCB->addItem("VRF Static Route", STATIC2);
        ui->configCB->insertSeparator(ROUTESEP);

        ui->configCB->addItem("VRF", VPN);
        break;
    }

    ui->configCB->setCurrentIndex(-1);
    ui->configCB->blockSignals(false);
    ui->configCB->setCurrentIndex(0);

}

void NetworkAssistantDialog::on_configCB_currentIndexChanged(int index)
{
    index++;
    qDeleteAll(ui->parentWidget->children());

    int os = ui->osCB->currentData().toInt();
    ui->writeBtn->setEnabled(false);

    QHBoxLayout *layout = new QHBoxLayout(ui->parentWidget);

    int conf = ui->configCB->currentData().toInt();
    switch (conf)
    {
    case SYSNAME:
    {
        SysnameWidget *sysnameWidget = new SysnameWidget(ui->parentWidget,os,ui->writeBtn,ui->configTE);
        layout->addWidget(sysnameWidget);
        connect(sysnameWidget, SIGNAL(setWriteBtnEnableSignal(bool)),this,SLOT(setWriteBtnEnableSlot(bool)));
        break;
    }
    case BATCHVLAN:
    {
        BatchVlanWidget *batchVlanWidget = new BatchVlanWidget(ui->parentWidget,os,ui->writeBtn,ui->configTE);
        layout->addWidget(batchVlanWidget);
        connect(batchVlanWidget, SIGNAL(setWriteBtnEnableSignal(bool)),this,SLOT(setWriteBtnEnableSlot(bool)));
        break;
    }
    case VLAN:
    {
        VlanWidget *vlanWidget = new VlanWidget(ui->parentWidget,os,ui->writeBtn,ui->configTE);
        layout->addWidget(vlanWidget);
        connect(vlanWidget, SIGNAL(setWriteBtnEnableSignal(bool)),this,SLOT(setWriteBtnEnableSlot(bool)));
        break;
    }
    case INTERFACE:
    {
        InterfaceWidget *interfaceWidget = new InterfaceWidget(ui->parentWidget,os,ui->writeBtn,ui->configTE);
        layout->addWidget(interfaceWidget);
        connect(interfaceWidget, SIGNAL(setWriteBtnEnableSignal(bool)),this,SLOT(setWriteBtnEnableSlot(bool)));
        break;
    }
    case PORTSWITCH:
    {
        PortSwitchWidget *portSwitchWidget = new PortSwitchWidget(ui->parentWidget,os,ui->writeBtn,ui->configTE);
        layout->addWidget(portSwitchWidget);
        ui->writeBtn->setEnabled(true);
        break;
    }
    case LINKTYPE:
    {
        LinkTypeWidget *linkTypeWidget = new LinkTypeWidget(ui->parentWidget,os,ui->writeBtn,ui->configTE);
        layout->addWidget(linkTypeWidget);
        connect(linkTypeWidget, SIGNAL(setWriteBtnEnableSignal(bool)),this,SLOT(setWriteBtnEnableSlot(bool)));
        break;
    }
    case ETHTRUNK:
    {
        TrunkWidget *trunkWidget = new TrunkWidget(ui->parentWidget,os,ui->writeBtn,ui->configTE);
        layout->addWidget(trunkWidget);
        connect(trunkWidget, SIGNAL(setWriteBtnEnableSignal(bool)),this,SLOT(setWriteBtnEnableSlot(bool)));
        break;
    }
    case LOADBALANCE:
    {
        LoadBalaceWidget *loadBalaceWidget = new LoadBalaceWidget(ui->parentWidget,os,ui->writeBtn,ui->configTE);
        layout->addWidget(loadBalaceWidget);
        connect(loadBalaceWidget, SIGNAL(setWriteBtnEnableSignal(bool)),this,SLOT(setWriteBtnEnableSlot(bool)));
        break;
    }
    case ALLOWPASS:
    {
        AllowPassWidget *allowPassWidget = new AllowPassWidget(ui->parentWidget,os,ui->writeBtn,ui->configTE);
        layout->addWidget(allowPassWidget);
        connect(allowPassWidget, SIGNAL(setWriteBtnEnableSignal(bool)),this,SLOT(setWriteBtnEnableSlot(bool)));
        break;
    }
    case DEFAULTVLAN:
    {
        DefaultVlanWidget *defaultVlanWidget = new DefaultVlanWidget(ui->parentWidget,os,ui->writeBtn,ui->configTE);
        layout->addWidget(defaultVlanWidget);
        connect(defaultVlanWidget, SIGNAL(setWriteBtnEnableSignal(bool)),this,SLOT(setWriteBtnEnableSlot(bool)));
        break;
    }
    case HYBRIDPORT:
    {
        HybridPortWidget *hybridPortWidget = new HybridPortWidget(ui->parentWidget,os,ui->writeBtn,ui->configTE);
        layout->addWidget(hybridPortWidget);
        connect(hybridPortWidget, SIGNAL(setWriteBtnEnableSignal(bool)),this,SLOT(setWriteBtnEnableSlot(bool)));
        break;
    }
    case MAPVLAN:
    {
        MapVlanWidget *mapVlanWidget = new MapVlanWidget(ui->parentWidget,os,ui->writeBtn,ui->configTE);
        layout->addWidget(mapVlanWidget);
        connect(mapVlanWidget, SIGNAL(setWriteBtnEnableSignal(bool)),this,SLOT(setWriteBtnEnableSlot(bool)));
        break;
    }
    case IP:
    {
        IpAssignWidget *ipAssignWidget = new IpAssignWidget(ui->parentWidget,os,ui->writeBtn,ui->configTE);
        layout->addWidget(ipAssignWidget);
        connect(ipAssignWidget, SIGNAL(setWriteBtnEnableSignal(bool)),this,SLOT(setWriteBtnEnableSlot(bool)));
        break;
    }
    case BINDVPN:
    {
        BindVpnWidget *bindVpnWidget = new BindVpnWidget(ui->parentWidget,os,ui->writeBtn,ui->configTE);
        layout->addWidget(bindVpnWidget);
        connect(bindVpnWidget, SIGNAL(setWriteBtnEnableSignal(bool)),this,SLOT(setWriteBtnEnableSlot(bool)));
        break;
    }
    case DOT1QTAG:
    {
        Dot1QTagWidget *dot1QTagWidget = new Dot1QTagWidget(ui->parentWidget,os,ui->writeBtn,ui->configTE);
        layout->addWidget(dot1QTagWidget);
        connect(dot1QTagWidget, SIGNAL(setWriteBtnEnableSignal(bool)),this,SLOT(setWriteBtnEnableSlot(bool)));
        break;
    }
    case QINQ:
    {
        QinQWidget *qinQWidget = new QinQWidget(ui->parentWidget,os,ui->writeBtn,ui->configTE);
        layout->addWidget(qinQWidget);
        connect(qinQWidget, SIGNAL(setWriteBtnEnableSignal(bool)),this,SLOT(setWriteBtnEnableSlot(bool)));
        break;
    }
    case DEFAULTROUTE:
    {
        RouteWidget *routeWidget0 = new RouteWidget(ui->parentWidget,os,0,ui->writeBtn,ui->configTE);
        layout->addWidget(routeWidget0);
        connect(routeWidget0, SIGNAL(setWriteBtnEnableSignal(bool)),this,SLOT(setWriteBtnEnableSlot(bool)));
        break;
    }
    case STATIC1:
    {
        RouteWidget *routeWidget1 = new RouteWidget(ui->parentWidget,os,1,ui->writeBtn,ui->configTE);
        layout->addWidget(routeWidget1);
        connect(routeWidget1, SIGNAL(setWriteBtnEnableSignal(bool)),this,SLOT(setWriteBtnEnableSlot(bool)));
        break;
    }
    case STATIC2:
    {
        RouteWidget *routeWidget2 = new RouteWidget(ui->parentWidget,os,2,ui->writeBtn,ui->configTE);
        layout->addWidget(routeWidget2);
        connect(routeWidget2, SIGNAL(setWriteBtnEnableSignal(bool)),this,SLOT(setWriteBtnEnableSlot(bool)));
        break;
    }
    case VPN:
    {
        VpnInstanceWidget *vpnInstanceWidget = new VpnInstanceWidget(ui->parentWidget,os,ui->writeBtn,ui->configTE);
        layout->addWidget(vpnInstanceWidget);
        connect(vpnInstanceWidget, SIGNAL(setWriteBtnEnableSignal(bool)),this,SLOT(setWriteBtnEnableSlot(bool)));
        break;
    }

    }
}

void NetworkAssistantDialog::on_sepBtn_clicked()
{
    int os = ui->osCB->currentData().toInt();
    if(os == 0)
    {
        ui->configTE->appendPlainText("#");
    }
    else if(os > 0)
    {
        ui->configTE->appendPlainText("!");
    }
}

void NetworkAssistantDialog::setWriteBtnEnableSlot(bool en)
{
    if(en)
        ui->writeBtn->setEnabled(true);
    else
        ui->writeBtn->setEnabled(false);
}

void NetworkAssistantDialog::on_exportBtn_clicked()
{
    QString path = QFileDialog::getSaveFileName(this, "Save DaNet Network Assistant Report As");

    if(!path.isEmpty())
    {
        if(!path.endsWith(".txt"))
            path += ".txt";

        QFile file(path);
        if(file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QTextStream stream(&file);
            QString configString = ui->configTE->toPlainText();
            stream << configString;

            file.close();
            QDesktopServices::openUrl(path);
        }
    }
}

void NetworkAssistantDialog::on_clearBtn_clicked()
{
    ui->configCB->setCurrentIndex(NONE);
    ui->configTE->clear();
}

void NetworkAssistantDialog::on_configTE_textChanged()
{
    QString text = ui->configTE->toPlainText().trimmed();
    if(text.isEmpty())
    {
        ui->exportBtn->setEnabled(false);
        ui->sepBtn->setEnabled(false);
    }
    else
    {
        ui->exportBtn->setEnabled(true);
        ui->sepBtn->setEnabled(true);
    }
}


