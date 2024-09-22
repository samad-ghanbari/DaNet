#include "addbrasaggdialog.h"
#include "ui_addbrasaggdialog.h"
#include "lib/database/danetdbman.h"
#include <QSqlQuery>
#include <QMessageBox>

AddBrasAggDialog::AddBrasAggDialog(QWidget *parent, DanetDbMan *db, const int area, const int BrasDevexId, const QString Bras) :
    QDialog(parent),
    ui(new Ui::AddBrasAggDialog),
    dbMan(db),
    brasDevexId(BrasDevexId)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowMinMaxButtonsHint);

    ui->brasLbl->setText(Bras);
    ui->exchCB->blockSignals(true);
    QSqlQuery *query = dbMan->selectExchanges(area);
    //`id`,`abbr`
    int id;
    QString abbr;
    while(query->next())
    {
        id = query->value(0).toInt();
        abbr = query->value(1).toString();
        ui->exchCB->addItem(abbr, id);
    }
    ui->exchCB->setCurrentIndex(-1);
    ui->exchCB->blockSignals(false);

}

AddBrasAggDialog::~AddBrasAggDialog()
{
    delete ui;
}

void AddBrasAggDialog::checkFormEntry()
{
    bool e = !ui->exchCB->currentText().isEmpty();
    bool ag1 = !ui->agg1CB->currentText().isEmpty();
    bool ag2 = !ui->agg2CB->currentText().isEmpty();

    if(e && ag1 && ag2)
        ui->okBtn->setEnabled(true);
    else
        ui->okBtn->setEnabled(false);
}

void AddBrasAggDialog::on_exchCB_currentIndexChanged(int index)
{
    index++;
    int exchId = ui->exchCB->currentData().toInt();
    QSqlQuery *query = dbMan->selectExchangeAgg(exchId, true);
    //`id`,`device_name`
    int id;
    QString agg;
    ui->agg1CB->clear();
    ui->agg2CB->clear();
    while(query->next())
    {
        id = query->value(0).toInt();
        agg = query->value(1).toString();
        ui->agg1CB->addItem(agg,id);
    }

    query = dbMan->selectExchangeAgg(exchId, false);
    //`id`,`device_name`
    while(query->next())
    {
        id = query->value(0).toInt();
        agg = query->value(1).toString();
        ui->agg2CB->addItem(agg,id);
    }

    checkFormEntry();
}

void AddBrasAggDialog::on_agg1CB_currentIndexChanged(int index)
{
    index++;
    checkFormEntry();
}

void AddBrasAggDialog::on_agg2CB_currentIndexChanged(int index)
{
    index++;
    checkFormEntry();
}

void AddBrasAggDialog::on_agg1SB_valueChanged(int arg1)
{
    arg1++;
    checkFormEntry();
}

void AddBrasAggDialog::on_Agg2SB_valueChanged(int arg1)
{
    arg1++;
    checkFormEntry();
}

void AddBrasAggDialog::on_cancelBtn_clicked()
{
    this->close();
}

void AddBrasAggDialog::on_okBtn_clicked()
{
    int brasExchId = dbMan->getDevexExchangeId(brasDevexId);
    if(brasExchId == -1)
    {
        QMessageBox::warning(this,"ERROR", "Shelf connection error.\n"+dbMan->lastError());
        return;
    }
    else
    {
        int aggExchId = ui->exchCB->currentData().toInt();
        int agg1Id = ui->agg1CB->currentData().toInt();
        int eth1 = ui->agg1SB->value();
        int agg2Id = ui->agg2CB->currentData().toInt();
        int eth2 = ui->Agg2SB->value();

        if(eth1 == eth2)
        {
            QMessageBox::warning(this,"ERROR", "Check eth-trunks values. They cannot be the same.");
            return ;
        }
        else
        {
            if(dbMan->insertBrasConnection(brasExchId,brasDevexId,aggExchId,agg1Id,eth1,agg2Id, eth2))
                this->close();
            else
                QMessageBox::warning(this,"ERROR", "Cannot insert BRAS connection.\n"+dbMan->lastError());
        }

    }


}
