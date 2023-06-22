#include "portbyportodfrequestdialog.h"
#include "ui_portbyportodfrequestdialog.h"
#include <QSqlQuery>
#include "lib/database/danetdbman.h"

PortByPortOdfRequestDialog::PortByPortOdfRequestDialog(QWidget *parent, DanetDbMan *DbMan, const int ExchId, const QString Abbr, const int Saloon, const int DeviceId, const QString Device, const QString itf, const bool END_PORT, const QList<int> PinIdList) :
    QDialog(parent),
    ui(new Ui::PortByPortOdfRequestDialog),
    dbMan(DbMan),
    exchId(ExchId),
    deviceId(DeviceId),
    pinIdList(PinIdList)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowMinMaxButtonsHint);


    ui->exchLbl->setText(Abbr);

    ui->saloonLbl->setText(dbMan->getSaloonName(Saloon));
    ui->deviceLbl->setText(Device);
    ui->intLbl->setText(itf);

    ui->odfSaloonCB->blockSignals(true);
    ui->odfSaloonCB->clear();
    QSqlQuery *query = dbMan->selectSaloons(false,true);
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
    ui->odfSaloonCB->setCurrentIndex(ui->odfSaloonCB->findData(Saloon));

    if(END_PORT)
    {
        ui->okBtn->setText("Confirm");
        ui->okBtn->setIcon(QIcon(":/Image/Done.png"));
    }

    ui->posCB->setEnabled(false);
    ui->pinCB->setEnabled(false);
    ui->okBtn->setEnabled(false);
}

PortByPortOdfRequestDialog::~PortByPortOdfRequestDialog()
{
    delete ui;
}

void PortByPortOdfRequestDialog::checkEntry()
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

int PortByPortOdfRequestDialog::getPinId()
{
    return pinId;
}

void PortByPortOdfRequestDialog::on_odfSaloonCB_currentIndexChanged(int index)
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

void PortByPortOdfRequestDialog::on_odfCB_currentIndexChanged(int index)
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

void PortByPortOdfRequestDialog::on_posCB_currentIndexChanged(int index)
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

void PortByPortOdfRequestDialog::on_pinCB_currentIndexChanged(int index)
{
    index++;
    checkEntry();
}

void PortByPortOdfRequestDialog::on_cancelBtn_clicked()
{
    pinId = -1;
    this->close();
}

void PortByPortOdfRequestDialog::on_okBtn_clicked()
{

    if(ui->pinCB->currentText().isEmpty())
        pinId = -1;
    else
        pinId = ui->pinCB->currentData().toInt();

    this->close();
}
