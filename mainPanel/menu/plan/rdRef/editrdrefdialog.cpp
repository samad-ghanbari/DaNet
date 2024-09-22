#include "editrdrefdialog.h"
#include "ui_editrdrefdialog.h"
#include "lib/database/danetdbman.h"
#include <QSqlQuery>
#include <QMessageBox>
#include <QRegExpValidator>

EditRdRefDialog::EditRdRefDialog(QWidget *parent, DanetDbMan *db, const int Id, const QString rd, const QString desc) :
    QDialog(parent),
    ui(new Ui::EditRdRefDialog),
    dbMan(db),
    id(Id),
    regex("^(\\d+)\\:(\\d+)$"),
    validator(new QRegExpValidator(regex, this))
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowMinMaxButtonsHint);

    ui->rdLE->setText(rd);
    ui->descTE->setText(desc);
    oldRd = rd;
    ui->okBtn->setEnabled(false);
}

EditRdRefDialog::~EditRdRefDialog()
{
    delete ui;
}

void EditRdRefDialog::checkEntry()
{
    bool d = !ui->descTE->toPlainText().isEmpty();
    bool r = ui->rdLE->hasAcceptableInput();

    if(d && r)
        ui->okBtn->setEnabled(true);
    else
        ui->okBtn->setEnabled(false);
}

void EditRdRefDialog::on_CancelBtn_clicked()
{
    this->close();
}

void EditRdRefDialog::on_okBtn_clicked()
{
    QString rd = ui->rdLE->text();
    QString desc = ui->descTE->toPlainText();
    QString username = dbMan->getLoggedinNameLastname();
    dbMan->insertLog(username, "Edit RD Reference: "+oldRd + " To RD: "+rd+" Desc: "+desc);

    if(dbMan->updateRdRef(id,rd, desc))
        this->close();
    else
        QMessageBox::information(this, "Error", "Can not Edit RD.\n"+dbMan->lastError());
}

void EditRdRefDialog::on_rdLE_textEdited(const QString &arg1)
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

void EditRdRefDialog::on_descTE_textChanged()
{
    checkEntry();
}
