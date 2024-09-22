#include "ipv4assigndialog.h"
#include "ui_ipv4assigndialog.h"
#include "lib/database/danetdbman.h"
#include <QMessageBox>

IpV4AssignDialog::IpV4AssignDialog(QWidget *parent, DanetDbMan *db, const int IpId, const QString Subnet, const QString Prefix, const QString Mask, const QString Bc, const QString Host, const QString Abbr, const QString App, const QString Desc, const QString Ip, const QString IpDesc) :
    QDialog(parent),
    ui(new Ui::IpV4AssignDialog),
    dbMan(db),
    ipId(IpId)
{
    ui->setupUi(this);

    editable = false;
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
    ui->descLE->setText(IpDesc);
    IP = Ip;

    if(dbMan->isIpEditable(ipId))
    {
        ui->infoLbl->clear();
        ui->infoLbl->setVisible(false);
        editable = true;
    }
    else
    {
        ui->infoLbl->setText("This IP is not editable. You cannot assign it.");
        ui->infoLbl->setVisible(true);
        editable = false;
    }

    ui->okBtn->setEnabled(false);
}

IpV4AssignDialog::~IpV4AssignDialog()
{
    delete ui;
}

void IpV4AssignDialog::on_descLE_textChanged(const QString &arg1)
{
    if(editable && !arg1.isEmpty())
        ui->okBtn->setEnabled(true);
    else
        ui->okBtn->setEnabled(false);
}

void IpV4AssignDialog::on_cancelBtn_clicked()
{
    this->close();
}

void IpV4AssignDialog::on_okBtn_clicked()
{
    QString d = ui->descLE->text();
    QString user = dbMan->getLoggedinNameLastname();

    if(dbMan->updateIp(ipId, d))
    {
        dbMan->insertLog(user,"Assign IP: "+IP+" desc: "+d);
        this->close();
    }
    else
        QMessageBox::warning(this,"ERROR", "Cannot Assign IP.\n"+dbMan->lastError());
}
