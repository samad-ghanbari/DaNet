#include "converttoduplex.h"
#include "ui_converttoduplex.h"
#include "lib/database/danetdbman.h"
#include <QMessageBox>

ConvertToDuplex::ConvertToDuplex(QWidget *parent, DanetDbMan *db, const int PinId, const QString exch, const QString saloon, const QString odf, const QString pos, const QString pinNo)
    : QDialog(parent)
    , ui(new Ui::ConvertToDuplex),
    dbMan(db),
    pinId(PinId)
{
    ui->setupUi(this);

    ui->setupUi(this);
    ui->confirmChB->setChecked(false);
    ui->okBtn->setEnabled(false);
    ui->abbrLbl->setText(exch);
    ui->saloonLbl->setText(saloon);
    QString currentBiDi = odf+" _ "+pos+" _ "+pinNo;
    ui->odfLbl->setText(currentBiDi);

    // find peer odf
}

ConvertToDuplex::~ConvertToDuplex()
{
    delete ui;
}

void ConvertToDuplex::on_confirmChB_toggled(bool checked)
{
    if(checked)
        ui->okBtn->setEnabled(true);
    else
        ui->okBtn->setEnabled(false);
}


void ConvertToDuplex::on_okBtn_clicked()
{
    if(dbMan->convertToDuplex(pinId))
    {
        this->close();
    }
    else
    {
        QMessageBox::warning(this,"ERROR", "Cannot Convert to Duplex PINs.\n");
    }
}

