#include "ipv4releasedialog.h"
#include "ui_ipv4releasedialog.h"
#include "lib/database/danetdbman.h"
#include <QMessageBox>

IpV4ReleaseDialog::IpV4ReleaseDialog(QWidget *parent, DanetDbMan *db, const int IpId, const QString Subnet, const QString Prefix, const QString Mask, const QString Bc, const QString Host, const QString Abbr, const QString App, const QString Desc, const QString Ip, const QString IpDesc) :
    QDialog(parent),
    ui(new Ui::IpV4ReleaseDialog),
    dbMan(db),
    ipId(IpId)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowMinMaxButtonsHint);

    ui->subnetLbl->setText(Subnet);
    ui->prefixLbl->setText(Prefix);
    ui->maskLbl->setText(Mask);
    ui->bcLbl->setText(Bc);
    ui->hostLbl->setText(Host);
    ui->abbrLbl->setText(Abbr);
    ui->appLbl->setText(App);
    ui->descLbl->setText(Desc);

    ui->ipLbl->setText(Ip);
    ui->ipDescLbl->setText(IpDesc);

    IP = Ip;
    desc = IpDesc;

    if(dbMan->isIpEditable(ipId))
    {
        ui->infoLbl->clear();
        ui->infoLbl->setVisible(false);
        ui->checkBox->setChecked(false);
        ui->checkBox->setEnabled(true);
    }
    else
    {
        ui->infoLbl->setText("This IP is not editable. You cannot release it.");
        ui->infoLbl->setVisible(true);
        ui->checkBox->setChecked(false);
        ui->checkBox->setEnabled(false);
    }

    ui->okBtn->setEnabled(false);

}

IpV4ReleaseDialog::~IpV4ReleaseDialog()
{
    delete ui;
}

void IpV4ReleaseDialog::on_cancelBtn_clicked()
{
    this->close();
}

void IpV4ReleaseDialog::on_okBtn_clicked()
{
    QString user = dbMan->getLoggedinNameLastname();

    if(dbMan->updateIp(ipId))//release IP
    {
        dbMan->insertLog(user, "Release IP: "+IP+" with Desc: "+desc);
        this->close();
    }
    else
        QMessageBox::warning(this,"ERROR", "IP is not released.\n"+dbMan->lastError());
}

void IpV4ReleaseDialog::on_checkBox_toggled(bool checked)
{
    if(checked)
        ui->okBtn->setEnabled(true);
    else
        ui->okBtn->setEnabled(false);
}
