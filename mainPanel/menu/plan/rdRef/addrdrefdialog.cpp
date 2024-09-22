#include "addrdrefdialog.h"
#include "ui_addrdrefdialog.h"
#include "lib/database/danetdbman.h"
#include <QSqlQuery>
#include <QMessageBox>
#include <QRegExpValidator>

AddRdRefDialog::AddRdRefDialog(QWidget *parent, DanetDbMan *db) :
    QDialog(parent),
    ui(new Ui::AddRdRefDialog),
    dbMan(db),
    regex("^(\\d+)\\:(\\d+)$"),
    validator(new QRegExpValidator(regex,this))
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowMinMaxButtonsHint);

    ui->rdLE->setValidator(validator);
}

AddRdRefDialog::~AddRdRefDialog()
{
    delete ui;
}

void AddRdRefDialog::checkEntry()
{
    bool d = !ui->descTE->toPlainText().isEmpty();
    bool r = ui->rdLE->hasAcceptableInput();

    if(d && r)
        ui->okBtn->setEnabled(true);
    else
        ui->okBtn->setEnabled(false);
}

void AddRdRefDialog::on_CancelBtn_clicked()
{
    this->close();
}

void AddRdRefDialog::on_okBtn_clicked()
{
    QString rd = ui->rdLE->text();
    QString desc = ui->descTE->toPlainText();
    QString username = dbMan->getLoggedinNameLastname();
    dbMan->insertLog(username, "Add RD Reference: "+rd + " Desc: "+desc);

    if(dbMan->insertRdRef(rd, desc))
        this->close();
    else
        QMessageBox::information(this, "Error", "Can not Add RD.\n"+dbMan->lastError());
}

void AddRdRefDialog::on_rdLE_textEdited(const QString &arg1)
{
    if(arg1.isEmpty())
    {
        ui->okBtn->setEnabled(false);
    }
    else
    {
        if(ui->rdLE->hasAcceptableInput())
        {
            ui->rdLE->setStyleSheet("QLineEdit{color: rgb(85,0,127)}");
            checkEntry();
        }
        else
        {
            ui->rdLE->setStyleSheet("QLineEdit{color: darkred}");
            ui->okBtn->setEnabled(false);
        }
    }

}

void AddRdRefDialog::on_descTE_textChanged()
{
    checkEntry();
}
