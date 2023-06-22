#include "comm_remove.h"
#include "ui_comm_remove.h"
#include "lib/database/danetdbman.h"
#include <QMessageBox>

namespace Commodity {

Remove::Remove(const int id, DanetDbMan *db, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Remove),
    dbMan(db),
    comm_id(id)
{
    ui->setupUi(this);
}

Remove::~Remove()
{
    delete ui;
}

void Remove::setValues(const QString comm, const QString purchase, const QString location, const QString code, const int total)
{
    ui->commLbl->setText(comm);
    ui->purLbl->setText(purchase);
    ui->locLbl->setText(location);
    ui->codeLbl->setText(code);
    ui->totalLbl->setText(QString::number(total));
}

void Remove::setLables(const QString titr, const QString cat)
{
    ui->titrLbl->setText(titr);
    ui->catLbl->setText(cat);
}

} // namespace 

void Commodity::Remove::on_okBtn_clicked()
{
    bool ok = false;
    ok = dbMan->deleteCommodity(comm_id);
    if(ok)
        this->close();
    else
        QMessageBox::warning(this, "Error", "There was an error during the operation!");
}
