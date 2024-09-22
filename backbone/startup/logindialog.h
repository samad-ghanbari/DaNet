#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
class DanetDbMan;

namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(DanetDbMan *db, QWidget *parent=0);
    ~LoginDialog();
    bool isMainDatabase(){return mainDb;}
    bool newReleaseAvailable(){return newRelease;}
    bool isOutOsService(){return outOfService;}
    bool isDatabaseConnected(){return databaseConnected;}
    QString getNewDanetVersion(){return NewDanetVer;}
    QString getNewDanetPath(){return NewDanetPath;}

signals:
    void dialogDone(bool ok);
    void closeDanetSignal();

private slots:
    void on_cancelBtn_clicked();
    void on_confirmBtn_clicked();
    void on_usernameLE_textChanged(const QString &arg1);
    void on_passwordLE_textChanged(const QString &arg1);

private:
    Ui::LoginDialog *ui;
    DanetDbMan *dbMan;
    bool mainDb, newRelease, outOfService, databaseConnected;
    QString NewDanetVer, NewDanetPath;
};

#endif // LOGINDIALOG_H
