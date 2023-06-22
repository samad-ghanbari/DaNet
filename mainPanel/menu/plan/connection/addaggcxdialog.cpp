#include "addaggcxdialog.h"
#include "ui_addaggcxdialog.h"
#include "lib/database/danetdbman.h"
#include <QSqlQuery>
#include <QMessageBox>

AddAggCxDialog::AddAggCxDialog(QWidget *parent, DanetDbMan *Db, const int area) :
    QDialog(parent),
    ui(new Ui::AddAggCxDialog),
    dbMan(Db)
{
    ui->setupUi(this);
    ui->okBtn->setEnabled(false);
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowMinMaxButtonsHint);

    ui->abbrCB->blockSignals(true);
    QSqlQuery *query = dbMan->selectExchanges(area);
    //`id`,`abbr`
    int id;
    QString abbr;
    while(query->next())
    {
        id = query->value(0).toInt();
        abbr = query->value(1).toString();
        ui->abbrCB->addItem(abbr,id);
    }
    ui->abbrCB->setCurrentIndex(-1);
    ui->abbrCB->blockSignals(false);

    ui->aggEthSB->setValue(19);
    ui->cxEthSB->setValue(4);
}

AddAggCxDialog::~AddAggCxDialog()
{
    delete ui;
}

void AddAggCxDialog::on_abbrCB_currentIndexChanged(int index)
{
    index++;
    int exchId = ui->abbrCB->currentData().toInt();
    ui->agg1CB->clear();
    ui->cxCB->clear();
    ui->okBtn->setEnabled(false);

    QSqlQuery *query = dbMan->selectExchangeAgg(exchId,true);
    //`id`,`deviceName`
    int id;
    QString name;
    while(query->next())
    {
        id = query->value(0).toInt();
        name = query->value(1).toString();
        ui->agg1CB->addItem(name,id);
    }

    query = dbMan->selectExchangeCx(exchId);
    int m = dbMan->exchangeKhvExcept(exchId);
    query = dbMan->selectExchangeCx(m);
    //`id`,`device_name`
    while(query->next())
    {
        id = query->value(0).toInt();
        name = query->value(1).toString();
        ui->cxCB->addItem(name,id);
    }
}

void AddAggCxDialog::on_agg1CB_currentIndexChanged(int index)
{
    index++;
    if(ui->agg1CB->currentText().isEmpty() || ui->cxCB->currentText().isEmpty())
        ui->okBtn->setEnabled(false);
    else
        ui->okBtn->setEnabled(true);
}

void AddAggCxDialog::on_cxCB_currentIndexChanged(int index)
{
    index++;
    if(ui->agg1CB->currentText().isEmpty() || ui->cxCB->currentText().isEmpty())
        ui->okBtn->setEnabled(false);
    else
        ui->okBtn->setEnabled(true);
}

void AddAggCxDialog::on_CancelBtn_clicked()
{
    this->close();
}

void AddAggCxDialog::on_okBtn_clicked()
{
    int exchId = ui->abbrCB->currentData().toInt();
    int agg1Id = ui->agg1CB->currentData().toInt();
    int agg1Eth = ui->aggEthSB->value();
    int cxId = ui->cxCB->currentData().toInt();
    int cxEth = ui->cxEthSB->value();
    QString agg1 = ui->agg1CB->currentText();
    QString cx = ui->cxCB->currentText();

    if(dbMan->insertAggCx(exchId, agg1Id,agg1, agg1Eth, cxId,cx, cxEth))
        this->close();
    else
        QMessageBox::warning(this,"ERROR", "Cannot insert connection.\n"+dbMan->lastError());

}





