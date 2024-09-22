#ifndef EDITUSERDIALOG_H
#define EDITUSERDIALOG_H

#include <QDialog>
class DanetDbMan;

namespace Ui {
class EditUserDialog;
}

class EditUserDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EditUserDialog(QWidget *parent, DanetDbMan *db, int Id,QString Name, QString Lastname,int Area, QString Username);
    ~EditUserDialog();

private slots:
    void on_passCB_toggled(bool checked);
    void on_adminPasswordLE_textChanged(const QString &arg1);
    void on_passwordLE_textChanged(const QString &arg1);
    void on_PassVerifyLE_textChanged(const QString &arg1);
    void on_nameLE_textChanged(const QString &arg1);
    void on_lastnameLE_textChanged(const QString &arg1);
    void on_areaLE_textChanged(const QString &arg1);
    void on_usernameLE_textChanged(const QString &arg1);
    void on_cancelBtn_clicked();
    void on_modifyBtn_clicked();

private:
    Ui::EditUserDialog *ui;
    DanetDbMan *dbMan;
    QString adminPass;
    int id;
};

#endif // EDITUSERDIALOG_H
