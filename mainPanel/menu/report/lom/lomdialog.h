#ifndef LOMDIALOG_H
#define LOMDIALOG_H

#include <QDialog>
#include <QMenu>
#include <QModelIndex>

class DanetDbMan;
class QStandardItemModel;

namespace Ui {
class LomDialog;
}

class LomDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LomDialog(QWidget *parent, DanetDbMan *db);
    explicit LomDialog(QWidget *parent, DanetDbMan *db, const int Area,const int ExchId, const int Type, const int SiteId, const QString Path);
    ~LomDialog();
    void init();

private slots:
    void on_areaCB_currentIndexChanged(int index);
    void on_abbrCB_currentIndexChanged(int index);
    void on_typeCB_currentIndexChanged(int index);
    void contextMenuSlot(QPoint ptr);
    void doubleClickedSlot(QModelIndex ind);
    void on_okBtn_clicked();
    void on_addBtn_clicked();
    void on_actionEdit_Item_triggered();
    void on_actionRemove_Item_triggered();
    void insertToModel(QString cat, QString comm, int amt, QString loc, QString pur, QString cod, QString des);

private:
    Ui::LomDialog *ui;
    DanetDbMan *dbMan;
    QStandardItemModel *model;
    QString path, commodity_type, commodity_info, location, purchase, code, desc ;
    int modelRow, row, amount;
    QMenu contextMenu;
};

#endif // LOMDIALOG_H
