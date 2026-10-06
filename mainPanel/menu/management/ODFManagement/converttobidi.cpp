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
    ui->abbrLbl->setText(exch);
    ui->saloonLbl->setText(saloon);
    QString currentDuplex = odf+" _ "+pos+" _ "+pinNo;
    ui->odfLbl->setText(currentDuplex);

    QList<QString> list = dbMan->splitPIN(pinNo);
    if(list.size() == 2)
    {
        QString odf1 = odf+" _ "+pos+" _ "+list[0];
        QString odf2 = odf+" _ "+pos+" _ "+list[1];

        ui->odfLbl_1->setText(odf1);
        ui->odfLbl_2->setText(odf2);
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
    if(dbMan->convertToBiDi(pinId))
    {
        this->close();
    }
    else
    {
        QMessageBox::warning(this,"ERROR", "Cannot Convert to BiDi PINs.\n");
    }
}

