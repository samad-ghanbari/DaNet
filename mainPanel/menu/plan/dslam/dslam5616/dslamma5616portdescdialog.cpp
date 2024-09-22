#include "dslamma5616portdescdialog.h"
#include "ui_dslamma5616portdescdialog.h"
#include "lib/database/danetdbman.h"
#include <QMessageBox>

DslamMa5616PortDescDialog::DslamMa5616PortDescDialog(QWidget *parent, DanetDbMan *db, const QString dslamName, const int PortId, const QString interface, const QString desc) :
    QDialog(parent),
    ui(new Ui::DslamMa5616PortDescDialog),
    dbMan(db),
    portId(PortId)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowMinMaxButtonsHint);

    ui->dslamLbl->setText(dslamName);
    ui->intLbl->setText(interface);
    ui->descLE->setText(desc);
    ui->okBtn->setEnabled(false);
}

DslamMa5616PortDescDialog::~DslamMa5616PortDescDialog()
{
    delete ui;
}

void DslamMa5616PortDescDialog::on_cancelBtn_clicked()
{
    this->close();
}

void DslamMa5616PortDescDialog::on_okBtn_clicked()
{
    QString desc = ui->descLE->text();
    if(dbMan->updateDslamInterface(portId, desc))
    {
        this->close();
    }
    else
        QMessageBox::warning(this, "ERROR","Cannot update port description.\n"+dbMan->lastError());
}

void DslamMa5616PortDescDialog::on_descLE_textChanged(const QString &arg1)
{
    if(arg1.isEmpty())
        ui->okBtn->setEnabled(true);
    else
        ui->okBtn->setEnabled(true);
}
