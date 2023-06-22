#ifndef DEVICEINSTALLDIALOG_H
#define DEVICEINSTALLDIALOG_H

#include <QDialog>
#include <QMenu>
class QSqlQuery;
class QSqlQueryModel;
class DanetDbMan;

namespace Ui {
class DeviceInstallDialog;
}

class DeviceInstallDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DeviceInstallDialog(QWidget *parent, DanetDbMan *db);
    ~DeviceInstallDialog();
    void fillDevicesTV();

private slots:
    void refreshSlot(int confirm);
    void on_areaCB_currentIndexChanged(int index);
    void deviceContextMenuSlot(QPoint ptr);
    void on_abbrCB_currentIndexChanged(const QString &arg1);
    void on_typeCB_currentIndexChanged(int index);
    void on_siteCB_currentIndexChanged(int index);
    void on_addDeviceBtn_clicked();
    void on_actionEditDevicePosition_triggered();
    void on_actionRemoveDevice_triggered();
    void on_actionChangeDevice_triggered();   
    void on_actionEdit_Description_triggered();
    void on_actionEdit_Purchase_triggered();

    void on_actionEdit_Device_Name_triggered();

private:
    Ui::DeviceInstallDialog *ui;
    DanetDbMan *dbMan;
    QSqlQueryModel *model;
    QMenu deviceContextMenu;
    /*
     0:`devex_id`, 1:`exchange_id`, 2:`saloon_no`, 3:`device_id`, 4:`saloon_name`,
    5 `row`, 6`rack`, 7`shelf`, 8`device`, 9`device_name`, 10`purchase`, 11`description`
    */
    int recordRow, clickedDevexId,clickedSaloonNo, clickedDeviceId, clickedRow, clickedRack, clickedShelf;
    QString clickedSaloon, clickedDevice, clickedDeviceName, clickedPurchase, clickedDesc;

};

#endif // DEVICEINSTALLDIALOG_H
