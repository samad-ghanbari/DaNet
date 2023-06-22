#ifndef REMOVEIPV4POOLDIALOG_H
#define REMOVEIPV4POOLDIALOG_H

#include <QDialog>
class DanetDbMan;

namespace Ui {
class RemoveIpV4PoolDialog;
}

class RemoveIpV4PoolDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RemoveIpV4PoolDialog(QWidget *parent, DanetDbMan *db,const int SubnetNo, const int SubnetId, const QString Subnet, const int Prefix, const QString Mask, const QString Bc, const int Host, const QString Abbr,const QString App, const QString Desc);
    ~RemoveIpV4PoolDialog();

private slots:
    void on_cancelBtn_clicked();
    void on_okBtn_clicked();
    void on_checkBox_toggled(bool checked);

private:
    Ui::RemoveIpV4PoolDialog *ui;
    DanetDbMan *dbMan;
    const int subnetId,prefix;
    const QString subnet, mask, bc;
    const int subnetNo;

};

#endif // REMOVEIPV4POOLDIALOG_H
