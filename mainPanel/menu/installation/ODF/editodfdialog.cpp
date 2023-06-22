#include "editodfdialog.h"
#include "ui_editodfdialog.h"
#include "lib/database/danetdbman.h"
#include <QMessageBox>
#include <QSqlQuery>

EditOdfDialog::EditOdfDialog(QWidget *parent, DanetDbMan *db, const int exchId, QString AreaAbbr, const int Saloon, const int row, const int column, const int OdfId, const int OdfNo, const int PosCount, const int ppp) :
    QDialog(parent),
    ui(new Ui::EditOdfDialog),
    dbMan(db)
{
    ui->setupUi(this);

    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowMinMaxButtonsHint);

    changed = false;

    ui->areaAbbrLbl->setText(AreaAbbr);
    ui->pppCB->setEnabled(false);
    ui->posWarningLbl->setVisible(false);

    ui->odfNoSB->setValue(OdfNo);

    odfId = OdfId;

    bool IS_SITE = dbMan->isSite(exchId);
    //saloon CB
    if(IS_SITE)
    {
        QSqlQuery *query = dbMan->selectSaloons(true,false);
        int saloonId;
        QString saloonStr;
        while(query->next())
        {
            saloonId = query->value(0).toInt();
            saloonStr = query->value(1).toString();
            ui->saloonCB->addItem(saloonStr, saloonId);
        }
    }
    else
    {
        QSqlQuery *query = dbMan->selectSaloons(false,true);
        int saloonId;
        QString saloonStr;
        while(query->next())
        {
            saloonId = query->value(0).toInt();
            saloonStr = query->value(1).toString();
            ui->saloonCB->addItem(saloonStr, saloonId);
        }
    }

    ui->saloonCB->setCurrentIndex(ui->saloonCB->findData(Saloon));


    //set pin per pos
    ui->pppCB->blockSignals(true);
    ui->pppCB->addItem("6", 6);
    ui->pppCB->addItem("12", 12);
    ui->pppCB->addItem("30", 30);
    ui->pppCB->addItem("72-Huawei Cabinet", 72);
    ui->pppCB->setCurrentIndex(-1);
    ui->pppCB->blockSignals(false);

    ui->pppCB->setCurrentIndex(ui->pppCB->findData(ppp));
    ui->posCountSB->setValue(PosCount);

    ui->odfRowSB->setValue(row);
    ui->odfColumnSB->setValue(column);

    if(dbMan->odfHasPOS(odfId))
    {
        ui->pppCB->setEnabled(false);
        ui->posWarningLbl->setVisible(true);
    }
    else
    {
        ui->pppCB->setEnabled(true);
        ui->posWarningLbl->setVisible(false);
    }

    ui->posCountSB->setMinimum(dbMan->getOdfMaxPosCount(odfId));

}

EditOdfDialog::~EditOdfDialog()
{
    delete ui;
}

void EditOdfDialog::checkEntry()
{
    bool ok = !ui->saloonCB->currentText().trimmed().isEmpty();
    if(ok)
        ui->modifyBtn->setEnabled(true);
    else
        ui->modifyBtn->setEnabled(false);
}

void EditOdfDialog::on_cancelBtn_clicked()
{
    this->close();
}

void EditOdfDialog::on_modifyBtn_clicked()
{
    int row, column, odfNo, posCount, ppp;
    odfNo = ui->odfNoSB->value();
    posCount = ui->posCountSB->value();
    ppp = -1;
    row = ui->odfRowSB->value();
    column = ui->odfColumnSB->value();

    QString username = dbMan->getLoggedinNameLastname();
    QString saloonName = ui->saloonCB->currentText();

    int exchangeId = dbMan->getOdfExchangeId(odfId);

    if(ui->pppCB->isEnabled())
        ppp = ui->pppCB->currentData().toInt();
    else
        ppp = -1;

    saloon = ui->saloonCB->currentData().toInt();

    if(dbMan->updateOdf(odfId, saloon, row, column, odfNo, posCount,ppp))
    {
        dbMan->insertLog(exchangeId, username, "Edit ODF "+QString::number(odfNo)+" In Saloon "+saloonName);

        changed = true;
        this->close();
    }
    else
        QMessageBox::information(this, "Error", "ODF modification failed.\n"+dbMan->lastError());

}

void EditOdfDialog::on_saloonCB_currentIndexChanged(int index)
{
    index++;
    checkEntry();
}

void EditOdfDialog::on_odfRowSB_valueChanged(int arg1)
{
    arg1++;
    checkEntry();
}

void EditOdfDialog::on_odfColumnSB_valueChanged(int arg1)
{
    arg1++;
    checkEntry();
}

void EditOdfDialog::on_odfNoSB_valueChanged(int arg1)
{
    arg1++;
    checkEntry();
}

void EditOdfDialog::on_posCountSB_valueChanged(int arg1)
{
    arg1++;
    checkEntry();
}

void EditOdfDialog::on_pppCB_currentIndexChanged(int index)
{
    index++;
    checkEntry();
}
