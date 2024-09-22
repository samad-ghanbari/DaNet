#ifndef TELEPHONESDIALOG_H
#define TELEPHONESDIALOG_H

#include <QDialog>
#include <QMenu>
#include <QModelIndex>
class SortFilterProxyModel;
class HeaderView;
class DanetDbMan;
class QSqlQueryModel;

namespace Ui {
class TelephonesDialog;
}

class TelephonesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TelephonesDialog(DanetDbMan *db, QWidget *parent = nullptr);
    ~TelephonesDialog();

private slots:
    void fillTV();
    void contextMenuSlot(QPoint ptr);
    void doubleClickedSlot(QModelIndex ind);
    void on_addBtn_clicked();
    void on_actionEdit_triggered();
    void on_actionRemove_triggered();
    void headerWidgetsChagedSlot();
    void on_actionView_triggered();

private:
    Ui::TelephonesDialog *ui;
    DanetDbMan *dbMan;
    SortFilterProxyModel *proxyModel;
    HeaderView *header;
    QMenu contextMenu;
    int id, row;
    QString contactName, dep,pos,phones;
};

#endif // TELEPHONESDIALOG_H
