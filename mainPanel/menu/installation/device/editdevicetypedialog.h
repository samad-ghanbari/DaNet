#ifndef EDITDEVICETYPEDIALOG_H
#define EDITDEVICETYPEDIALOG_H

#include <QDialog>
#include <QMap>
class DanetDbMan;

namespace Ui {
class EditDeviceTypeDialog;
}

class EditDeviceTypeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EditDeviceTypeDialog(QWidget *parent, DanetDbMan *db,const int ExchId, const QString Abbr, const int saloon, const int Row, const int Rack, const int Shelf,
                                  const int DevexId, const QString purchase, const int DeviceId, const QString DeviceName);
    ~EditDeviceTypeDialog();

private slots:
    void on_deviceCB_currentIndexChanged(int index);
    void on_cancelBtn_clicked();
    void on_okBtn_clicked();

    void on_checkBox_toggled(bool checked);

    void on_purCB_currentTextChanged(const QString &arg1);

private:
    Ui::EditDeviceTypeDialog *ui;
    DanetDbMan *dbMan;
    const int exchId,devexId, row, rack,shelf;
    const QString deviceName;
    int deviceId, oldStartShelf,oldStartSlot,oldStartSubslot;
    QMap<int, QString> deviceMap;
    bool S5300Device;
};

#endif // EDITDEVICETYPEDIALOG_H
