#include "removerdrefdialog.h"
#include "ui_removerdrefdialog.h"
#include "lib/database/danetdbman.h"
#include <QMessageBox>

RemoveRdRefDialog::RemoveRdRefDialog(QWidget *parent, DanetDbMan *db, int Id, const QString Rd, const QString Desc) :
    QDialog(parent),
    ui(new Ui::RemoveRdRefDialog),
    dbMan(db),
    id(Id),
    rd(Rd),
    desc(Desc)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowMinMaxButtonsHint);

    ui->rdLbl->setText(rd);
    ui->descTE->setText(desc);

    ui->removeBtn->setEnabled(false);
    ui->checkBox->setChecked(false);
}

RemoveRdRefDialog::~RemoveRdRefDialog()
{
    delete ui;
}

void RemoveRdRefDialog::on_cancelBtn_clicked()
{
    this->close();
}

void RemoveRdRefDialog::on_removeBtn_clicked()
{
    QString username = dbMan->getLoggedinNameLastname();
    dbMan->insertLog(username, "Remove RD Reference: "+rd+" Desc: "+desc);

    if(dbMan->deleteRdRef(id))
        this->close();
    else
        QMessageBox::information(this, "Error", "Can not remove RD.\n"+dbMan->lastError());
}

void RemoveRdRefDialog::on_checkBox_toggled(bool checked)
{
    if(checked)
        ui->removeBtn->setEnabled(true);
    else
        ui->removeBtn->setEnabled(false);
}
