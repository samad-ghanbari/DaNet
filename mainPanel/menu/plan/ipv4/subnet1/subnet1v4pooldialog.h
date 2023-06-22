#ifndef SUBNET1V4POOLDIALOG_H
#define SUBNET1V4POOLDIALOG_H

#include <QDialog>
#include <QMenu>
#include <QModelIndex>

class QSqlQueryModel;
class SortFilterProxyModel;
class HeaderView;
class DanetDbMan;

namespace Ui {
class Subnet1V4PoolDialog;
}

class Subnet1V4PoolDialog : public QDialog
{
    Q_OBJECT

public:
    explicit Subnet1V4PoolDialog(QWidget *parent, DanetDbMan *db, const int NetPoolId, const QString net, const QString NetDesc);
    ~Subnet1V4PoolDialog();
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

private:
    Ui::Subnet1V4PoolDialog *ui;
    DanetDbMan *dbMan;
    QSqlQueryModel *model;
    SortFilterProxyModel *proxyModel;
    HeaderView *header;
    const int netPoolId;
    QMenu contextMenu;
    //`id`, `subnet`, `prefix`, `mask`, `bc`, `hosts`, `area_no`, `area`, `exchange_id`,
    //`abbr`, `exchange`,`department`, `application`, `description`
    int Row,Id,Area,Prefix, Host,currentArea,ExchId, DepNo;
    QString Subnet,Mask,Bc,Application,Ap,Desc, Exchange, Abbr, Dep;

};

#endif // SUBNET1V4POOLDIALOG_H
