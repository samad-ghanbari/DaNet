#include "mergesubnet1v4dialog.h"
#include "ui_mergesubnet1v4dialog.h"
#include "lib/database/danetdbman.h"
#include "lib/netTools/nettools.h"
#include <QSqlQuery>
#include <QMessageBox>

MergeSubnet1V4Dialog::MergeSubnet1V4Dialog(QWidget *parent, DanetDbMan *db, const int SubnetPoolId, const QString Abbr, const QString App, const QString Subnet, const int Prefix) :
    QDialog(parent),
    ui(new Ui::MergeSubnet1V4Dialog),
    dbMan(db),
    subnetPoolId(SubnetPoolId),
    prefix(Prefix)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowMinMaxButtonsHint);

    subnet = NetTools::netId(Subnet, prefix);
    mask = NetTools::prefixToSubnetMask(prefix);
    bc = NetTools::broadcast(subnet, prefix);
    host = NetTools::prefixHostCount(prefix);

    ui->okBtn->setEnabled(false);
    ui->checkBox->setChecked(false);
    ui->infoLbl->setVisible(false);
    //There is no adjacent network. Adjacent network might have been splitted itself.
    //Adjacent network should have no description.

    ui->mGb->setVisible(false);
    ui->gb2->setVisible(false);

    ui->abbrLbl->setText(Abbr);
    ui->appLbl->setText(App);

    ui->netLbl1->setText(subnet);
    ui->prefixLbl1->setText(QString::number(prefix));
    ui->maskLbl1->setText(mask);
    ui->bcLbl1->setText(bc);
    ui->hostLbl1->setText(QString::number(host));

    int sub2C = dbMan->getSubnetPoolCount(2,subnetPoolId,1);
    int ipC = dbMan->getIpPoolCount(subnetPoolId,1);


    // net1 can be mereged with next net or not, it should be netId for bigger bundle
    QString newNetId = NetTools::netId(subnet, (prefix-1));

    if(sub2C > 0)
    {
        ui->infoLbl->setText("Cannot be merged when it has 2d-Subnet.");
        ui->infoLbl->setVisible(true);
        ui->checkBox->setChecked(false);
        ui->checkBox->setEnabled(false);
        ui->okBtn->setEnabled(false);
    }
    else if(ipC > 0)
    {
        ui->infoLbl->setText("Subnet cannot be merged when it has IP Pool.\nYou need to remove IP pools first.");
        ui->infoLbl->setVisible(true);
        ui->checkBox->setChecked(false);
        ui->checkBox->setEnabled(false);
        ui->okBtn->setEnabled(false);
    }
    else if(subnet.compare(newNetId) != 0)
    {
        // Network Id error
        ui->infoLbl->setText("Selected subnet must be the start of the bundle.");
        ui->infoLbl->setVisible(true);
        ui->checkBox->setChecked(false);
        ui->checkBox->setEnabled(false);
        ui->okBtn->setEnabled(false);
    }
    else
    {
        //calculate adjacent network
        adjacentSubnet = NetTools::adjacentNetworkIp(subnet, prefix);
        if(adjacentSubnet.compare("-1",Qt::CaseInsensitive) == 0)
        {
            // no adjacent
            ui->infoLbl->setText("There is no adjacent subnet. Adjacent subnet might have been splitted itself.");
            ui->infoLbl->setVisible(true);
            ui->checkBox->setChecked(false);
            ui->checkBox->setEnabled(false);
            ui->okBtn->setEnabled(false);
        }
        else
        {
            //have adjacent
            //get adjacent id
            adjacentSubnetPoolId = dbMan->getSubnetPool1Id(adjacentSubnet, prefix);

            sub2C = dbMan->getSubnetPoolCount(2,adjacentSubnetPoolId,1);
            if(sub2C > 0)
            {
                ui->infoLbl->setText("Cannot be merged when adjacent net has 2d-Subnet");
                ui->infoLbl->setVisible(true);
                ui->checkBox->setChecked(false);
                ui->checkBox->setEnabled(false);
                ui->okBtn->setEnabled(false);
            }
            ipC = dbMan->getIpPoolCount(adjacentSubnetPoolId, 1);
            if(ipC > 0)
            {
                // have IP pool
                ui->infoLbl->setText("Subnet cannot be merged when adjacent net has IP Pool.\nYou need to remove IP pools first.");
                ui->infoLbl->setVisible(true);
                ui->checkBox->setChecked(false);
                ui->checkBox->setEnabled(false);
                ui->okBtn->setEnabled(false);
            }
            else
            {
                if(adjacentSubnetPoolId == -1)
                {
                    // no adjacent
                    ui->infoLbl->setText("There is no adjacent subnet. Adjacent subnet might have been splitted itself.");
                    ui->infoLbl->setVisible(true);
                    ui->checkBox->setChecked(false);
                    ui->checkBox->setEnabled(false);
                    ui->okBtn->setEnabled(false);
                }
                else
                {
                    QSqlQuery *query = dbMan->selectSubnetPool1Record(adjacentSubnetPoolId);
                    //`subnet`, `prefix`, `mask`, `bc`, `hosts`, `area`, `exchange_id`,`department`, `application`, `description`
                    if(query->next())
                    {
                        adjacentSubnet = query->value(0).toString();
                        int prefix2 = query->value(1).toInt();
                        QString mask2 = query->value(2).toString();
                        adjacentBc = query->value(3).toString();
                        int host2 = query->value(4).toInt();


                        ui->mGb->setVisible(true);
                        ui->gb2->setVisible(true);
                        //set values
                        ui->netLbl2->setText(adjacentSubnet);
                        ui->prefixLbl2->setText(QString::number(prefix2));
                        ui->maskLbl2->setText(mask2);
                        ui->bcLbl2->setText(adjacentBc);
                        ui->hostLbl2->setText(QString::number(host2));

                        //merged values
                        mPrefix = prefix -1;
                        mSubnet = subnet;
                        mBc = NetTools::broadcast(mSubnet, mPrefix);
                        mMask = NetTools::prefixToSubnetMask(mPrefix);
                        mHost = NetTools::prefixHostCount(mPrefix);

                        ui->netLbl->setText(mSubnet);
                        ui->prefixLbl->setText(QString::number(mPrefix));
                        ui->maskLbl->setText(mMask);
                        ui->bcLbl->setText(mBc);
                        ui->hostLbl->setText(QString::number(mHost));



                    }
                    else
                    {
                        // no adjacent
                        ui->infoLbl->setText("There is no adjacent subnet. Adjacent subnet might have been splitted itself.");
                        ui->infoLbl->setVisible(true);
                        ui->checkBox->setChecked(false);
                        ui->checkBox->setEnabled(false);
                        ui->okBtn->setEnabled(false);
                    }
                }
            }
        }


    }
}

MergeSubnet1V4Dialog::~MergeSubnet1V4Dialog()
{
    delete ui;
}

void MergeSubnet1V4Dialog::on_checkBox_toggled(bool checked)
{
    if(checked)
        ui->okBtn->setEnabled(true);
    else
        ui->okBtn->setEnabled(false);
}

void MergeSubnet1V4Dialog::on_cancelBtn_clicked()
{
    this->close();
}

void MergeSubnet1V4Dialog::on_okBtn_clicked()
{
    if(dbMan->updateSubnetPool1(subnetPoolId,mSubnet,mPrefix,mMask,mBc,mHost))
    {
        if(dbMan->deleteSubnetPool(1,adjacentSubnetPoolId, 1))
        {
            this->close();
        }
        else
        {
            dbMan->updateSubnetPool1(subnetPoolId,subnet,prefix,mask,bc,host);
            QMessageBox::warning(this, "ERROR", "Cannot merge subnets.\n"+dbMan->lastError());
        }
    }
    else
        QMessageBox::warning(this, "ERROR", "Cannot merge subnets.\n"+dbMan->lastError());

}
