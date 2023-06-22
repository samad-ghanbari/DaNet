#include "ippoolanalyserdialog.h"
#include "ui_ippoolanalyserdialog.h"
#include "lib/netTools/nettools.h"
#include <QFileDialog>
#include <QStringList>


IpPoolAnalyserDialog::IpPoolAnalyserDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::IpPoolAnalyserDialog)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowMinMaxButtonsHint);

    //os
    ui->osCB->addItem("HUAWEI OS", 0);
    ui->osCB->addItem("CISCO IOS", 1);
    ui->osCB->addItem("CISCO IOS-XR", 2);
}

IpPoolAnalyserDialog::~IpPoolAnalyserDialog()
{
    delete ui;
}

void IpPoolAnalyserDialog::emptyParameters()
{
    ui->fileLbl->clear();
    ui->totalIpLbl->clear();
    ui->poolContLbl->clear();
    ui->publicIpLbl->clear();
    ui->privateIpLbl->clear();
}

void IpPoolAnalyserDialog::huaweiIpPoolAnalyse()
{
    if(configFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QString line, ip, mask;
        QStringList list;
        int prefix,ipPoolCount=0, totalIpCount=0,publicIpCount=0, privateIpCount=0, host;
        bool privateIp;

        QTextStream configStream(&configFile);
        line = configStream.readLine().trimmed();

        while(!line.isNull())
        {
            //purify remove ---- More ----  and space
            if(line.contains("---- More ----",Qt::CaseInsensitive))
                line = line.remove("---- More ----",Qt::CaseInsensitive);
            line = line.trimmed();

            //
            if(line.contains("Pool-Name", Qt::CaseInsensitive))
            {
                line = line.remove("Pool-Name", Qt::CaseInsensitive);
                line = line.remove(":", Qt::CaseInsensitive);
                line = line.trimmed();
                if(!line.isEmpty())
                    ipPoolCount++;
            }
            else
                if(line.contains("Gateway", Qt::CaseInsensitive) && line.contains("Mask", Qt::CaseInsensitive))
                {
                    list = line.split("Mask",QString::SkipEmptyParts);
                    ip = list.at(0);
                    mask = list.at(1);

                    ip = ip.remove("Gateway", Qt::CaseInsensitive);
                    ip = ip.remove(":", Qt::CaseInsensitive);
                    ip = ip.trimmed();

                    mask = mask.remove("Mask", Qt::CaseInsensitive);
                    mask = mask.remove(":", Qt::CaseInsensitive);
                    mask = mask.trimmed();

                    if(NetTools::ipv4Validate(ip) && NetTools::subnetMaskValidate(mask))
                    {
                        prefix = NetTools::subnetMaskToPrefix(mask);
                        privateIp = NetTools::isPrivateIp(ip, prefix);
                        host = NetTools::prefixHostCount(prefix);

                        totalIpCount += host;
                        if(privateIp)
                            privateIpCount += host;
                        else
                            publicIpCount += host;
                    }
                }

            line = configStream.readLine().trimmed();
        }

        configFile.close();

        ui->poolContLbl->setText(QString::number(ipPoolCount));
        ui->totalIpLbl->setText(QString::number(totalIpCount));
        ui->publicIpLbl->setText(QString::number(publicIpCount));
        ui->privateIpLbl->setText(QString::number(privateIpCount));
    }
}

void IpPoolAnalyserDialog::IOSIpPoolAnalyse()
{
}

void IpPoolAnalyserDialog::IOSXRIpPoolAnalyse()
{
}

void IpPoolAnalyserDialog::on_osCB_currentIndexChanged(int index)
{
    index++;
    emptyParameters();
    int os = ui->osCB->currentData().toInt();
    switch (os)
    {
    case 0://huawei
        ui->cmdLbl->setText("display ip pool");
        ui->btn->setEnabled(true);
        break;
    case 1://ios
        ui->cmdLbl->setText("");
        ui->btn->setEnabled(false);
        break;
    case 2://ios xr
        ui->cmdLbl->setText("");
        ui->btn->setEnabled(false);
        break;
    }
}

void IpPoolAnalyserDialog::on_btn_clicked()
{
    emptyParameters();
    QString configFileName = QFileDialog::getOpenFileName(this,"IP Pool Configuration");
    if(configFileName.isEmpty())
    {
        return;
    }
    else
    {
        ui->fileLbl->setText(configFileName);

        configFile.setFileName(configFileName);

        int os = ui->osCB->currentData().toInt();
        switch (os)
        {
        case 0:
            huaweiIpPoolAnalyse();
            break;
        case 1:
            IOSIpPoolAnalyse();
            break;
        case 2:
            IOSXRIpPoolAnalyse();
            break;
        }
    }


}
