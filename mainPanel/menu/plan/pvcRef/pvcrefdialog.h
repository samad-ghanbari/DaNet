#ifndef PVCREFDIALOG_H
#define PVCREFDIALOG_H

#include <QDialog>
#include <QMenu>

class QSqlQuery;
class QSqlQueryModel;
class SortFilterProxyModel;
class HeaderView;
class DanetDbMan;

namespace Ui {
class PvcRefDialog;
}

class PvcRefDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PvcRefDialog(QWidget *parent, DanetDbMan *db);
    ~PvcRefDialog();
    void fillTV(bool RESET);


private:
    Ui::PvcRefDialog *ui;
    DanetDbMan *dbMan;
    SortFilterProxyModel *proxyModel;
    HeaderView *header;

    QMenu contextMenu;
    int row,id, vpi, vci;
    QString desc;

private slots:
    void refreshSlotReset(int done);
    void refreshSlot(int done);
    void headerWidgetsChagedSlot();
    void contextMenuSlot(QPoint ptr);
    void on_actionEditPvcRef_triggered();
    void on_actionRemovePvcRef_triggered();
    void on_addBtn_clicked();
    void on_actionExport_PDF_triggered();
};

#endif // PVCREFDIALOG_H
