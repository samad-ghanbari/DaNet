#ifndef EDITDEVICEPOSITIONDIALOG_H
#define EDITDEVICEPOSITIONDIALOG_H

#include <QDialog>
#include <QMap>
class DanetDbMan;

namespace Ui {
class EditDevicePositionDialog;
}

class EditDevicePositionDialog : public QDialog
{
    Q_OBJECT
//abbr,clickedDevexId, clickedSaloon,clickedRow,
//    clickedRack,clickedShelf,clickedDeviceName
public:
    explicit EditDevicePositionDialog(QWidget *parent, DanetDbMan *db, const int ExchId, const QString abbr,  const int DevexId, const int DeviceId, const int Saloon, const int Row, const int Rack,
                                     const int Shelf, const QString DeviceName);
    ~EditDevicePositionDialog();
    void entryCheck();

private slots:
    void on_cancelBtn_clicked();
    void on_okBtn_clicked();
    void on_rowSB_valueChanged(int arg1);
    void on_rackSB_valueChanged(int arg1);
    void on_shelfSB_valueChanged(int arg1);
    void on_checkBox_toggled(bool checked);
    void on_saloonCB_currentIndexChanged(int index);

private:
    Ui::EditDevicePositionDialog *ui;
    DanetDbMan *dbMan;
    const int exchId,saloon,devexId,deviceId, row, rack, shelf;
    const QString oldDeviceName;
};

#endif // EDITDEVICEPOSITIONDIALOG_H
