#include "uninstalledaddeditdialog.h"
#include "ui_uninstalledaddeditdialog.h"
#include "lib/database/danetdbman.h"
#include <QMessageBox>
#include <QSqlQuery>

UninstalledAddEditDialog::UninstalledAddEditDialog(DanetDbMan *db, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UninstalledAddEditDialog),
    dbMan(db)
{
    ui->setupUi(this);
    id = -1;
    int a = dbMan->getLoggedInDefaultArea();
    ui->areaCB->addItem("2", 2);
    ui->areaCB->addItem("3", 3);
    ui->areaCB->addItem("4", 4);
    ui->areaCB->addItem("5", 5);
    ui->areaCB->addItem("6", 6);
    ui->areaCB->addItem("7", 7);
    ui->areaCB->addItem("8", 8);
    ui->areaCB->setCurrentIndex(ui->areaCB->findData(a));
}

UninstalledAddEditDialog::~UninstalledAddEditDialog()
{
    delete ui;
}

void UninstalledAddEditDialog::setValues(int Id, int area, QString exch, QString commodity, int amount, QString letter, QString desc)
{
    id = Id;
    ui->label->setText("Modification");
    ui->areaCB->setCurrentIndex(ui->areaCB->findData(area));
    ui->exchLE->setText(exch);
    ui->commLE->setText(commodity);
    ui->amountSB->setValue(amount);
    ui->letterLE->setText(letter);
    ui->descTE->setText(desc);
}

void UninstalledAddEditDialog::on_okBtn_clicked()
{
    int area = ui->areaCB->currentData().toInt();
    QString exch = ui->exchLE->text().trimmed();
    QString comm = ui->commLE->text().trimmed();
    int amount = ui->amountSB->value();
    QString letter = ui->letterLE->text().trimmed();
    QString desc = ui->descTE->toPlainText().trimmed();


    if(comm.isEmpty() || exch.isEmpty() || (area < 2))
    {
        QMessageBox::warning(this, "Error", "Input parameters area not provided!");
        return;
    }


    if(id > 0)
    {
        //edit
        dbMan->updateUninstalled(id,area,exch, comm,amount,letter, desc);
    }
    else
    {
        //new
        dbMan->insertUninstalled(area,exch, comm,amount,letter, desc);
    }

    this->close();
}
