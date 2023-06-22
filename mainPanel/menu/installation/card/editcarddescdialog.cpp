#include "editcarddescdialog.h"
#include "ui_editcarddescdialog.h"
#include "lib/database/danetdbman.h"
#include <QMessageBox>

EditCardDescDialog::EditCardDescDialog(QWidget *parent, DanetDbMan *db, const QString abbr, const int DevexId, const QString device, const int SlotsId, const QString card, const int shelf, const int slot, const int subSlot, const QString desc) :
    QDialog(parent),
    ui(new Ui::EditCardDescDialog),
    dbMan(db),
    slotsId(SlotsId)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowMinMaxButtonsHint);


    ui->abbrLbl->setText(abbr);

    int saloon = dbMan->getDevexSaloon(DevexId);
     ui->saloonLbl->setText(dbMan->getSaloonName(saloon));


    ui->deviceLbl->setText(device);
    ui->cardLbl->setText(card);

    QString position = "";

    if(shelf > -1)
        position = QString::number(shelf);

    if(slot > -1)
    {
        if(position.isEmpty())
            position = QString::number(slot);
        else
            position += "/"+ QString::number(slot);
    }

    if(subSlot > -1)
        position += "/"+QString::number(subSlot);

    ui->positionLbl->setText(position);

    ui->descTE->setText(desc);
    ui->okBtn->setEnabled(false);
}

EditCardDescDialog::~EditCardDescDialog()
{
    delete ui;
}

void EditCardDescDialog::on_cancelBtn_clicked()
{
    this->close();
}

void EditCardDescDialog::on_okBtn_clicked()
{
    QString desc = ui->descTE->toPlainText();

    if(dbMan->updateSlotDesc(slotsId,desc))
        this->close();
    else
        QMessageBox::warning(this, "ERROR", "Cannot update description.\n"+dbMan->lastError());
}

void EditCardDescDialog::on_descTE_textChanged()
{
    ui->okBtn->setEnabled(true);
}
