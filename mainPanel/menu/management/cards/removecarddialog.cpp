#include "removecarddialog.h"
#include "ui_removecarddialog.h"
#include <QCryptographicHash>
#include "lib/database/danetdbman.h"
#include <QMessageBox>


RemoveCardDialog::RemoveCardDialog(QWidget *parent, DanetDbMan *db, int Id, QString card, QString device, QString type) :
    QDialog(parent),
    ui(new Ui::RemoveCardDialog),
    dbMan(db),
    id(Id)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowMinMaxButtonsHint);

    ui->cardLbl->setText(card);
    ui->deviceLbl->setText(device);
    ui->typeLbl->setText(type);
}

RemoveCardDialog::~RemoveCardDialog()
{
    delete ui;
}

void RemoveCardDialog::on_cancelBtn_clicked()
{
    this->close();
}


void RemoveCardDialog::on_removeBtn_clicked()
{
    if(dbMan->deleteCard(id))
    {
        this->close();
    }
    else
        QMessageBox::information(this, "Error", "Can not remove card.\n"+dbMan->lastError());
}

void RemoveCardDialog::on_checkBox_toggled(bool checked)
{
    if(checked)
        ui->removeBtn->setEnabled(true);
    else
        ui->removeBtn->setEnabled(false);
}
