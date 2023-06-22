#include "editbrasaggdialog.h"
#include "ui_editbrasaggdialog.h"
#include "lib/database/danetdbman.h"
#include <QSqlQuery>
#include <QMessageBox>

EditBrasAggDialog::EditBrasAggDialog(QWidget *parent, DanetDbMan *db, const int Id,const int BrasId, const int Area, const QString BrasName, const int ExchId, const int Agg1Id, const int Eth1, const int Agg2Id, const int Eth2) :
    QDialog(parent),
    ui(new Ui::EditBrasAggDialog),
    dbMan(db),
    id(Id),
    brasId(BrasId),
    currentAgg1Eth(Eth1),
    currentAgg2Eth(Eth2)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowMinMaxButtonsHint);

    ui->brasLbl->setText(BrasName);
    ui->exchCB->blockSignals(true);
    QSqlQuery *query = dbMan->selectExchAndSites(Area);
    //`id`,`abbr`
    int ID;
    QString ABBR;
    while(query->next())
    {
        ID = query->value(0).toInt();
        ABBR = query->value(1).toString();
        ui->exchCB->addItem(ABBR, ID);
    }
    ui->exchCB->setCurrentIndex(ui->exchCB->findData(ExchId));
    ui->exchCB->blockSignals(false);

    ui->agg1CB->blockSignals(true);
    ui->agg2CB->blockSignals(true);
    query = dbMan->selectExchangeAgg(ExchId, true);
    //`id`,`device_name`
    QString AGG;
    ui->agg1CB->clear();
    ui->agg2CB->clear();
    while(query->next())
    {
        ID = query->value(0).toInt();
        AGG = query->value(1).toString();
        ui->agg1CB->addItem(AGG,ID);
    }

    query = dbMan->selectExchangeAgg(ExchId, false);
    //`id`,`shelf_name`
    while(query->next())
    {
        ID = query->value(0).toInt();
        AGG = query->value(1).toString();
        ui->agg2CB->addItem(AGG,ID);
    }

    ui->agg1CB->setCurrentIndex(ui->agg1CB->findData(Agg1Id));
    ui->agg2CB->setCurrentIndex(ui->agg2CB->findData(Agg2Id));

    ui->agg1CB->blockSignals(false);
    ui->agg2CB->blockSignals(false);

    ui->eth1SB->blockSignals(true);
    ui->eth2SB->blockSignals(true);
    ui->eth1SB->setValue(Eth1);
    ui->eth2SB->setValue(Eth2);
    ui->eth1SB->blockSignals(false);
    ui->eth2SB->blockSignals(false);

    ui->okBtn->setEnabled(false);

}

EditBrasAggDialog::~EditBrasAggDialog()
{
    delete ui;
}

void EditBrasAggDialog::checkFormEntry()
{
    bool e = !ui->exchCB->currentText().isEmpty();
    bool ag1 = !ui->agg1CB->currentText().isEmpty();
    bool ag2 = !ui->agg2CB->currentText().isEmpty();

    if(e && ag1 && ag2)
        ui->okBtn->setEnabled(true);
    else
        ui->okBtn->setEnabled(false);
}

void EditBrasAggDialog::on_okBtn_clicked()
{
    int aggExchId = ui->exchCB->currentData().toInt();
    int agg1Id = ui->agg1CB->currentData().toInt();
    int agg2Id = ui->agg2CB->currentData().toInt();
    int eth1 = ui->eth1SB->value();
    int eth2 = ui->eth2SB->value();
    QString agg1 = ui->agg1CB->currentText();
    QString agg2 = ui->agg2CB->currentText();

    if(dbMan->updateBrasConnection(id,aggExchId,agg1Id,eth1,agg2Id, eth2))
    {
        if(currentAgg1Eth != eth1)
        {
            dbMan->deleteEtherTrunk(brasId, currentAgg1Eth);
            dbMan->insertEtherTrunk(brasId,eth1,agg1);
        }
        if(currentAgg2Eth != eth2)
        {
            dbMan->deleteEtherTrunk(brasId, currentAgg2Eth);
            dbMan->insertEtherTrunk(brasId,eth2,agg2);
        }

        this->close();
    }
    else
        QMessageBox::warning(this,"ERROR", "Cannot update BRAS connection.\n"+dbMan->lastError());
}

void EditBrasAggDialog::on_cancelBtn_clicked()
{
    this->close();
}

void EditBrasAggDialog::on_exchCB_currentIndexChanged(int index)
{
    index++;
    int exchId = ui->exchCB->currentData().toInt();
    QSqlQuery *query = dbMan->selectExchangeAgg(exchId, true);
    //`id`,`shelf_name`
    int ID;
    QString AGG;
    ui->agg1CB->clear();
    ui->agg2CB->clear();
    while(query->next())
    {
        ID = query->value(0).toInt();
        AGG = query->value(1).toString();
        ui->agg1CB->addItem(AGG,ID);
    }

    query = dbMan->selectExchangeAgg(exchId, false);
        //`id`,`shelf_name`
        while(query->next())
        {
            ID = query->value(0).toInt();
            AGG = query->value(1).toString();
            ui->agg2CB->addItem(AGG,ID);
        }

    checkFormEntry();
}

void EditBrasAggDialog::on_agg1CB_currentIndexChanged(int index)
{
    index++;
    checkFormEntry();
}

void EditBrasAggDialog::on_eth1SB_valueChanged(int arg1)
{
    arg1++;
    checkFormEntry();
}

void EditBrasAggDialog::on_agg2CB_currentIndexChanged(int index)
{
    index++;
    checkFormEntry();
}

void EditBrasAggDialog::on_eth2SB_valueChanged(int arg1)
{
    arg1++;
    checkFormEntry();
}
