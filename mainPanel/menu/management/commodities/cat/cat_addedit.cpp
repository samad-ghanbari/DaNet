#include "cat_addedit.h"
#include "ui_cat_addedit.h"
#include "lib/database/danetdbman.h"
#include <QMessageBox>

namespace Commodity
{

namespace Cat
{

AddEdit::AddEdit(DanetDbMan *db, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddEdit),
    dbMan(db)
{
    ui->setupUi(this);
    id = -1;
}

AddEdit::~AddEdit()
{
    delete ui;
}

void AddEdit::setValues(int ID, QString CAT)
{
    id = ID;
    cat = CAT;
    ui->catLE->setText(cat);
}

void AddEdit::on_okBtn_clicked()
{
    cat = ui->catLE->text().trimmed();
    if(cat.isEmpty())
        return;

    bool ok = false;
    if(id > 0)
    {
        //modify
        ok =dbMan->updateCommodityCat(id, cat);
    }
    else
    {
        //insert
        ok = dbMan->insertCommodityCat(cat);
    }

    if(ok)
        this->close();
    else
    {
        QMessageBox::warning(this, "Error", "There was an error during the performing of the operation.");
    }
}
}
}
