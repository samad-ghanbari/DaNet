#ifndef SALOONMANDIALOG_H
#define SALOONMANDIALOG_H

#include <QDialog>
#include <QMenu>

class QSqlQueryModel;
class DanetDbMan;

namespace Ui {
class SaloonManDialog;
}

class SaloonManDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SaloonManDialog(QWidget *parent, DanetDbMan *db);
    ~SaloonManDialog();
    void fillTV();

private slots:
    void refreshSlot(int done);
    void contextMenuSlot(QPoint ptr);
    void on_actionEdit_Saloon_triggered();
    void on_actionRemove_Saloon_triggered();
    void on_addSaloonBtn_clicked();
    void on_adminLE_textChanged(const QString &arg1);

private:
    Ui::SaloonManDialog *ui;
    DanetDbMan *dbMan;
    QSqlQueryModel *model;
    QMenu contextMenu;
    int id, row;
    QString saloon;
    bool AdminUserFlag;
};

#endif // SALOONMANDIALOG_H
