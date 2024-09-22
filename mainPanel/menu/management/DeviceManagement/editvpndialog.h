#ifndef EDITVPNDIALOG_H
#define EDITVPNDIALOG_H

#include <QDialog>
#include <QRegExpValidator>
class DanetDbMan;

namespace Ui {
class EditVPNDialog;
}

class EditVPNDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EditVPNDialog(QWidget *parent, DanetDbMan *db,const QString abbr,const QString saloon,const QString device,const int VpnId,const QString Vpn,const QString RD,const QString Desc,const QString exp,const QString imp);
    ~EditVPNDialog();
    void enableOkBtn();

private slots:
    void on_cancelBtn_clicked();
    void on_okBtn_clicked();
    void on_vpnLE_textChanged(const QString &arg1);
    void on_rdLE_textChanged(const QString &arg1);
    void on_descLE_textChanged(const QString &arg1);
    void on_expLE_textChanged(const QString &arg1);
    void on_impLE_textChanged(const QString &arg1);

private:
    Ui::EditVPNDialog *ui;
    DanetDbMan *dbMan;
    const int vpnId;
    QRegExp rdReg, imexReg;
    QRegExpValidator *rdValidator, *imexValidator;
};

#endif // EDITVPNDIALOG_H
