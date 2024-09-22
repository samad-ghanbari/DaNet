#include "adduserdialog.h"
#include "ui_adduserdialog.h"
#include <QCryptographicHash>
#include "lib/database/danetdbman.h"
#include <QMessageBox>
#include <QIntValidator>

AddUserDialog::AddUserDialog(QWidget *parent, DanetDbMan *db) :
    QDialog(parent),
    ui(new Ui::AddUserDialog),
    dbMan(db)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowMinMaxButtonsHint);
    adminHashedPassword = dbMan->getUserHashedPassword(ADMIN);

    QIntValidator *validator =new QIntValidator(this);
    validator->setBottom(2);
    validator->setTop(9);
    ui->areaLE->setValidator(validator);
}

AddUserDialog::~AddUserDialog()
{
    delete ui;
}

void AddUserDialog::resetLE()
{
    ui->okBtn->setEnabled(false);
    ui->passwordLE->clear();
    ui->passwordVerifyLE->clear();
}

void AddUserDialog::on_cancelBtn_clicked()
{
    this->close();
}

void AddUserDialog::on_adminPasswordLE_textChanged(const QString &arg1)
{
    ui->groupBox->setEnabled(false);
    QString hashMd5 = QString(QCryptographicHash::hash(arg1.toUtf8(), QCryptographicHash::Md5).toHex());
    if(adminHashedPassword.compare(hashMd5, Qt::CaseSensitive) == 0)
        ui->groupBox->setEnabled(true);

}

void AddUserDialog::on_passwordLE_textChanged(const QString &arg1)
{
    if(arg1.compare(ui->passwordVerifyLE->text(), Qt::CaseSensitive) == 0)
    {
        if(ui->nameLE->text().isEmpty() | ui->lastnameLE->text().isEmpty() |
                ui->areaLE->text().isEmpty() | ui->usernameLE->text().isEmpty())
            ui->okBtn->setEnabled(false);
        else
            ui->okBtn->setEnabled(true);
    }
    else
        ui->okBtn->setEnabled(false);
}

void AddUserDialog::on_passwordVerifyLE_textChanged(const QString &arg1)
{
    if(arg1.compare(ui->passwordLE->text(), Qt::CaseSensitive) == 0)
    {
        if(ui->nameLE->text().isEmpty() | ui->lastnameLE->text().isEmpty() |
                ui->areaLE->text().isEmpty() | ui->usernameLE->text().isEmpty())
            ui->okBtn->setEnabled(false);
        else
            ui->okBtn->setEnabled(true);
    }
    else
        ui->okBtn->setEnabled(false);
}

void AddUserDialog::on_nameLE_textChanged()
{
    ui->okBtn->setEnabled(false);
    ui->passwordLE->clear();
    ui->passwordVerifyLE->clear();
}

void AddUserDialog::on_lastnameLE_textChanged()
{
    resetLE();
}

void AddUserDialog::on_areaLE_textChanged()
{
    resetLE();
}

void AddUserDialog::on_usernameLE_textChanged()
{
    resetLE();
}

void AddUserDialog::on_okBtn_clicked()
{
    QString name, lastname, username, password;
    int area;

    name = ui->nameLE->text();
    lastname = ui->lastnameLE->text();
    area = ui->areaLE->text().toInt();
    username = ui->usernameLE->text();
    password = ui->passwordLE->text();

    if(dbMan->insertUser(name, lastname, area, username, password))
    {
        this->close();
    }
    else
    {
        QMessageBox::information(this, "Error", "Can not add new user.\n"+dbMan->lastError());
    }

}





