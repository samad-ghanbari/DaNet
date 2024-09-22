#ifndef BACKBONE_H
#define BACKBONE_H

#include <QObject>

class DanetDbMan;
class QSplashScreen;
class MainPanel;
class LoginDialog;
class NewRelease;
class OutOfService;

class BackBone : public QObject
{
    Q_OBJECT
public:
    explicit BackBone(QObject *parent = 0);
    ~BackBone();
    void closeAll();
    void initiate();

private slots:
    void loginSlot(bool ok);
    void closeDanetSlot();
    void showLoginDialogSlot();

private:
    DanetDbMan *dbMan;
    LoginDialog *loginDialog;
    QSplashScreen *splash;
    MainPanel *mainPanel;

};

#endif // BACKBONE_H
