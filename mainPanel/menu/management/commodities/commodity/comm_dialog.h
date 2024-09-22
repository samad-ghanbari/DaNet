#ifndef COMM_DIALOG_H
#define COMM_DIALOG_H

#include <QDialog>
#include <QMenu>
#include <QModelIndex>
class SortFilterProxyModel;
class HeaderView;
class DanetDbMan;
class QSqlQueryModel;

namespace Commodity
{

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(DanetDbMan *db,const int ID, const QString CAT, QWidget *parent = nullptr);
    ~Dialog();

signals:
    void changeSignal();

private slots:
    void contextMenuSlot(QPoint ptr);
    void doubleClickedSlot(QModelIndex ind);
    void on_addBtn_clicked();
    void fillTV();
    void on_actionEdit_Commodity_triggered();
    void on_actionRemove_Commodity_triggered();
    void changeSlot();
    void headerWidgetsChagedSlot();

private:
    Ui::Dialog *ui;
    DanetDbMan *dbMan;
    SortFilterProxyModel *proxyModel;
    HeaderView *header;
    const int cat_id;
    const QString cat;
    int row, comm_id, purchase_id, total, left;
    QString commodity, code, location, purchase, ts;
    QMenu contextMenu;
    bool adminUser, enabled;

};

}
#endif // COMM_DIALOG_H
