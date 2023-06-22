#include "subnet1v4releasedialog.h"
#include "ui_subnet1v4releasedialog.h"
#include "lib/database/danetdbman.h"
#include <QMessageBox>

Subnet1V4ReleaseDialog::Subnet1V4ReleaseDialog(QWidget *parent, DanetDbMan *db, const int SubnetPoolId, const QString subnet, const int prefix, const QString mask, const QString bc, const int host, const int area,const QString abbr, const QString app, const QString desc) :
    QDialog(parent),
    ui(new Ui::Subnet1V4ReleaseDialog),
    dbMan(db),
    subnetPoolId(SubnetPoolId)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowMinMaxButtonsHint);

    ui->netLbl->setText(subnet);
    ui->prefixLbl->setText(QString::number(prefix));
    ui->maskLbl->setText(mask);
    ui->bcLbl->setText(bc);
    ui->hostLbl->setText(QString::number(host));

    Network = subnet+"/"+QString::number(prefix);
    Desc = desc;

    ui->areaLbl->setText(QString::number(area));
    if(area == 9)
        ui->areaLbl->setText("Karaj");

    ui->abbrLbl->setText(abbr);
    ui->appLbl->setText(app);
    ui->descLbl->setText(desc);

    int ipC = dbMan->getIpPoolCount(subnetPoolId, 1);
    if(ipC > 0)
        ui->infoLbl->setText("Subnet cannot be released when it has related IP pool.");

    int subC = dbMan->getSubnetPoolCount(2,subnetPoolId, 1);
    if(subC > 0)
        ui->infoLbl->setText("Subnet cannot be released when 2d-Subnet exists.");

    if( (ipC > 0) || (subC > 0))
    {
        // not allow
        ui->okBtn->setEnabled(false);
        ui->checkBox->setChecked(false);
        ui->checkBox->setEnabled(false);
        ui->infoLbl->setVisible(true);
    }
    else
    {
        //possible to release
        ui->okBtn->setEnabled(false);
        ui->checkBox->setChecked(false);
        ui->checkBox->setEnabled(true);
        ui->infoLbl->setVisible(false);

    }
}

Subnet1V4ReleaseDialog::~Subnet1V4ReleaseDialog()
{
    delete ui;
}

void Subnet1V4ReleaseDialog::on_checkBox_toggled(bool checked)
{
    if(checked)
        ui->okBtn->setEnabled(true);
    else
        ui->okBtn->setEnabled(false);
}

void Subnet1V4ReleaseDialog::on_cancelBtn_clicked()
{
    this->close();
}

void Subnet1V4ReleaseDialog::on_okBtn_clicked()
{
    QString user = dbMan->getLoggedinNameLastname();

    if(dbMan->updateSubnetPool1(subnetPoolId,-1,-1,-1,"\0","\0"))
    {
        dbMan->insertLog(user,"Release Subnet1: "+Network+" with desc: "+Desc);
        this->close();
    }
    else
        QMessageBox::warning(this,"ERROR", "Cannot release subnet.\n"+dbMan->lastError());
}
