#ifndef ADDIPV4POOLDIALOG_H
#define ADDIPV4POOLDIALOG_H

#include <QDialog>
class DanetDbMan;

namespace Ui {
class AddIpV4PoolDialog;
}

class AddIpV4PoolDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddIpV4PoolDialog(QWidget *parent, DanetDbMan *db,const int SubnetNo, const int SubnetId, const QString Subnet, const int Prefix, const QString Mask, const QString Bc, const int Host, const QString Abbr, const QString App, const QString Desc);
    ~AddIpV4PoolDialog();

private slots:   
    void on_checkBox_toggled(bool checked);
    void on_addBtn_clicked();
    void on_cancelBtn_clicked();

private:
    Ui::AddIpV4PoolDialog *ui;
    DanetDbMan *dbMan;
    const int subnetId,prefix;
    const QString subnet,bc;
    const int subnetNo;
};

#endif // ADDIPV4POOLDIALOG_H
