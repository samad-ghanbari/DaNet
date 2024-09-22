#ifndef ADDDEVICEDIALOG_H
#define ADDDEVICEDIALOG_H

#include <QDialog>
class DanetDbMan;

namespace Ui {
class AddDeviceDialog;
}

class AddDeviceDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddDeviceDialog(QWidget *parent, DanetDbMan *db);
    ~AddDeviceDialog();
    void entryCheck();

private slots:
    void on_cancelBtn_clicked();
    void on_okBtn_clicked();
    void on_deviceLE_textChanged(const QString &arg1);
    void on_srCB_toggled(bool checked);
    void on_typeLE_textChanged(const QString &arg1);
    void on_vendorLE_textChanged(const QString &arg1);
    void on_slotCountSB_valueChanged(int arg1);
    void on_startSlotNoSB_valueChanged(int arg1);
    void on_shelfChB_toggled(bool checked);
    void on_subslotChB_toggled(bool checked);
    void on_lineSlotCountSB_valueChanged(int arg1);

private:
    Ui::AddDeviceDialog *ui;
    DanetDbMan *dbMan;
};

#endif // ADDDEVICEDIALOG_H
