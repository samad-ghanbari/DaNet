#ifndef SUBNET2V4RELEASEDIALOG_H
#define SUBNET2V4RELEASEDIALOG_H

#include <QDialog>
class DanetDbMan;

namespace Ui {
class Subnet2V4ReleaseDialog;
}

class Subnet2V4ReleaseDialog : public QDialog
{
    Q_OBJECT

public:
    explicit Subnet2V4ReleaseDialog(QWidget *parent, DanetDbMan *db, const int SubnetPoolId, const QString subnet, const int prefix, const QString mask, const QString bc, const int host, const int area, const QString abbr, const QString app, const QString desc);
    ~Subnet2V4ReleaseDialog();

private slots:
    void on_checkBox_toggled(bool checked);
    void on_cancelBtn_clicked();
    void on_okBtn_clicked();

private:
    Ui::Subnet2V4ReleaseDialog *ui;
    DanetDbMan *dbMan;
    const int subnetPoolId;
    QString Network, Desc;
};

#endif // SUBNET2V4RELEASEDIALOG_H
