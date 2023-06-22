#ifndef EXCHANGESITESREPORTDIALOG_H
#define EXCHANGESITESREPORTDIALOG_H

#include <QDialog>
#include <QSqlQuery>
class DanetDbMan;
class QSqlQueryModel;
class QStandardItemModel;

namespace Ui {
class ExchangeSitesReportDialog;
}

class ExchangeSitesReportDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ExchangeSitesReportDialog(QWidget *parent, DanetDbMan *db);
    ~ExchangeSitesReportDialog();
    void initTreeModel(const QString Mother);
    int getNode(const int uplinkId);

private slots:
    void on_cancelBtn_clicked();
    void on_okBtn_clicked();
    void on_areaCB_currentIndexChanged(int index);
    void on_abbrCB_currentIndexChanged(int index);

private:
    Ui::ExchangeSitesReportDialog *ui;
    DanetDbMan *dbMan;
    QStandardItemModel *treeModel, *tableModel;
};

#endif // EXCHANGESITESREPORTDIALOG_H
