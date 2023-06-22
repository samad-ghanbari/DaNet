#include "editethertrunkdialog.h"
#include "ui_editethertrunkdialog.h"
#include "lib/database/danetdbman.h"
#include <QMessageBox>


EditEtherTrunkDialog::EditEtherTrunkDialog(QWidget *parent, DanetDbMan *db, int EtherId, QString Abbr, QString Saloon, QString Device, int Ether, QString Desc) :
    QDialog(parent),
    ui(new Ui::EditEtherTrunkDialog),
    dbMan(db),
    etherId(EtherId),
    ether(Ether),
    desc(Desc)

{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowMinMaxButtonsHint);

    ui->abbrLbl->setText(Abbr);
    ui->saloonLbl->setText(Saloon);
    ui->deviceLbl->setText(Device);

    ui->etherSB->setValue(ether);
    ui->descLE->setText(desc);

    ui->okBtn->setEnabled(false);
}

EditEtherTrunkDialog::~EditEtherTrunkDialog()
{
    delete ui;
}

void EditEtherTrunkDialog::on_cancelBtn_clicked()
{
    this->close();
}

void EditEtherTrunkDialog::on_okBtn_clicked()
{
    int E = ui->etherSB->value();
    QString D = ui->descLE->text();

    if(dbMan->updateEtherTrunk(etherId, E, D))
    {
        this->close();
    }
    else
        QMessageBox::warning(this, "Error", "Cannot modify Ether-Trunk.\n"+dbMan->lastError());
}

void EditEtherTrunkDialog::on_descLE_textChanged(const QString &arg1)
{
    if( ((ui->etherSB->value() != ether) || (arg1.trimmed().compare(desc,Qt::CaseSensitive) != 0) )&& !arg1.isEmpty() )
        ui->okBtn->setEnabled(true);
    else
        ui->okBtn->setEnabled(false);
}

void EditEtherTrunkDialog::on_etherSB_valueChanged(int arg1)
{
    if( ( (arg1 != ether) || (ui->descLE->text().trimmed().compare(desc,Qt::CaseSensitive) != 0) ) && !ui->descLE->text().isEmpty() )
        ui->okBtn->setEnabled(true);
    else
        ui->okBtn->setEnabled(false);
}
