#include "profiledialog.h"
#include "ui_profiledialog.h"
#include "lib/database/danetdbman.h"
#include <QCryptographicHash>
#include <QMessageBox>

ProfileDialog::ProfileDialog(QWidget *parent, DanetDbMan *db) :
    QDialog(parent),
    ui(new Ui::ProfileDialog),
    dbMan(db)
{
    ui->setupUi(this);

    int workingArea = dbMan->getLoggedInDefaultArea();
    name = dbMan->getLoggedinNameLastname();
    username = dbMan->getLoggedinUserame();
    userId = dbMan->getLoggedinUserId();
    currentPass = dbMan->getLoggedinUserPassword();

    //area
    ui->areaCB->blockSignals(true);
    ui->areaCB->addItem("2", 2);
    ui->areaCB->addItem("3", 3);
    ui->areaCB->addItem("4", 4);
    ui->areaCB->addItem("5", 5);
    ui->areaCB->addItem("6", 6);
    ui->areaCB->addItem("7", 7);
    ui->areaCB->addItem("8", 8);
    ui->areaCB->addItem("Karaj", 9);
    ui->areaCB->setCurrentIndex(ui->areaCB->findData(workingArea));
    ui->areaCB->blockSignals(false);


    ui->passChB->setChecked(false);
    ui->passFrame->hide();

    ui->nameLbl->setText(name);
    ui->usernameLE->setText(username);
    ui->okBtn->setEnabled(false);
    ui->areaCB->setEnabled(false);

}

ProfileDialog::~ProfileDialog()
{
    delete ui;
}

void ProfileDialog::checkEntry()
{
    if(permitFlag)
    {
        bool USERNAME = !ui->usernameLE->text().trimmed().isEmpty();
        bool AREA = !ui->areaCB->currentText().isEmpty();
        bool PCB = ui->passChB->isChecked();
        QString p1 = ui->passLE->text().trimmed();
        QString p2 = ui->passConfirmLE->text().trimmed();
        bool PC = false;
        if(!p1.isEmpty() && (p1.compare(p2) == 0) )
            PC = true;

        if(USERNAME && AREA && ( (PCB && PC) || (!PCB) ) )
            ui->okBtn->setEnabled(true);
        else
            ui->okBtn->setEnabled(false);
    }
    else
        ui->okBtn->setEnabled(false);
}

void ProfileDialog::on_cancelBtn_clicked()
{
    this->close();
}

void ProfileDialog::on_okBtn_clicked()
{
    if(permitFlag)
    {
        QString UserName = ui->usernameLE->text().trimmed();
        int Area = ui->areaCB->currentData().toInt();
        bool PCB = ui->passChB->isChecked();
        QString Pass = ui->passLE->text().trimmed();
        if(Area != dbMan->getLoggedInDefaultArea())
            dbMan->setUserDefaultArea(Area);

        if(PCB)
        {
            Pass = QString(QCryptographicHash::hash(Pass.toUtf8(), QCryptographicHash::Md5).toHex());

            if(dbMan->updateUser(userId, UserName,Area, Pass))
                this->close();
            else
                QMessageBox::warning(this, "ERROR", "Cannot update user profile.\n"+dbMan->lastError());
        }
        else
        {
            if(dbMan->updateUser(userId, UserName,Area))
                this->close();
            else
                QMessageBox::warning(this, "ERROR", "Cannot update user profile.\n"+dbMan->lastError());
        }
    }
}

void ProfileDialog::on_usernameLE_textChanged(const QString &arg1)
{
    if(arg1.isEmpty())
        ui->okBtn->setEnabled(false);
    else
        checkEntry();
}

void ProfileDialog::on_areaCB_currentIndexChanged(int index)
{
    index++;
    checkEntry();
}

void ProfileDialog::on_passChB_toggled(bool checked)
{
    if(checked)
        ui->passFrame->show();
    else
    {
        ui->passFrame->hide();
        ui->passLE->clear();
        ui->passConfirmLE->clear();
    }

    checkEntry();
}

void ProfileDialog::on_passLE_textChanged(const QString &arg1)
{
    QString confirm = ui->passConfirmLE->text();
    if(arg1.compare(confirm) == 0)
    {
        ui->passLbl->setStyleSheet("QLabel{color: rgb(0, 85, 0);}");
        ui->passConfirmLbl->setStyleSheet("QLabel{color: rgb(0, 85, 0);}");
    }
    else
    {
        ui->passLbl->setStyleSheet("QLabel{color: rgb(85, 0, 0);}");
        ui->passConfirmLbl->setStyleSheet("QLabel{color: rgb(85, 0, 0);}");
    }

    checkEntry();
}

void ProfileDialog::on_passConfirmLE_textChanged(const QString &arg1)
{
    QString pass = ui->passLE->text();
    if(arg1.compare(pass) == 0)
    {
        ui->passLbl->setStyleSheet("QLabel{color: rgb(0, 85, 0);}");
        ui->passConfirmLbl->setStyleSheet("QLabel{color: rgb(0, 85, 0);}");
    }
    else
    {
        ui->passLbl->setStyleSheet("QLabel{color: rgb(85, 0, 0);}");
        ui->passConfirmLbl->setStyleSheet("QLabel{color: rgb(85, 0, 0);}");
    }

    checkEntry();
}

void ProfileDialog::on_currentPassLE_textChanged(const QString &arg1)
{
    QString hashMd5 = QString(QCryptographicHash::hash(arg1.toUtf8(), QCryptographicHash::Md5).toHex());
    if(hashMd5.compare(currentPass) == 0)
    {
        ui->frame->setEnabled(true);
        permitFlag = true;

        ui->okBtn->setEnabled(true);
    }
    else
    {
        permitFlag = false;
        ui->frame->setEnabled(false);
        ui->passLE->clear();
        ui->passConfirmLE->clear();
        ui->passChB->setChecked(false);
        ui->passFrame->hide();
    }
}
