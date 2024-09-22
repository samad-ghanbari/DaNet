#include "addodfdialog.h"
#include "ui_addodfdialog.h"
#include "lib/database/danetdbman.h"
#include <QMessageBox>
#include <QSqlQuery>

AddOdfDialog::AddOdfDialog(QWidget *parent, DanetDbMan *db, int exchId, QString areaAbbr, int saloonNo) :
    QDialog(parent),
    ui(new Ui::AddOdfDialog),
    dbMan(db)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowMinMaxButtonsHint);


    exchangeId = exchId;
    ui->areaAbbrLbl->setText(areaAbbr);
    Added = false;
    SaloonNo = saloonNo;

    bool IS_SITE = dbMan->isSite(exchId);
    //saloon CB
    ui->saloonCB->blockSignals(true);
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
    ui->saloonCB->setCurrentIndex(-1);
    ui->saloonCB->blockSignals(false);

    ui->saloonCB->setCurrentIndex(ui->saloonCB->findData(saloonNo));



    //set pin per pos
    ui->pppCB->addItem("6", 6);
    ui->pppCB->addItem("12", 12);
    ui->pppCB->addItem("30", 30);
    ui->pppCB->addItem("72-Huawei Cabinet", 72);
    ui->pppCB->setCurrentIndex(-1);

    ui->okBtn->setEnabled(false);

    int minOdfNo = dbMan->getExchangeMaxOdfNo(exchId, saloonNo);
    minOdfNo++;
//    ui->odfNoSB->setMinimum(minOdfNo);
    ui->odfNoSB->setValue(minOdfNo);

}

AddOdfDialog::~AddOdfDialog()
{
    delete ui;
}

void AddOdfDialog::setInfoLabel(QString info)
{
    ui->infoLbl->setText(info);
}

void AddOdfDialog::clearInfoLable()
{
    ui->infoLbl->clear();
}

void AddOdfDialog::checkEntry()
{
    if(ui->pppCB->currentText().trimmed().isEmpty())
        ui->okBtn->setEnabled(false);
    else
        ui->okBtn->setEnabled(true);

}

bool AddOdfDialog::addedOrNot()
{
    return Added;
}

int AddOdfDialog::getSaloon()
{
    return SaloonNo;
}

void AddOdfDialog::on_cancelBtn_clicked()
{
    this->close();
}

void AddOdfDialog::on_okBtn_clicked()
{
    int odfNo = ui->odfNoSB->value();
    int posCount = ui->posCountSB->value();
    int ppp = ui->pppCB->currentData().toInt();
    int row = ui->odfRowSB->value();
    int column = ui->odfColumnSB->value();

    QString username = dbMan->getLoggedinNameLastname();
    QString Saloon = ui->saloonCB->currentText();
    int saloon = ui->saloonCB->currentData().toInt();

    if(dbMan->insertOdf(exchangeId,saloon,row,column, odfNo, posCount, ppp))
    {
        dbMan->insertLog(exchangeId, username, "Add ODF "+QString::number(odfNo)+" In Saloon "+Saloon);
        SaloonNo = saloon;
        Added = true;
        this->close();
    }
    else
        QMessageBox::information(this, "Error", "Can not Insert New ODF.\n"+dbMan->lastError());
}

void AddOdfDialog::on_pppCB_currentIndexChanged(int index)
{
    index++;
    ui->okBtn->setEnabled(false);
    checkEntry();

    int ppp = ui->pppCB->currentData().toInt();
    ui->posCountSB->blockSignals(true);
    switch (ppp)
    {
    case 6:
        ui->posCountSB->setValue(1);
        setInfoLabel("This type of ODF can contain 1 POS.");
        break;

    case 12:
        ui->posCountSB->setValue(1);
        setInfoLabel("This type of ODF can contain 1 POS.");
        break;

    case 30:
        ui->posCountSB->setValue(3);
        setInfoLabel("This type of ODF can contain 3 POS.");
        break;

    case 72:
        ui->posCountSB->setValue(8);
        setInfoLabel("This type of ODF can contain 8 POS.");
        break;

    default:
        clearInfoLable();
        break;
    }

    ui->posCountSB->blockSignals(false);
}

void AddOdfDialog::on_posCountSB_valueChanged(int arg1)
{
    arg1++;
    checkEntry();
}

void AddOdfDialog::on_odfNoSB_valueChanged(int arg1)
{
    arg1++;
    checkEntry();
}

void AddOdfDialog::on_odfRowSB_valueChanged(int arg1)
{
    arg1++;
    checkEntry();
}

void AddOdfDialog::on_odfColumnSB_valueChanged(int arg1)
{
    arg1++;
    checkEntry();
}

void AddOdfDialog::on_saloonCB_currentIndexChanged(int index)
{
    index++;
    int saloonNo = ui->saloonCB->currentData().toInt();
    int minOdfNo = dbMan->getExchangeMaxOdfNo(exchangeId, saloonNo);
    minOdfNo++;
//    ui->odfNoSB->setMinimum(minOdfNo);
    ui->odfNoSB->setValue(minOdfNo);

    checkEntry();
}
