#ifndef ADDDEVICEINSTALLDIALOG_H
#define ADDDEVICEINSTALLDIALOG_H

#include <QDialog>
#include <QMap>

class DanetDbMan;

namespace Ui {
class AddDeviceInstallDialog;
}

class AddDeviceInstallDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddDeviceInstallDialog(QWidget *parent, DanetDbMan *db, const int ExchId, const QString Abbr , bool DSLAM = false);
    ~AddDeviceInstallDialog();
    void checkEntry();
    bool isDone();
    int getDevexId();

private slots:
    void on_cancelBtn_clicked();
    void on_okBtn_clicked();

    void on_deviceCB_currentIndexChanged(int index);
    void on_purchaseCB_currentIndexChanged(int index);
    void on_saloonCB_currentIndexChanged(int index);

private:
    Ui::AddDeviceInstallDialog *ui;
    DanetDbMan *DbMan;
    const int exchId;
    int devexId;
    bool ADDED;
};

#endif // ADDDEVICEINSTALLDIALOG_H
