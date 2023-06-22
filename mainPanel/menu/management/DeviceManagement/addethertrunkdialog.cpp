#include "addethertrunkdialog.h"
#include "ui_addethertrunkdialog.h"
#include "lib/database/danetdbman.h"
#include <QMessageBox>

AddEtherTrunkDialog::AddEtherTrunkDialog(QWidget *parent, DanetDbMan *db, int DevexId, QString Abbr, QString Saloon, QString Device) :
    QDialog(parent),
    ui(new Ui::AddEtherTrunkDialog),
    dbMan(db),
    devexId(DevexId)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowMinMaxButtonsHint);

    ui->abbrLbl->setText(Abbr);
    ui->saloonLbl->setText(Saloon);
    ui->deviceLbl->setText(Device);

    ui->okBtn->setEnabled(false);
}

AddEtherTrunkDialog::~AddEtherTrunkDialog()
{
    delete ui;
}

void AddEtherTrunkDialog::on_cancelBtn_clicked()
{
    this->close();
}

void AddEtherTrunkDialog::on_okBtn_clicked()
{
    int ether = ui->etherSB->value();
    QString desc = ui->descLE->text();
    if(dbMan->insertEtherTrunk(devexId, ether, desc))
    {
        this->close();
    }
    else
        QMessageBox::warning(this,"Error", "Cannot insert Ether-Trunk.  Duplicate Ether-Trunk entry is not permitted.\n"+dbMan->lastError());
}

void AddEtherTrunkDialog::on_descLE_textChanged(const QString &arg1)
{
    if(arg1.isEmpty())
        ui->okBtn->setEnabled(false);
    else
        ui->okBtn->setEnabled(true);
}

void AddEtherTrunkDialog::on_etherSB_valueChanged(int arg1)
{
    arg1++;
    if(ui->descLE->text().isEmpty())
        ui->okBtn->setEnabled(false);
    else
        ui->okBtn->setEnabled(true);
}
