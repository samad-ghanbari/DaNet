#ifndef REMOVEDEVICEDIALOG_H
#define REMOVEDEVICEDIALOG_H

#include <QDialog>
class DanetDbMan;

namespace Ui {
class RemoveDeviceDialog;
}

class RemoveDeviceDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RemoveDeviceDialog(QWidget *parent, DanetDbMan *db, int Id, QString device, QString type, QString vendor);
    ~RemoveDeviceDialog();

private slots:
    void on_cancelBtn_clicked();
    void on_removeBtn_clicked();
    void on_checkBox_toggled(bool checked);

private:
    Ui::RemoveDeviceDialog *ui;
    DanetDbMan *dbMan;
    int id;
};

#endif // REMOVEDEVICEDIALOG_H
