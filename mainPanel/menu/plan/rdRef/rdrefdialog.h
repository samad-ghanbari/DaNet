#ifndef RDREFDIALOG_H
#define RDREFDIALOG_H

#include <QDialog>
#include <QMenu>

class QSqlQuery;
class QSqlQueryModel;
class SortFilterProxyModel;
class HeaderView;
class DanetDbMan;

namespace Ui {
class RdRefDialog;
}

class RdRefDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RdRefDialog(QWidget *parent, DanetDbMan *db);
    ~RdRefDialog();
    void fillTV(bool RESET);

private:
    Ui::RdRefDialog *ui;
    DanetDbMan *dbMan;
    SortFilterProxyModel *proxyModel;
    HeaderView *header;
    QMenu contextMenu;
    int row,id;
    QString desc, rd;

private slots:
    void refreshSlot(int done);
    void refreshSlotReset(int done);
    void headerWidgetsChagedSlot();
    void contextMenuSlot(QPoint ptr);
    void on_addBtn_clicked();
    void on_actionExport_PDF_triggered();
    void on_actionEditRdRef_triggered();
    void on_actionRemoveRdRef_triggered();
};

#endif // RDREFDIALOG_H
