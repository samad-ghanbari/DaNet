#ifndef UNINSTALLEDDIALOG_H
#define UNINSTALLEDDIALOG_H

#include <QDialog>
#include <QMenu>
#include <QModelIndex>
class SortFilterProxyModel;
class HeaderView;
class DanetDbMan;
class QSqlQueryModel;

namespace Ui {
class UninstalledDialog;
}

class UninstalledDialog : public QDialog
{
    Q_OBJECT

public:
    explicit UninstalledDialog(DanetDbMan *db, QWidget *parent = nullptr);
    ~UninstalledDialog();

private slots:
    void fillTV();
    void contextMenuSlot(QPoint ptr);
    void headerWidgetsChagedSlot();
    void on_addBtn_clicked();
    void on_actionEdit_triggered();
    void on_actionRemove_triggered();

private:
    Ui::UninstalledDialog *ui;
    DanetDbMan *dbMan;
    SortFilterProxyModel *proxyModel;
    HeaderView *header;
    QMenu contextMenu;
    QString user, exchange, commodity , letter ,desc,ts;
    int id,row,area,user_id, amount,loggedInUserId;
};

#endif // UNINSTALLEDDIALOG_H
