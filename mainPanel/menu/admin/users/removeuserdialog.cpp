#include "removeuserdialog.h"
#include "ui_removeuserdialog.h"
#include <QCryptographicHash>
#include <QMessageBox>
#include "lib/database/danetdbman.h"

RemoveUserDialog::RemoveUserDialog(QWidget *parent, DanetDbMan *db, int Id, QString name,QString lastname,int area, QString username) :
    QDialog(parent),
    ui(new Ui::RemoveUserDialog),
    dbMan(db)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowMinMaxButtonsHint);

    ui->removeBtn->setAcceptDrops(true);
    adminHashPass = dbMan->getUserHashedPassword(ADMIN);

    id = Id;
    ui->nameLbl->setText(name);
    ui->lastnameLbl->setText(lastname);
    ui->areaLbl->setText(QString::number(area));
    ui->usernameLbl->setText(username);

    if(username.compare(ADMIN, Qt::CaseInsensitive) == 0)
    {
        ui->adminPassLE->setEnabled(false);
        ui->removeBtn->setEnabled(false);
    }

}

RemoveUserDialog::~RemoveUserDialog()
{
    delete ui;
}


void RemoveUserDialog::on_cancelBtn_clicked()
{
    this->close();
}

void RemoveUserDialog::on_adminPassLE_textChanged(const QString &arg1)
{
    ui->removeBtn->setEnabled(false);
    QString hashMd5 = QString(QCryptographicHash::hash(arg1.toUtf8(), QCryptographicHash::Md5).toHex());
    if(hashMd5.compare(adminHashPass, Qt::CaseSensitive) == 0)
        ui->removeBtn->setEnabled(true);
}

void RemoveUserDialog::on_removeBtn_clicked()
{
    if(dbMan->deleteUser(id))
    {
        this->close();
    }
    else
        QMessageBox::information(this, "Error", "Can not remove user.\n"+dbMan->lastError());}
