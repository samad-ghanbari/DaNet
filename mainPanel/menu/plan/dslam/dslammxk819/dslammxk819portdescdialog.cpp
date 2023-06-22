#include "dslammxk819portdescdialog.h"
#include "ui_dslammxk819portdescdialog.h"
#include "lib/database/danetdbman.h"
#include <QMessageBox>

DslamMxk819PortDescDialog::DslamMxk819PortDescDialog(QWidget *parent, DanetDbMan *db, const QString dslamName, const int PortId, const QString interface, const QString desc) :
    QDialog(parent),
    ui(new Ui::DslamMxk819PortDescDialog),
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

DslamMxk819PortDescDialog::~DslamMxk819PortDescDialog()
{
    delete ui;
}

void DslamMxk819PortDescDialog::on_cancelBtn_clicked()
{
    this->close();
}

void DslamMxk819PortDescDialog::on_okBtn_clicked()
{
    QString desc = ui->descLE->text();
    if(dbMan->updateDslamInterface(portId, desc))
    {
        this->close();
    }
    else
        QMessageBox::warning(this, "ERROR","Cannot update port description.\n"+dbMan->lastError());
}

void DslamMxk819PortDescDialog::on_descLE_textChanged(const QString &arg1)
{
    if(arg1.isEmpty())
        ui->okBtn->setEnabled(true);
    else
        ui->okBtn->setEnabled(true);
}
