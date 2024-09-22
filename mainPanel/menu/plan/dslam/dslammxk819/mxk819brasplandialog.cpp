#include "mxk819brasplandialog.h"
#include "ui_mxk819brasplandialog.h"
#include <QSqlQuery>
#include "lib/database/danetdbman.h"
#include <QMessageBox>

Mxk819BrasPlanDialog::Mxk819BrasPlanDialog(QWidget *parent, DanetDbMan *Db, const int Area, const int ExchId, const int DslamId, const QString dslamName) :
    QDialog(parent),
    ui(new Ui::Mxk819BrasPlanDialog),
    dbMan(Db),
    area(Area),
    dslamId(DslamId)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowMinMaxButtonsHint);

    ui->tabWidget->tabBar()->hide();

    ui->dslamLbl->setText(dslamName);

    device = dbMan->getDslamDevice(dslamId); // EQ-T-P-SN

    //
    motherId = dbMan->getSiteMotherId(ExchId);
    QString mother = dbMan->getExchangeAbbr(motherId);
    QString str1 = "Ether-Trunk to "+mother +" Agg1 :";
    QString str2 = "Ether-Trunk to "+mother +" Agg2 :";
    ui->b1a1Lbl->setText(str1);
    ui->b1a2Lbl->setText(str2);
    ui->b2a1Lbl->setText(str1);
    ui->b2a2Lbl->setText(str2);

    QList<int> vlanList = dbMan->getDslamPppoeVlanList(DslamId);
    foreach(int i, vlanList)
        ui->vlan500CB->addItem(QString::number(i));

    vlanList = dbMan->getDslamInetVlanList(DslamId);
        foreach(int i,vlanList)
            ui->vlan600CB->addItem(QString::number(i));

    vlanList = dbMan->getDslamSinetVlanList(DslamId);
        foreach(int i, vlanList)
            ui->vlan700CB->addItem(QString::number(i));

    vlanList = dbMan->getDslamVoipVlanList(DslamId);
        foreach(int i, vlanList)
            ui->vlan800CB->addItem(QString::number(i));



    //BRAS1
    ui->bras1CB->blockSignals(true);
    QSqlQuery *query;
    if(motherId == 37)
        query = dbMan->selectBras(38,true);
    else
        query = dbMan->selectBras(motherId,true);

    //id, name
    int ID;
    QString NAME;
    while(query->next())
    {
        ID = query->value(0).toInt();
        NAME = query->value(1).toString();

        ui->bras1CB->addItem(NAME, ID);
    }
    ui->bras1CB->setCurrentIndex(-1);
    ui->bras1CB->blockSignals(false);
    if(ui->bras1CB->count() > 0)
        ui->bras1CB->setCurrentIndex(0);

    //bras2
    ui->bras2CB->blockSignals(true);
    if(motherId == 37)
        query = dbMan->selectBras(38,false);
    else
        query = dbMan->selectBras(motherId,false);

    //id, name
    while(query->next())
    {
        ID = query->value(0).toInt();
        NAME = query->value(1).toString();

        ui->bras2CB->addItem(NAME, ID);
    }
    ui->bras2CB->setCurrentIndex(-1);
    ui->bras2CB->blockSignals(false);
    if(ui->bras2CB->count() > 0)
        ui->bras2CB->setCurrentIndex(0);


    if(dbMan->dslamPppoeExistance(dslamId))
    {
        QList<int> bras = dbMan->getDslamPppoeBrasId(dslamId);
        int b1 = bras.at(0);
        int b2 = bras.at(1);

        ui->bras1CB->setCurrentIndex(ui->bras1CB->findData(b1));
        if(b2 > -1)
            ui->bras2CB->setCurrentIndex(ui->bras2CB->findData(b2));
        else
            ui->bras2EnCB->setChecked(false);

        query = dbMan->selectDslamPppoe(dslamId);
        int v;
        while(query->next())
        {
            v = query->value(0).toInt();
            if(v > 499 && v < 600) ui->vlan500CB->setCurrentText(QString::number(v));
            if(v > 599 && v < 700) ui->vlan600CB->setCurrentText(QString::number(v));
            if(v > 699 && v < 800) ui->vlan700CB->setCurrentText(QString::number(v));
            if(v > 799 && v < 900) ui->vlan800CB->setCurrentText(QString::number(v));
        }
    ui->okBtn->setEnabled(false);
    }
}

Mxk819BrasPlanDialog::~Mxk819BrasPlanDialog()
{
    delete ui;
}

void Mxk819BrasPlanDialog::checkEntry()
{
    bool b1 = !ui->bras1CB->currentText().isEmpty();
    bool b2 = !ui->bras2CB->currentText().isEmpty() || !ui->bras2EnCB->isChecked();

    bool v5 = !ui->v500DescLE->text().isEmpty();
    bool v6 = !ui->v600DescLE->text().isEmpty();
    bool v7 = !ui->v700DescLE->text().isEmpty();
    bool v8 = !ui->v800DescLE->text().isEmpty();

    if(b1)
        ui->next1Btn->setEnabled(true);
    else
        ui->next1Btn->setEnabled(false);

    if(b2)
        ui->next2Btn->setEnabled(true);
    else
        ui->next2Btn->setEnabled(false);


    if(b1 && b2 && v5 && v6 && v7 && v8)
        ui->okBtn->setEnabled(true);
    else
        ui->okBtn->setEnabled(false);
}

void Mxk819BrasPlanDialog::fillDesc()
{
    if(agg.isEmpty())
    {
        ui->v500DescLE->clear();
        ui->v600DescLE->clear();
        ui->v700DescLE->clear();
        ui->v800DescLE->clear();
    }
    else
    {
        QString vlan = ui->vlan500CB->currentText();
        vlan = agg+vlan+"- "+device;
        ui->v500DescLE->setText(vlan);

        vlan = ui->vlan600CB->currentText();
        vlan = agg+vlan+"- "+device;
        ui->v600DescLE->setText(vlan);

        vlan = ui->vlan700CB->currentText();
        vlan = agg+vlan+"- "+device;
        ui->v700DescLE->setText(vlan);

        vlan = ui->vlan800CB->currentText();
        vlan = agg+vlan+"- "+device;
        ui->v800DescLE->setText(vlan);
    }

    checkEntry();
}

void Mxk819BrasPlanDialog::on_bras1CB_currentIndexChanged(int index)
{
    index++;
    int brasId = ui->bras1CB->currentData().toInt();
    QList<int> list;
    if(motherId == 37)
        list = dbMan->getBrasAggIdEth(brasId, 38);
    else
        list = dbMan->getBrasAggIdEth(brasId, motherId);

    if(list.at(0) > -1)
    {
        ui->bras1Agg1EthSB->setValue(list.at(1));
        ui->bras1Agg2EthSB->setValue(list.at(3));

        int agg1Id = list.at(0);

        agg = dbMan->getAggNameBrief(agg1Id, true);// NN-S9303-AGG#

        if(!agg.isEmpty())
        {
            agg += "- PPPOE- VLAN";
            ui->infoLbl->setVisible(false);
        }else
        {
            ui->infoLbl->setVisible(true);
        }

    }
    else
    {
        ui->bras1Agg1EthSB->setValue(-1);
        ui->bras1Agg2EthSB->setValue(-1);

        ui->v500DescLE->clear();
        ui->v600DescLE->clear();
        ui->v700DescLE->clear();
        ui->v800DescLE->clear();
    }

    fillDesc();

}

void Mxk819BrasPlanDialog::on_bras2CB_currentIndexChanged(int index)
{
    index++;
    int brasId = ui->bras2CB->currentData().toInt();
    QList<int> list;
    if(motherId == 37)
        list = dbMan->getBrasAggIdEth(brasId, 38);
    else
        list = dbMan->getBrasAggIdEth(brasId, motherId);

    if(list.at(0) > -1)
    {
        ui->bras2Agg1EthSB->setValue(list.at(1));
        ui->bras2Agg2EthSB->setValue(list.at(3));
    }
    else
    {
        ui->bras2Agg1EthSB->setValue(-1);
        ui->bras2Agg2EthSB->setValue(-1);

        ui->v500DescLE->clear();
        ui->v600DescLE->clear();
        ui->v700DescLE->clear();
        ui->v800DescLE->clear();
    }

    checkEntry();
}

void Mxk819BrasPlanDialog::on_bras2EnCB_toggled(bool checked)
{
    if(checked)
    {
        ui->bras2GB->setEnabled(true);
        ui->bras2GB->setVisible(true);
    }
    else
    {
        ui->bras2GB->setEnabled(false);
        ui->bras2GB->setVisible(false);
    }
}

void Mxk819BrasPlanDialog::on_okBtn_clicked()
{
    bool ok = true;

    if(dbMan->dslamPppoeExistance(dslamId))
        ok = ok && dbMan->deleteDslamPppoe(dslamId);

    if(ok)
    {
        int bras1 = ui->bras1CB->currentData().toInt();

        bool b2 = ui->bras2EnCB->isChecked();
        int bras2 = -1;
        if(b2)
            bras2 = ui->bras2CB->currentData().toInt();

        int v500 = ui->vlan500CB->currentText().toInt();
        int v600 = ui->vlan600CB->currentText().toInt();
        int v700 = ui->vlan700CB->currentText().toInt();
        int v800 = ui->vlan800CB->currentText().toInt();

        QString v500Desc = ui->v500DescLE->text();
        QString v600Desc = ui->v600DescLE->text();
        QString v700Desc = ui->v700DescLE->text();
        QString v800Desc = ui->v800DescLE->text();

        ok = true;
        ok = ok && dbMan->insertDslamPppoe(dslamId, bras1, bras2, v500, v500Desc);
        ok = ok && dbMan->insertDslamPppoe(dslamId, bras1, bras2, v600, v600Desc);
        ok = ok && dbMan->insertDslamPppoe(dslamId, bras1, bras2, v700, v700Desc);
        ok = ok && dbMan->insertDslamPppoe(dslamId, bras1, bras2, v800, v800Desc);

        if(ok)
        {
            this->close();
        }
        else
        {
            if(dbMan->deleteDslamPppoe(dslamId))
                QMessageBox::warning(this, "ERROR", "Cannot insert BRAS PPPOE parameters. Removed existing parameters successfully.\n"+dbMan->lastError());
            else
                QMessageBox::warning(this, "ERROR", "Cannot insert BRAS PPPOE parameters. parameters remained.\n"+dbMan->lastError());
        }

    }
    else
    {
        QMessageBox::warning(this, "ERROR", "Try again. Old plan must be removed first.");
    }
}

void Mxk819BrasPlanDialog::on_vlan500CB_currentIndexChanged(int index)
{
    ui->vlan600CB->setCurrentIndex(index);
    ui->vlan700CB->setCurrentIndex(index);
    ui->vlan800CB->setCurrentIndex(index);

    fillDesc();
}

void Mxk819BrasPlanDialog::on_vlan600CB_currentIndexChanged(int index)
{
    index++;
    fillDesc();
}

void Mxk819BrasPlanDialog::on_vlan700CB_currentIndexChanged(int index)
{
    index++;
    fillDesc();
}

void Mxk819BrasPlanDialog::on_vlan800CB_currentIndexChanged(int index)
{
    index++;
    fillDesc();
}

void Mxk819BrasPlanDialog::on_vlanBackBtn_clicked()
{
    ui->tabWidget->setCurrentIndex(1);
}

void Mxk819BrasPlanDialog::on_next1Btn_clicked()
{
    ui->tabWidget->setCurrentIndex(1);
}

void Mxk819BrasPlanDialog::on_Back2Btn_clicked()
{
    ui->tabWidget->setCurrentIndex(0);
}

void Mxk819BrasPlanDialog::on_next2Btn_clicked()
{
    ui->tabWidget->setCurrentIndex(2);
}
