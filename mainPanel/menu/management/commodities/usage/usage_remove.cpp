#include "usage_remove.h"
#include "ui_usage_remove.h"
#include "lib/database/danetdbman.h"
#include <QMessageBox>

namespace Commodity {
namespace Usage {

Remove::Remove(const int id, DanetDbMan *db, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Remove),
    dbMan(db),
    usage_id(id)
{
    ui->setupUi(this);
}

Remove::~Remove()
{
    delete ui;
}

void Remove::setValues(const QString letter, const int amount, const QString desc)
{
    ui->letterLbl->setText(letter);
    ui->descTE->setText(desc);
    ui->amountLbl->setText(QString::number(amount));
}

void Remove::setLables(const QString titr, const QString comm)
{
    ui->titrLbl->setText(titr);
    ui->commLbl->setText(comm);
}

void Remove::on_okBtn_clicked()
{
    bool ok = false;
    ok = dbMan->deleteCommodityUsage(usage_id);
    if(ok)
        this->close();
    else
        QMessageBox::warning(this, "Error", "There was an error during the operation!");
}

} // namespace Usage
} // namespace Commodity
