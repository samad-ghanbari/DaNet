#ifndef ADDVPNDIALOG_H
#define ADDVPNDIALOG_H

#include <QDialog>
#include <QRegExpValidator>

class DanetDbMan;

namespace Ui {
class AddVPNDialog;
}

class AddVPNDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddVPNDialog(QWidget *parent, DanetDbMan *db, const int DevexId, QString Abbr, QString Saloon, QString Device);
    ~AddVPNDialog();
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
    Ui::AddVPNDialog *ui;
    DanetDbMan *dbMan;
    const int devexId;
    QRegExp rdReg, imexReg;
    QRegExpValidator *rdValidator, *imexValidator;
};

#endif // ADDVPNDIALOG_H
