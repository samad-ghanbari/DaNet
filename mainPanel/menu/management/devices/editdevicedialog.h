#ifndef EDITDEVICEDIALOG_H
#define EDITDEVICEDIALOG_H

#include <QDialog>
class DanetDbMan;
namespace Ui {
class EditDeviceDialog;
}

class EditDeviceDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EditDeviceDialog(QWidget *parent, DanetDbMan *db, const int Id,const QString device,const bool sr,const QString type,const QString vendor,const int slotCount,const int lineSlotCount, const int startShelf, const int startSlot, const int startSubslot);
    ~EditDeviceDialog();
    void entryCheck();

private slots:
    void on_cancelBtn_clicked();
    void on_deviceLE_textChanged(const QString &arg1);
    void on_typeLE_textChanged(const QString &arg1);
    void on_vendorLE_textChanged(const QString &arg1);
    void on_modifyBtn_clicked();
    void on_slotCountSB_valueChanged(int arg1);
    void on_srCB_toggled(bool checked);
    void on_slotStartNoSB_valueChanged(int arg1);
    void on_shelfChB_toggled(bool checked);
    void on_subslotChB_toggled(bool checked);

    void on_lineSlotCountSB_valueChanged(int arg1);

private:
    Ui::EditDeviceDialog *ui;
    DanetDbMan *dbMan;
    int id;
};

#endif // EDITDEVICEDIALOG_H
