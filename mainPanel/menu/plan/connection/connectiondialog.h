#ifndef CONNECTIONDIALOG_H
#define CONNECTIONDIALOG_H

#include <QDialog>
#include <QMenu>
class DanetDbMan;
class QSqlQueryModel;

namespace Ui {
class ConnectionDialog;
}

class ConnectionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ConnectionDialog(QWidget *parent, DanetDbMan *db);
    ~ConnectionDialog();
    void fillBrasTV();
    void fillAggCxTV();


private slots:
    void refreshBrasSlot(int done);
    void refreshAggCxSlot(int done);
    void on_brasAreaCB_currentIndexChanged(int index);
    void on_brasCB_currentIndexChanged(int index);
    void contextMenuSlot(QPoint ptr);
    void aggCxContextMenuSlot(QPoint ptr);
    void on_brasAddBtn_clicked();
    void on_aggCxAreaCB_currentIndexChanged(int index);
    void on_aggCxAddBtn_clicked();
    void on_actionEdit_BRAS_Connection_triggered();
    void on_actionRemove_BRAS_Connection_triggered();
    void on_actionEdit_Inter_Connection_triggered();
    void on_actionRemove_Inter_Connection_triggered();

private:
    Ui::ConnectionDialog *ui;
    DanetDbMan *dbMan;
    QSqlQueryModel *model, *aggCxModel;

    QMenu contextMenu,aggCxContextMenu;
    //bras
    int id,brasId,brasExchId,aggExchId,agg1DevexId,agg2DevexId, eth1,eth2;
    QString aggAbbr, agg1, agg2;
    //agg-CX
    int Row,Id,ExchId,AggDevexId,CxDevexId,AggEth,CxEth;
    QString Abbr,Agg,Cx;
};

#endif // CONNECTIONDIALOG_H
