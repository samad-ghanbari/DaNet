#ifndef NETV4POOLDIALOG_H
#define NETV4POOLDIALOG_H

#include <QDialog>
#include <QMenu>
#include <QModelIndex>

class QSqlQuery;
class QSqlQueryModel;
class SortFilterProxyModel;
class HeaderView;
class DanetDbMan;

namespace Ui {
class NetV4PoolDialog;
}

class NetV4PoolDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NetV4PoolDialog(QWidget *parent, DanetDbMan *db);
    ~NetV4PoolDialog();
    void fillIpTV();

private slots:
    void refreshSlot(int done);
    void headerWidgetsChagedSlot();
    void contextMenuSlot(QPoint ptr);
    void doubleClickedSlot(QModelIndex ind);
    void on_addBtn_clicked();
    void on_actionEditNetwork_triggered();
    void on_actionRemveNetwork_triggered();
    void on_actionSubNetworkPool_triggered();
    void on_actionNetwork_Assignment_triggered();
    void on_actionRefresh_triggered();
    void on_actionExport_PDF_triggered();

    void on_searchBtn_clicked();

private:
    Ui::NetV4PoolDialog *ui;
    DanetDbMan *dbMan;
    SortFilterProxyModel *proxyModel;
    HeaderView *header;

    QMenu contextMenu;

    int Id,Prefix, Host, row;
    QString Net,Mask,Bc,Desc;

};

#endif // NETV4POOLDIALOG_H
