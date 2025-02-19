#include "mainpanel.h"
#include "ui_mainpanel.h"
#include <QMenu>
#include <QHBoxLayout>
#include <QSpacerItem>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include "lib/database/danetdbman.h"
#include "lib/headerview.h"
#include "lib/sortfilterproxymodel.h"
#include <QFontDatabase>
#include <QShortcut>
// main panel
#include "lib/interfacemodel.h"
#include "mainPanel/assigninterfacedialog.h"
#include "mainPanel/editinterfacedialog.h"
#include "mainPanel/emptyinterfacedialog.h"
#include "mainPanel/localdevicedialog.h"
#include "mainPanel/changemoduledialog.h"
#include "mainPanel/changeinterfacetypedialog.h"
#include "mainPanel/exchangeinfodialog.h"
#include "mainPanel/exportpdfdialog.h"
#include "mainPanel/exportexceldialog.h"
#include "mainPanel/changeinterfaceodfdialog.h"
#include "mainPanel/OdfAssignDetach/assigninterfaceodfdialog.h"
#include "mainPanel/OdfAssignDetach/detachinterfaceodfdialog.h"
//admin
#include "mainPanel/menu/admin/users/usermandialog.h"
#include "mainPanel/menu/admin/recommendations/recommendationsviewdialog.h"
#include "mainPanel/menu/admin/log/clearlogdialog.h"

//installation
#include "mainPanel/menu/installation/ODF/odfmandialog.h"
#include "mainPanel/menu/installation/device/deviceinstalldialog.h"
#include "mainPanel/menu/installation/card/cardinstalldialog.h"
#include "mainPanel/menu/installation/exchangeSite/exchangesitemandialog.h"
// management
#include "mainPanel/menu/management/DeviceManagement/devicemanagementdialog.h"
#include "mainPanel/menu/management/ODFManagement/odfmanagementdialog.h"
#include "mainPanel/menu/management/commodities/cat/cat_dialog.h"
#include "mainPanel/menu/management/uninstalled/uninstalleddialog.h"
#include "mainPanel/menu/management/letters/lettersdialog.h"
#include "mainPanel/menu/management/telephones/telephonesdialog.h"
#include "mainPanel/menu/management/purchases/purchasemandialog.h"
#include "mainPanel/menu/management/interfaceType/interfacetypedialog.h"
#include "mainPanel/menu/management/modules/modulesmandialog.h"
#include "mainPanel/menu/management/devices/devicemandialog.h"
#include "mainPanel/menu/management/cards/cardmandialog.h"
#include "mainPanel/menu/management/saloon/saloonmandialog.h"
//plan
#include "mainPanel/menu/plan/ipv4/net/netv4pooldialog.h"
#include "mainPanel/menu/plan/commercial/reservetocommercialdialog.h"
#include "mainPanel/menu/plan/connection/connectiondialog.h"
#include "mainPanel/menu/plan/NAS/brasnasipmandialog.h"
#include "mainPanel/menu/plan/dslam/dslamdialog.h"
#include "mainPanel/menu/plan/dslam/localNewDslam/localnewdslamdialog.h"
#include "mainPanel/menu/plan/dslam/newSitePlan/newsitedslamdialog.h"
#include "mainPanel/menu/plan/dslam/removeDslam/removedslamdialog.h"
#include "mainPanel/menu/plan/accessSingleSwitch/accesssingleswitchdialog.h"
#include "mainPanel/menu/plan/accessDoubleSwitch/accessdoubleswitchdialog.h"
#include "mainPanel/menu/plan/accessBrasConfig/accessbrasconfigdialog.h"
#include "mainPanel/menu/plan/SBCIpLB/sbciploopbackdialog.h"
#include "mainPanel/menu/plan/pvcRef/pvcrefdialog.h"
#include "mainPanel/menu/plan/vlanRef/vlanrefdialog.h"
#include "mainPanel/menu/plan/rdRef/rdrefdialog.h"
#include "mainPanel/menu/plan/vsiRef/vsirefdialog.h"

//REPORT
#include "mainPanel/menu/report/interface/interfacereportdialog.h"
#include "mainPanel/menu/report/odf/odfreportdialog.h"
#include "mainPanel/menu/report/sites/exchangesitesreportdialog.h"
#include "mainPanel/menu/report/floorplan/floorplanreportdialog.h"
#include "mainPanel/menu/report/device/reportdevicedialog.h"
#include "mainPanel/menu/report/ipv4plan/net/reportnetworkv4dialog.h"
#include "mainPanel/menu/report/ipv4plan/subnet/reportsubnetv4dialog.h"
#include "mainPanel/menu/report/ipv4plan/ip/reportipv4dialog.h"
#include "mainPanel/menu/report/lom/lomdialog.h"
#include "mainPanel/menu/report/DSLAM/reportdslamdialog.h"
#include "mainPanel/menu/report/commercial/reportreservetocommercialdialog.h"
#include "mainPanel/menu/report/DSLAM/reporttotaldslamplandialog.h"

//Tools
#include "mainPanel/menu/tools/NetworkCalculator/networkcalculatordialog.h"
#include "mainPanel/menu/tools/ipPoolAnalyser/ippoolanalyserdialog.h"
#include "mainPanel/menu/tools/ConfigurationAnalyser/configurationanalyserdialog.h"
#include "mainPanel/menu/tools/NetworkAssistant/networkassistantdialog.h"
#include "mainPanel/menu/tools/ModuleAnalyser/moduleanalyserdialog.h"

//stat
#include "mainPanel/menu/stat/exchangesstatdialog.h"
#include "mainPanel/menu/stat/devicecountdialog.h"
#include "mainPanel/menu/stat/portsstatdialog.h"
#include "mainPanel/menu/stat/cardstatdialog.h"
#include "mainPanel/menu/stat/devicesstatdialog.h"

//LOG
#include "mainPanel/menu/log/recentLog/recentlogdialog.h"

//profile
#include "mainPanel/menu/profile/profiledialog.h"

//help
#include "mainPanel/menu/help/about/aboutdialog2.h"
#include "mainPanel/menu/help/recommendation/recommendationdialog.h"
#include "mainPanel/menu/help/regexHelp/regexphelpdialog.h"

// table index
#define INT_ID 0
#define EXCHANGE_ID 1
#define AREA_NO 2
#define SALOON_NO 3
#define DEVICE_ID 4
#define DEVEX_ID 5
#define SLOTS_ID 6
#define PIN_ID 7
#define AREA 8
#define ABBR 9
#define SALOON_NAME 10
#define DEVICE 11
#define INTERFACE_TYPE 12
#define INTERFACE 13
#define MODULE 14
#define LABEL 15
#define PEER_ABBR 16
#define PEER_DEVICE 17
#define PEER_INTERFACE 18
#define PEER_LABEL 19
#define ETH_TRUNK 20
#define DESC 21

/*
0`int_id`,1`exchange_id`,2`area_no`,3`saloon_no`,4`device_id`,5`devex_id`,6`slots_id`,7`pin_id`,
8`peer_exchange_id`,9`area`,10`abbr`,11`saloon_name`,12`device`,13 `interface_type`,14`interface`,
15`module`, 16`label`,17`peer_abbr`,18`peer_device`,19`peer_interface`,20`peer_label`,21`ether_trunk`
*/

MainPanel::MainPanel(DanetDbMan *db, bool adminUser, const bool mainDB, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainPanel),
    dbMan(db),
    portTVProxyModel(nullptr),
    portHeaderView(nullptr),
    mainDb(mainDB)
{
    ui->setupUi(this);

    QFontDatabase::addApplicationFont(":/font/tahoma.ttf");
    QFontDatabase::addApplicationFont(":/font/tahomabd.ttf");

    /// status bar
    recordCountLbl.setAlignment(Qt::AlignRight);
    userLbl.setAlignment(Qt::AlignLeft);
    recordCountLbl.setStyleSheet("QLabel{color:#00007f;}");
    userLbl.setStyleSheet("QLabel{color:#00007f;}");
    ui->statusBar->addPermanentWidget(&userLbl,0);
    ui->statusBar->addPermanentWidget(&recordCountLbl,1);
    userLbl.setText(dbMan->getLoggedinLastname());
    recordCountLbl.setText("Record Count: 0 ");

    ///// port table view

    portTVProxyModel = new SortFilterProxyModel(this);
    ui->portTV->setModel(portTVProxyModel);
    portHeaderView = new HeaderView(22,MODULE, ui->portTV);
    ui->portTV->setHorizontalHeader(portHeaderView);


    /*# INT_ID
# EXCHANGE_ID
# AREA_NO
# SALOON_NO
# DEVICE_ID
# DEVEX_ID
# DEVCARD_ID
# PIN_ID
# PEER_EXCHANGE_ID
*/
    portHeaderView->hideWidgets(INT_ID);
    portHeaderView->hideWidgets(EXCHANGE_ID);
    portHeaderView->hideWidgets(AREA_NO);
    portHeaderView->hideWidgets(SALOON_NO);
    portHeaderView->hideWidgets(DEVICE_ID);
    portHeaderView->hideWidgets(DEVEX_ID);
    portHeaderView->hideWidgets(SLOTS_ID);
    portHeaderView->hideWidgets(PIN_ID);
    portHeaderView->hideWidgets(INTERFACE_TYPE);

    portHeaderView->hideComboBox(AREA);
    portHeaderView->setLableText(AREA, "Area");
    portHeaderView->setLineEditPlaceHolder(AREA, "RegExp");

    portHeaderView->setLableText(ABBR, "Abbr");
    portHeaderView->hideComboBox(ABBR);
    portHeaderView->setLineEditPlaceHolder(ABBR, "RegExp");

    portHeaderView->setLableText(SALOON_NAME,"Saloon");
    portHeaderView->hideComboBox(SALOON_NAME);
    portHeaderView->setLineEditPlaceHolder(SALOON_NAME, "RegExp");

    portHeaderView->hideComboBox(DEVICE);
    portHeaderView->setLableText(DEVICE, "Device");
    portHeaderView->setLineEditPlaceHolder(DEVICE, "RegExp");

    portHeaderView->setLableText(INTERFACE_TYPE,"Interface\nType");
    portHeaderView->hideComboBox(INTERFACE_TYPE);
    portHeaderView->setLineEditPlaceHolder(INTERFACE_TYPE, "RegExp");

    portHeaderView->hideComboBox(INTERFACE);
    portHeaderView->setLableText(INTERFACE, "Interface");
    portHeaderView->setLineEditPlaceHolder(INTERFACE, "RegExp");

    //portHeaderView->hideLineEdit(MODULE);
    portHeaderView->setLableText(MODULE,"Module");
    portHeaderView->setLineEditPlaceHolder(MODULE, "RegExp");
    portHeaderView->hideComboBox(MODULE);

    portHeaderView->hideComboBox(LABEL);
    portHeaderView->setLableText(LABEL, "Label");
    portHeaderView->setLineEditPlaceHolder(LABEL, "RegExp");

    portHeaderView->hideComboBox(PEER_ABBR);
    portHeaderView->setLableText(PEER_ABBR, "Peer\nAbbr");
    portHeaderView->setLineEditPlaceHolder(PEER_ABBR, "RegExp");

    portHeaderView->hideComboBox(PEER_DEVICE);
    portHeaderView->setLableText(PEER_DEVICE, "Peer\nDevice");
    portHeaderView->setLineEditPlaceHolder(PEER_DEVICE, "RegExp");

    portHeaderView->hideComboBox(PEER_INTERFACE);
    portHeaderView->setLableText(PEER_INTERFACE, "Peer\nInterface");
    portHeaderView->setLineEditPlaceHolder(PEER_INTERFACE, "RegExp");

    portHeaderView->hideComboBox(PEER_LABEL);
    portHeaderView->setLableText(PEER_LABEL, "Peer\nLabel");
    portHeaderView->setLineEditPlaceHolder(PEER_LABEL, "RegExp");

    portHeaderView->hideComboBox(ETH_TRUNK);
    portHeaderView->setLableText(ETH_TRUNK, "Ether\nTrunk");
    portHeaderView->setLineEditPlaceHolder(ETH_TRUNK, "RegExp");

    portHeaderView->hideComboBox(DESC);
    portHeaderView->setLableText(DESC, "Description");
    portHeaderView->setLineEditPlaceHolder(DESC, "RegExp");


    workingArea = QString::number(dbMan->getLoggedInDefaultArea());

    fillPortTV(true);

    // context menu
    ui->portTV->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->portTV, SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(portContextMenuSlot(QPoint)));

    if(mainDb)
        connect(ui->portTV, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(portTV2ClickSlot(QModelIndex)));

    if(mainDb)
    {
        portContextMenu.addAction(ui->actionAssignInterface);
        portContextMenu.addAction(ui->actionModifyInterface);
        portContextMenu.addAction(ui->actionEmptyInterface);
        portContextMenu.addAction(ui->actionChange_Module);
        portContextMenu.addAction(ui->actionChange_Interface_Type);
        portContextMenu.addSeparator();
        portContextMenu.addAction(ui->actionViewODFPosition);
        portContextMenu.addAction(ui->actionChange_ODF);
        portContextMenu.addAction(ui->actionAssign_ODF);
        portContextMenu.addAction(ui->actionDetach_ODF);
        portContextMenu.addSeparator();
        portContextMenu.addAction(ui->actionReservePortToCommercialDep);
        portContextMenu.addSeparator();
        portContextMenu.addAction(ui->actionExchange_Info);
        portContextMenu.addAction(ui->actionLocalDevice);
        portContextMenu.addSeparator();
        portContextMenu.addAction(ui->actionExport_PDF);
        portContextMenu.addAction(ui->actionExport_Excel);
        portContextMenu.addSeparator();
        portContextMenu.addAction(ui->actionRefresh);
    }
    else
    {
        portContextMenu.addAction(ui->actionViewODFPosition);
        portContextMenu.addSeparator();
        portContextMenu.addAction(ui->actionExchange_Info);
        portContextMenu.addAction(ui->actionLocalDevice);
        portContextMenu.addSeparator();
        portContextMenu.addAction(ui->actionExport_PDF);
        portContextMenu.addAction(ui->actionExport_Excel);
        portContextMenu.addSeparator();
        portContextMenu.addAction(ui->actionRefresh);
    }


    QShortcut *clearShortcut = new QShortcut(Qt::Key_F + Qt::CTRL,this);
    connect(clearShortcut, SIGNAL(activated()), this, SLOT(clearFilter()));

    QShortcut *exportPdfShortcut = new QShortcut(Qt::Key_P + Qt::SHIFT,this);
    connect(exportPdfShortcut, SIGNAL(activated()), this, SLOT(on_actionExport_PDF_triggered()));

    if(mainDb)
    {
        /////  admin menu
        if(adminUser)
        {
            QMenu *adminMenu = new QMenu("&Admin",this);
            ui->menubar->addMenu(adminMenu);
            adminMenu->addAction(ui->actionAdminUsers);
            adminMenu->addSeparator();
            adminMenu->addAction(ui->actionView_Recommendations);
            adminMenu->addSeparator();
            adminMenu->addAction(ui->actionClear_LOG);
        }
        /////  Installation Menu
        QMenu *installationMenu = new QMenu("&Installation", this);
        ui->menubar->addMenu(installationMenu);
        installationMenu->addAction(ui->actionExchSiteMan);
        installationMenu->addAction(ui->actionOdfMan);
        installationMenu->addAction(ui->actionDeviceInstall);
        installationMenu->addAction(ui->actionInstallCard);

        /////  management menu
        QMenu *managementMenu = new QMenu("&Management", this);
        ui->menubar->addMenu(managementMenu);
        managementMenu->addAction(ui->actionManageDevice);
        managementMenu->addSeparator();
        managementMenu->addAction(ui->actionManageODF);
//        managementMenu->addAction(ui->actionLink_Batch_to_ODF);
//        managementMenu->addAction(ui->actionUnlik_Batch_ODF);
        managementMenu->addSeparator();
        managementMenu->addAction(ui->actionCommodity);
        managementMenu->addAction(ui->actionUninstalled_Commodities);
        managementMenu->addSeparator();
        managementMenu->addAction(ui->actionPurchase_Management);
        managementMenu->addAction(ui->actionInterface_Type);
        managementMenu->addAction(ui->actionSFP_Modules);
        managementMenu->addSeparator();
        managementMenu->addAction(ui->actionSaloon);
        managementMenu->addAction(ui->actionAdminDevices);
        managementMenu->addAction(ui->actionAdminCards);
        managementMenu->addSeparator();
        managementMenu->addAction(ui->actionLetters);
        managementMenu->addAction(ui->actionPhone_Numbers);

        /////  PLAN menu
        QMenu *planMenu = new QMenu("&Plan", this);
        ui->menubar->addMenu(planMenu);
        planMenu->addAction(ui->actionIPv4Plan);
        planMenu->addAction(ui->actionIPv6_Plan);
        planMenu->addSeparator();
        planMenu->addAction(ui->actionSystemNo);
        planMenu->addSeparator();
        planMenu->addAction(ui->actionConnection);
        planMenu->addAction(ui->actionBRAS_NAS_IP);
        planMenu->addAction(ui->actionDSLAMPlan);
        planMenu->addAction(ui->actionLocal_New_DSLAM_Plan);
        planMenu->addAction(ui->actionNew_SitePlan);
        planMenu->addAction(ui->actionRemove_DSLAM);
        planMenu->addSeparator();
        QMenu *accessSwPlanMenu = new QMenu("Access Switch Plan", this);
        planMenu->addMenu(accessSwPlanMenu);
        accessSwPlanMenu->addAction(ui->actionAccessBRASConfig);
        accessSwPlanMenu->addAction(ui->actionAccessDepSingleSwitchPlan);
        accessSwPlanMenu->addAction(ui->actionAccessDepDoubleSwitchPlan);
        planMenu->addSeparator();
        planMenu->addAction(ui->actionSBCIPLoopBackPlan);
        planMenu->addSeparator();
        planMenu->addAction(ui->actionReserve_To_Commercial_Dep);
        planMenu->addSeparator();
        QMenu *referenceMenu = new QMenu("Reference", this);
        planMenu->addMenu(referenceMenu);
        referenceMenu->addAction(ui->actionVLAN_Reference);
        referenceMenu->addAction(ui->actionRD_Reference);
        referenceMenu->addAction(ui->actionVSI_Reference);
        referenceMenu->addAction(ui->actionPVC_Reference);

        ui->actionIPv6_Plan->setEnabled(false);
    }
    ////// REPORT menu
    QMenu *reportMenu = new QMenu("&Report", this);
    ui->menubar->addMenu(reportMenu);
    reportMenu->addAction(ui->actionReportExchange_Devices);
    reportMenu->addAction(ui->actionReportFloorPlan);
    reportMenu->addAction(ui->actionReportExchange_Sites);
    reportMenu->addAction(ui->actionReportInterfaces);
    reportMenu->addAction(ui->actionReportODF);
    reportMenu->addSeparator();
    reportMenu->addAction(ui->actionReportNetwork_Pool);
    reportMenu->addAction(ui->actionReportSubnet_Pool);
    reportMenu->addAction(ui->actionReportIP_Pool);
    reportMenu->addSeparator();
    reportMenu->addAction(ui->actionLom);
    reportMenu->addSeparator();
    reportMenu->addAction(ui->actionReportDSLAM);
    reportMenu->addAction(ui->actionReportTotal_DSLAM_Plan);
    reportMenu->addSeparator();
    reportMenu->addAction(ui->actionCommercial_Reserved_Interfaces);

    ////// Statistics menu
    QMenu *statMenu = new QMenu("&Statistics", this);
    ui->menubar->addMenu(statMenu);
    statMenu->addAction(ui->actionExchanges_Sites_stat);
    statMenu->addAction(ui->actionDeviceCount);
    statMenu->addAction(ui->actionDevices_Status);
    statMenu->addAction(ui->actionCardsStat);
    statMenu->addAction(ui->actionPortsStat);

    if(mainDb)
    {
        ////// TOOLS menu
        QMenu *toolsMenu = new QMenu("&Tools", this);
        ui->menubar->addMenu(toolsMenu);
        toolsMenu->addAction(ui->actionNetwork_Calculator);
        toolsMenu->addAction(ui->actionNetwork_Assistant);
        toolsMenu->addAction(ui->actionConfiguration_Analyser);
        toolsMenu->addAction(ui->actionIP_Pool_Analyser);
        toolsMenu->addAction(ui->actionToolsModule_Analyser);
    }
    ////// LOG menu
    QMenu *logMenu = new QMenu("&LOG", this);
    ui->menubar->addMenu(logMenu);
    logMenu->addAction(ui->actionRecent_LOG);

    if(mainDb)
    {
        ////// PROFILE menu
        QMenu *profileMenu = new QMenu("Pro&file", this);
        ui->menubar->addMenu(profileMenu);
        profileMenu->addAction(ui->actionProfile);
    }

    ////// HELP menu
    QMenu *helpMenu = new QMenu("&Help", this);
    ui->menubar->addMenu(helpMenu);
    helpMenu->addAction(ui->actionAbout_DaNet);
    helpMenu->addAction(ui->actionRegex_Help);
    helpMenu->addAction(ui->actionRecommendation);

    connect(portHeaderView, SIGNAL(sectionsWidgetsSignal()), this, SLOT(portTVHeaderWidgetsChagedSlot()));

    QString dep = QString(DEPARTMENT);
    if(!(dep.compare("tarahi") == 0))
    {

        ui->actionAccessBRASConfig->setEnabled(false);
        ui->actionAccessDepDoubleSwitchPlan->setEnabled(false);
        ui->actionAccessDepSingleSwitchPlan->setEnabled(false);
        ui->actionAdminCards->setEnabled(false);
        ui->actionAdminDevices->setEnabled(false);
        ui->actionAdminUsers->setEnabled(false);
        ui->actionAssignInterface->setEnabled(false);
        ui->actionAssign_ODF->setEnabled(false);
        ui->actionBRAS_NAS_IP->setEnabled(false);
        ui->actionCardsStat->setEnabled(false);
        ui->actionChange_Interface_Type->setEnabled(false);
        ui->actionChange_Module->setEnabled(false);
        ui->actionChange_ODF->setEnabled(false);
        ui->actionClear_LOG->setEnabled(false);
        ui->actionCommercial_Reserved_Interfaces->setEnabled(false);
        ui->actionCommodity->setEnabled(false);
        ui->actionConnection->setEnabled(false);
        ui->actionDSLAMPlan->setEnabled(false);
        ui->actionDetach_ODF->setEnabled(false);
        ui->actionDeviceCount->setEnabled(false);
        ui->actionDeviceInstall->setEnabled(false);
        ui->actionDevices_Status->setEnabled(false);
        ui->actionEmptyInterface->setEnabled(false);
        ui->actionExchanges_Sites_stat->setEnabled(false);
        ui->actionExport_Excel->setEnabled(false);
        ui->actionIPv4Plan->setEnabled(false);
        ui->actionIPv6_Plan->setEnabled(false);
        ui->actionInstallCard->setEnabled(false);
        ui->actionInterface_Type->setEnabled(false);
        ui->actionLetters->setEnabled(false);
        ui->actionLink_Batch_to_ODF->setEnabled(false);
        ui->actionLocal_New_DSLAM_Plan->setEnabled(false);
        ui->actionLom->setEnabled(false);
        ui->actionManageDevice->setEnabled(false);
        ui->actionModifyInterface->setEnabled(false);
        ui->actionNew_SitePlan->setEnabled(false);
        ui->actionPVC_Reference->setEnabled(false);
        ui->actionPortsStat->setEnabled(false);
        ui->actionPurchase_Management->setEnabled(false);
        ui->actionRD_Reference->setEnabled(false);
        ui->actionRemove_DSLAM->setEnabled(false);
        ui->actionReportDSLAM->setEnabled(true);
        ui->actionReportExchange_Devices->setEnabled(true);
        ui->actionReportExchange_Sites->setEnabled(true);
        ui->actionReportFloorPlan->setEnabled(false);
        ui->actionReportIP_Pool->setEnabled(true);
        ui->actionReportInterfaces->setEnabled(true);
        ui->actionReportNetwork_Pool->setEnabled(true);
        ui->actionReportSubnet_Pool->setEnabled(true);
        ui->actionReportTotal_DSLAM_Plan->setEnabled(true);
        ui->actionReservePortToCommercialDep->setEnabled(false);
        ui->actionReserve_To_Commercial_Dep->setEnabled(false);
        ui->actionSBCIPLoopBackPlan->setEnabled(false);
        ui->actionSFP_Modules->setEnabled(false);
        ui->actionSaloon->setEnabled(false);
        ui->actionUninstalled_Commodities->setEnabled(false);
        ui->actionUnlik_Batch_ODF->setEnabled(false);
        ui->actionVLAN_Reference->setEnabled(false);
        ui->actionVSI_Reference->setEnabled(false);
        ui->actionRecent_LOG->setEnabled(false);
        ui->actionSystemNo->setEnabled(false);
        ui->actionReportIP_Pool->setEnabled(false);
        ui->actionReportNetwork_Pool->setEnabled(false);
        ui->actionReportSubnet_Pool->setEnabled(false);
        ui->actionReportTotal_DSLAM_Plan->setEnabled(false);

    }
}

MainPanel::~MainPanel()
{
    delete ui;
}

void MainPanel::fillPortTV(bool RESET)
{
    QString abbr = portHeaderView->getLineEditText(ABBR);

    QSqlQuery *query;
    query = dbMan->selectViewInterfaces(workingArea);


    /*
0`int_id`,1`exchange_id`,2`area_no`,3`saloon_no`,4`device_id`,5`devex_id`,6`slots_id`,7`pin_id`,
8`peer_exchange_id`,9`area`,10`abbr`,11`saloon_name`,12`device`,13 `interface_type`,14`interface`,
15`module`, 16`label`,17`peer_abbr`,18`peer_device`,19`peer_interface`,20`peer_label`,21`ether_trunk`
    */

    InterfaceModel *model = new InterfaceModel(this);
    model->setSpecificColumn(14);
    model->setQuery(*query);
    portTVProxyModel->setSourceModel(model);
    portTVProxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);

    ui->portTV->hideColumn(INT_ID);
    ui->portTV->hideColumn(EXCHANGE_ID);
    ui->portTV->hideColumn(AREA_NO);
    ui->portTV->hideColumn(SALOON_NO);
    ui->portTV->hideColumn(DEVICE_ID);
    ui->portTV->hideColumn(DEVEX_ID);
    ui->portTV->hideColumn(SLOTS_ID);
    ui->portTV->hideColumn(PIN_ID);
    ui->portTV->hideColumn(INTERFACE_TYPE);

    portHeaderView->blockSignals(true);
    if(RESET)
    {
        portHeaderView->resetHeaderWidgets();
    }
    portHeaderView->blockSignals(false);

    portHeaderView->setLineEditBlockSignal(AREA, true); //setComboBoxBlockSignal(AREA,true);
    portHeaderView->setLineEditText(AREA,workingArea);
    portHeaderView->setLineEditBlockSignal(AREA, false);


    portHeaderView->setLineEditBlockSignal(ABBR, true);
    if(RESET)
        portHeaderView->setLineEditText(ABBR, "");
    else
        portHeaderView->setLineEditText(ABBR, abbr);
    portHeaderView->setLineEditBlockSignal(ABBR, false);

    portTVHeaderWidgetsChagedSlot();


    ui->portTV->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    //ui->portTV->horizontalHeader()->setSectionResizeMode(PEER_LABEL,QHeaderView::Stretch);
    ui->portTV->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->portTV->verticalHeader()->hide();
    recordCountLbl.setText("Record Count: "+QString::number(portTVProxyModel->rowCount())+" ");
}

void MainPanel::selectRowSlot(int i)
{
    i++;
    ui->portTV->selectRow(clickedRow);
}

void MainPanel::refreshSlot(int i)
{    i++;
     fillPortTV();
      ui->portTV->selectRow(clickedRow);
}

void MainPanel::portContextMenuSlot(QPoint ptr)
{   
    /*
0`int_id`,1`exchange_id`,2`area_no`,3`saloon_no`,4`device_id`,5`devex_id`,6`slots_id`,7`pin_id`,
8`peer_exchange_id`,9`area`,10`abbr`,11`saloon_name`,12`device`,13 `interface_type`,14`interface`,
15`module`, 16`label`,17`peer_abbr`,18`peer_device`,19`peer_interface`,20`peer_label`,21`ether_trunk`
    */
    QModelIndex clickedIndex = ui->portTV->indexAt(ptr);
    ptr.setY(ptr.y() + 80);

    clickedIndex = clickedIndex.sibling(clickedIndex.row(), INT_ID);
    clickedIntId = portTVProxyModel->data(clickedIndex).toInt();
    clickedRow = clickedIndex.row();


    if( clickedIntId > 0 )
    {

        clickedIndex = clickedIndex.sibling(clickedIndex.row(), EXCHANGE_ID); // exch id
        clickedExchId = portTVProxyModel->data(clickedIndex).toInt();

        clickedIndex = clickedIndex.sibling(clickedIndex.row(), AREA_NO); // area no
        clickedAreaNo = portTVProxyModel->data(clickedIndex).toInt();

        clickedIndex = clickedIndex.sibling(clickedIndex.row(), SALOON_NO); // saloon no
        clickedSaloonNo = portTVProxyModel->data(clickedIndex).toInt();

        clickedIndex = clickedIndex.sibling(clickedIndex.row(), DEVICE_ID); // device
        clickedDeviceId = portTVProxyModel->data(clickedIndex).toInt();

        clickedIndex = clickedIndex.sibling(clickedIndex.row(), DEVEX_ID);
        clickedDevexId = portTVProxyModel->data(clickedIndex).toInt();

        clickedIndex = clickedIndex.sibling(clickedIndex.row(), PIN_ID); // pin id
        if(portTVProxyModel->data(clickedIndex).isNull())
            clickedPinId = -1;
        else
            clickedPinId = portTVProxyModel->data(clickedIndex).toInt();

        clickedIndex = clickedIndex.sibling(clickedIndex.row(), ABBR); // abbr
        clickedAbbr = portTVProxyModel->data(clickedIndex).toString();

        clickedIndex = clickedIndex.sibling(clickedIndex.row(), SALOON_NAME); // saloon
        clickedSaloon = portTVProxyModel->data(clickedIndex).toString();

        clickedIndex = clickedIndex.sibling(clickedIndex.row(), DEVICE); // device
        clickedDevice = portTVProxyModel->data(clickedIndex).toString();

        clickedIndex = clickedIndex.sibling(clickedIndex.row(), INTERFACE_TYPE); // pint type
        clickedIntType = portTVProxyModel->data(clickedIndex).toString();

        clickedIndex = clickedIndex.sibling(clickedIndex.row(), INTERFACE);
        clickedInt = portTVProxyModel->data(clickedIndex).toString();

        clickedIndex = clickedIndex.sibling(clickedIndex.row(), MODULE); // module
        clickedModule = portTVProxyModel->data(clickedIndex).toString();

        clickedIndex = clickedIndex.sibling(clickedIndex.row(), LABEL);
        clickedLabel = portTVProxyModel->data(clickedIndex).toString();

        clickedIndex = clickedIndex.sibling(clickedIndex.row(), PEER_ABBR); // peer exch
        clickedPeerAbbr = portTVProxyModel->data(clickedIndex).toString();

        clickedIndex = clickedIndex.sibling(clickedIndex.row(), PEER_DEVICE); // peer device
        clickedPeerDevice = portTVProxyModel->data(clickedIndex).toString();

        clickedIndex = clickedIndex.sibling(clickedIndex.row(), PEER_INTERFACE);
        clickedPeerInt = portTVProxyModel->data(clickedIndex).toString();

        clickedIndex = clickedIndex.sibling(clickedIndex.row(), PEER_LABEL); // peer label
        clickedPeerLabel = portTVProxyModel->data(clickedIndex).toString();

        clickedIndex = clickedIndex.sibling(clickedIndex.row(), ETH_TRUNK); // ether trunk
        if(portTVProxyModel->data(clickedIndex).isNull())
            clickedEther = "";
        else
            clickedEther = portTVProxyModel->data(clickedIndex).toString();

        clickedIndex = clickedIndex.sibling(clickedIndex.row(), DESC); // peer label
        clickedDesc = portTVProxyModel->data(clickedIndex).toString();

        ui->actionExchange_Info->setEnabled(true);
        ui->actionChange_Module->setEnabled(true);


        //odf-rj45
        bool elec = (clickedIntType.contains("RJ45", Qt::CaseInsensitive))?true:false;
        if(elec)
        {
            ui->actionViewODFPosition->setEnabled(false);
            ui->actionChange_ODF->setEnabled(false);
            ui->actionAssign_ODF->setEnabled(false);
            ui->actionDetach_ODF->setEnabled(false);
        }
        else
        { //optical
            //has pin
            if(clickedPinId > -1)
            {
                ui->actionViewODFPosition->setEnabled(true);
                ui->actionChange_ODF->setEnabled(true);
                ui->actionAssign_ODF->setEnabled(false);
                ui->actionDetach_ODF->setEnabled(true);
            }
            else
            {
                ui->actionViewODFPosition->setEnabled(false);
                ui->actionChange_ODF->setEnabled(false);
                ui->actionAssign_ODF->setEnabled(true);
                ui->actionDetach_ODF->setEnabled(false);
            }
        }


        //
        if(!clickedPeerDevice.isEmpty() || !clickedPeerAbbr.isEmpty() ) //assigned
        {
            ui->actionAssignInterface->setEnabled(false);
            ui->actionModifyInterface->setEnabled(true);
            ui->actionEmptyInterface->setEnabled(true);
            ui->actionLocalDevice->setEnabled(true);
            ui->actionReservePortToCommercialDep->setEnabled(false);
        }
        else
        {
            ui->actionAssignInterface->setEnabled(true);
            ui->actionModifyInterface->setEnabled(false);
            ui->actionEmptyInterface->setEnabled(false);
            ui->actionLocalDevice->setEnabled(true);
            ui->actionReservePortToCommercialDep->setEnabled(true);
        }
    }
    else
    {
        ui->actionAssignInterface->setEnabled(false);
        ui->actionModifyInterface->setEnabled(false);
        ui->actionEmptyInterface->setEnabled(false);
        ui->actionLocalDevice->setEnabled(false);
        ui->actionViewODFPosition->setEnabled(false);
        ui->actionExchange_Info->setEnabled(false);
        ui->actionChange_Module->setEnabled(false);
        ui->actionAssign_ODF->setEnabled(false);
        ui->actionDetach_ODF->setEnabled(false);
    }

    portContextMenu.popup(ui->portTV->mapToGlobal(ptr));

}

void MainPanel::portTV2ClickSlot(QModelIndex indx)
{
    indx = indx.sibling(indx.row(), INT_ID);
    clickedRow = indx.row();
    clickedIntId = portTVProxyModel->data(indx).toInt();
    indx = indx.sibling(indx.row(), PEER_DEVICE);
    clickedPeerDevice = portTVProxyModel->data(indx).toString();

    indx = indx.sibling(indx.row(), DEVEX_ID);
    clickedDevexId = portTVProxyModel->data(indx).toInt();

    //    if(!dbMan->isDslamPlannedInterface(clickedIntId, clickedDevexId))
    //    {
    indx = indx.sibling(indx.row(), ABBR);
    clickedAbbr = portTVProxyModel->data(indx).toString();
    indx = indx.sibling(indx.row(), SALOON_NAME);
    clickedSaloon = portTVProxyModel->data(indx).toString();
    indx = indx.sibling(indx.row(), DEVICE);
    clickedDevice = portTVProxyModel->data(indx).toString();
    indx = indx.sibling(indx.row(), INTERFACE_TYPE);
    clickedIntType = portTVProxyModel->data(indx).toString();
    indx = indx.sibling(indx.row(), INTERFACE);
    clickedInt = portTVProxyModel->data(indx).toString();
    indx = indx.sibling(indx.row(), MODULE);
    clickedModule = portTVProxyModel->data(indx).toString();
    indx = indx.sibling(indx.row(), LABEL);
    clickedLabel = portTVProxyModel->data(indx).toString();
    indx = indx.sibling(indx.row(), PEER_ABBR);
    clickedPeerAbbr = portTVProxyModel->data(indx).toString();
    indx = indx.sibling(indx.row(), PEER_INTERFACE);
    clickedPeerInt = portTVProxyModel->data(indx).toString();
    indx = indx.sibling(indx.row(), PEER_LABEL);
    clickedPeerLabel = portTVProxyModel->data(indx).toString();
    indx = indx.sibling(indx.row(), ETH_TRUNK);
    if(portTVProxyModel->data(indx).isNull())
        clickedEther="";
    else
        clickedEther = portTVProxyModel->data(indx).toString();


    indx = indx.sibling(indx.row(), DESC);
    clickedDesc = portTVProxyModel->data(indx).toString();

    if(!clickedPeerDevice.isEmpty() || !clickedPeerAbbr.isEmpty() ) //assigned
    {
        EditInterfaceDialog *editInterfaceDialog = new EditInterfaceDialog(this, dbMan,clickedIntId,clickedDevexId,clickedDevice,
                                                                           clickedInt,clickedLabel,clickedModule, clickedEther, clickedPeerAbbr, clickedPeerDevice,clickedPeerInt, clickedPeerLabel,clickedDesc);

        editInterfaceDialog->setAttribute(Qt::WA_DeleteOnClose);

        connect(editInterfaceDialog,SIGNAL(finished(int)), this,SLOT(refreshSlot(int)));
        editInterfaceDialog->show();
    }
    else
    {
        AssignInterfaceDialog *assignInterfaceDialog = new AssignInterfaceDialog(this, dbMan, clickedIntId,clickedDevice, clickedInt,clickedLabel,clickedModule);

        assignInterfaceDialog->setAttribute(Qt::WA_DeleteOnClose);

        connect(assignInterfaceDialog,SIGNAL(finished(int)), this,SLOT(refreshSlot(int)));
        assignInterfaceDialog->show();
    }
    //    }
}

void MainPanel::on_actionAssignInterface_triggered()
{
    clickedLabel = clickedLabel.replace(" ","");
    AssignInterfaceDialog *assignInterfaceDialog = new AssignInterfaceDialog(this, dbMan,clickedIntId,clickedDevice,                                                      clickedInt,clickedLabel,clickedModule);

    assignInterfaceDialog->setAttribute(Qt::WA_DeleteOnClose);

    connect(assignInterfaceDialog,SIGNAL(finished(int)), this,SLOT(refreshSlot(int)));
    assignInterfaceDialog->show();
}

void MainPanel::on_actionEmptyInterface_triggered()
{
    EmptyInterfaceDialog *emptyInterfaceDialog = new EmptyInterfaceDialog(this, dbMan,clickedIntId, clickedDevexId, clickedDevice,
                                                                          clickedInt,clickedLabel,clickedModule, clickedEther, clickedPeerAbbr, clickedPeerDevice,clickedPeerInt, clickedPeerLabel, clickedDesc);

    emptyInterfaceDialog->setAttribute(Qt::WA_DeleteOnClose);

    connect(emptyInterfaceDialog,SIGNAL(finished(int)), this,SLOT(refreshSlot(int)));
    emptyInterfaceDialog->show();
}

void MainPanel::on_actionModifyInterface_triggered()
{
    clickedLabel = clickedLabel.replace(" ","");
    EditInterfaceDialog *editInterfaceDialog = new EditInterfaceDialog(this, dbMan,clickedIntId,clickedDevexId, clickedDevice,clickedInt,clickedLabel,clickedModule, clickedEther,clickedPeerAbbr, clickedPeerDevice,clickedPeerInt, clickedPeerLabel, clickedDesc);

    editInterfaceDialog->setAttribute(Qt::WA_DeleteOnClose);

    QObject::connect(editInterfaceDialog,SIGNAL(finished(int)), this,SLOT(refreshSlot(int)));
    editInterfaceDialog->show();
}

void MainPanel::on_actionViewODFPosition_triggered()
{
    int posId = dbMan->getPosId(clickedPinId);
    int odfId = dbMan->getOdfId(posId);

    ODFManagementDialog *odfManagementDialog = new ODFManagementDialog(this, dbMan,mainDb,clickedAreaNo,clickedExchId,odfId, posId,clickedPinId);

    odfManagementDialog->setAttribute(Qt::WA_DeleteOnClose);

    connect(odfManagementDialog,SIGNAL(finished(int)), this,SLOT(selectRowSlot(int)));
    odfManagementDialog->showMaximized();
}

void MainPanel::on_actionLocalDevice_triggered()
{
    LocalDeviceDialog *localDeviceDialog = new LocalDeviceDialog(this, dbMan, clickedDeviceId,clickedAbbr,clickedSaloon,clickedDevice, clickedDevexId);
    localDeviceDialog->setAttribute(Qt::WA_DeleteOnClose);
    QObject::connect(localDeviceDialog,SIGNAL(finished(int)), this,SLOT(selectRowSlot(int)));
    localDeviceDialog->showMaximized();
}

//admin
void MainPanel::on_actionAdminUsers_triggered()
{
    UserManDialog *userManDialog = new UserManDialog(this, dbMan);
    userManDialog->setAttribute(Qt::WA_DeleteOnClose);
    userManDialog->showMaximized();
}

void MainPanel::on_actionAdminDevices_triggered()
{
    DeviceManDialog *deviceManDialog = new DeviceManDialog(this, dbMan);
    deviceManDialog->setAttribute(Qt::WA_DeleteOnClose);
    connect(deviceManDialog,SIGNAL(finished(int)), this,SLOT(refreshSlot(int)));
    deviceManDialog->showMaximized();
}

void MainPanel::on_actionAdminCards_triggered()
{
    CardManDialog *cardManDialog = new CardManDialog(this, dbMan);
    cardManDialog->setAttribute(Qt::WA_DeleteOnClose);
    connect(cardManDialog,SIGNAL(finished(int)), this,SLOT(refreshSlot(int)));
    cardManDialog->showMaximized();
}
//installation
void MainPanel::on_actionOdfMan_triggered()
{
    OdfManDialog *odfManDialog = new OdfManDialog(this, dbMan);
    odfManDialog->setAttribute(Qt::WA_DeleteOnClose);
    odfManDialog->showMaximized();
}

void MainPanel::on_actionDeviceInstall_triggered()
{
    DeviceInstallDialog  *deviceInstallDialog = new DeviceInstallDialog(this, dbMan);
    deviceInstallDialog->setAttribute(Qt::WA_DeleteOnClose);
    connect(deviceInstallDialog, SIGNAL(finished(int)), this, SLOT(refreshSlot(int)));
    deviceInstallDialog->showMaximized();
}

void MainPanel::portTVHeaderWidgetsChagedSlot()
{

    QString Area,Abbr, Saloon,Device,IntType,Interface,Module,Label,PeerExch,PeerDevice,PeerInt, PeerLabel, EtherTrunk, Desc;
    Area = portHeaderView->sections[AREA]->lineEdit->text();

    if(Area.compare(workingArea, Qt::CaseInsensitive))
    {
        workingArea = Area;
        fillPortTV(true);
    }

    Abbr = portHeaderView->sections[ABBR]->lineEdit->text();
    if(Abbr.compare("") == 0) Abbr = "abbr";
    Saloon = portHeaderView->sections[SALOON_NAME]->lineEdit->text();
    Device = portHeaderView->sections[DEVICE]->lineEdit->text();
    IntType = portHeaderView->sections[INTERFACE_TYPE]->lineEdit->text();//->comboBox->currentText();
    Interface = portHeaderView->sections[INTERFACE]->lineEdit->text();
    Module = portHeaderView->sections[MODULE]->lineEdit->text();
    Label = portHeaderView->sections[LABEL]->lineEdit->text();
    PeerExch = portHeaderView->sections[PEER_ABBR]->lineEdit->text();
    PeerDevice = portHeaderView->sections[PEER_DEVICE]->lineEdit->text();
    PeerInt = portHeaderView->sections[PEER_INTERFACE]->lineEdit->text();
    PeerLabel = portHeaderView->sections[PEER_LABEL]->lineEdit->text();
    EtherTrunk = portHeaderView->sections[ETH_TRUNK]->lineEdit->text();
    Desc = portHeaderView->sections[DESC]->lineEdit->text();

    QMap<int, QString> mapString;
    QMap<int, QRegExp> mapRegExp;
    QRegExp reg;
    reg.setCaseSensitivity(Qt::CaseInsensitive);

    if(!Abbr.isEmpty())
    {
        reg.setPattern(Abbr);
        mapRegExp.insert(ABBR, reg);
    }
    if(!Saloon.isEmpty())
    {
        reg.setPattern(Saloon);
        mapRegExp.insert(SALOON_NAME, reg);
    }
    if(!Device.isEmpty())
    {
        reg.setPattern(Device);
        mapRegExp.insert(DEVICE, reg);
    }
    if(!IntType.isEmpty())
    {
        reg.setPattern(IntType);
        mapRegExp.insert(INTERFACE_TYPE, reg);
    }
    if(!Interface.isEmpty())
    {
        //Interface = QRegExp::escape(Interface);
        reg.setPattern(Interface);
        mapRegExp.insert(INTERFACE, reg);
    }
    if(!Module.isEmpty())
    {
        reg.setPattern(Module);
        mapRegExp.insert(MODULE, reg);
    }
    if(!Label.isEmpty())
    {
        //Label = QRegExp::escape(Label);
        reg.setPattern(Label);
        mapRegExp.insert(LABEL, reg);
    }
    if(!PeerExch.isEmpty())
    {
        //PeerExch = QRegExp::escape(PeerExch);
        reg.setPattern(PeerExch);
        mapRegExp.insert(PEER_ABBR, reg);
    }
    if(!PeerDevice.isEmpty())
    {
        //PeerDevice = QRegExp::escape(PeerDevice);
        reg.setPattern(PeerDevice);
        mapRegExp.insert(PEER_DEVICE, reg);
    }
    if(!PeerInt.isEmpty())
    {
        //PeerInt = QRegExp::escape(PeerInt);
        reg.setPattern(PeerInt);
        mapRegExp.insert(PEER_INTERFACE, reg);
    }
    if(!PeerLabel.isEmpty())
    {
        //PeerLabel = QRegExp::escape(PeerLabel);
        reg.setPattern(PeerLabel);
        mapRegExp.insert(PEER_LABEL, reg);
    }
    if(!EtherTrunk.isEmpty())
    {
        //EtherTrunk = QRegExp::escape(EtherTrunk);
        reg.setPattern(EtherTrunk);
        mapRegExp.insert(ETH_TRUNK, reg);
    }
    if(!Desc.isEmpty())
    {
        //EtherTrunk = QRegExp::escape(EtherTrunk);
        reg.setPattern(Desc);
        mapRegExp.insert(DESC, reg);
    }

    portTVProxyModel->setColumnsFilter(mapString, mapRegExp);

    ui->portTV->hideColumn(INT_ID);
    ui->portTV->hideColumn(EXCHANGE_ID);
    ui->portTV->hideColumn(AREA_NO);
    ui->portTV->hideColumn(SALOON_NO);
    ui->portTV->hideColumn(DEVICE_ID);
    ui->portTV->hideColumn(DEVEX_ID);
    ui->portTV->hideColumn(SLOTS_ID);
    ui->portTV->hideColumn(PIN_ID);

    ui->portTV->horizontalHeader()->setSectionResizeMode(LABEL,QHeaderView::Stretch);
    ui->portTV->horizontalHeader()->setSectionResizeMode(PEER_LABEL,QHeaderView::Stretch);
    ui->portTV->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->portTV->verticalHeader()->hide();

    recordCountLbl.setText("Record Count: "+QString::number(portTVProxyModel->rowCount())+" ");
}

void MainPanel::on_actionExchSiteMan_triggered()
{
    ExchangeSiteManDialog *exchangeSiteManDialog = new ExchangeSiteManDialog(this, dbMan);
    exchangeSiteManDialog->setAttribute(Qt::WA_DeleteOnClose);
    connect(exchangeSiteManDialog, SIGNAL(finished(int)), this, SLOT(refreshSlot(int)));
    exchangeSiteManDialog->showMaximized();
}

void MainPanel::on_actionRefresh_triggered()
{
    fillPortTV(false);
    ui->portTV->selectRow(clickedRow);
}

// management
void MainPanel::on_actionManageDevice_triggered()
{
    DeviceManagementDialog *deviceManagementDialog = new DeviceManagementDialog(this, dbMan);
    deviceManagementDialog->setAttribute(Qt::WA_DeleteOnClose);
    connect(deviceManagementDialog, SIGNAL(finished(int)), this, SLOT(refreshSlot(int)));
    deviceManagementDialog->showMaximized();
}

void MainPanel::on_actionLetters_triggered()
{
    LettersDialog *lettersDialog = new LettersDialog(dbMan, this);
    lettersDialog->setAttribute(Qt::WA_DeleteOnClose);
    lettersDialog->showMaximized();
}

void MainPanel::on_actionPhone_Numbers_triggered()
{
    TelephonesDialog *telephonesDialog= new TelephonesDialog(dbMan,this);
    telephonesDialog->setAttribute(Qt::WA_DeleteOnClose);
    telephonesDialog->showMaximized();
}

void MainPanel::on_actionCommodity_triggered()
{
    Commodity::Cat::Dialog *commodityCatDialog = new Commodity::Cat::Dialog(dbMan, this);
    commodityCatDialog->setAttribute(Qt::WA_DeleteOnClose);
    commodityCatDialog->show();
}

void MainPanel::on_actionUninstalled_Commodities_triggered()
{
    UninstalledDialog *uninstalledDialog = new UninstalledDialog(dbMan, this);
    uninstalledDialog->setAttribute(Qt::WA_DeleteOnClose);
    uninstalledDialog->showMaximized();
}

void MainPanel::on_actionPurchase_Management_triggered()
{
    PurchaseManDialog *purchaseManDialog = new PurchaseManDialog(this, dbMan);
    purchaseManDialog->setAttribute(Qt::WA_DeleteOnClose);
    purchaseManDialog->show();
}

void MainPanel::on_actionSFP_Modules_triggered()
{
    ModulesManDialog *modulesManDialog = new ModulesManDialog(this, dbMan);
    modulesManDialog->setAttribute(Qt::WA_DeleteOnClose);
    connect(modulesManDialog, SIGNAL(finished(int)), this, SLOT(refreshSlot(int)));
    modulesManDialog->show();
}

void MainPanel::on_actionManageODF_triggered()
{
    ODFManagementDialog *odfManagementDialog = new ODFManagementDialog(this, dbMan);
    odfManagementDialog->setAttribute(Qt::WA_DeleteOnClose);
    odfManagementDialog->showMaximized();
}
//plan
void MainPanel::on_actionIPv4Plan_triggered()
{
    NetV4PoolDialog *netV4PoolDialog = new NetV4PoolDialog(this, dbMan);
    netV4PoolDialog->setAttribute(Qt::WA_DeleteOnClose);
    netV4PoolDialog->showMaximized();
}

void MainPanel::on_actionReserve_To_Commercial_Dep_triggered()
{
    ReserveToCommercialDialog *reserveToCommercialDialog = new ReserveToCommercialDialog(this, dbMan);
    reserveToCommercialDialog->setAttribute(Qt::WA_DeleteOnClose);
    connect(reserveToCommercialDialog,SIGNAL(finished(int)), this,SLOT(refreshSlot(int)));
    reserveToCommercialDialog->show();
}

void MainPanel::on_actionConnection_triggered()
{
    ConnectionDialog *connectionDialog = new ConnectionDialog(this, dbMan);
    connectionDialog->setAttribute(Qt::WA_DeleteOnClose);
    connectionDialog->showMaximized();
}

void MainPanel::on_actionDSLAMPlan_triggered()
{
    DslamDialog *dslamDialog = new DslamDialog(this, dbMan);
    dslamDialog->setAttribute(Qt::WA_DeleteOnClose);
    QObject::connect(dslamDialog,SIGNAL(finished(int)), this,SLOT(refreshSlot(int)));
    dslamDialog->showMaximized();
}

void MainPanel::on_actionAccessDepSingleSwitchPlan_triggered()
{
    AccessSingleSwitchDialog *accessSingleSwitchDialog = new AccessSingleSwitchDialog(this,dbMan);
    accessSingleSwitchDialog->setAttribute(Qt::WA_DeleteOnClose);
    QObject::connect(accessSingleSwitchDialog,SIGNAL(finished(int)), this,SLOT(refreshSlot(int)));
    accessSingleSwitchDialog->show();
}

void MainPanel::on_actionAccessDepDoubleSwitchPlan_triggered()
{
    AccessDoubleSwitchDialog *accessDoubleSwitchDialog = new AccessDoubleSwitchDialog(this,dbMan);
    accessDoubleSwitchDialog->setAttribute(Qt::WA_DeleteOnClose);
    connect(accessDoubleSwitchDialog,SIGNAL(finished(int)), this,SLOT(refreshSlot(int)));
    accessDoubleSwitchDialog->show();
}

void MainPanel::on_actionBRAS_NAS_IP_triggered()
{
    BrasNasIpManDialog *brasNasIpManDialog = new BrasNasIpManDialog(this, dbMan);
    brasNasIpManDialog->setAttribute(Qt::WA_DeleteOnClose);
    brasNasIpManDialog->show();
}

void MainPanel::on_actionInstallCard_triggered()
{
    CardInstallDialog *cardInstallDialog = new CardInstallDialog(this, dbMan);
    cardInstallDialog->setAttribute(Qt::WA_DeleteOnClose);
    QObject::connect(cardInstallDialog,SIGNAL(finished(int)), this,SLOT(refreshSlot(int)));
    cardInstallDialog->showMaximized();
}

void MainPanel::on_actionChange_Module_triggered()
{
    QString abbr = QString::number(clickedAreaNo)+"-"+clickedAbbr;

    ChangeModuleDialog *changeModuleDialog = new ChangeModuleDialog(this, dbMan,clickedIntId,abbr, clickedSaloon, clickedDevice, clickedIntType, clickedInt, clickedModule);
    changeModuleDialog->setAttribute(Qt::WA_DeleteOnClose);
    connect(changeModuleDialog,SIGNAL(finished(int)), this,SLOT(refreshSlot(int)));
    changeModuleDialog->show();
}

void MainPanel::on_actionSBCIPLoopBackPlan_triggered()
{
    SbcIpLoopBackDialog *sbcIpLoopBackDialog = new SbcIpLoopBackDialog(this, dbMan);
    sbcIpLoopBackDialog->setAttribute(Qt::WA_DeleteOnClose);
    sbcIpLoopBackDialog->show();
}

void MainPanel::on_actionExchange_Info_triggered()
{
    ExchangeInfoDialog *exchangeInfoDialog = new ExchangeInfoDialog(this,dbMan,clickedExchId);
    exchangeInfoDialog->setAttribute(Qt::WA_DeleteOnClose);
    connect(exchangeInfoDialog,SIGNAL(finished(int)), this,SLOT(selectRowSlot(int)));
    exchangeInfoDialog->show();
}

// REPORT
void MainPanel::on_actionReportInterfaces_triggered()
{
    InterfaceReportDialog *interfaceReportDialog = new InterfaceReportDialog(this, dbMan);
    interfaceReportDialog->setAttribute(Qt::WA_DeleteOnClose);
    interfaceReportDialog->showMaximized();
}

void MainPanel::on_actionReportODF_triggered()
{
    OdfReportDialog *odfReportDialog = new OdfReportDialog(this, dbMan);
    odfReportDialog->setAttribute(Qt::WA_DeleteOnClose);
    odfReportDialog->showMaximized();
}

void MainPanel::on_actionExport_PDF_triggered()
{
    QList<int> intIds, pinIds, posIds;
    QModelIndex ind = portTVProxyModel->index(0,0);
    for(int row = 0; row < portTVProxyModel->rowCount(); row++)
    {
        ind = ind.sibling(row, INT_ID);
        intIds << portTVProxyModel->data(ind).toInt();
        ind = ind.sibling(row, PIN_ID);
        pinIds << portTVProxyModel->data(ind).toInt();
    }
    posIds = dbMan->getPosIds(pinIds);

    ExportPdfDialog *exportPdfDialog = new ExportPdfDialog(this,dbMan, intIds, posIds, pinIds);
    exportPdfDialog->setAttribute(Qt::WA_DeleteOnClose);
    exportPdfDialog->showMaximized();
}

void MainPanel::on_actionReportExchange_Sites_triggered()
{
    ExchangeSitesReportDialog *exchangeSitesReportDialog = new ExchangeSitesReportDialog(this, dbMan);
    exchangeSitesReportDialog->setAttribute(Qt::WA_DeleteOnClose);
    exchangeSitesReportDialog->showMaximized();
}

void MainPanel::on_actionReportFloorPlan_triggered()
{
    FloorPlanReportDialog *floorPlanReportDialog = new FloorPlanReportDialog(this, dbMan);
    floorPlanReportDialog->setAttribute(Qt::WA_DeleteOnClose);
    floorPlanReportDialog->showMaximized();
}

void MainPanel::on_actionReportExchange_Devices_triggered()
{
    ReportDeviceDialog *reportDeviceDialog = new ReportDeviceDialog(this, dbMan);
    reportDeviceDialog->setAttribute(Qt::WA_DeleteOnClose);
    reportDeviceDialog->showMaximized();

}

void MainPanel::on_actionReportNetwork_Pool_triggered()
{
    ReportNetworkV4Dialog *reportNetworkV4Dialog = new ReportNetworkV4Dialog(this, dbMan);
    reportNetworkV4Dialog->setAttribute(Qt::WA_DeleteOnClose);
    reportNetworkV4Dialog->showMaximized();
}

void MainPanel::on_actionReportSubnet_Pool_triggered()
{
    ReportSubnetV4Dialog *reportSubnetV4Dialog = new ReportSubnetV4Dialog(this, dbMan);
    reportSubnetV4Dialog->setAttribute(Qt::WA_DeleteOnClose);
    reportSubnetV4Dialog->showMaximized();
}

void MainPanel::on_actionReportIP_Pool_triggered()
{
    ReportIpV4Dialog *reportIpV4Dialog = new ReportIpV4Dialog(this, dbMan);
    reportIpV4Dialog->setAttribute(Qt::WA_DeleteOnClose);
    reportIpV4Dialog->showMaximized();
}

void MainPanel::on_actionNetwork_Calculator_triggered()
{
    NetworkCalculatorDialog *networkCalculatorDialog = new NetworkCalculatorDialog(this);
    networkCalculatorDialog->setAttribute(Qt::WA_DeleteOnClose);
    networkCalculatorDialog->showMaximized();
}

void MainPanel::on_actionReportDSLAM_triggered()
{
    ReportDSLAMDialog  *reportDSLAMDialog = new ReportDSLAMDialog(this, dbMan);
    reportDSLAMDialog->setAttribute(Qt::WA_DeleteOnClose);
    reportDSLAMDialog->showMaximized();
}

void MainPanel::on_actionIP_Pool_Analyser_triggered()
{
    IpPoolAnalyserDialog *ipPoolAnalyserDialog = new IpPoolAnalyserDialog(this);
    ipPoolAnalyserDialog->setAttribute(Qt::WA_DeleteOnClose);
    ipPoolAnalyserDialog->show();
}

void MainPanel::on_actionConfiguration_Analyser_triggered()
{
    ConfigurationAnalyserDialog *configurationAnalyserDialog = new ConfigurationAnalyserDialog(this);
    configurationAnalyserDialog->setAttribute(Qt::WA_DeleteOnClose);
    configurationAnalyserDialog->showMaximized();
}

void MainPanel::on_actionNetwork_Assistant_triggered()
{
    NetworkAssistantDialog *networkAssistantDialog = new NetworkAssistantDialog(this);
    networkAssistantDialog->setAttribute(Qt::WA_DeleteOnClose);
    networkAssistantDialog->showMaximized();
}

void MainPanel::on_actionRecent_LOG_triggered()
{
    RecentLogDialog *recentLogDialog = new RecentLogDialog(this, dbMan);
    recentLogDialog->setAttribute(Qt::WA_DeleteOnClose);
    recentLogDialog->showMaximized();
}

void MainPanel::on_actionProfile_triggered()
{
    ProfileDialog profileDialog(this, dbMan);
    profileDialog.exec();
    workingArea = dbMan->getLoggedInDefaultArea();
    portHeaderView->setComboBoxCurrentData(AREA,workingArea);
    fillPortTV(true);
}

void MainPanel::on_actionRecommendation_triggered()
{
    RecommendationDialog *recommendationDialog = new RecommendationDialog(this, dbMan);
    recommendationDialog->setAttribute(Qt::WA_DeleteOnClose);
    recommendationDialog->show();
}

void MainPanel::on_actionAbout_DaNet_triggered()
{
    //AboutDialog aboutDialog(this);
    AboutDialog2 aboutDialog(this);
    aboutDialog.exec();
}

void MainPanel::on_actionView_Recommendations_triggered()
{
    RecommendationsViewDialog recommendationsViewDialog(this, dbMan);
    recommendationsViewDialog.exec();
}

void MainPanel::on_actionCommercial_Reserved_Interfaces_triggered()
{
    ReportReserveToCommercialDialog *reportReserveToCommercialDialog = new ReportReserveToCommercialDialog(this, dbMan);
    reportReserveToCommercialDialog->setAttribute(Qt::WA_DeleteOnClose);
    reportReserveToCommercialDialog->show();
}

void MainPanel::on_actionClear_LOG_triggered()
{
    ClearLogDialog clearLogDialog(this,dbMan);
    clearLogDialog.exec();
}

void MainPanel::on_actionLom_triggered()
{
    LomDialog *lomDialog = new LomDialog(this, dbMan);
    lomDialog->setAttribute(Qt::WA_DeleteOnClose);
    lomDialog->show();
}

void MainPanel::on_actionIPv6_Plan_triggered()
{
}

void MainPanel::on_actionVLAN_Reference_triggered()
{
    VlanRefDialog *vlanRefDialog = new VlanRefDialog(this, dbMan);
    vlanRefDialog->setAttribute(Qt::WA_DeleteOnClose);
    vlanRefDialog->showMaximized();

}

void MainPanel::on_actionRD_Reference_triggered()
{
    RdRefDialog *rdRefDialog = new RdRefDialog(this, dbMan);
    rdRefDialog->setAttribute(Qt::WA_DeleteOnClose);
    rdRefDialog->showMaximized();
}

void MainPanel::on_actionPVC_Reference_triggered()
{
    PvcRefDialog *pvcRefDialog = new PvcRefDialog(this, dbMan);
    pvcRefDialog->setAttribute(Qt::WA_DeleteOnClose);
    pvcRefDialog->showMaximized();
}

void MainPanel::on_actionVSI_Reference_triggered()
{
    VsiRefDialog *vsiRefDialog = new VsiRefDialog(this, dbMan);
    vsiRefDialog->setAttribute(Qt::WA_DeleteOnClose);
    vsiRefDialog->showMaximized();
}

void MainPanel::on_actionExchanges_Sites_stat_triggered()
{
    ExchangesStatDialog *exchangesStatDialog = new ExchangesStatDialog(this, dbMan);
    exchangesStatDialog->setAttribute(Qt::WA_DeleteOnClose);
    exchangesStatDialog->show();
}

void MainPanel::on_actionPortsStat_triggered()
{
    PortsStatDialog *portsStatDialog = new PortsStatDialog(this, dbMan);
    portsStatDialog->setAttribute(Qt::WA_DeleteOnClose);
    portsStatDialog->show();
}

void MainPanel::on_actionCardsStat_triggered()
{
    CardStatDialog *cardStatDialog = new CardStatDialog(this, dbMan);
    cardStatDialog->setAttribute(Qt::WA_DeleteOnClose);
    cardStatDialog->show();
}

void MainPanel::clearFilter()
{
    int exchId = portHeaderView->getComboBoxCurrentData(ABBR).toInt();
    fillPortTV(true);
    portHeaderView->setComboBoxCurrentData(ABBR, exchId);
}

void MainPanel::on_actionReportTotal_DSLAM_Plan_triggered()
{
    ReportTotalDSLAMPlanDialog *reportTotalDSLAMPlanDialog = new ReportTotalDSLAMPlanDialog(this, dbMan);
    reportTotalDSLAMPlanDialog->setAttribute(Qt::WA_DeleteOnClose);
    reportTotalDSLAMPlanDialog->showMaximized();
}

void MainPanel::on_actionNew_SitePlan_triggered()
{
    NewSiteDslamDialog *newSiteDslamDialog = new NewSiteDslamDialog(this, dbMan);
    newSiteDslamDialog->setAttribute(Qt::WA_DeleteOnClose);
    connect(newSiteDslamDialog,SIGNAL(finished(int)), this,SLOT(refreshSlot(int)));
    newSiteDslamDialog->startPlan();
}

void MainPanel::on_actionLocal_New_DSLAM_Plan_triggered()
{
    LocalNewDslamDialog *localNewDslamDialog = new LocalNewDslamDialog(this, dbMan);
    localNewDslamDialog->setAttribute(Qt::WA_DeleteOnClose);
    connect(localNewDslamDialog,SIGNAL(finished(int)), this,SLOT(refreshSlot(int)));
    localNewDslamDialog->show();
}

void MainPanel::on_actionDeviceCount_triggered()
{
    DeviceCountDialog *deviceCountDialog = new DeviceCountDialog(this, dbMan);
    deviceCountDialog->setAttribute(Qt::WA_DeleteOnClose);
    deviceCountDialog->show();
}

void MainPanel::on_actionDevices_Status_triggered()
{
    DevicesStatDialog *devicesStatDialog = new DevicesStatDialog(this, dbMan);
    devicesStatDialog->setAttribute(Qt::WA_DeleteOnClose);
    devicesStatDialog->showMaximized();
}

void MainPanel::on_actionChange_ODF_triggered()
{
    int posId = dbMan->getPosId(clickedPinId);
    int odfId = dbMan->getOdfId(posId);

    ChangeInterfaceOdfDialog *changeInterfaceOdfDialog = new ChangeInterfaceOdfDialog(this,dbMan,clickedExchId, clickedLabel,odfId, posId, clickedPinId, clickedIntId);
    changeInterfaceOdfDialog->setAttribute(Qt::WA_DeleteOnClose);
    connect(changeInterfaceOdfDialog,SIGNAL(finished(int)), this,SLOT(refreshSlot(int)));
    changeInterfaceOdfDialog->show();

}

void MainPanel::on_actionExport_Excel_triggered()
{
    QList<int> intIds;
    QModelIndex ind = portTVProxyModel->index(0,0);
    for(int row = 0; row < portTVProxyModel->rowCount(); row++)
    {
        ind = ind.sibling(row, INT_ID);
        intIds << portTVProxyModel->data(ind).toInt();
        ind = ind.sibling(row, PIN_ID);
    }

    ExportExcelDialog *exportExcelDialog = new ExportExcelDialog(this, dbMan, intIds);
    exportExcelDialog->setAttribute(Qt::WA_DeleteOnClose);
    exportExcelDialog->showMaximized();
}

void MainPanel::on_actionSaloon_triggered()
{
    SaloonManDialog *saloonManDialog = new SaloonManDialog(this, dbMan);
    saloonManDialog->setAttribute(Qt::WA_DeleteOnClose);
    connect(saloonManDialog,SIGNAL(finished(int)), this,SLOT(refreshSlot(int)));
    saloonManDialog->show();
}

void MainPanel::on_actionToolsModule_Analyser_triggered()
{
    ModuleAnalyserDialog *moduleAnalyserDialog = new ModuleAnalyserDialog(this);
    moduleAnalyserDialog->setAttribute(Qt::WA_DeleteOnClose);
    moduleAnalyserDialog->showMaximized();
}

void MainPanel::on_actionInterface_Type_triggered()
{
    InterfaceTypeDialog *interfaceTypeDialog = new InterfaceTypeDialog(this, dbMan);
    interfaceTypeDialog->setAttribute(Qt::WA_DeleteOnClose);
    interfaceTypeDialog->show();
}

void MainPanel::on_actionChange_Interface_Type_triggered()
{
    QString abbr = QString::number(clickedAreaNo)+"-"+clickedAbbr;

    ChangeInterfaceTypeDialog *changeInterfaceTypeDialog = new ChangeInterfaceTypeDialog(this, dbMan, clickedExchId, clickedIntId,abbr, clickedSaloon, clickedDevice, clickedIntType, clickedInt, clickedModule);
    changeInterfaceTypeDialog->setAttribute(Qt::WA_DeleteOnClose);
    connect(changeInterfaceTypeDialog,SIGNAL(finished(int)), this,SLOT(refreshSlot(int)));
    changeInterfaceTypeDialog->show();
}

void MainPanel::on_actionReservePortToCommercialDep_triggered()
{
    ReserveToCommercialDialog *reserveToCommercialDialog = new ReserveToCommercialDialog(this, dbMan, clickedAreaNo,clickedExchId,clickedDevexId,clickedIntId);
    reserveToCommercialDialog->setAttribute(Qt::WA_DeleteOnClose);
    connect(reserveToCommercialDialog,SIGNAL(finished(int)), this,SLOT(refreshSlot(int)));
    reserveToCommercialDialog->show();
}

void MainPanel::on_actionRegex_Help_triggered()
{
    RegexpHelpDialog *regexpHelpDialog = new RegexpHelpDialog(this);
    regexpHelpDialog->setAttribute(Qt::WA_DeleteOnClose);
    regexpHelpDialog->showMaximized();
}

void MainPanel::on_actionRemove_DSLAM_triggered()
{
    RemoveDSLAMDialog *removeDslamDialog = new RemoveDSLAMDialog(this,dbMan);
    removeDslamDialog->setAttribute(Qt::WA_DeleteOnClose);
    removeDslamDialog->showMaximized();
}

void MainPanel::on_actionAccessBRASConfig_triggered()
{
    AccessBRASConfigDialog *AccessBrasDialog = new AccessBRASConfigDialog(this,dbMan);
    AccessBrasDialog->setAttribute(Qt::WA_DeleteOnClose);
    AccessBrasDialog->show();
}

void MainPanel::on_actionAssign_ODF_triggered()
{
    if(clickedPinId == -1)
    {
        AssignInterfaceOdfDialog *assignInterfaceOdfDialog = new AssignInterfaceOdfDialog(this,dbMan,clickedExchId, clickedLabel, clickedIntId);
        assignInterfaceOdfDialog->setAttribute(Qt::WA_DeleteOnClose);
        connect(assignInterfaceOdfDialog,SIGNAL(finished(int)), this,SLOT(refreshSlot(int)));
        assignInterfaceOdfDialog->show();
    }
}

void MainPanel::on_actionDetach_ODF_triggered()
{
    if(clickedPinId > 0)
    {
        int posId = dbMan->getPosId(clickedPinId);
        int odfId = dbMan->getOdfId(posId);

        DetachInterfaceOdfDialog *detachInterfaceOdfDialog = new DetachInterfaceOdfDialog(this,dbMan,clickedExchId, clickedLabel,odfId, posId, clickedPinId, clickedIntId);
        detachInterfaceOdfDialog->setAttribute(Qt::WA_DeleteOnClose);
        connect(detachInterfaceOdfDialog,SIGNAL(finished(int)), this,SLOT(refreshSlot(int)));
        detachInterfaceOdfDialog->show();
    }
}

void MainPanel::on_actionLink_Batch_to_ODF_triggered()
{
}

void MainPanel::on_actionUnlik_Batch_ODF_triggered()
{
}

void MainPanel::on_actionSystemNo_triggered()
{

}
