#ifndef SUBNET2V4POOLDIALOG_H
#define SUBNET2V4POOLDIALOG_H

#include <QDialog>
#include <QMenu>
#include <QModelIndex>

class QSqlQueryModel;
class SortFilterProxyModel;
class HeaderView;
class DanetDbMan;

namespace Ui {
class Subnet2V4PoolDialog;
}

class Subnet2V4PoolDialog : public QDialog
{
    Q_OBJECT

public:
    explicit Subnet2V4PoolDialog(QWidget *parent, DanetDbMan *db, const int SubnetPool1Id, const QString net, const QString NetDesc);
    ~Subnet2V4PoolDialog();
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
    void on_actionView_Subnet_Pool_triggered();
    void on_addSubnetBtn_clicked();
    void on_RemoveSubnetBtn_clicked();

private:
    Ui::Subnet2V4PoolDialog *ui;
    DanetDbMan *dbMan;
    QSqlQueryModel *model;
    SortFilterProxyModel *proxyModel;
    HeaderView *header;
    QMenu contextMenu;
    const int subnetPool1Id;


    //`id`, `subnet`, `prefix`, `mask`, `bc`, `hosts`, `area_no`, `area`, `exchange_id`,
    //`abbr`, `exchange`, `application`, `description`
    int Row,Id,Area,Prefix, Host,currentArea,ExchId;
    QString Subnet,Mask,Bc,Application,Ap,Desc, Exchange, Abbr;

};

#endif // SUBNET2V4POOLDIALOG_H
