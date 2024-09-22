#include "localdevicedialog.h"
#include "ui_localdevicedialog.h"
#include "lib/database/danetdbman.h"
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QStringListModel>
#include <QHBoxLayout>
#include <QSpacerItem>
#include "mainPanel/deviceWidgets/ASR9010/asr9010widget.h"
#include "mainPanel/deviceWidgets/ASR1013/asr1013widget.h"
#include "mainPanel/deviceWidgets/ASR1002/asr1002widget.h"
#include "mainPanel/deviceWidgets/C7613/c7613widget.h"
#include "mainPanel/deviceWidgets/CX200/cx200widget.h"
#include "mainPanel/deviceWidgets/ME60X8/me60x8widget.h"
#include "mainPanel/deviceWidgets/ME60X16/me60x16widget.h"
#include "mainPanel/deviceWidgets/NE40X8/ne40x8widget.h"
#include "mainPanel/deviceWidgets/NE40X16/ne40x16widget.h"
#include "mainPanel/deviceWidgets/NE5000X16/ne5000X16widget.h"
#include "mainPanel/deviceWidgets/NE5000X16A/ne5000X16Awidget.h"
#include "mainPanel/deviceWidgets/S2403H/s2403hwidget.h"
#include "mainPanel/deviceWidgets/S5300/s5300widget.h"
#include "mainPanel/deviceWidgets/S9303/s9303widget.h"
#include "mainPanel/deviceWidgets/S9306/s9306widget.h"

LocalDeviceDialog::LocalDeviceDialog(QWidget *parent, DanetDbMan *db, const int deviceId, QString Abbr, QString Saloon, QString Device, const int DevexId) :
    QDialog(parent),
    ui(new Ui::LocalDeviceDialog),
    dbMan(db),
    vlanModel(new QSqlQueryModel(this)),
    etherModel(new QSqlQueryModel(this)),
    vpnModel(new QSqlQueryModel(this)),
    slotModel(new QSqlQueryModel(this)),
    devexId(DevexId),
    expModel(new QStringListModel(this)),
    impModel(new QStringListModel(this))
{
    ui->setupUi(this);
    this->resize(parent->width()-50, parent->height()-50);
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowMinMaxButtonsHint);


    QString deviceName = dbMan->getDevexDeviceName(devexId);
    deviceName = deviceName.remove(" ");
    device = Device+" <"+deviceName+" >";
    abbr = Abbr;
    saloon = Saloon;

    ui->exchLbl->setText(Abbr);
    ui->saloonLbl->setText(Saloon);
    ui->deviceLbl->setText(device);

    QSqlQuery *query = dbMan->selectDevice(deviceId);
//`id`, `device`, `sr`, `type`, `vendor`,  `slot_count`, `start_slot`
    if(query->next())
    {
        ui->vendorLbl->setText(query->value(4).toString());
        ui->slotCountLbl->setText(query->value(5).toString());
        ui->slotStartLbl->setText(query->value(6).toString());
    }

    ui->vlanTV->verticalHeader()->hide();
    ui->vlanTV->horizontalHeader()->setStretchLastSection(true);
    ui->vlanTV->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->vlanTV->setModel(vlanModel);

    ui->etherTV->verticalHeader()->hide();
    ui->etherTV->horizontalHeader()->setStretchLastSection(true);
    ui->etherTV->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->etherTV->setModel(etherModel);


    ui->vpnTV->verticalHeader()->hide();
    ui->vpnTV->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->vpnTV->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->vpnTV->setModel(vpnModel);

    ui->expLV->setModel(expModel);
    ui->impLV->setModel(impModel);

    if(Device.contains("ASR9010", Qt::CaseInsensitive))
        drawASR9010();
    else if(Device.contains("ASR1013", Qt::CaseInsensitive))
        drawASR1013();
    else if(Device.contains("ASR1002", Qt::CaseInsensitive))
        drawASR1002();
    else if(Device.contains("7613", Qt::CaseInsensitive))
        drawC7613();
    else if(Device.contains("CX200", Qt::CaseInsensitive))
        drawCX200();
    else if(Device.contains("ME60X16", Qt::CaseInsensitive))
        drawME60X16();
    else if(Device.contains("ME60X8", Qt::CaseInsensitive))
        drawME60X8();
    else if(Device.contains("NE40X16", Qt::CaseInsensitive) || Device.contains("NE40EX16", Qt::CaseInsensitive))
        drawNE40X16();
    else if(Device.contains("NE40X8", Qt::CaseInsensitive) || Device.contains("NE40EX8", Qt::CaseInsensitive))
        drawNE40X8();
    else if(Device.contains("CX600X16", Qt::CaseInsensitive))
        drawCX600X16();
    else if(Device.contains("CX600X8", Qt::CaseInsensitive))
        drawCX600X8();
    else if(Device.contains("NE5000X16A", Qt::CaseInsensitive))
        drawNE5000X16A();
    else if(Device.contains("NE5000X16", Qt::CaseInsensitive))
        drawNE5000X16();
    else if(Device.contains("S2403H", Qt::CaseInsensitive))
        drawS2403H();
    else if(Device.contains("S5300", Qt::CaseInsensitive))
        drawS5300();
    else if(Device.contains("S9303", Qt::CaseInsensitive))
        drawS9303();
    else if(Device.contains("S9306", Qt::CaseInsensitive))
        drawS9306();
    else
    {
        ui->tabWidget->removeTab(0);
    }


    fillVlanTV();
    fillEtherTrunkTV();
    fillVpnViews();
}

LocalDeviceDialog::~LocalDeviceDialog()
{
    delete ui;
    delete vlanModel;
    delete etherModel;
    delete vpnModel;
    delete expModel;
    delete impModel;
    vlanModel = NULL;
    etherModel = NULL;
    vpnModel = NULL;
    expModel = NULL;
    impModel = NULL;
}

void LocalDeviceDialog::fillVlanTV()
{
    QSqlQuery *query = dbMan->selectVlans(devexId);
    vlanModel->setQuery(*query);
    ui->vlanTV->hideColumn(0);
    vlanModel->setHeaderData(1, Qt::Horizontal,"VLAN");
    vlanModel->setHeaderData(2, Qt::Horizontal,"Description");

}

void LocalDeviceDialog::fillEtherTrunkTV()
{
    QSqlQuery *query = dbMan->selectEtherTrunks(devexId);
    etherModel->setQuery(*query);
    ui->etherTV->hideColumn(0);
    etherModel->setHeaderData(1, Qt::Horizontal,"Ether Trunk");
    etherModel->setHeaderData(2, Qt::Horizontal,"Description");

}

void LocalDeviceDialog::fillVpnViews()
{
    QSqlQuery *query = dbMan->selectVPN(devexId);
    //`id`, `vpn`, `rd`, `description`
    vpnModel->setQuery(*query);
    ui->vpnTV->hideColumn(0);
    vpnModel->setHeaderData(1, Qt::Horizontal,"VPN");
    vpnModel->setHeaderData(2, Qt::Horizontal,"Route Distinguisher");
    vpnModel->setHeaderData(3, Qt::Horizontal,"Description");

    QModelIndex ind = vpnModel->index(0, 0);
    ui->vpnTV->selectRow(0);
    ui->vpnTV->clicked(ind);
}

void LocalDeviceDialog::drawASR9010()
{
    slotModel->clear();
    qDeleteAll(ui->deviceParentWidget->children());

    QSqlQuery *query = dbMan->selectViewSlotsBrief(devexId);
    //`shelf`, `slot`, `sub_slot`, `card`
    slotModel->setQuery(*query);

    QHBoxLayout *layout = new QHBoxLayout(ui->deviceParentWidget);
    layout->addItem(new QSpacerItem(10,10,QSizePolicy::Expanding,QSizePolicy::Minimum));
    ASR9010Widget *asr = new ASR9010Widget(this, slotModel);
    layout->addWidget(asr);
    layout->addItem(new QSpacerItem(10,10,QSizePolicy::Expanding,QSizePolicy::Minimum));

}

void LocalDeviceDialog::drawASR1013()
{
    slotModel->clear();
    qDeleteAll(ui->deviceParentWidget->children());

    QSqlQuery *query = dbMan->selectViewSlotsBrief(devexId);
    //`shelf`, `slot`, `sub_slot`, `card`
    slotModel->setQuery(*query);

    QHBoxLayout *layout = new QHBoxLayout(ui->deviceParentWidget);
    layout->addItem(new QSpacerItem(10,10,QSizePolicy::Expanding,QSizePolicy::Minimum));
    ASR1013Widget *asr = new ASR1013Widget(this, slotModel);
    layout->addWidget(asr);
    layout->addItem(new QSpacerItem(10,10,QSizePolicy::Expanding,QSizePolicy::Minimum));

}

void LocalDeviceDialog::drawASR1002()
{
    slotModel->clear();
    qDeleteAll(ui->deviceParentWidget->children());

    QSqlQuery *query = dbMan->selectViewSlotsBrief(devexId);
    //`shelf`, `slot`, `sub_slot`, `card`
    slotModel->setQuery(*query);

    QHBoxLayout *layout = new QHBoxLayout(ui->deviceParentWidget);
    layout->addItem(new QSpacerItem(10,10,QSizePolicy::Expanding,QSizePolicy::Minimum));
    ASR1002Widget *asr = new ASR1002Widget(this, slotModel);
    layout->addWidget(asr);
    layout->addItem(new QSpacerItem(10,10,QSizePolicy::Expanding,QSizePolicy::Minimum));

}

void LocalDeviceDialog::drawC7613()
{
    slotModel->clear();
    qDeleteAll(ui->deviceParentWidget->children());

    QSqlQuery *query = dbMan->selectViewSlotsBrief(devexId);
    //`shelf`, `slot`, `sub_slot`, `card`
    slotModel->setQuery(*query);

    QHBoxLayout *layout = new QHBoxLayout(ui->deviceParentWidget);
    layout->addItem(new QSpacerItem(10,10,QSizePolicy::Expanding,QSizePolicy::Minimum));
    C7613Widget *c7613 = new C7613Widget(this,slotModel);
    layout->addWidget(c7613);
    layout->addItem(new QSpacerItem(10,10,QSizePolicy::Expanding,QSizePolicy::Minimum));
}

void LocalDeviceDialog::drawCX200()
{
    slotModel->clear();
    qDeleteAll(ui->deviceParentWidget->children());


    QHBoxLayout *layout = new QHBoxLayout(ui->deviceParentWidget);
    layout->addItem(new QSpacerItem(10,10,QSizePolicy::Expanding,QSizePolicy::Minimum));
    CX200Widget *cx200 = new CX200Widget(this);
    layout->addWidget(cx200);
    layout->addItem(new QSpacerItem(10,10,QSizePolicy::Expanding,QSizePolicy::Minimum));
}

void LocalDeviceDialog::drawME60X16()
{
    slotModel->clear();
    qDeleteAll(ui->deviceParentWidget->children());

    QSqlQuery *query = dbMan->selectViewSlotsBrief(devexId);
    //`shelf`, `slot`, `sub_slot`, `card`
    slotModel->setQuery(*query);

    QHBoxLayout *layout = new QHBoxLayout(ui->deviceParentWidget);
    layout->addItem(new QSpacerItem(10,10,QSizePolicy::Expanding,QSizePolicy::Minimum));
    ME60X16Widget *me60x16 = new ME60X16Widget(this, slotModel);
    layout->addWidget(me60x16);
    layout->addItem(new QSpacerItem(10,10,QSizePolicy::Expanding,QSizePolicy::Minimum));
}

void LocalDeviceDialog::drawME60X8()
{
    slotModel->clear();
    qDeleteAll(ui->deviceParentWidget->children());

    QSqlQuery *query = dbMan->selectViewSlotsBrief(devexId);
    //`shelf`, `slot`, `sub_slot`, `card`
    slotModel->setQuery(*query);

    QHBoxLayout *layout = new QHBoxLayout(ui->deviceParentWidget);
    layout->addItem(new QSpacerItem(10,10,QSizePolicy::Expanding,QSizePolicy::Minimum));
    ME60X8Widget *me60x8 = new ME60X8Widget(this, slotModel);
    layout->addWidget(me60x8);
    layout->addItem(new QSpacerItem(10,10,QSizePolicy::Expanding,QSizePolicy::Minimum));
}

void LocalDeviceDialog::drawNE40X16()
{
    slotModel->clear();
    qDeleteAll(ui->deviceParentWidget->children());

    QSqlQuery *query = dbMan->selectViewSlotsBrief(devexId);
    //`shelf`, `slot`, `sub_slot`, `card`
    slotModel->setQuery(*query);

    QHBoxLayout *layout = new QHBoxLayout(ui->deviceParentWidget);
    layout->addItem(new QSpacerItem(10,10,QSizePolicy::Expanding,QSizePolicy::Minimum));
    NE40X16Widget *ne40x16 = new NE40X16Widget(this,true, slotModel);
    layout->addWidget(ne40x16);
    layout->addItem(new QSpacerItem(10,10,QSizePolicy::Expanding,QSizePolicy::Minimum));
}

void LocalDeviceDialog::drawNE40X8()
{
    slotModel->clear();
    qDeleteAll(ui->deviceParentWidget->children());

    QSqlQuery *query = dbMan->selectViewSlotsBrief(devexId);
    //`shelf`, `slot`, `sub_slot`, `card`
    slotModel->setQuery(*query);

    QHBoxLayout *layout = new QHBoxLayout(ui->deviceParentWidget);
    layout->addItem(new QSpacerItem(10,10,QSizePolicy::Expanding,QSizePolicy::Minimum));
    NE40X8Widget *ne40x8 = new NE40X8Widget(this,true, slotModel);
    layout->addWidget(ne40x8);
    layout->addItem(new QSpacerItem(10,10,QSizePolicy::Expanding,QSizePolicy::Minimum));
}

void LocalDeviceDialog::drawCX600X16()
{
    slotModel->clear();
    qDeleteAll(ui->deviceParentWidget->children());

    QSqlQuery *query = dbMan->selectViewSlotsBrief(devexId);
    //`shelf`, `slot`, `sub_slot`, `card`
    slotModel->setQuery(*query);

    QHBoxLayout *layout = new QHBoxLayout(ui->deviceParentWidget);
    layout->addItem(new QSpacerItem(10,10,QSizePolicy::Expanding,QSizePolicy::Minimum));
    NE40X16Widget *cx600x16 = new NE40X16Widget(this,false, slotModel);
    layout->addWidget(cx600x16);
    layout->addItem(new QSpacerItem(10,10,QSizePolicy::Expanding,QSizePolicy::Minimum));
}

void LocalDeviceDialog::drawCX600X8()
{
    slotModel->clear();
    qDeleteAll(ui->deviceParentWidget->children());

    QSqlQuery *query = dbMan->selectViewSlotsBrief(devexId);
    //`shelf`, `slot`, `sub_slot`, `card`
    slotModel->setQuery(*query);

    QHBoxLayout *layout = new QHBoxLayout(ui->deviceParentWidget);
    layout->addItem(new QSpacerItem(10,10,QSizePolicy::Expanding,QSizePolicy::Minimum));
    NE40X8Widget *cx600x8 = new NE40X8Widget(this,false, slotModel);
    layout->addWidget(cx600x8);
    layout->addItem(new QSpacerItem(10,10,QSizePolicy::Expanding,QSizePolicy::Minimum));
}

void LocalDeviceDialog::drawNE5000X16A()
{
    slotModel->clear();
    qDeleteAll(ui->deviceParentWidget->children());

    QSqlQuery *query = dbMan->selectViewSlotsBrief(devexId);
    //`shelf`, `slot`, `sub_slot`, `card`
    slotModel->setQuery(*query);

    QHBoxLayout *layout = new QHBoxLayout(ui->deviceParentWidget);
    layout->addItem(new QSpacerItem(10,10,QSizePolicy::Expanding,QSizePolicy::Minimum));
    NE5000X16AWidget *ne5000X16AWidget = new NE5000X16AWidget(this,true, slotModel);
    layout->addWidget(ne5000X16AWidget);
    layout->addItem(new QSpacerItem(10,10,QSizePolicy::Expanding,QSizePolicy::Minimum));
}

void LocalDeviceDialog::drawNE5000X16()
{
    slotModel->clear();
    qDeleteAll(ui->deviceParentWidget->children());

    QSqlQuery *query = dbMan->selectViewSlotsBrief(devexId);
    //`shelf`, `slot`, `sub_slot`, `card`
    slotModel->setQuery(*query);

    QHBoxLayout *layout = new QHBoxLayout(ui->deviceParentWidget);
    layout->addItem(new QSpacerItem(10,10,QSizePolicy::Expanding,QSizePolicy::Minimum));
    NE5000X16Widget *ne5000 = new NE5000X16Widget(this, slotModel);
    layout->addWidget(ne5000);
    layout->addItem(new QSpacerItem(10,10,QSizePolicy::Expanding,QSizePolicy::Minimum));
}

void LocalDeviceDialog::drawS2403H()
{
    slotModel->clear();
    qDeleteAll(ui->deviceParentWidget->children());

    QHBoxLayout *layout = new QHBoxLayout(ui->deviceParentWidget);
    layout->addItem(new QSpacerItem(10,10,QSizePolicy::Expanding,QSizePolicy::Minimum));
    S2403HWidget *s2403 = new S2403HWidget(this);
    layout->addWidget(s2403);
    layout->addItem(new QSpacerItem(10,10,QSizePolicy::Expanding,QSizePolicy::Minimum));
}

void LocalDeviceDialog::drawS5300()
{
    slotModel->clear();
    qDeleteAll(ui->deviceParentWidget->children());


    QHBoxLayout *layout = new QHBoxLayout(ui->deviceParentWidget);
    layout->addItem(new QSpacerItem(10,10,QSizePolicy::Expanding,QSizePolicy::Minimum));
    S5300Widget *s5300 = new S5300Widget(this);
    layout->addWidget(s5300);
    layout->addItem(new QSpacerItem(10,10,QSizePolicy::Expanding,QSizePolicy::Minimum));
}

void LocalDeviceDialog::drawS9303()
{
    slotModel->clear();
    qDeleteAll(ui->deviceParentWidget->children());

    QSqlQuery *query = dbMan->selectViewSlotsBrief(devexId);
    //`shelf`, `slot`, `sub_slot`, `card`
    slotModel->setQuery(*query);

    QHBoxLayout *layout = new QHBoxLayout(ui->deviceParentWidget);
    layout->addItem(new QSpacerItem(10,10,QSizePolicy::Expanding,QSizePolicy::Minimum));
    S9303Widget *s9303 = new S9303Widget(this, slotModel);
    layout->addWidget(s9303);
    layout->addItem(new QSpacerItem(10,10,QSizePolicy::Expanding,QSizePolicy::Minimum));
}

void LocalDeviceDialog::drawS9306()
{
    slotModel->clear();
    qDeleteAll(ui->deviceParentWidget->children());

    QSqlQuery *query = dbMan->selectViewSlotsBrief(devexId);
    //`shelf`, `slot`, `sub_slot`, `card`
    slotModel->setQuery(*query);

    QHBoxLayout *layout = new QHBoxLayout(ui->deviceParentWidget);
    layout->addItem(new QSpacerItem(10,10,QSizePolicy::Expanding,QSizePolicy::Minimum));
    S9306Widget *s9306 = new S9306Widget(this, slotModel);
    layout->addWidget(s9306);
    layout->addItem(new QSpacerItem(10,10,QSizePolicy::Expanding,QSizePolicy::Minimum));
}

void LocalDeviceDialog::on_okBtn_clicked()
{
    this->close();
}


void LocalDeviceDialog::on_vpnTV_clicked(const QModelIndex &index)
{
    QModelIndex ind = index.sibling(index.row(), 0); // vpn id index
    int vpnId = vpnModel->data(ind).toInt();
    QStringList exp,imp;

    if(vpnId > 0)
    {
        QSqlQuery *query = dbMan->selectVpnRT(vpnId);
        //`id`,`exp_imp`,`rt`

        bool expImp;
        while(query->next())
        {
            expImp = query->value(1).toBool();
            if(expImp) // imp
            {
                imp.append(query->value(2).toString());
            }
            else //exp
            {
                exp.append(query->value(2).toString());
            }
        }
    }

    expModel->setStringList(exp);
    impModel->setStringList(imp);

}

