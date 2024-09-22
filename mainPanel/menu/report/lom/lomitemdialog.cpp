#include "lomitemdialog.h"
#include "ui_lomitemdialog.h"
#include <QSqlQuery>
#include "lib/database/danetdbman.h"
#include <QMessageBox>

LomItemDialog::LomItemDialog(DanetDbMan *db) :
    QDialog(nullptr),
    ui(new Ui::LomItemDialog),
    dbMan(db)
{
    ui->setupUi(this);

    CommType = "";
    CommInfo = "";
    Amount = 0;
    Purchase = "";
    Location = "";
    Code = "";
    Desc = "";

    done = false;

    QSqlQuery *query = dbMan->selectPurchases();
    //`id`,`abbr`,`purchase`
    int i;
    QString s, a;
    while(query->next())
    {
        i = query->value(0).toInt();
        a = query->value(1).toString();
        s = query->value(2).toString();
        ui->purchaseCB->addItem(a+" - "+s, i);
    }

    ui->commCatCB->blockSignals(true);
    query = dbMan->selectCommodityCat();
    // `id`, `cat`
    while(query->next())
    {
        i = query->value(0).toInt();
        s = query->value(1).toString();
        ui->commCatCB->addItem(s, i);
    }
    ui->commCatCB->setCurrentIndex(-1);
    ui->commCatCB->blockSignals(false);
}

LomItemDialog::~LomItemDialog()
{
    delete ui;
}


void LomItemDialog::on_commCatCB_currentIndexChanged(int index)
{
    index++;
    commInfoMap.clear();
    ui->commCB->blockSignals(true);
    ui->commCB->clear();
    ui->purchaseCB->setCurrentIndex(-1);
    ui->codeLE->setText("");
    ui->locationLE->setText("");


    index = ui->commCatCB->currentData().toInt();
    QSqlQuery *query = dbMan->selectViewCommodity(index, true);
    //`cat_id`, `cat`, 2`comm_id`, 3`commodity`, 4`purchase_id`, 5`purchase`, 6`code`, 7`location`, 8`total_amount`, 9`left_amount`, 10 ts,  11 enabled
    int i;
    QString temp;
    QVariantList vl;

    while(query->next())
    {
        temp = query->value(11).toString().trimmed();

        if(temp.compare("Enabled",Qt::CaseInsensitive) == 0)
        {
            i = query->value(2).toInt();
            temp = query->value(3).toString();
            ui->commCB->addItem(temp, i);
            vl.clear();
            vl << query->value(4) << query->value(6) << query->value(7)<< query->value(9); //purchase code location left
            commInfoMap[i] = vl;
        }
    }
    ui->commCB->setCurrentIndex(-1);
    ui->commCB->blockSignals(false);

}

void LomItemDialog::on_commCB_currentIndexChanged(int index)
{
    index++;
    index = ui->commCB->currentData().toInt();
    if(!commInfoMap[index].isEmpty())
    {
        int i = commInfoMap[index][0].toInt();//pur
        ui->purchaseCB->setCurrentIndex(ui->purchaseCB->findData(i));
        QString temp = commInfoMap[index][1].toString(); //code
        ui->codeLE->setText(temp);
        temp = commInfoMap[index][2].toString(); //location
        ui->locationLE->setText(temp);
        ui->amountSB->setMaximum(commInfoMap[index][3].toInt());
    }
}

void LomItemDialog::setLabel(QString lbl)
{
    ui->label->setText(lbl);
}

void LomItemDialog::setOkBtnText(QString txt)
{
    ui->okBtn->setText(txt);
}

void LomItemDialog::setValues(QString type, QString info, int amount, QString purchase,  QString location, QString code, QString desc)
{
    ui->commCatCB->blockSignals(true);
    ui->commCatCB->setCurrentText(type);
    ui->commCatCB->blockSignals(false);
    ui->commCB->setFocus();

    ui->commCB->blockSignals(true);
    ui->commCB->setCurrentText(info);
    ui->amountSB->setValue(amount);
    ui->purchaseCB->setCurrentText(purchase);
    ui->locationLE->setText(location);
    ui->codeLE->setText(code);
    ui->commCB->blockSignals(false);

    ui->descTE->setText(desc);
}

void LomItemDialog::on_okBtn_clicked()
{
    CommType = ui->commCatCB->currentText().trimmed();
    CommInfo = ui->commCB->currentText().trimmed();
    Amount = ui->amountSB->value();
    Purchase = ui->purchaseCB->currentText().trimmed();
    Location = ui->locationLE->text().trimmed();
    Code = ui->codeLE->text().trimmed();
    Desc = ui->descTE->toPlainText().trimmed();

    if(Amount > 0)
    {
        done = true;
        this->close();
    }
    else
        QMessageBox::warning(this,"Error", "Amount cannot be zero value.");

}



