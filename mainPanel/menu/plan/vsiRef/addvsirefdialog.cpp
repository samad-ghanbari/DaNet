#include "addvsirefdialog.h"
#include "ui_addvsirefdialog.h"
#include "lib/database/danetdbman.h"
#include <QSqlQuery>
#include <QMessageBox>

AddVsiRefDialog::AddVsiRefDialog(QWidget *parent, DanetDbMan *db) :
    QDialog(parent),
    ui(new Ui::AddVsiRefDialog),
    dbMan(db)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowMinMaxButtonsHint);

}

AddVsiRefDialog::~AddVsiRefDialog()
{
    delete ui;
}

void AddVsiRefDialog::checkEntry()
{
    bool d = ui->descTE->toPlainText().isEmpty();
    if(d)
        ui->okBtn->setEnabled(false);
    else
        ui->okBtn->setEnabled(true);
}

void AddVsiRefDialog::on_CancelBtn_clicked()
{
    this->close();
}

void AddVsiRefDialog::on_okBtn_clicked()
{
    int vsi = ui->vsiSB->value();
    QString desc = ui->descTE->toPlainText();

    QString username = dbMan->getLoggedinNameLastname();
    dbMan->insertLog(username, "Add VSI Reference: "+QString::number(vsi) + " Desc: "+desc);

    if(dbMan->insertVsiRef(vsi, desc))
        this->close();
    else
        QMessageBox::information(this, "Error", "Can not Add VSI.\n"+dbMan->lastError());
}

void AddVsiRefDialog::on_descTE_textChanged()
{
    checkEntry();
}
