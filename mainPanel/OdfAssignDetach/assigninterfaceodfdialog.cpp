#include "assigninterfaceodfdialog.h"
#include "ui_assigninterfaceodfdialog.h"
#include "lib/database/danetdbman.h"
#include <QSqlQuery>
#include <QMessageBox>

AssignInterfaceOdfDialog::AssignInterfaceOdfDialog(QWidget *parent, DanetDbMan *DbMan,  const int ExchId, const QString clickedLabel, const int IntId) :
    QDialog(parent),
    ui(new Ui::AssignInterfaceOdfDialog),
    dbMan(DbMan),
    exchId(ExchId),
    intId(IntId)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowMinMaxButtonsHint);

    ui->devLbl->setText(clickedLabel);

    ui->odfSaloonCB->blockSignals(true);
    ui->odfSaloonCB->clear();
    QList<QString> list = dbMan->getExistOdfSaloonNo(exchId);
    QSqlQuery *query = dbMan->selectSaloons(list);
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

    ui->odfCB->setEnabled(false);
    ui->posCB->setEnabled(false);
    ui->pinCB->setEnabled(false);
}

AssignInterfaceOdfDialog::~AssignInterfaceOdfDialog()
{
    delete ui;
}

void AssignInterfaceOdfDialog::checkEntry()
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

void AssignInterfaceOdfDialog::on_cancelBtn_clicked()
{
    this->close();
}

void AssignInterfaceOdfDialog::on_okBtn_clicked()
{
    int pinId = ui->pinCB->currentData().toInt();
    if(dbMan->assignInterfaceOdfPosition(intId, pinId))
    {
        //log
        QString odf = dbMan->getOdfPosPinString(pinId);
        QString dev = ui->devLbl->text();
        int exchId = dbMan->getInterfaceExchangeId(intId);
        QString username = dbMan->getLoggedinNameLastname();
        dbMan->insertLog(exchId, username, "Assign ODF "+ odf+" to "+dev);

        this->close();
    }
    else
    {
        QMessageBox::warning(this,"Error", "Cannot Assign the ODF position.");
    }

}

void AssignInterfaceOdfDialog::on_odfSaloonCB_currentIndexChanged(int index)
{
    index++;
    int s = ui->odfSaloonCB->currentData().toInt();

    odfMap = dbMan->getExchangeOdfMap(exchId, s);
    ui->okBtn->setEnabled(false);
    ui->odfCB->blockSignals(true);
    ui->odfCB->clear();
    ui->posCB->clear();
    ui->pinCB->clear();
    ui->posCB->setEnabled(false);
    ui->pinCB->setEnabled(false);
    ui->odfCB->setEnabled(true);
    foreach (int key, odfMap.keys())
        ui->odfCB->addItem(QString::number(key), odfMap.value(key));
    ui->odfCB->setCurrentIndex(-1);
    ui->odfCB->blockSignals(false);
}

void AssignInterfaceOdfDialog::on_odfCB_currentIndexChanged(int index)
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

void AssignInterfaceOdfDialog::on_posCB_currentIndexChanged(int index)
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
        ui->pinCB->addItem(pin, PinId);
    }
    ui->pinCB->setCurrentIndex(-1);
    ui->pinCB->blockSignals(false);
}

void AssignInterfaceOdfDialog::on_pinCB_currentIndexChanged(int index)
{
    index++;
    checkEntry();
}


