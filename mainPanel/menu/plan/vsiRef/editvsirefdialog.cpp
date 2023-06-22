#include "editvsirefdialog.h"
#include "ui_editvsirefdialog.h"
#include "lib/database/danetdbman.h"
#include <QSqlQuery>
#include <QMessageBox>

EditVsiRefDialog::EditVsiRefDialog(QWidget *parent, DanetDbMan *db, const int Id, const int vsi, const QString desc) :
    QDialog(parent),
    ui(new Ui::EditVsiRefDialog),
    dbMan(db),
    id(Id),
    oldVSI(QString::number(vsi))
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowMinMaxButtonsHint);

    ui->vsiSB->setValue(vsi);
    ui->descTE->setText(desc);

    ui->okBtn->setEnabled(false);
}

EditVsiRefDialog::~EditVsiRefDialog()
{
    delete ui;
}

void EditVsiRefDialog::checkEntry()
{
    bool d =ui->descTE->toPlainText().isEmpty();

    if(d)
        ui->okBtn->setEnabled(false);
    else
        ui->okBtn->setEnabled(true);
}

void EditVsiRefDialog::on_CancelBtn_clicked()
{
    this->close();
}

void EditVsiRefDialog::on_okBtn_clicked()
{
    int vsi = ui->vsiSB->value();
    QString desc = ui->descTE->toPlainText();
    QString username = dbMan->getLoggedinNameLastname();
    dbMan->insertLog(username, "Edit VSI Reference: "+oldVSI+" To VSI: "+QString::number(vsi) + " Desc: "+desc);

    if(dbMan->updateVsiRef(id,vsi, desc))
        this->close();
    else
        QMessageBox::information(this, "Error", "Can not edit VSI.\n"+dbMan->lastError());
}

void EditVsiRefDialog::on_vsiSB_valueChanged(int arg1)
{
    arg1++;
    checkEntry();
}

void EditVsiRefDialog::on_descTE_textChanged()
{
    checkEntry();
}
