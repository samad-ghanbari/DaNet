#include "networkcalculatordialog.h"
#include "ui_networkcalculatordialog.h"
#include "lib/netTools/nettools.h"
#include <QRegExpValidator>
#include <QStandardItemModel>
#include <QStringList>
#include <QStringListModel>

NetworkCalculatorDialog::NetworkCalculatorDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NetworkCalculatorDialog),
    model(new QStandardItemModel(this)),
    LVModel(new QStringListModel(this)),
    ipRange("(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])"),
    ipRegex("^" + ipRange+ "\\." + ipRange+ "\\." + ipRange+"\\." + ipRange + "$"),
    ipValidator(new QRegExpValidator(ipRegex, this))
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowMinMaxButtonsHint);

    //NETWORK INFO
    ui->classCB->addItem("CLASS A", 1);
    ui->classCB->addItem("CLASS B", 2);
    ui->classCB->addItem("CLASS C", 3);
    ui->classCB->addItem("CLASS D", 4);
    ui->classCB->addItem("CLASS E", 5);

    ui->NIPClassLE->setValidator(ipValidator);

    //Splitting
    ui->SIPLE->setValidator(ipValidator);
    ui->SMaskLE->setValidator(ipValidator);
    ui->SMaskLE->setText("255.255.255.0");
    ui->STreeView->setModel(model);
    ui->STreeView->setAnimated(true);
    ui->STreeView->setFont(QFont("Times", 14));
    ui->STreeView->setHeaderHidden(true);
    ui->STreeView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->STreeView->setIndentation(100);

    //calculator
    ui->CIpLE->setValidator(ipValidator);
    ui->CMaskLE->setValidator(ipValidator);
    ui->C2MaskLE->setValidator(ipValidator);

    ui->C2PrefixSB->setValue(24);
    ui->C2MaskLE->setText("255.255.255.0");

    ui->CPrefixSB->setValue(24);
    ui->CMaskLE->setText("255.255.255.0");

    ui->CNextBundlesLV->setModel(LVModel);

}

NetworkCalculatorDialog::~NetworkCalculatorDialog()
{
    delete ui;
}

void NetworkCalculatorDialog::splitIp(QString ip, int prefix)
{
    NetTools::ipSplits(model, ip, prefix);
}

void NetworkCalculatorDialog::ipCalculate()
{
    if(ui->CIpLE->hasAcceptableInput())
    {
        QString ip = ui->CIpLE->text();
        int prefix = ui->CPrefixSB->value();

        QString temp = NetTools::netId(ip, prefix);
        ui->CNetIdLbl->setText(temp);

        temp = NetTools::netFirstIp(ip, prefix);
        ui->CFirstIpLbl->setText(temp);

        temp = NetTools::netLastIp(ip, prefix);
        ui->CLastIpLbl->setText(temp);

        temp = NetTools::broadcast(ip, prefix);
        ui->CBCLbl->setText(temp);

        int t = NetTools::prefixHostCount(prefix);
        ui->CHostCountLbl->setText(QString::number(t));

        QStringList list = NetTools::bundles(ip, prefix);
        LVModel->setStringList(list);

    }
    else
    {
        emptyIpCalculate();
    }
}

void NetworkCalculatorDialog::emptyIpCalculate()
{
    ui->CNetIdLbl->clear();
    ui->CFirstIpLbl->clear();
    ui->CLastIpLbl->clear();
    ui->CBCLbl->clear();
    ui->CHostCountLbl->clear();
    LVModel->setStringList(QStringList());
}

void NetworkCalculatorDialog::on_classCB_currentIndexChanged(int index)
{
    index++;
    QMap<QString, int> map;
    QString temp;
    int classId = ui->classCB->currentData().toInt();
    //1:A, 2:B, 3:C, 4:D, 5:E
    switch (classId)
    {
    case 1:
        ui->NprivateNetLbl->setText(NetTools::classAPrivateNetwork());
        ui->NprivateMaskLbl->setText(NetTools::classAPrivateNetworkMask());
        ui->NdefaultMaskLbl->setText(NetTools::classADefaultSubnetMask());
        map = NetTools::classAFirstOctetRange();
        temp = QString::number(map.value("start")) + "-"+QString::number(map.value("end"));
        ui->NfirstOctetRangeLbl->setText(temp);
        ui->NfirstMSBLbl->setText(NetTools::classAFirstOctetMSB());
        ui->NhostCountLbl->setText(NetTools::classAHostPerNetwork());
        ui->NsubnetCountLbl->setText(NetTools::classANumberOfNetwork());
        break;
    case 2:
        ui->NprivateNetLbl->setText(NetTools::classBPrivateNetwork());
        ui->NprivateMaskLbl->setText(NetTools::classBPrivateNetworkMask());
        ui->NdefaultMaskLbl->setText(NetTools::classBDefaultSubnetMask());
        map = NetTools::classBFirstOctetRange();
        temp = QString::number(map.value("start")) + "-"+QString::number(map.value("end"));
        ui->NfirstOctetRangeLbl->setText(temp);
        ui->NfirstMSBLbl->setText(NetTools::classBFirstOctetMSB());
        ui->NhostCountLbl->setText(NetTools::classBHostPerNetwork());
        ui->NsubnetCountLbl->setText(NetTools::classBNumberOfNetwork());

        break;
    case 3:
        ui->NprivateNetLbl->setText(NetTools::classCPrivateNetwork());
        ui->NprivateMaskLbl->setText(NetTools::classCPrivateNetworkMask());
        ui->NdefaultMaskLbl->setText(NetTools::classCDefaultSubnetMask());
        map = NetTools::classCFirstOctetRange();
        temp = QString::number(map.value("start")) + "-"+QString::number(map.value("end"));
        ui->NfirstOctetRangeLbl->setText(temp);
        ui->NfirstMSBLbl->setText(NetTools::classCFirstOctetMSB());
        ui->NhostCountLbl->setText(NetTools::classCHostPerNetwork());
        ui->NsubnetCountLbl->setText(NetTools::classCNumberOfNetwork());

        break;
    case 4:
        ui->NprivateNetLbl->setText(NetTools::classDPrivateNetwork());
        ui->NprivateMaskLbl->setText(NetTools::classDPrivateNetworkMask());
        ui->NdefaultMaskLbl->setText(NetTools::classDDefaultSubnetMask());
        map = NetTools::classDFirstOctetRange();
        temp = QString::number(map.value("start")) + "-"+QString::number(map.value("end"));
        ui->NfirstOctetRangeLbl->setText(temp);
        ui->NfirstMSBLbl->setText(NetTools::classDFirstOctetMSB());
        ui->NhostCountLbl->setText(NetTools::classDHostPerNetwork());
        ui->NsubnetCountLbl->setText(NetTools::classDNumberOfNetwork());

        break;
    case 5:
        ui->NprivateNetLbl->setText(NetTools::classEPrivateNetwork());
        ui->NprivateMaskLbl->setText(NetTools::classEPrivateNetworkMask());
        ui->NdefaultMaskLbl->setText(NetTools::classEDefaultSubnetMask());
        map = NetTools::classEFirstOctetRange();
        temp = QString::number(map.value("start")) + "-"+QString::number(map.value("end"));
        ui->NfirstOctetRangeLbl->setText(temp);
        ui->NfirstMSBLbl->setText(NetTools::classEFirstOctetMSB());
        ui->NhostCountLbl->setText(NetTools::classEHostPerNetwork());
        ui->NsubnetCountLbl->setText(NetTools::classENumberOfNetwork());

        break;
    }
}

void NetworkCalculatorDialog::on_NIPClassLE_textChanged(const QString &arg1)
{
    ui->NclassLbl->clear();
    if(ui->NIPClassLE->hasAcceptableInput())
    {
        QString cl = NetTools::ipClassIdentify(arg1);
        if(cl.toInt() != -1)
            ui->NclassLbl->setText( " Class "+cl);
    }
    else
    {
        if(arg1.isEmpty())
            ui->NclassLbl->clear();
        else
            ui->NclassLbl->setText("Invalid IP");
    }
}

void NetworkCalculatorDialog::on_SIPLE_textChanged(const QString &arg1)
{
    model->clear();
    if(ui->SIPLE->hasAcceptableInput())
    {
        int prefix = ui->SPrefixSB->value();
        QString net = NetTools::netId(arg1 , prefix);
        splitIp(net, prefix);
    }
}

void NetworkCalculatorDialog::on_SPrefixSB_valueChanged(int arg1)
{
    model->clear();
    QString mask = NetTools::prefixToSubnetMask(arg1);
    ui->SMaskLE->setText(mask);
}

void NetworkCalculatorDialog::on_SMaskLE_textChanged(const QString &arg1)
{
    model->clear();

    if(ui->SMaskLE->hasAcceptableInput())
    {
        if(NetTools::subnetMaskValidate(arg1))
        {
            int prefix = NetTools::subnetMaskToPrefix(arg1);
            ui->SPrefixSB->setValue(prefix);
            if(ui->SIPLE->hasAcceptableInput())
            {
                QString ip = ui->SIPLE->text();
                int prefix = ui->SPrefixSB->value();
                QString net = NetTools::netId(ip , prefix);
                splitIp(net, prefix);
            }

           ui->SMaskLE->setStyleSheet("QLineEdit{color: rgb(85, 0, 127);}");
        }
        else
            ui->SMaskLE->setStyleSheet("QLineEdit{color:darkred}");
    }
    else
        ui->SMaskLE->setStyleSheet("QLineEdit{color:darkred}");
}
////
void NetworkCalculatorDialog::on_CIpLE_textChanged(const QString &arg1)
{
    if(arg1.isEmpty())
        emptyIpCalculate();
    else
    {
        if(ui->CIpLE->hasAcceptableInput())
            ipCalculate();
        else
            emptyIpCalculate();
    }
}

void NetworkCalculatorDialog::on_CPrefixSB_valueChanged(int arg1)
{
    QString mask = NetTools::prefixToSubnetMask(arg1);
    ui->CMaskLE->setText(mask);
}

void NetworkCalculatorDialog::on_CMaskLE_textChanged(const QString &arg1)
{
    if(ui->CMaskLE->hasAcceptableInput())
    {
        if(NetTools::subnetMaskValidate(arg1))
        {
            int p = NetTools::subnetMaskToPrefix(arg1);
            ui->CPrefixSB->setValue(p);
            ui->CMaskLE->setStyleSheet("QLineEdit{color: rgb(85, 0, 127);}");
            ipCalculate();
        }
        else
        {
            ui->CMaskLE->setStyleSheet("QLineEdit{color:darkred}");
            emptyIpCalculate();
        }
    }
    else
    {
        ui->CMaskLE->setStyleSheet("QLineEdit{color:darkred}");
        emptyIpCalculate();
    }

}
////
void NetworkCalculatorDialog::on_C2PrefixSB_valueChanged(int arg1)
{
    QString mask = NetTools::prefixToSubnetMask(arg1);
    ui->C2MaskLE->setText(mask);
}

void NetworkCalculatorDialog::on_C2MaskLE_textChanged(const QString &arg1)
{
    if(ui->C2MaskLE->hasAcceptableInput())
    {
        if(NetTools::subnetMaskValidate(arg1))
        {
            int prefix = NetTools::subnetMaskToPrefix(arg1);
            ui->C2PrefixSB->setValue(prefix);
            ui->C2MaskLE->setStyleSheet("QLineEdit{color: rgb(85, 0, 127);}");
        }
        else
        {
            ui->C2MaskLE->setStyleSheet("QLineEdit{color:darkred}");
        }

    }
    else
    {
        ui->C2MaskLE->setStyleSheet("QLineEdit{color:darkred}");
    }

}

////
