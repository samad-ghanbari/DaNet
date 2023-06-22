#ifndef EDITDEVICENAMEDIALOG_H
#define EDITDEVICENAMEDIALOG_H

#include <QDialog>
class DanetDbMan;

namespace Ui {
class EditDeviceNameDialog;
}

class EditDeviceNameDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EditDeviceNameDialog(QWidget *parent,DanetDbMan *DbMan,const int DevexId, QString DeviceName,QString AreaAbbr,QString Site,QString Device);
    ~EditDeviceNameDialog();

private slots:
    void on_cancelBtn_clicked();

    void on_okBtn_clicked();

    void on_checkBox_toggled(bool checked);

    void on_siteNameLE_textChanged(const QString &arg1);

private:
    Ui::EditDeviceNameDialog *ui;
    DanetDbMan *dbMan;
    const int devexId;
};

#endif // EDITDEVICENAMEDIALOG_H
