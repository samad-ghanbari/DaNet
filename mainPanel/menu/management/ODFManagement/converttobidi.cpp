#include "converttobidi.h"
#include "ui_converttobidi.h"
#include "lib/database/danetdbman.h"
#include <QMessageBox>

ConvertToBiDi::ConvertToBiDi(QWidget *parent, DanetDbMan *db, const int PinId, const QString exch, const QString saloon, const QString odf, const QString pos, const QString pinNo)
    : QDialog(parent)
    , ui(new Ui::ConvertToBiDi),
    dbMan(db),
    pinId(PinId)
{
    ui->setupUi(this);
    ui->confirmChB->setChecked(false);
    ui->okBtn->setEnabled(false);
    ui->abbrLbl->setText(exch);
    ui->saloonLbl->setText(saloon);
    ui->odfLbl->setText(odf);
    ui->posLbl->setText(pos);
    ui->pinLbl->setText(pinNo);

    QList<QString> list = dbMan->splitPIN(pinNo);
    if(list.size() == 2)
    {
        ui->pin1LE->setText(list[0]);
        ui->pin2LE->setText(list[1]);
    }


}

ConvertToBiDi::~ConvertToBiDi()
{
    delete ui;
}

void ConvertToBiDi::on_cancelBtn_clicked()
{
    this->close();
}


void ConvertToBiDi::on_confirmChB_toggled(bool checked)
{
    if(checked)
        ui->okBtn->setEnabled(true);
    else
        ui->okBtn->setEnabled(false);
}


void ConvertToBiDi::on_okBtn_clicked()
{
    QString pin1LE = ui->pin1LE->text().trimmed();
    QString pin2LE = ui->pin2LE->text().trimmed();

    if(pin1LE.isEmpty() || pin2LE.isEmpty())
    {
        QMessageBox::warning(this,"ERROR", "New PINs cannot be empty");
    }
    else
    {
        if(dbMan->convertToBiDi(pinId, pin1LE, pin2LE))
        {
            this->close();
        }
        else
        {
            QMessageBox::warning(this,"ERROR", "Cannot Convert to BiDi PINs.\n");
        }
    }
}

