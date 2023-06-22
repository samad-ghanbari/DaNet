#ifndef IPV4RELEASEDIALOG_H
#define IPV4RELEASEDIALOG_H

#include <QDialog>
class DanetDbMan;

namespace Ui {
class IpV4ReleaseDialog;
}

class IpV4ReleaseDialog : public QDialog
{
    Q_OBJECT

public:
    explicit IpV4ReleaseDialog(QWidget *parent, DanetDbMan *db, const int IpId, const QString Subnet, const QString Prefix, const QString Mask, const QString Bc, const QString Host, const QString Abbr,const QString App, const QString Desc, const QString Ip, const QString IpDesc);
    ~IpV4ReleaseDialog();

private slots:
    void on_cancelBtn_clicked();
    void on_okBtn_clicked();
    void on_checkBox_toggled(bool checked);

private:
    Ui::IpV4ReleaseDialog *ui;
    DanetDbMan *dbMan;
    const int ipId;
    QString IP,desc;
};

#endif // IPV4RELEASEDIALOG_H
