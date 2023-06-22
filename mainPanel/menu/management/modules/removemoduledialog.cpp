#include "removemoduledialog.h"
#include "ui_removemoduledialog.h"
#include "lib/database/danetdbman.h"
#include <QMessageBox>

RemoveModuleDialog::RemoveModuleDialog(QWidget *parent, DanetDbMan *db, const int Id, const QString module, const QString vendor, const double km) :
    QDialog(parent),
    ui(new Ui::RemoveModuleDialog),
    dbMan(db),
    id(Id)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowMinMaxButtonsHint);

    ui->modLbl->setText(module);
    ui->vendorLbl->setText(vendor);
    ui->kmLbl->setText(QString::number(km));
    ui->okBtn->setEnabled(false);
    ui->checkBox->setChecked(false);
}

RemoveModuleDialog::~RemoveModuleDialog()
{
    delete ui;
}

void RemoveModuleDialog::on_checkBox_toggled(bool checked)
{
    if(checked)
        ui->okBtn->setEnabled(true);
    else
        ui->okBtn->setEnabled(false);
}

void RemoveModuleDialog::on_CancelBtn_clicked()
{
    this->close();
}

void RemoveModuleDialog::on_okBtn_clicked()
{
    if(dbMan->deleteModule(id))
    {
        this->close();
    }
    else
        QMessageBox::warning(this,"ERROR", "Cannot remove module.\n"+dbMan->lastError());
}
