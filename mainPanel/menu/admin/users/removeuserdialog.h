#ifndef REMOVEUSERDIALOG_H
#define REMOVEUSERDIALOG_H

#include <QDialog>
class DanetDbMan;

namespace Ui {
class RemoveUserDialog;
}

class RemoveUserDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RemoveUserDialog(QWidget *parent, DanetDbMan *db, int id, QString name, QString lastname, int area, QString username);
    ~RemoveUserDialog();

private slots:
    void on_cancelBtn_clicked();
    void on_adminPassLE_textChanged(const QString &arg1);
    void on_removeBtn_clicked();

private:
    Ui::RemoveUserDialog *ui;
    DanetDbMan *dbMan;
    QString adminHashPass;
    int id;

};

#endif // REMOVEUSERDIALOG_H
