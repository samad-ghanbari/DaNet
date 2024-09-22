#ifndef ADDUSERDIALOG_H
#define ADDUSERDIALOG_H

#include <QDialog>

class DanetDbMan;

namespace Ui {
class AddUserDialog;
}

class AddUserDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddUserDialog(QWidget *parent, DanetDbMan *db);
    ~AddUserDialog();
    void resetLE();

private slots:
    void on_cancelBtn_clicked();
    void on_adminPasswordLE_textChanged(const QString &arg1);
    void on_passwordLE_textChanged(const QString &arg1);
    void on_nameLE_textChanged();
    void on_lastnameLE_textChanged();
    void on_areaLE_textChanged();
    void on_usernameLE_textChanged();
    void on_passwordVerifyLE_textChanged(const QString &arg1);
    void on_okBtn_clicked();

private:
    Ui::AddUserDialog *ui;
    DanetDbMan *dbMan;
    QString adminHashedPassword;

};

#endif // ADDUSERDIALOG_H
