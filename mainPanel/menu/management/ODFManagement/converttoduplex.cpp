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

    ui->confirmChB->setChecked(false);
    ui->okBtn->setEnabled(false);
    ui->abbrLbl->setText(exch);
    ui->saloonLbl->setText(saloon);
    ui->odfLbl->setText(odf);
    ui->posLbl->setText(pos);
    ui->pin1Lbl->setText(pinNo);

    peerPinId = dbMan->findPeerBiDi_PinId(pinId);
    QMap<QString, QString> peer = dbMan->odfPosPin(peerPinId);
    ui->pin2Lbl->setText(peer["pin"]);

    QString duplexPins = dbMan->createDuplexPinName(pinNo, peer["pin"]);
    ui->duplexPin->setText(duplexPins);

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


    // check one pin is empty or not
    bool empty1 = dbMan->isPinEmpty(pinId);
    bool empty2 = dbMan->isPinEmpty(peerPinId);

    QString duplexPin = ui->duplexPin->text().trimmed();
    if(duplexPin.isEmpty())
    {
        QMessageBox::warning(this,"ERROR", "Duplex PIN name cannot be empty.");
        return;
    }
    else if(!empty1 && !empty2)
    {
        QMessageBox::warning(this,"ERROR", "One of the PINs should be empty.");
        return;
    }
    else if (pinId == peerPinId)
    {
        QMessageBox::warning(this,"ERROR", "Two BiDi PINs cannot be detected.");
        return;
    }
    else
    {
        if(dbMan->convertToDuplex(pinId, peerPinId, duplexPin))
        {
            this->close();
        }
        else
        {
            QMessageBox::warning(this,"ERROR", "Cannot Convert to Duplex PINs.\n");
        }
    }

}


void ConvertToDuplex::on_cancelBtn_clicked()
{
    this->close();
}

