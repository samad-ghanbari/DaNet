#include "editdevicedescdialog.h"
#include "ui_editdevicedescdialog.h"
#include "lib/database/danetdbman.h"
#include <QMessageBox>

EditDeviceDescDialog::EditDeviceDescDialog(QWidget *parent, DanetDbMan *db, const QString abbr, const QString saloon,  const int DevexId,
                                           const int Row, const int Rack, const int Shelf, const QString DeviceName, const QString desc) :
    QDialog(parent),
    ui(new Ui::EditDeviceDescDialog),
    dbMan(db),
    devexId(DevexId)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowMinMaxButtonsHint);

    ui->abbrLbl->setText(abbr);
    ui->saloonLbl->setText(saloon);
    ui->deviceLbl->setText(DeviceName);

    ui->RRSLbl->setText(QString::number(Row)+'*'+QString::number(Rack)+'*'+QString::number(Shelf));

    ui->descTE->setText(desc);

    ui->okBtn->setEnabled(false);
    ui->checkBox->setChecked(false);
    ui->checkBox->setEnabled(false);
}

EditDeviceDescDialog::~EditDeviceDescDialog()
{
    delete ui;
}

void EditDeviceDescDialog::on_cancelBtn_clicked()
{
    this->close();
}

void EditDeviceDescDialog::on_okBtn_clicked()
{

    QString desc = ui->descTE->toPlainText();

    if(dbMan->updateDevexDescription(devexId, desc))
    {
        this->close();
    }
    else
        QMessageBox::warning(this, "Error", "Can not edit device description.\n"+dbMan->lastError());

}

void EditDeviceDescDialog::on_checkBox_toggled(bool checked)
{
    if(checked)
        ui->okBtn->setEnabled(true);
    else
        ui->okBtn->setEnabled(false);
}

void EditDeviceDescDialog::on_descTE_textChanged()
{
    ui->checkBox->setEnabled(true);
}

