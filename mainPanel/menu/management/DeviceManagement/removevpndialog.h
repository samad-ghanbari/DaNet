#ifndef REMOVEVPNDIALOG_H
#define REMOVEVPNDIALOG_H

#include <QDialog>
class DanetDbMan;

namespace Ui {
class RemoveVPNDialog;
}

class RemoveVPNDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RemoveVPNDialog(QWidget *parent,DanetDbMan *db,const QString abbr,const QString saloon, const QString device, const int VpnId,const QString Vpn,const QString RD,const QString Desc, const QString exp, const QString imp);
    ~RemoveVPNDialog();

private slots:
    void on_checkBox_toggled(bool checked);

    void on_cancelBtn_clicked();

    void on_okBtn_clicked();

private:
    Ui::RemoveVPNDialog *ui;
    DanetDbMan *dbMan;
    const int vpnId;
};

#endif // REMOVEVPNDIALOG_H
