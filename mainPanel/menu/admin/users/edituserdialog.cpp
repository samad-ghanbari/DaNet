#include "edituserdialog.h"
#include "ui_edituserdialog.h"
#include <QCryptographicHash>
#include <QMessageBox>
#include "lib/database/danetdbman.h"
#include <QIntValidator>

EditUserDialog::EditUserDialog(QWidget *parent, DanetDbMan *db, int Id, QString Name, QString LastName, int Area, QString Username) :
    QDialog(parent),
    ui(new Ui::EditUserDialog),
    dbMan(db)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowMinMaxButtonsHint);

    ui->frame->setEnabled(false);
    ui->modifyBtn->setEnabled(false);
    ui->passCB->setChecked(false);
    ui->passFrame->setEnabled(false);

    adminPass = dbMan->getUserHashedPassword(ADMIN);
    ui->areaLE->setValidator(new QIntValidator(2,9, this));

    id = Id;
    ui->nameLE->setText(Name);
    ui->lastnameLE->setText(LastName);
    ui->areaLE->setText(QString::number(Area));
    ui->usernameLE->setText(Username);
    ui->modifyBtn->setEnabled(false);
}

EditUserDialog::~EditUserDialog()
{
    delete ui;
}

void EditUserDialog::on_passCB_toggled(bool checked)
{
    if(checked)
    {
        ui->passFrame->setEnabled(true);
        ui->modifyBtn->setEnabled(false);
    }
    else
    {
        ui->passwordLE->clear();
        ui->PassVerifyLE->clear();
        ui->passFrame->setEnabled(false);
        if(ui->nameLE->text().isEmpty() || ui->lastnameLE->text().isEmpty() ||
                ui->areaLE->text().isEmpty() || ui->usernameLE->text().isEmpty() )
            ui->modifyBtn->setEnabled(false);
        else
            ui->modifyBtn->setEnabled(true);
    }
}

void EditUserDialog::on_adminPasswordLE_textChanged(const QString &arg1)
{
    ui->modifyBtn->setEnabled(false);
    ui->frame->setEnabled(false);
    ui->passFrame->setEnabled(false);
    ui->passCB->setChecked(false);
    QString hashMd5 = QString(QCryptographicHash::hash(arg1.toUtf8(), QCryptographicHash::Md5).toHex());
    if(hashMd5.compare(adminPass, Qt::CaseSensitive) == 0)
    {
        ui->frame->setEnabled(true);
        if(ui->nameLE->text().isEmpty() || ui->lastnameLE->text().isEmpty() ||
                ui->areaLE->text().isEmpty() || ui->usernameLE->text().isEmpty() )
            ui->modifyBtn->setEnabled(false);
        else
            ui->modifyBtn->setEnabled(true);

    }
}

void EditUserDialog::on_passwordLE_textChanged(const QString &arg1)
{
    QString arg2 = ui->PassVerifyLE->text();
    if(arg1.isEmpty() || (arg1.compare(arg2, Qt::CaseSensitive) != 0 ))
        ui->modifyBtn->setEnabled(false);
    else
    {
        if(ui->nameLE->text().isEmpty() || ui->lastnameLE->text().isEmpty() ||
                ui->areaLE->text().isEmpty() || ui->usernameLE->text().isEmpty() )
            ui->modifyBtn->setEnabled(false);
        else
            ui->modifyBtn->setEnabled(true);
    }
}

void EditUserDialog::on_PassVerifyLE_textChanged(const QString &arg1)
{
    QString arg2 = ui->passwordLE->text();
    if(arg1.isEmpty() || (arg1.compare(arg2, Qt::CaseSensitive) != 0 ))
        ui->modifyBtn->setEnabled(false);
    else
    {
        if(ui->nameLE->text().isEmpty() || ui->lastnameLE->text().isEmpty() ||
                ui->areaLE->text().isEmpty() || ui->usernameLE->text().isEmpty() )
            ui->modifyBtn->setEnabled(false);
        else
            ui->modifyBtn->setEnabled(true);
    }
}

void EditUserDialog::on_nameLE_textChanged(const QString &arg1)
{
    ui->passwordLE->clear();
    ui->PassVerifyLE->clear();
    if(ui->passCB->isChecked())
        ui->modifyBtn->setEnabled(false);
    else
    {
        if(arg1.isEmpty() || ui->lastnameLE->text().isEmpty() || ui->areaLE->text().isEmpty()
                || ui->usernameLE->text().isEmpty())
            ui->modifyBtn->setEnabled(false);
        else
            ui->modifyBtn->setEnabled(true);
    }
}

void EditUserDialog::on_lastnameLE_textChanged(const QString &arg1)
{
    ui->passwordLE->clear();
    ui->PassVerifyLE->clear();
    if(ui->passCB->isChecked())
        ui->modifyBtn->setEnabled(false);
    else
    {
        if(arg1.isEmpty() || ui->nameLE->text().isEmpty() || ui->areaLE->text().isEmpty()
                || ui->usernameLE->text().isEmpty())
            ui->modifyBtn->setEnabled(false);
        else
            ui->modifyBtn->setEnabled(true);
    }
}

void EditUserDialog::on_areaLE_textChanged(const QString &arg1)
{
    ui->passwordLE->clear();
    ui->PassVerifyLE->clear();
    if(ui->passCB->isChecked())
        ui->modifyBtn->setEnabled(false);
    else
    {
        if(arg1.isEmpty() || ui->lastnameLE->text().isEmpty() || ui->nameLE->text().isEmpty()
                || ui->usernameLE->text().isEmpty())
            ui->modifyBtn->setEnabled(false);
        else
            ui->modifyBtn->setEnabled(true);
    }
}

void EditUserDialog::on_usernameLE_textChanged(const QString &arg1)
{
    ui->passwordLE->clear();
    ui->PassVerifyLE->clear();
    if(ui->passCB->isChecked())
        ui->modifyBtn->setEnabled(false);
    else
    {
        if(arg1.isEmpty() || ui->lastnameLE->text().isEmpty() || ui->areaLE->text().isEmpty()
                || ui->nameLE->text().isEmpty())
            ui->modifyBtn->setEnabled(false);
        else
            ui->modifyBtn->setEnabled(true);
    }
}

void EditUserDialog::on_cancelBtn_clicked()
{
    this->close();
}

void EditUserDialog::on_modifyBtn_clicked()
{
    QString name, lastname, username, password;
    int area;
    name = ui->nameLE->text();
    lastname = ui->lastnameLE->text();
    username = ui->usernameLE->text();
    area = ui->areaLE->text().toInt();
    password = ui->passwordLE->text();

    if(ui->passCB->isChecked())
    {
        if(dbMan->updateUserProfileAndPassword(id, name, lastname, area, username, password))
        {
            this->close();
        }
        else
            QMessageBox::information(this, "Error", "Can not edit user.\n"+dbMan->lastError());
    }
    else
    {
        if(dbMan->updateUserProfile(id, name, lastname, area, username))
        {
            this->close();
        }
        else
            QMessageBox::information(this, "Error", "Can not edit user.\n"+dbMan->lastError());
    }
}






