#include "backbone.h"
#include <QSqlQuery>
#include <QVariant>
#include <QSplashScreen>
#include <QTimer>
#include <QMessageBox>
#include "lib/database/danetdbman.h"
#include "backbone/startup/logindialog.h"
#include "backbone/startup/newrelease.h"
#include "backbone/startup/outofservice.h"
#include "mainPanel/mainpanel.h"


BackBone::BackBone(QObject *parent) :
    QObject(parent),
    dbMan(new DanetDbMan(this)),
    loginDialog(NULL),
    splash(new QSplashScreen(QPixmap(":/Image/splash.png"))),
    mainPanel(nullptr)
{}

BackBone::~BackBone()
{
    closeAll();
}

void BackBone::closeAll()
{
    if(mainPanel)
    {
        delete mainPanel;
        mainPanel = NULL;
    }
    if(splash)
    {
        delete splash;
        splash = NULL;
    }

    delete loginDialog;
    loginDialog = NULL;
}

void BackBone::initiate()
{
    splash->setEnabled(true);
    splash->show();
    QTimer::singleShot(500, this,SLOT(showLoginDialogSlot()));
}

void BackBone::closeDanetSlot()
{
    splash->close();

    closeAll();
    exit(0);
}

void BackBone::showLoginDialogSlot()
{
    loginDialog = new LoginDialog(dbMan);
    connect(loginDialog, SIGNAL(dialogDone(bool)), this, SLOT(loginSlot(bool)));
    connect(loginDialog, SIGNAL(closeDanetSignal()), this, SLOT(closeDanetSlot()));
    loginDialog->show();
    splash->setPixmap(QPixmap(":/Image/splash_login.png"));
    splash->move(splash->x(), splash->y()-280);

}

void BackBone::loginSlot(bool ok)
{
    splash->close();
    delete splash;
    splash = NULL;

    bool databaseConnected = loginDialog->isDatabaseConnected();
    bool mainDb = loginDialog->isMainDatabase();
    bool newReleaseFlag = loginDialog->newReleaseAvailable();
    bool outOfServiceFlag = loginDialog->isOutOsService();
    QString newVer = loginDialog->getNewDanetVersion();
    QString path = loginDialog->getNewDanetPath();

    delete loginDialog;
    loginDialog = NULL;

    if(mainDb)
    {
        if(databaseConnected)
        {
            if(newReleaseFlag)
            {
                NewRelease newDanetRelease;
                newDanetRelease.setVersionLabel(newVer);
                newDanetRelease.setPathLabel(path);
                newDanetRelease.exec();
                closeAll();
                exit(0);

            }
            else if(outOfServiceFlag)
            {
                OutOfService outOfService;
                outOfService.exec();
                closeAll();
                exit(0);
            }
            else
            {
                if(ok && dbMan->isUserLoggedIn())
                {
                    bool adminUser = false;
                    QString username = dbMan->getLoggedinUserame();
                    if(username.compare(ADMIN,Qt::CaseInsensitive) == 0)
                        adminUser = true;


                    mainPanel = new MainPanel(dbMan,adminUser, mainDb);
                    mainPanel->showMaximized();
                }
            }
        }
        else
        {
            QMessageBox msg;
            msg.setWindowTitle("DaNet Database Error!");
            msg.setText("Database connection failed.");
            msg.setInformativeText("Check your database server.");
            msg.setStyleSheet("QLabel{min-width: 250px;color: darkred; font: bold 12px;}");
            msg.setWindowIcon(QIcon(":/Image/error.png"));
            msg.exec();
            closeAll();
            exit(0);
        }
    }
    else
    {
        //backup database
        if(databaseConnected)
        {
            if(ok && dbMan->isUserLoggedIn())
            {
                mainPanel = new MainPanel(dbMan,false, mainDb);
                mainPanel->showMaximized();
            }
        }
        else
        {
            QMessageBox msg;
            msg.setWindowTitle("DaNet Database Error!");
            msg.setText("Database connection failed.");
            msg.setInformativeText("Backup database should be restored first.");
            msg.setStyleSheet("QLabel{min-width: 250px;color: darkred; font: bold 12px;}");
            msg.setWindowIcon(QIcon(":/Image/error.png"));
            msg.exec();
            closeAll();
            exit(0);
        }
    }
}


