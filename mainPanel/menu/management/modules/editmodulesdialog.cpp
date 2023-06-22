#include "editmodulesdialog.h"
#include "ui_editmodulesdialog.h"
#include "lib/database/danetdbman.h"
#include <QMessageBox>

EditModulesDialog::EditModulesDialog(QWidget *parent, DanetDbMan *db, const int Id, const QString Module,const QString vendor, const double km) :
    QDialog(parent),
    ui(new Ui::EditModulesDialog),
    dbMan(db),
    id(Id)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowMinMaxButtonsHint);

    ui->modLE->setText(Module);
    ui->vendorLE->setText(vendor);
    ui->kmSB->setValue(km);
    ui->okBtn->setEnabled(false);
}

EditModulesDialog::~EditModulesDialog()
{
    delete ui;
}

void EditModulesDialog::entryCheck()
{
    bool m = !ui->modLE->text().isEmpty();
    bool v = !ui->vendorLE->text().isEmpty();
    bool k = (ui->kmSB->value() > 0)? true : false;

    if(m && v && k)
        ui->okBtn->setEnabled(true);
    else
        ui->okBtn->setEnabled(false);
}

void EditModulesDialog::on_modLE_textChanged(const QString &arg1)
{
    if(arg1.isEmpty())
        ui->okBtn->setEnabled(false);
    else
        entryCheck();
}

void EditModulesDialog::on_CancelBtn_clicked()
{
    this->close();
}

void EditModulesDialog::on_okBtn_clicked()
{
    QString mod = ui->modLE->text().trimmed();
    QString vendor = ui->vendorLE->text().trimmed();
    double km = ui->kmSB->value();

    if(dbMan->updateModule(id,mod,vendor,km))
    {
        this->close();
    }
    else
        QMessageBox::warning(this,"ERROR", "Cannot update module.\n"+dbMan->lastError());
}

void EditModulesDialog::on_vendorLE_textChanged(const QString &arg1)
{
    if(arg1.isEmpty())
        ui->okBtn->setEnabled(false);
    else
        entryCheck();
}

void EditModulesDialog::on_kmSB_valueChanged(double arg1)
{
    if(arg1 == 0)
        ui->okBtn->setEnabled(false);
    else
        entryCheck();
}
