#include "removepvcrefdialog.h"
#include "ui_removepvcrefdialog.h"
#include "lib/database/danetdbman.h"
#include <QMessageBox>

RemovePvcRefDialog::RemovePvcRefDialog(QWidget *parent, DanetDbMan *db, const int Id, const int vpi, const int vci, QString desc) :
    QDialog(parent),
    ui(new Ui::RemovePvcRefDialog),
    dbMan(db),
    id(Id)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowMinMaxButtonsHint);

    ui->vpiLbl->setText(QString::number(vpi));
    ui->vciLbl->setText(QString::number(vci));
    ui->descTE->setText(desc);

    ui->removeBtn->setEnabled(false);
    ui->checkBox->setChecked(false);
}

RemovePvcRefDialog::~RemovePvcRefDialog()
{
    delete ui;
}

void RemovePvcRefDialog::on_cancelBtn_clicked()
{
    this->close();
}

void RemovePvcRefDialog::on_removeBtn_clicked()
{
    QString pvc = ui->vpiLbl->text()+"/"+ui->vciLbl->text();
    QString desc = ui->descTE->toPlainText();
    QString username = dbMan->getLoggedinNameLastname();
    dbMan->insertLog(username, "Remove PVC: "+pvc+" Desc: "+desc);

    if(dbMan->deletePvcRef(id))
        this->close();
    else
        QMessageBox::information(this, "Error", "Can not remove PVC.\n"+dbMan->lastError());
}

void RemovePvcRefDialog::on_checkBox_toggled(bool checked)
{
    if(checked)
        ui->removeBtn->setEnabled(true);
    else
        ui->removeBtn->setEnabled(false);
}
