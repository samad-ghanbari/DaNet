#ifndef EDITDEVICEPURCHASEDIALOG_H
#define EDITDEVICEPURCHASEDIALOG_H

#include <QDialog>
#include <QMap>
class DanetDbMan;

namespace Ui {
class EditDevicePurchaseDialog;
}

class EditDevicePurchaseDialog : public QDialog
{
    Q_OBJECT
//abbr,clickedDevexId, clickedSaloon,clickedRow,
//    clickedRack,clickedShelf,clickedDeviceName
public:
    explicit EditDevicePurchaseDialog(QWidget *parent, DanetDbMan *db, const QString abbr, const QString saloon,  const int DevexId, const int Row, const int Rack,
                                     const int Shelf, const QString DeviceName, const QString purchase);
    ~EditDevicePurchaseDialog();

private slots:
    void on_cancelBtn_clicked();
    void on_okBtn_clicked();
    void on_checkBox_toggled(bool checked);
    void on_purchaseCB_currentIndexChanged(int index);

private:
    Ui::EditDevicePurchaseDialog *ui;
    DanetDbMan *dbMan;
    const int devexId;
};

#endif // EDITDEVICEPURCHASEDIALOG_H
