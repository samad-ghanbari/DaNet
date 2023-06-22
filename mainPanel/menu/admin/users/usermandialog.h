#ifndef USERMANDIALOG_H
#define USERMANDIALOG_H

#include <QDialog>
#include <QMenu>

class DanetDbMan;
class QSqlQuery;
class QSqlQueryModel;

namespace Ui {
class UserManDialog;
}

class UserManDialog : public QDialog
{
    Q_OBJECT

public:
    explicit UserManDialog(QWidget *parent, DanetDbMan *db);
    ~UserManDialog();
    void fillUserTableView(QSqlQuery *query);

private slots:
    void contextMenuSlot(QPoint ptr);
    void on_addNewUserBtn_clicked();
    void on_actionRemove_triggered();
    void on_actionEditUser_triggered();

private:
    Ui::UserManDialog *ui;
    DanetDbMan *dbMan;
    QSqlQueryModel *userModel;
    QMenu contextMenu;
    int row,id, area;
    QString name, lastname, username;
};

#endif // USERMANDIALOG_H
