#include "usage_addedit.h"
#include "ui_usage_addedit.h"
#include "lib/database/danetdbman.h"
#include <QSqlQuery>
#include <QMessageBox>

namespace Commodity {
namespace Usage {

Addedit::Addedit(const int commId, const int left, DanetDbMan *db, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Addedit),
    dbMan(db),
    comm_id(commId),
    usage_id(-1)
{
    ui->setupUi(this);
    ui->amountSB->setMaximum(left);
}

Addedit::~Addedit()
{
    delete ui;
}

void Addedit::setValues(const int usageId, const QString letter, const int left, const int amount, const QString desc)
{
    usage_id = usageId;
    ui->letterLE->setText(letter);
    ui->descTE->setText(desc);

    ui->amountSB->setMaximum(amount+left);
    ui->amountSB->setValue(amount);
}

void Addedit::setLables(const QString titr, const QString comm)
{
    ui->titrLbl->setText(titr);
    ui->commLbl->setText(comm);
}

void Addedit::on_okBtn_clicked()
{
    QString letter = ui->letterLE->text().trimmed();
    int amount = ui->amountSB->value();
    QString desc = ui->descTE->toPlainText().trimmed();


    if(desc.isEmpty())
    {
        QMessageBox::warning(this, "Error", "description cannot be empty!");
        return ;
    }
    if(amount  <= 0)
    {
        QMessageBox::warning(this, "Error", "Amount must be greater that 0!");
        return ;
    }

    bool ok = false;
    if(usage_id > 0)
    {
        //update
        ok = dbMan->updateCommodityUsage(usage_id,letter, amount,desc);
    }
    else
    {
        //insert
        ok = dbMan->insertCommodityUsage(comm_id,letter, amount, desc);
    }

    if(ok)
        this->close();
    else
        QMessageBox::warning(this, "Error", "There was an error during the operation!");
}

} // namespace Usage
} // namespace Commodity
