#ifndef CARDMANDIALOG_H
#define CARDMANDIALOG_H

#include <QDialog>
#include <QMenu>
class QSqlQuery;
class QSqlQueryModel;
class DanetDbMan;

namespace Ui {
class CardManDialog;
}

class CardManDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CardManDialog(QWidget *parent, DanetDbMan *db);
    ~CardManDialog();
    void fillCardTV();
    void fillCardTV(const QString Dev);

private:
    Ui::CardManDialog *ui;
    DanetDbMan *dbMan;
    QMenu contextMenu;
    QSqlQueryModel *model;
    int row,id, adsl, vdsl, shdsl, e, G, G10, G40, G100;
    QString card,device, type;
    bool AdminUserFlag;

private slots:
    void refreshSlot(int i);
    void contextMenuSlot(QPoint ptr);
    void on_adminAddCardBtn_clicked();
    void on_actionAdminEditCard_triggered();
    void on_actionAdminRemoveCard_triggered();

    void on_filterLE_textChanged(const QString &arg1);
    void on_adminLE_textChanged(const QString &arg1);
};

#endif // CARDMANDIALOG_H
