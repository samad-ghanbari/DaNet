#include "editaggcxdialog.h"
#include "ui_editaggcxdialog.h"
#include "lib/database/danetdbman.h"
#include <QSqlQuery>
#include <QMessageBox>

EditAggCxDialog::EditAggCxDialog(QWidget *parent, DanetDbMan *Db, const int Id, const int area, const int exchId, const int aggId, const int aggEth, const int cxId, const int cxEth) :
    QDialog(parent),
    ui(new Ui::EditAggCxDialog),
    dbMan(Db),
    id(Id),
    currentAggEth(aggEth),
    currentCxEth(cxEth)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowMinMaxButtonsHint);

    ui->abbrCB->blockSignals(true);
    QSqlQuery *query = dbMan->selectExchAndSites(area);
    //`id`,`abbr`
    int ID;
    QString abbr;
    while(query->next())
    {
        ID = query->value(0).toInt();
        abbr = query->value(1).toString();
        ui->abbrCB->addItem(abbr,ID);
    }
    ui->abbrCB->setCurrentIndex(ui->abbrCB->findData(exchId));
    ui->abbrCB->blockSignals(false);
    //...........

    ui->agg1CB->clear();
    ui->cxCB->clear();
    ui->agg1CB->blockSignals(true);
    ui->cxCB->blockSignals(true);

    query = dbMan->selectExchangeAgg(exchId,true);
    //`id`,`device_name`

    QString name;
    while(query->next())
    {
        ID = query->value(0).toInt();
        name = query->value(1).toString();
        ui->agg1CB->addItem(name,ID);
    }

    query = dbMan->selectExchangeCx(exchId);
    //`id`,`device_name`
    while(query->next())
    {
        ID = query->value(0).toInt();
        name = query->value(1).toString();
        ui->cxCB->addItem(name,ID);
    }

    ui->agg1CB->setCurrentIndex(ui->agg1CB->findData(aggId));
    ui->cxCB->setCurrentIndex(ui->cxCB->findData(cxId));

    ui->agg1CB->blockSignals(false);
    ui->cxCB->blockSignals(false);

    ui->aggEthSB->blockSignals(true);
    ui->cxEthSB->blockSignals(true);
    ui->aggEthSB->setValue(aggEth);
    ui->cxEthSB->setValue(cxEth);
    ui->aggEthSB->blockSignals(false);
    ui->cxEthSB->blockSignals(false);



    ui->okBtn->setEnabled(false);
}

EditAggCxDialog::~EditAggCxDialog()
{
    delete ui;
}

void EditAggCxDialog::on_CancelBtn_clicked()
{
    this->close();
}

void EditAggCxDialog::on_okBtn_clicked()
{
    int exchId = ui->abbrCB->currentData().toInt();
    int aggId = ui->agg1CB->currentData().toInt();
    int aggEth = ui->aggEthSB->value();
    int cxId = ui->cxCB->currentData().toInt();
    int cxEth = ui->cxEthSB->value();
    QString agg = ui->agg1CB->currentText();
    QString cx = ui->cxCB->currentText();

    if(dbMan->updateAggCx(id,exchId,aggId,aggEth,cxId,cxEth))
    {
        if(currentAggEth != aggEth)
        {
            dbMan->deleteEtherTrunk(aggId,currentAggEth);
            dbMan->insertEtherTrunk(aggId,aggEth,cx);
        }
        if(currentCxEth != cxEth)
        {
            dbMan->deleteEtherTrunk(cxId,currentCxEth);
            dbMan->insertEtherTrunk(cxId,cxEth,agg);
        }

        this->close();
    }
    else
        QMessageBox::warning(this,"ERROR","Cannot update connection.\n"+dbMan->lastError());
}

void EditAggCxDialog::on_abbrCB_currentIndexChanged(int index)
{
    index++;
    int exchId = ui->abbrCB->currentData().toInt();
    ui->agg1CB->clear();
    ui->cxCB->clear();
    ui->okBtn->setEnabled(false);

    QSqlQuery *query = dbMan->selectExchangeAgg(exchId,true);
    //`id`,`device_name`
    int ID;
    QString name;
    while(query->next())
    {
        ID = query->value(0).toInt();
        name = query->value(1).toString();
        ui->agg1CB->addItem(name,ID);
    }

    query = dbMan->selectExchangeCx(exchId);
    //`id`,`device_name`
    while(query->next())
    {
        ID = query->value(0).toInt();
        name = query->value(1).toString();
        ui->cxCB->addItem(name,ID);
    }
}

void EditAggCxDialog::on_agg1CB_currentIndexChanged(int index)
{
    index++;
    if(ui->agg1CB->currentText().isEmpty() || ui->cxCB->currentText().isEmpty())
        ui->okBtn->setEnabled(false);
    else
        ui->okBtn->setEnabled(true);
}

void EditAggCxDialog::on_cxCB_currentIndexChanged(int index)
{
    index++;
    if(ui->agg1CB->currentText().isEmpty() || ui->cxCB->currentText().isEmpty())
        ui->okBtn->setEnabled(false);
    else
        ui->okBtn->setEnabled(true);
}

void EditAggCxDialog::on_aggEthSB_valueChanged(int arg1)
{
    arg1++;
    if(ui->agg1CB->currentText().isEmpty() || ui->cxCB->currentText().isEmpty())
        ui->okBtn->setEnabled(false);
    else
        ui->okBtn->setEnabled(true);
}

void EditAggCxDialog::on_cxEthSB_valueChanged(int arg1)
{
    arg1++;
    if(ui->agg1CB->currentText().isEmpty() || ui->cxCB->currentText().isEmpty())
        ui->okBtn->setEnabled(false);
    else
        ui->okBtn->setEnabled(true);
}
