#ifndef EXCHANGESITEMANDIALOG_H
#define EXCHANGESITEMANDIALOG_H

#include <QDialog>
#include <QMenu>

class QSqlQuery;
class QSqlQueryModel;
class SortFilterProxyModel;
class HeaderView;
class DanetDbMan;


namespace Ui {
class ExchangeSiteManDialog;
}

class ExchangeSiteManDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ExchangeSiteManDialog(QWidget *parent, DanetDbMan *db);
    ~ExchangeSiteManDialog();

private:
    Ui::ExchangeSiteManDialog *ui;
    DanetDbMan *dbMan;
    SortFilterProxyModel *proxyModel;
    HeaderView *header;
    QMenu contextMenu;
    int row,id, area, type_no,motherId,uplinkId, cascade, node, workingArea;
    QString exch, abbr,type, cascadeString, nodeString,dataSaloon, switchSaloon, pcmSaloon, mother,uplink, address;

private slots:
    void refreshSlot(int confirm);
    void fillExchSiteTV(bool RESET=false);
    void headerWidgetsChagedSlot();
    void contextMenuSlot(QPoint ptr);
    void on_actionEditExchangeSite_triggered();
    void on_actionRemoveExchangeSite_triggered();
    void on_addBtn_clicked();
    void on_actionExport_PDF_triggered();
};

#endif // EXCHANGESITEMANDIALOG_H
