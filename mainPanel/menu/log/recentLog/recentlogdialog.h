#ifndef RECENTLOGDIALOG_H
#define RECENTLOGDIALOG_H

#include <QDialog>

class SortFilterProxyModel;
class HeaderView;
class DanetDbMan;
class QSqlQuery;
class QSqlQueryModel;

namespace Ui {
class RecentLogDialog;
}

class RecentLogDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RecentLogDialog(QWidget *parent, DanetDbMan *db);
    ~RecentLogDialog();
    void fillRecentLog(bool RESET);
    void fillLog(bool RESET);

private slots:
    void on_recentLogChB_toggled(bool checked);
    void on_areaCB_currentIndexChanged(int index);
    void on_abbrCB_currentIndexChanged(int index);
    void headerWidgetsChagedSlot();
//    void on_startDE_dateChanged(const QDate &date);

private:
    Ui::RecentLogDialog *ui;
    DanetDbMan *dbMan;
    SortFilterProxyModel *proxyModel;
    HeaderView *header;
    QSqlQuery *query;
    QSqlQueryModel *model;
};

#endif // RECENTLOGDIALOG_H
