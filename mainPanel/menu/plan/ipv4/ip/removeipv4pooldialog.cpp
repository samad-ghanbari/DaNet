#include "removeipv4pooldialog.h"
#include "ui_removeipv4pooldialog.h"
#include "lib/database/danetdbman.h"
#include <QMessageBox>

RemoveIpV4PoolDialog::RemoveIpV4PoolDialog(QWidget *parent, DanetDbMan *db, const int SubnetNo, const int SubnetId, const QString Subnet, const int Prefix, const QString Mask, const QString Bc, const int Host, const QString Abbr, const QString App, const QString Desc) :
    QDialog(parent),
    ui(new Ui::RemoveIpV4PoolDialog),
    dbMan(db),
    subnetId(SubnetId),
    prefix(Prefix),
    subnet(Subnet),
    mask(Mask),
    bc(Bc),
    subnetNo(SubnetNo)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowMinMaxButtonsHint);

    ui->subnetLbl->setText(subnet);
    ui->prefixLbl->setText(QString::number(Prefix));
    ui->maskLbl->setText(Mask);
    ui->bcLbl->setText(bc);
    ui->hostLbl->setText(QString::number(Host));

    ui->abbrLbl->setText(Abbr);
    ui->appLbl->setText(App);
    ui->descLbl->setText(Desc);

    bool assigned = false;
    int ipC = -1;

    if((subnetNo>0) && (subnetNo < 4) )
    {
        ipC = dbMan->getIpPoolCount(subnetId, subnetNo);
        assigned = dbMan->isIpAssigned(subnetId, subnetNo);
    }


    if(ipC > 0)
    {
        if(assigned)
        {
            ui->infoLbl->setText("IP pool cannot be removed when it has assigned IP.");
            ui->infoLbl->setVisible(true);
            ui->checkBox->setChecked(false);
            ui->checkBox->setEnabled(false);
        }
        else
        {
            ui->infoLbl->clear();
            ui->infoLbl->setVisible(false);
            ui->checkBox->setChecked(false);
            ui->checkBox->setEnabled(true);
        }
    }
    else
    {
        ui->infoLbl->setText("IP pool does not exist.");
        ui->infoLbl->setVisible(true);
        ui->checkBox->setChecked(false);
        ui->checkBox->setEnabled(false);
    }

    ui->okBtn->setEnabled(false);
}

RemoveIpV4PoolDialog::~RemoveIpV4PoolDialog()
{
    delete ui;
}

void RemoveIpV4PoolDialog::on_cancelBtn_clicked()
{
    this->close();
}

void RemoveIpV4PoolDialog::on_okBtn_clicked()
{
    if(dbMan->deleteIpPool(subnetId, subnetNo))
    {
        this->close();
    }
    else
        QMessageBox::warning(this, "ERROR", "Cannot Remove IP pool.\n"+dbMan->lastError());
}

void RemoveIpV4PoolDialog::on_checkBox_toggled(bool checked)
{
    if(checked)
        ui->okBtn->setEnabled(true);
    else
        ui->okBtn->setEnabled(false);
}
