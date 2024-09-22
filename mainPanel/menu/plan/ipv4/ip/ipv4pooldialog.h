#ifndef IPV4POOLDIALOG_H
#define IPV4POOLDIALOG_H

#include <QDialog>
#include <QMenu>
class QSqlQueryModel;
class QSortFilterProxyModel;
class DanetDbMan;

namespace Ui {
class IpV4PoolDialog;
}

class IpV4PoolDialog : public QDialog
{
    Q_OBJECT

public:
    explicit IpV4PoolDialog(QWidget *parent, DanetDbMan *db,const int SubnetNo, const int SubnetId, const QString Subnet, const int Prefix, const QString Mask, const QString Bc, const int Host,const int Area, const QString Abbr, const QString App, const QString Desc);
    ~IpV4PoolDialog();
    void fillTV();

private slots:
    void refreshSlot(int i);
    void refreshAddRemoveSlot(int i);
    void on_actionAssign_IP_triggered();
    void contextMenuSlot(QPoint ptr);
    void on_actionrelease_IP_triggered();
    void on_addIpBtn_clicked();
    void on_removeIpBtn_clicked();
    void on_actionRefresh_triggered();
    void on_searchLE_textChanged(const QString &arg1);
    void on_actionExport_PDF_triggered();

private:
    Ui::IpV4PoolDialog *ui;
    DanetDbMan *dbMan;
    QSortFilterProxyModel *model;
    QSqlQueryModel *m;
    const int subnetId,prefix,host;
    const QString subnet,mask,bc,abbr,app,desc;
    const int subnetNo;

    QMenu contextMenu;
    //`id`, `ip`, `description`, `editable`
    int ipId, Row;
    QString ip,ipDesc;
    bool editable;
    QList<QString> subnetInfo;// subnet-prefix-mask-broadcast-hostcount-exch & desc
};

#endif // IPV4POOLDIALOG_H
