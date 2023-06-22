#ifndef SUBNET3V4POOLDIALOG_H
#define SUBNET3V4POOLDIALOG_H

#include <QDialog>
#include <QMenu>
#include <QModelIndex>

class QSqlQueryModel;
class SortFilterProxyModel;
class HeaderView;
class DanetDbMan;

namespace Ui {
class Subnet3V4PoolDialog;
}

class Subnet3V4PoolDialog : public QDialog
{
    Q_OBJECT

public:
    explicit Subnet3V4PoolDialog(QWidget *parent, DanetDbMan *db, const int SubnetPool2Id, const QString Subnet, const QString SubnetDesc);
    ~Subnet3V4PoolDialog();
    void fillIpTV(int area);

private slots:
    void refreshCurrentAreaSlot(int i);
    void headerWidgetsChagedSlot();
    void contextMenuSlot(QPoint ptr);
    void doubleClickedSlot(QModelIndex clickedIndex);
    void on_actionNetwork_Assignment_triggered();
    void on_actionNetwork_Release_triggered();
    void on_actionSplitNetwork_triggered();
    void on_actionMergenets_triggered();
    void on_actionView_Ip_Pool_triggered();
    void on_actionExport_PDF_triggered();
    void on_actionRefresh_triggered();
    void on_addSubnetBtn_clicked();
    void on_RemoveSubnetBtn_clicked();

private:
    Ui::Subnet3V4PoolDialog *ui;
    DanetDbMan *dbMan;
    QSqlQueryModel *model;
    SortFilterProxyModel *proxyModel;
    HeaderView *header;
    const int subnetPool2Id;

    QMenu contextMenu;

    //`id`, `subnet`, `prefix`, `mask`, `bc`, `hosts`, `area_no`, `area`, `exchange_id`,
    //`abbr`, `exchange`, `application`, `description`
    int Row,Id,Area,Prefix, Host,currentArea,ExchId;
    QString Subnet,Mask,Bc,Application,Ap,Desc, Exchange, Abbr;

};

#endif // SUBNET3V4POOLDIALOG_H
