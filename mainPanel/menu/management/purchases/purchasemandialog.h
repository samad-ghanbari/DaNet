#ifndef PURCHASEMANDIALOG_H
#define PURCHASEMANDIALOG_H

#include <QDialog>
#include <QMenu>

class QSqlQueryModel;
class DanetDbMan;

namespace Ui {
class PurchaseManDialog;
}

class PurchaseManDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PurchaseManDialog(QWidget *parent, DanetDbMan *db);
    ~PurchaseManDialog();
    void fillTV();

private slots:
    void refreshSlot(int i);
    void contextMenuSlot(QPoint ptr);
    void on_actionEdit_Purchase_triggered();
    void on_actionRemove_Purchase_triggered();
    void on_addPurchaseBtn_clicked();
    void on_adminLE_textChanged(const QString &arg1);

private:
    Ui::PurchaseManDialog *ui;
    DanetDbMan *dbMan;
    QSqlQueryModel *model;
    QMenu contextMenu;
    bool AdminUserFlag;
    int id, row;
    QString abbr, purchase;
};

#endif // PURCHASEMANDIALOG_H
