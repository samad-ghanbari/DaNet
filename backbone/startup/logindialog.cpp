#include "logindialog.h"
#include "ui_logindialog.h"
#include "lib/database/danetdbman.h"
#include <QMessageBox>
#include <QSqlQuery>



LoginDialog::LoginDialog(DanetDbMan *db, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog),
    dbMan(db)
{
    ui->setupUi(this);

    mainDb = true;
    newRelease = false;
    outOfService = false;
    databaseConnected = false;
    NewDanetVer = "";
    NewDanetPath = "";

    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    //    setAttribute(Qt::WA_TranslucentBackground);

    ui->dbCb->addItem("Main Database", 0);
    ui->dbCb->addItem("Backup Database", 1);
    ui->dbCb->setCurrentIndex(0);

    ui->confirmBtn->setEnabled(false);
    ui->invalidLbl->hide();
    ui->usernameLE->setFocus();
    ui->versionLbl->setText("Version "+QString(DANET_VER));
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

void LoginDialog::on_cancelBtn_clicked()
{
    emit closeDanetSignal();
    this->close();
}

void LoginDialog::on_confirmBtn_clicked()
{
    QString username, pass;
    username = ui->usernameLE->text();
    pass = ui->passwordLE->text();


    if(ui->dbCb->currentData().toInt() == 0)
        mainDb = true;
    else
        mainDb = false;

    //database connection
    if(mainDb)
    {
        if(dbMan->connectMainDb())
        {
            databaseConnected = true;
            //check version
            NewDanetVer = "";
            NewDanetPath = "";
            QSqlQuery *query = dbMan->selectLastVersion();
            if(query->next())
            {
                NewDanetVer = query->value(0).toString();
                NewDanetVer = NewDanetVer.trimmed();
                NewDanetPath = query->value(1).toString();
            }


            if(NewDanetVer.startsWith("-", Qt::CaseInsensitive))
            {
                outOfService = true;
                this->close();
            }
            else if(NewDanetVer.compare(DANET_VER, Qt::CaseInsensitive) > 0)
            {
                newRelease = true;
                this->close();
            }
        }
        else
            databaseConnected = false;
    }
    else
    {
        if(dbMan->connectBackupDb())
            databaseConnected = true;
        else
            databaseConnected = false;
    }

    if(databaseConnected)
    {
        if(dbMan->confirmUser(username, pass))
        {
            this->close();
            emit dialogDone(true);
        }
        else
            ui->invalidLbl->show();
    }
    else
    {
        this->close();
        emit dialogDone(false);
    }

}

void LoginDialog::on_usernameLE_textChanged(const QString &arg1)
{
    ui->invalidLbl->hide();
    if(arg1.isEmpty() | ui->passwordLE->text().isEmpty())
        ui->confirmBtn->setEnabled(false);
    else ui->confirmBtn->setEnabled(true);
}

void LoginDialog::on_passwordLE_textChanged(const QString &arg1)
{
    ui->invalidLbl->hide();
    if(arg1.isEmpty() | ui->usernameLE->text().isEmpty())
        ui->confirmBtn->setEnabled(false);
    else ui->confirmBtn->setEnabled(true);
}
