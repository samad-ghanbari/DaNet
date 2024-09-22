#ifndef REMOVEDEVICEINSTALLDIALOG_H
#define REMOVEDEVICEINSTALLDIALOG_H

#include <QDialog>
class DanetDbMan;

namespace Ui {
class RemoveDeviceInstallDialog;
}

class RemoveDeviceInstallDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RemoveDeviceInstallDialog(QWidget *parent, DanetDbMan *db, const int DevexId, const QString abbr, const QString saloon, const QString rowRackShelf, const QString device, const QString deviceName);
    ~RemoveDeviceInstallDialog();

private slots:
    void on_okBtn_clicked();
    void on_cancelBtn_clicked();
    void on_checkBox_toggled(bool checked);

private:
    Ui::RemoveDeviceInstallDialog *ui;
    DanetDbMan *dbMan;
    const int devexId;
};

#endif // REMOVEDEVICEINSTALLDIALOG_H
