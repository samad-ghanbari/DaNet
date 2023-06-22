#include "addmoduledialog.h"
#include "ui_addmoduledialog.h"
#include "lib/database/danetdbman.h"
#include <QMessageBox>

AddModuleDialog::AddModuleDialog(QWidget *parent, DanetDbMan *db) :
    QDialog(parent),
    ui(new Ui::AddModuleDialog),
    dbMan(db)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowMinMaxButtonsHint);

    ui->okBtn->setEnabled(false);
}

AddModuleDialog::~AddModuleDialog()
{
    delete ui;
}

void AddModuleDialog::entryCheck()
{
    bool m = !ui->modLE->text().isEmpty();
    bool v = !ui->vendorLE->text().isEmpty();
    bool k = (ui->kmSB->value() > 0)? true : false;
    if(m && v && k)
        ui->okBtn->setEnabled(true);
    else
        ui->okBtn->setEnabled(false);
}

void AddModuleDialog::on_CancelBtn_clicked()
{
    this->close();
}

void AddModuleDialog::on_okBtn_clicked()
{
    QString mod = ui->modLE->text().trimmed();
    QString vendor = ui->vendorLE->text().trimmed();
    double km = ui->kmSB->value();

    if(dbMan->insertModule(mod, vendor, km))
    {
        this->close();
    }
    else
        QMessageBox::warning(this,"ERROR", "Cannot insert module.\n"+dbMan->lastError());
}

void AddModuleDialog::on_modLE_textChanged(const QString &arg1)
{
    if(arg1.isEmpty())
        ui->okBtn->setEnabled(false);
    else
        entryCheck();
}

void AddModuleDialog::on_vendorLE_textChanged(const QString &arg1)
{
    if(arg1.isEmpty())
        ui->okBtn->setEnabled(false);
    else
        entryCheck();
}

void AddModuleDialog::on_kmSB_valueChanged(double arg1)
{
    if(arg1 == 0)
        ui->okBtn->setEnabled(false);
    else
        entryCheck();
}
