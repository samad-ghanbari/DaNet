#include "removeethertrunkdialog.h"
#include "ui_removeethertrunkdialog.h"
#include "lib/database/danetdbman.h"
#include <QMessageBox>


RemoveEtherTrunkDialog::RemoveEtherTrunkDialog(QWidget *parent, DanetDbMan *db, int EtherId, QString Abbr, QString Saloon, QString Device, int Ether, QString Desc) :
    QDialog(parent),
    ui(new Ui::RemoveEtherTrunkDialog),
    dbMan(db),
    etherId(EtherId)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowMinMaxButtonsHint);

    ui->checkBox->setChecked(false);
    ui->okBtn->setEnabled(false);

    ui->abbrLbl->setText(Abbr);
    ui->saloonLbl->setText(Saloon);
    ui->deviceLbl->setText(Device);
    ui->etherLbl->setText(QString::number(Ether));
    ui->descLbl->setText(Desc);

}

RemoveEtherTrunkDialog::~RemoveEtherTrunkDialog()
{
    delete ui;
}

void RemoveEtherTrunkDialog::on_checkBox_toggled(bool checked)
{
    if(checked)
        ui->okBtn->setEnabled(true);
    else
        ui->okBtn->setEnabled(false);
}

void RemoveEtherTrunkDialog::on_cancelBtn_clicked()
{
    this->close();
}

void RemoveEtherTrunkDialog::on_okBtn_clicked()
{
    if(dbMan->deleteEtherTrunk(etherId))
    {
        this->close();
    }
    else
        QMessageBox::warning(this, "Error", "Cannot remove Ether-Trunk.\n"+dbMan->lastError());
}







