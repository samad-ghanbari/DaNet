#ifndef MAINPANEL_H
#define MAINPANEL_H

#include <QMainWindow>
#include <QLabel>
#include <QMenu>
#include <QModelIndex>

class DanetDbMan;
class SortFilterProxyModel;
class HeaderView;
class QLabel;

namespace Ui {
class MainPanel;
}

class MainPanel : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainPanel(DanetDbMan *db, bool adminUser,const bool mainDB, QWidget *parent=0);
    ~MainPanel();

private slots:
    void fillPortTV(bool RESET=false);
    void selectRowSlot(int i);
    void refreshSlot(int i);
    void on_actionAdminUsers_triggered();
    void on_actionAdminDevices_triggered();
    void on_actionAdminCards_triggered();
    void on_actionExchSiteMan_triggered();
    void on_actionOdfMan_triggered();
    void on_actionDeviceInstall_triggered();
    void portTVHeaderWidgetsChagedSlot();
    void portContextMenuSlot(QPoint ptr);
    void portTV2ClickSlot(QModelIndex indx);
    void on_actionAssignInterface_triggered();
    void on_actionEmptyInterface_triggered();
    void on_actionModifyInterface_triggered();
    void on_actionViewODFPosition_triggered();
    void on_actionLocalDevice_triggered();
    void on_actionRefresh_triggered();
    void on_actionManageDevice_triggered();
    void on_actionManageODF_triggered();
    void on_actionIPv4Plan_triggered();
    void on_actionReserve_To_Commercial_Dep_triggered();
    void on_actionDSLAMPlan_triggered();
    void on_actionPurchase_Management_triggered();
    void on_actionConnection_triggered();
    void on_actionSFP_Modules_triggered();
    void on_actionAccessDepSingleSwitchPlan_triggered();
    void on_actionAccessDepDoubleSwitchPlan_triggered();
    void on_actionBRAS_NAS_IP_triggered();
    void on_actionInstallCard_triggered();
    void on_actionChange_Module_triggered();
    void on_actionSBCIPLoopBackPlan_triggered();
    void on_actionExchange_Info_triggered();
    void on_actionReportInterfaces_triggered();
    void on_actionReportODF_triggered();
    void on_actionExport_PDF_triggered();
    void on_actionReportExchange_Sites_triggered();
    void on_actionReportExchange_Devices_triggered();
    void on_actionReportNetwork_Pool_triggered();
    void on_actionReportIP_Pool_triggered();
    void on_actionNetwork_Calculator_triggered();
    void on_actionReportDSLAM_triggered();
    void on_actionIP_Pool_Analyser_triggered();
    void on_actionConfiguration_Analyser_triggered();
    void on_actionNetwork_Assistant_triggered();
    void on_actionRecent_LOG_triggered();
    void on_actionProfile_triggered();
    void on_actionRecommendation_triggered();
    void on_actionAbout_DaNet_triggered();
    void on_actionView_Recommendations_triggered();
    void on_actionCommercial_Reserved_Interfaces_triggered();
    void on_actionClear_LOG_triggered();
    void on_actionReportFloorPlan_triggered();
    void on_actionLom_triggered();
    void on_actionIPv6_Plan_triggered();
    void on_actionReportSubnet_Pool_triggered();
    void on_actionVLAN_Reference_triggered();
    void on_actionRD_Reference_triggered();
    void on_actionPVC_Reference_triggered();
    void on_actionVSI_Reference_triggered();
    void on_actionExchanges_Sites_stat_triggered();
    void on_actionPortsStat_triggered();
    void on_actionCardsStat_triggered();
    void clearFilter();
    void on_actionReportTotal_DSLAM_Plan_triggered();
    void on_actionNew_SitePlan_triggered();
    void on_actionLocal_New_DSLAM_Plan_triggered();
    void on_actionDeviceCount_triggered();
    void on_actionDevices_Status_triggered();
    void on_actionChange_ODF_triggered();
    void on_actionExport_Excel_triggered();
    void on_actionSaloon_triggered();
    void on_actionToolsModule_Analyser_triggered();
    void on_actionInterface_Type_triggered();
    void on_actionChange_Interface_Type_triggered();
    void on_actionReservePortToCommercialDep_triggered();
    void on_actionRegex_Help_triggered();
    void on_actionRemove_DSLAM_triggered();
    void on_actionAccessBRASConfig_triggered();

    void on_actionLetters_triggered();

    void on_actionPhone_Numbers_triggered();

    void on_actionCommodity_triggered();

    void on_actionUninstalled_Commodities_triggered();

    void on_actionAssign_ODF_triggered();

    void on_actionDetach_ODF_triggered();

    void on_actionLink_Batch_to_ODF_triggered();

    void on_actionUnlik_Batch_ODF_triggered();

private:
    Ui::MainPanel *ui;
    DanetDbMan *dbMan;
    SortFilterProxyModel *portTVProxyModel;
    HeaderView *portHeaderView;
    bool mainDb;
    QMenu portContextMenu;
    QModelIndex clickedIndex;
    QLabel userLbl,recordCountLbl;
    int clickedRow, clickedIntId, clickedPinId,clickedDeviceId,clickedDevexId, clickedAreaNo, clickedSaloonNo, clickedExchId;
    QString workingArea, clickedSaloon,clickedLabel,clickedPeerLabel, clickedAbbr,clickedPeerAbbr, clickedDevice, clickedModule, clickedIntType,
    clickedInt, clickedPeerInt, clickedPeerDevice,clickedEther, clickedDesc;

};

#endif // MAINPANEL_H
