#ifndef VLANREFDIALOG_H
#define VLANREFDIALOG_H

#include <QDialog>
#include <QMenu>

class QSqlQuery;
class QSqlQueryModel;
class SortFilterProxyModel;
class HeaderView;
class DanetDbMan;

namespace Ui {
class VlanRefDialog;
}

class VlanRefDialog : public QDialog
{
    Q_OBJECT

public:
    explicit VlanRefDialog(QWidget *parent, DanetDbMan *db);
    ~VlanRefDialog();
    void fillTV(bool RESET, const int Range);

private:
    Ui::VlanRefDialog *ui;
    DanetDbMan *dbMan;
    SortFilterProxyModel *proxyModel;
    HeaderView *header;
    QMenu contextMenu;
    int row,id, vlan, currentRange;
    QString desc;

private slots:
    void refreshSlot(int done);
    void refreshSlotReset(int done);
    void headerWidgetsChagedSlot();
    void contextMenuSlot(QPoint ptr);
    void on_actionEditVlanRef_triggered();
    void on_actionRemoveVlanRef_triggered();
    void on_addBtn_clicked();
    void on_actionExport_PDF_triggered();
};

#endif // VLANREFDIALOG_H
