#ifndef DEVICEMANAGEMENTDIALOG_H
#define DEVICEMANAGEMENTDIALOG_H

#include <QDialog>
#include <QMap>
#include <QMenu>
#include <QFile>

class DanetDbMan;
class QSqlQueryModel;
class QStringListModel;
class MapModel;
class QStandardItemModel;

namespace Ui {
class DeviceManagementDialog;
}

class DeviceManagementDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DeviceManagementDialog(QWidget *parent, DanetDbMan *db);
    ~DeviceManagementDialog();
    void clearModels();
    void fillVlanTV();
    void fillEtherTrunkTV();
    void fillVpnViews();
    void fillModulesTV();
    void drawASR9010();
    void drawC7613();
    void drawCX200();
    void drawME60X16();
    void drawME60X8();
    void drawNE40X16();
    void drawNE40X8();
    void drawCX600X16();
    void drawCX600X8();
    void drawNE5000X16A();
    void drawNE5000X16();
    void drawS2403H();
    void drawS5300();
    void drawS9303();
    void drawS9306();
    void drawDslam4s();
    void drawdslam16s(const QString DeviceName);
    void huaweiModulesAnalyser();
    void IOSModulesAnalyser();
    void IOSXRModulesAnalyser();
    void setModelModule(const QString interface, const QString module);

private slots:
    void refreshVlanSlot(int i);
    void refreshEtherSlot(int i);
    void refreshVpnSlot(int i);
    void on_okBtn_clicked();
    void paintDevice(const QString device);
    void on_checkBox_toggled(bool checked);
    void on_currentTab_currentChanged(int index);
    void on_newTab_currentChanged(int index);
    void on_areaCB_currentIndexChanged(int index);
    void on_abbrCB_currentIndexChanged(int index);
    void on_typeCB_currentIndexChanged(int index);
    void on_siteCB_currentIndexChanged(int index);
    void on_deviceCB_currentIndexChanged(int index);
    void on_addVlanBtn_clicked();
    void on_addEtherBtn_clicked();
    void on_addVpnBtn_clicked();
    void on_actionEditVlan_triggered();
    void on_actionRemoveVlan_triggered();
    void on_actionEditEtherTrunk_triggered();
    void on_actionRemoveEtherTrunk_triggered();
    void on_actionEditVPN_triggered();
    void on_actionRemoveVPN_triggered();
    void vlanContextMenuSlot(QPoint ptr);
    void etherContextMenuSlot(QPoint ptr);
    void vpnContextMenuSlot(QPoint ptr);
    void on_vpnTV_clicked(const QModelIndex &index);
    void on_currVpnTV_clicked(const QModelIndex &index);
    void on_tabWidget_currentChanged(int index);
    void on_newVpnTV_clicked(const QModelIndex &index);
    void on_huaweiConfigLoadBtn_clicked();
    void on_ciscoConfigLoadBtn_clicked();
    void on_modBrowsBtn_clicked();
    void on_modChB_toggled(bool checked);
    void on_modOkBtn_clicked();

private:
    Ui::DeviceManagementDialog *ui;
    DanetDbMan *dbMan;
    QSqlQueryModel *vlanModel, *etherModel, *vpnModel, *slotModel;
    MapModel *newVlanModel, *newEtherModel;
    QStandardItemModel *newVpnModel, *moduleModel;
    QStringListModel *expModel, *impModel, *newExpModel, *newImpModel;

    QMap<int,QString> vlanDescMap, etherDescMap, newExpMap, newImpMap,idModuleMap;
    QMap<int,int> devexIdDeviceId;
    QMap<int, QString> devexIdSaloon;
    QMenu vlanContextMenu, etherContextMenu, vpnContextMenu;
    int DeviceVendor;// 1:huawei  2:IOS   3: IOS-XR
    bool SR;
    QFile configFile;
    //context
    int row,vlanId,etherId,vlan,ether, vpnId;
    QString vlanDesc, etherDesc,vpnRD, vpn,vpnDesc;
};

#endif // DEVICEMANAGEMENTDIALOG_H
