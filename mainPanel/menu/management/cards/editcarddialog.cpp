#include "editcarddialog.h"
#include "ui_editcarddialog.h"
#include "lib/database/danetdbman.h"
#include <QSqlQuery>
#include <QCryptographicHash>
#include <QMessageBox>

EditCardDialog::EditCardDialog(QWidget *parent, DanetDbMan *db, int Id, QString card, QString device, QString type, int adsl, int vdsl, int shdsl, int e, int G, int G10, int G40, int G100) :
    QDialog(parent),
    ui(new Ui::EditCardDialog),
    dbMan(db),
    cardId(Id)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowMinMaxButtonsHint);

    QSqlQuery *query = dbMan->selectDevices(true);
    //`id`, `device`, `sr`, `type`

    QString dev,dvType0, dvType1;
    int id;
    while(query->next())
    {
        id = query->value(0).toInt();
        dev = query->value(3).toString()+" "+query->value(1).toString();;
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

    ui->cardLE->setText(card);
    ui->deviceCB->setCurrentText(device);
    ui->typeLE->setText(type);
    ui->adslSB->setValue(adsl);
    ui->vdslSB->setValue(vdsl);
    ui->shdslSB->setValue(shdsl);
    ui->eSB->setValue(e);
    ui->G1SB->setValue(G);
    ui->G10SB->setValue(G10);
    ui->G40SB->setValue(G40);
    ui->G100SB->setValue(G100);

    ui->modifyBtn->setEnabled(false);

}

EditCardDialog::~EditCardDialog()
{
    delete ui;
}

bool EditCardDialog::lineEditsNotEmpty()
{
    if(ui->cardLE->text().isEmpty() || ui->typeLE->text().isEmpty())
        return false;
    else
        return true;
}

void EditCardDialog::on_cancelBtn_clicked()
{
    this->close();
}

void EditCardDialog::on_cardLE_textChanged()
{
    if(lineEditsNotEmpty())
        ui->modifyBtn->setEnabled(true);
    else
        ui->modifyBtn->setEnabled(false);
}

void EditCardDialog::on_typeLE_textChanged()
{
    if(lineEditsNotEmpty())
        ui->modifyBtn->setEnabled(true);
    else
        ui->modifyBtn->setEnabled(false);
}

void EditCardDialog::on_modifyBtn_clicked()
{
    int deviceId,adsl, vdsl, shdsl, e, G, G10, G40, G100;
    QString card, type,device;

    device = ui->deviceCB->currentText();
    deviceId = ui->deviceCB->currentData().toInt();
    card = ui->cardLE->text();
    type = ui->typeLE->text();
    adsl = ui->adslSB->value();
    vdsl = ui->vdslSB->value();
    shdsl = ui->shdslSB->value();
    e = ui->eSB->value();
    G = ui->G1SB->value();
    G10 = ui->G10SB->value();
    G40 = ui->G40SB->value();
    G100 = ui->G100SB->value();


    if(dbMan->updateCard(cardId, card,deviceId, device, type,adsl, vdsl,shdsl, e, G, G10, G40, G100))
    {
        this->close();
    }
    else
        QMessageBox::information(this, "Error", "Can not edit card.\n"+dbMan->lastError());
}

void EditCardDialog::on_deviceCB_currentIndexChanged(int index)
{
    if(lineEditsNotEmpty() && index >= 0)
        ui->modifyBtn->setEnabled(true);
    else
        ui->modifyBtn->setEnabled(false);
}

void EditCardDialog::on_adslSB_valueChanged(int arg1)
{
    arg1++;
    if(lineEditsNotEmpty())
        ui->modifyBtn->setEnabled(true);
    else
        ui->modifyBtn->setEnabled(false);
}

void EditCardDialog::on_vdslSB_valueChanged(int arg1)
{
    arg1++;
    if(lineEditsNotEmpty())
        ui->modifyBtn->setEnabled(true);
    else
        ui->modifyBtn->setEnabled(false);
}

void EditCardDialog::on_shdslSB_valueChanged(int arg1)
{
    arg1++;
    if(lineEditsNotEmpty())
        ui->modifyBtn->setEnabled(true);
    else
        ui->modifyBtn->setEnabled(false);
}

void EditCardDialog::on_eSB_valueChanged(int arg1)
{
    arg1++;
    if(lineEditsNotEmpty())
        ui->modifyBtn->setEnabled(true);
    else
        ui->modifyBtn->setEnabled(false);
}

void EditCardDialog::on_G1SB_valueChanged(int arg1)
{
    arg1++;
    if(lineEditsNotEmpty())
        ui->modifyBtn->setEnabled(true);
    else
        ui->modifyBtn->setEnabled(false);
}

void EditCardDialog::on_G10SB_valueChanged(int arg1)
{
    arg1++;
    if(lineEditsNotEmpty())
        ui->modifyBtn->setEnabled(true);
    else
        ui->modifyBtn->setEnabled(false);
}

void EditCardDialog::on_G40SB_valueChanged(int arg1)
{
    arg1++;
    if(lineEditsNotEmpty())
        ui->modifyBtn->setEnabled(true);
    else
        ui->modifyBtn->setEnabled(false);
}

void EditCardDialog::on_G100SB_valueChanged(int arg1)
{
    arg1++;
    if(lineEditsNotEmpty())
        ui->modifyBtn->setEnabled(true);
    else
        ui->modifyBtn->setEnabled(false);
}
