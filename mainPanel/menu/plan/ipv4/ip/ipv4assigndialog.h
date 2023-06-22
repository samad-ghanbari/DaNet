#ifndef IPV4ASSIGNDIALOG_H
#define IPV4ASSIGNDIALOG_H

#include <QDialog>
class DanetDbMan;

namespace Ui {
class IpV4AssignDialog;
}

class IpV4AssignDialog : public QDialog
{
    Q_OBJECT

public:
    explicit IpV4AssignDialog(QWidget *parent, DanetDbMan *db, const int IpId, const QString Subnet, const QString Prefix, const QString Mask, const QString Bc, const QString Host, const QString Abbr,const QString App, const QString Desc, const QString Ip, const QString IpDesc);
    ~IpV4AssignDialog();

private slots:
    void on_descLE_textChanged(const QString &arg1);
    void on_cancelBtn_clicked();
    void on_okBtn_clicked();

private:
    Ui::IpV4AssignDialog *ui;
    DanetDbMan *dbMan;
    const int ipId;
    bool editable;
    QString IP;
};

#endif // IPV4ASSIGNDIALOG_H
