#ifndef VSIREFDIALOG_H
#define VSIREFDIALOG_H

#include <QDialog>
#include <QMenu>

class QSqlQuery;
class QSqlQueryModel;
class SortFilterProxyModel;
class HeaderView;
class DanetDbMan;

namespace Ui {
class VsiRefDialog;
}

class VsiRefDialog : public QDialog
{
    Q_OBJECT

public:
    explicit VsiRefDialog(QWidget *parent, DanetDbMan *db);
    ~VsiRefDialog();
    void fillTV(bool RESET);

private:
    Ui::VsiRefDialog *ui;
    DanetDbMan *dbMan;
    SortFilterProxyModel *proxyModel;
    HeaderView *header;
    QMenu contextMenu;
    int row,id, vsi;
    QString desc;

private slots:
    void refreshSlot(int done);
    void refreshSlotReset(int done);
    void headerWidgetsChagedSlot();
    void contextMenuSlot(QPoint ptr);
    void on_actionEditVsiRef_triggered();
    void on_actionRemoveVsiRef_triggered();
    void on_addBtn_clicked();
    void on_actionExport_PDF_triggered();
};

#endif // VSIREFDIALOG_H
