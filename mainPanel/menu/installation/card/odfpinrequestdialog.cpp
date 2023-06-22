#include "odfpinrequestdialog.h"
#include "ui_odfpinrequestdialog.h"
#include <QSqlQuery>
#include "lib/database/danetdbman.h"


OdfPinRequestDialog::OdfPinRequestDialog(QWidget *parent, DanetDbMan *Db, const int ExchId, const QList<int> PinIdList, const QString exch, const int saloon) :
    QDialog(parent),
    ui(new Ui::OdfPinRequestDialog),
    dbMan(Db),
    exchId(ExchId),
    pinIdList(PinIdList)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowMinMaxButtonsHint);

    pinId = -1;

    ui->exchLbl->setText(exch);

    ui->odfSaloonCB->blockSignals(true);
    ui->odfSaloonCB->clear();
    QSqlQuery *query = dbMan->selectSaloons();
    int saloonId;
    QString saloonStr;
    while(query->next())
    {
        saloonId = query->value(0).toInt();
        saloonStr = query->value(1).toString();
        ui->odfSaloonCB->addItem(saloonStr, saloonId);
    }
    ui->odfSaloonCB->setCurrentIndex(-1);
    ui->odfSaloonCB->blockSignals(false);
    ui->odfSaloonCB->setCurrentIndex(ui->odfSaloonCB->findData(saloon));
}

OdfPinRequestDialog::~OdfPinRequestDialog()
{
    delete ui;
}

void OdfPinRequestDialog::checkEntry()
{
    bool s = !ui->odfSaloonCB->currentText().isEmpty();
    bool o = !ui->odfCB->currentText().isEmpty();
    bool p = !ui->posCB->currentText().isEmpty();
    bool pi = !ui->pinCB->currentText().isEmpty();

    if(s && o && p && pi)
        ui->okBtn->setEnabled(true);
    else
        ui->okBtn->setEnabled(false);
}

int OdfPinRequestDialog::getOdfNo()
{
    return odfNo;
}

int OdfPinRequestDialog::getPpp()
{
    return ppp;
}

int OdfPinRequestDialog::getPosNo()
{
    return posNo;
}

int OdfPinRequestDialog::getPinId()
{
    return pinId;
}

void OdfPinRequestDialog::on_cancelBtn_clicked()
{
    pinId = -1;
    this->close();
}

void OdfPinRequestDialog::on_okBtn_clicked()
{
    if(ui->odfCB->currentText().isEmpty())
    {
        odfNo = -1;
        ppp = -1;
    }
    else
    {
        odfNo = ui->odfCB->currentText().toInt();
        int odfId = ui->odfCB->currentData().toInt();
        ppp = dbMan->getOdfPinPerPos(odfId);
    }

    if(ui->posCB->currentText().isEmpty())
        posNo = -1;
    else
        posNo = ui->posCB->currentText().toInt();

    if(ui->pinCB->currentText().isEmpty())
        pinId = -1;
    else
        pinId = ui->pinCB->currentData().toInt();

    this->close();
}

void OdfPinRequestDialog::on_odfSaloonCB_currentIndexChanged(int index)
{
    index++;
    int s = ui->odfSaloonCB->currentData().toInt();

    odfMap = dbMan->getExchangeOdfMap(exchId, s);
    ui->okBtn->setEnabled(false);
    ui->odfCB->blockSignals(true);
    ui->odfCB->clear();
    ui->posCB->clear();
    ui->pinCB->clear();
    foreach (int key, odfMap.keys())
        ui->odfCB->addItem(QString::number(key), odfMap.value(key));
    ui->odfCB->setCurrentIndex(-1);
    ui->odfCB->blockSignals(false);
}

void OdfPinRequestDialog::on_odfCB_currentIndexChanged(int index)
{
    index++;
    ui->posCB->setEnabled(true);
    ui->pinCB->setEnabled(false);
    ui->okBtn->setEnabled(false);
    int odfId = ui->odfCB->currentData().toInt();
    posMap.clear();
    posMap = dbMan->getOdfPosMap(odfId);
    ui->posCB->blockSignals(true);
    ui->posCB->clear();
    ui->pinCB->clear();
    foreach (int key, posMap.keys())
        ui->posCB->addItem(QString::number(key), posMap.value(key));
    ui->posCB->setCurrentIndex(-1);
    ui->posCB->blockSignals(false);
}

void OdfPinRequestDialog::on_posCB_currentIndexChanged(int index)
{
    index++;
    int posId = ui->posCB->currentData().toInt();
    ui->pinCB->clear();
    ui->pinCB->setEnabled(true);
    ui->okBtn->setEnabled(false);
    QSqlQuery *query = dbMan->selectEmptyPin(posId);
    //`id`,`pin_no`
    QString pin;
    int PinId;
    ui->pinCB->blockSignals(true);
    while(query->next())
    {
        PinId = query->value(0).toInt();
        pin = query->value(1).toString();
        if(!pinIdList.contains(PinId))
            ui->pinCB->addItem(pin, PinId);
    }
    ui->pinCB->setCurrentIndex(-1);
    ui->pinCB->blockSignals(false);
}

void OdfPinRequestDialog::on_pinCB_currentIndexChanged(int index)
{
    index++;
    checkEntry();
}
