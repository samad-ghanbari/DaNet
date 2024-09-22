#include "addcarddialog.h"
#include "ui_addcarddialog.h"
#include "lib/database/danetdbman.h"
#include <QSqlQuery>
#include <QCryptographicHash>
#include <QMessageBox>
#include <QRegExpValidator>

AddCardDialog::AddCardDialog(QWidget *parent, DanetDbMan *db) :
    QDialog(parent),
    ui(new Ui::AddCardDialog),
    dbMan(db)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowMinMaxButtonsHint);

    QSqlQuery *query = dbMan->selectDevices(true);
    //`id`, `device`, `sr`, `type`
    int id;
    ui->deviceCB->blockSignals(true);
    QString dev,dvType0, dvType1;
    while(query->next())
    {
        id = query->value(0).toInt();
        dev = query->value(3).toString()+" "+query->value(1).toString();
        dvType1 = query->value(3).toString();
        if(dvType1.contains("AGG", Qt::CaseInsensitive))
            dvType1 = "AGG";
        if(dvType1.contains("BRAS", Qt::CaseInsensitive))
            dvType1 = "BRAS";
        if(dvType0.isEmpty())
            dvType0 = dvType1;
        if((dvType0 != dvType1))
        {
            ui->deviceCB->insertSeparator(ui->deviceCB->count());
            dvType0 = dvType1;
        }


        ui->deviceCB->addItem(dev,id);

    }
    ui->deviceCB->setCurrentIndex(-1);
    ui->deviceCB->blockSignals(false);

}

AddCardDialog::~AddCardDialog()
{
    delete ui;
}

bool AddCardDialog::inputsNotEmpty()
{
    if(ui->cardLE->text().isEmpty() || ui->typeLE->text().isEmpty())
        return false;
    else
        return true;
}

void AddCardDialog::on_cancelBtn_clicked()
{
    this->close();
}

void AddCardDialog::on_cardLE_textChanged(const QString &arg1)
{
    if(arg1.startsWith("_"))
    {
        ui->eSB->setValue(0);
        ui->G1SB->setValue(0);
        ui->G100SB->setValue(0);
        ui->G10SB->setValue(0);
        ui->G40SB->setValue(0);

        ui->adslSB->setValue(0);
        ui->vdslSB->setValue(0);
        ui->shdslSB->setValue(0);

        ui->GB1->setEnabled(false);
        ui->GB2->setEnabled(false);
    }
    else
    {
        ui->GB1->setEnabled(true);
        ui->GB2->setEnabled(true);
    }

    if(inputsNotEmpty())
        ui->okBtn->setEnabled(true);
    else
        ui->okBtn->setEnabled(false);
}

void AddCardDialog::on_deviceCB_currentIndexChanged(int index)
{
    index++;
    if(inputsNotEmpty())
        ui->okBtn->setEnabled(true);
    else
        ui->okBtn->setEnabled(false);
}

void AddCardDialog::on_typeLE_textChanged()
{
    if(inputsNotEmpty())
        ui->okBtn->setEnabled(true);
    else
        ui->okBtn->setEnabled(false);
}

void AddCardDialog::on_okBtn_clicked()
{
    int deviceId,adsl,vdsl,shdsl, e, G, G10, G40, G100;
    QString device;
    QString card, type;
    card = ui->cardLE->text();
    type = ui->typeLE->text();
    device = ui->deviceCB->currentText();
    deviceId = ui->deviceCB->currentData().toInt();
    adsl = ui->adslSB->value();
    vdsl = ui->vdslSB->value();
    shdsl = ui->shdslSB->value();
    e = ui->eSB->value();
    G = ui->G1SB->value();
    G10 = ui->G10SB->value();
    G40 = ui->G40SB->value();
    G100 = ui->G100SB->value();

    if(dbMan->insertCard(card,deviceId, device, type, adsl,vdsl,shdsl,e,G, G10, G40, G100))
    {
        this->close();
    }
    else
         QMessageBox::information(this, "Error", "Can not add new card.\n"+dbMan->lastError());
}

void AddCardDialog::on_adslSB_valueChanged(int arg1)
{
    arg1++;
    if(inputsNotEmpty())
        ui->okBtn->setEnabled(true);
    else
        ui->okBtn->setEnabled(false);
}

void AddCardDialog::on_vdslSB_valueChanged(int arg1)
{
    arg1++;
    if(inputsNotEmpty())
        ui->okBtn->setEnabled(true);
    else
        ui->okBtn->setEnabled(false);
}

void AddCardDialog::on_shdslSB_valueChanged(int arg1)
{
    arg1++;
    if(inputsNotEmpty())
        ui->okBtn->setEnabled(true);
    else
        ui->okBtn->setEnabled(false);
}

void AddCardDialog::on_eSB_valueChanged(int arg1)
{
    arg1++;
    if(inputsNotEmpty())
        ui->okBtn->setEnabled(true);
    else
        ui->okBtn->setEnabled(false);
}

void AddCardDialog::on_G1SB_valueChanged(int arg1)
{
    arg1++;
    if(inputsNotEmpty())
        ui->okBtn->setEnabled(true);
    else
        ui->okBtn->setEnabled(false);
}

void AddCardDialog::on_G10SB_valueChanged(int arg1)
{
    arg1++;
    if(inputsNotEmpty())
        ui->okBtn->setEnabled(true);
    else
        ui->okBtn->setEnabled(false);
}

void AddCardDialog::on_G40SB_valueChanged(int arg1)
{
    arg1++;
    if(inputsNotEmpty())
        ui->okBtn->setEnabled(true);
    else
        ui->okBtn->setEnabled(false);
}

void AddCardDialog::on_G100SB_valueChanged(int arg1)
{
    arg1++;
    if(inputsNotEmpty())
        ui->okBtn->setEnabled(true);
    else
        ui->okBtn->setEnabled(false);
}


