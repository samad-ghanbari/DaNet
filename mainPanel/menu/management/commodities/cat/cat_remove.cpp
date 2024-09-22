#include "cat_remove.h"
#include "ui_cat_remove.h"
#include "lib/database/danetdbman.h"

namespace Commodity
{
namespace Cat {
Remove::Remove(DanetDbMan *db, const int ID, const QString CAT, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Remove),
    dbMan(db),
    id(ID)
{
    ui->setupUi(this);
    ui->catLbl->setText(CAT);
}

Remove::~Remove()
{
    delete ui;
}

void Remove::on_okBtn_clicked()
{
    dbMan->deleteCommodityCat(id);
    this->close();
}
}
}
