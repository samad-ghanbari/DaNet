#include "uninstalledremovedialog.h"
#include "ui_uninstalledremovedialog.h"
#include "lib/database/danetdbman.h"
#include <QMessageBox>

UninstalledRemoveDialog::UninstalledRemoveDialog(DanetDbMan *db, const int ID, const int area, const QString exch, QString commodity, const int amount, const QString letter, const QString desc, const QString ts, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UninstalledRemoveDialog),
    dbMan(db),
    id(ID)
{
    ui->setupUi(this);
    ui->okBtn->setEnabled(false);
    ui->confirmChb->setChecked(false);

    ui->exchLbl->setText(QString::number(area)+'-'+exch);
    ui->commLbl->setText(commodity);
    ui->amountLbl->setText(QString::number(amount));
    ui->letterLbl->setText(letter);
    ui->descTE->setText(desc);
    ui->tsLbl->setText(ts);
}

UninstalledRemoveDialog::~UninstalledRemoveDialog()
{
    delete ui;
}

void UninstalledRemoveDialog::on_confirmChb_toggled(bool checked)
{
    if(checked)
        ui->okBtn->setEnabled(true);
    else
        ui->okBtn->setEnabled(false);
}

void UninstalledRemoveDialog::on_okBtn_clicked()
{
    if(dbMan->deleteUninstalled(id))
        this->close();
    else
        QMessageBox::warning(this, "Error", "There was an error during the operation!");
}
