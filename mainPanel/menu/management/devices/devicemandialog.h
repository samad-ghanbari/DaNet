#ifndef DEVICEMANDIALOG_H
#define DEVICEMANDIALOG_H

#include <QDialog>
#include <QMenu>

class DanetDbMan;
class QSqlQuery;
class QSqlQueryModel;

namespace Ui {
class DeviceManDialog;
}

class DeviceManDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DeviceManDialog(QWidget *parent, DanetDbMan *db);
    ~DeviceManDialog();
    void fillDeviceTV();


private:
    Ui::DeviceManDialog *ui;
    DanetDbMan *dbMan;
    QMenu contextMenu;
    QSqlQueryModel *model;
    int id,slotCount,lineSlotCount, startSlot, row, startShelf,startSubslot;
    bool sr, netDevices,AdminUserFlag;
    QString device, type, vendor;

private slots:
    void refreshSlot(int i);
    void contextMenuSlot(QPoint ptr);
    void on_adminAddDeviceBtn_clicked();
    void on_actionAdminEditDevice_triggered();
    void on_actionAdminRemoveDevice_triggered();
    void on_netDevicesCB_toggled(bool checked);
    void on_adminLE_textChanged(const QString &arg1);
};


#endif // DEVICEMANDIALOG_H
