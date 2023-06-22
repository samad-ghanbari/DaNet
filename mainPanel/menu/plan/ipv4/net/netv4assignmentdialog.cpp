#include "netv4assignmentdialog.h"
#include "ui_netv4assignmentdialog.h"
#include "lib/database/danetdbman.h"
#include "lib/netTools/nettools.h"
#include <QMessageBox>
#include <QSqlQuery>

NetV4AssignmentDialog::NetV4AssignmentDialog(QWidget *parent, DanetDbMan *db, const int NetPoolId, const QString net, const int prefix, const QString desc) :
    QDialog(parent),
    ui(new Ui::NetV4AssignmentDialog),
    dbMan(db),
    netPoolId(NetPoolId)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowMinMaxButtonsHint);

    ui->netLbl->setText(net);
    ui->prefixLbl->setText(QString::number(prefix));
    Network = net+"/"+QString::number(prefix);
    bc = NetTools::broadcast(net,prefix);
    mask = NetTools::prefixToSubnetMask(prefix);
    host = NetTools::prefixHostCount(prefix);
    ui->maskLbl->setText(mask);
    ui->bcLbl->setText(bc);
    ui->hostLbl->setText(QString::number(host));

    ui->descLE->blockSignals(true);
    ui->descLE->setText(desc);
    ui->descLE->blockSignals(false);

    ui->okBtn->setEnabled(false);

}

NetV4AssignmentDialog::~NetV4AssignmentDialog()
{
    delete ui;
}

void NetV4AssignmentDialog::on_descLE_textChanged(const QString &arg1)
{
    if(arg1.isEmpty())
        ui->okBtn->setEnabled(false);
    else
        ui->okBtn->setEnabled(true);
}

void NetV4AssignmentDialog::on_cancelBtn_clicked()
{
    this->close();
}

void NetV4AssignmentDialog::on_okBtn_clicked()
{
    QString desc = ui->descLE->text();
    QString user = dbMan->getLoggedinNameLastname();

    if(dbMan->updateNetPool(netPoolId,mask,bc,host, desc))
    {
        dbMan->insertLog(user,"Assign Network: "+Network+" Desc:"+desc);
        this->close();
    }
    else
        QMessageBox::warning(this,"ERROR", "Cannot assign network.\n"+dbMan->lastError());
}

