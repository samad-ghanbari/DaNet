#include "removenasipdialog.h"
#include "ui_removenasipdialog.h"
#include "lib/database/danetdbman.h"
#include <QMessageBox>

RemoveNasIpDialog::RemoveNasIpDialog(QWidget *parent, DanetDbMan *db, const int Id, const int Area, const QString BRAS, const QString Nas) :
    QDialog(parent),
    ui(new Ui::RemoveNasIpDialog),
    dbMan(db),
    id(Id)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowMinMaxButtonsHint);

    ui->areaLbl->setText(QString::number(Area));
    ui->brasLbl->setText(BRAS);
    ui->nasLbl->setText(Nas);

    ui->checkBox->setChecked(false);
    ui->okBtn->setEnabled(false);

}

RemoveNasIpDialog::~RemoveNasIpDialog()
{
    delete ui;
}

void RemoveNasIpDialog::on_checkBox_toggled(bool checked)
{
    if(checked)
        ui->okBtn->setEnabled(true);
    else
        ui->okBtn->setEnabled(false);
}

void RemoveNasIpDialog::on_cancelBtn_clicked()
{
    this->close();
}

void RemoveNasIpDialog::on_okBtn_clicked()
{
    if(dbMan->deleteBrasNasIp(id))
        this->close();
    else
        QMessageBox::warning(this, "Error", "Cannot remove NAS IP.\n"+dbMan->lastError());
}
