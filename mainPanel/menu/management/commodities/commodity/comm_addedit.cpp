#include "comm_addedit.h"
#include "ui_comm_addedit.h"
#include "lib/database/danetdbman.h"
#include <QSqlQuery>
#include <QMessageBox>

namespace Commodity {

AddEdit::AddEdit(const int catId, DanetDbMan *db, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddEdit),
    dbMan(db),
    cat_id(catId),
    comm_id(-1)
{
    ui->setupUi(this);

    QSqlQuery *query = dbMan->selectPurchases();
    //`id`,`abbr`,`purchase`
    int i;
    QString s, a;
    while(query->next())
    {
        i = query->value(0).toInt();
        a = query->value(1).toString();
        s = query->value(2).toString();
        ui->purCB->addItem(a+" - "+s, i);
    }

    ui->enCB->addItem("Enabled", 1);
    ui->enCB->addItem("Disabled", 0);

}

AddEdit::~AddEdit()
{
    delete ui;
}

void AddEdit::setValues(const int comId, const QString comm, const int purchaseId, const QString location, const QString code, const int total, const int left, const bool en)
{
    comm_id = comId;
    ui->commLE->setText(comm);
    ui->purCB->setCurrentIndex(ui->purCB->findData(purchaseId));
    ui->locLE->setText(location);
    ui->codeLE->setText(code);
    ui->totalSB->setValue(total);
    ui->totalSB->setMinimum(total-left);
    ui->enCB->setCurrentIndex(ui->enCB->findData(en));

}

void AddEdit::setLables(const QString titr, const QString cat)
{
    ui->titrLbl->setText(titr);
    ui->catLbl->setText(cat);
}

void AddEdit::on_okBtn_clicked()
{
    QString commodity = ui->commLE->text().trimmed();
    int purchase_id = ui->purCB->currentData().toInt();
    QString location = ui->locLE->text().trimmed();
    QString code = ui->codeLE->text().trimmed();
    int total = ui->totalSB->value();
    bool en = ui->enCB->currentData().toBool();

    if(commodity.isEmpty())
    {
        QMessageBox::warning(this, "Error", "Commodity name cannot be empty!");
        return ;
    }

    bool ok = false;
    if(comm_id > 0)
    {
        //update
        ok = dbMan->updateCommodity(comm_id,commodity, purchase_id,code,location,total, en);
    }
    else
    {
        //insert
        ok = dbMan->insertCommodity(cat_id,commodity,purchase_id,code,location,total, en);
    }

    if(ok)
        this->close();
    else
        QMessageBox::warning(this, "Error", "There was an error during the operation!");
}

} // namespace Commodity


