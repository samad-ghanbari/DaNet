#include "vpninstancewidget.h"
#include "ui_vpninstancewidget.h"
#include <QPlainTextEdit>
#include <QPushButton>

VpnInstanceWidget::VpnInstanceWidget(QWidget *parent,const int OS,QPushButton *Btn, QPlainTextEdit *TE) :
    QWidget(parent),
    ui(new Ui::VpnInstanceWidget),
    btn(Btn),
    te(TE),
    os(OS)
{
    ui->setupUi(this);

    connect(btn,SIGNAL(clicked(bool)),this,SLOT(write()));

    if(os == 0)
    {
        ui->vpnLbl->setText("VPN Instance: ");
        ui->vpnLE->setPlaceholderText("VPN Instance");
        ui->vpnDescLbl->setText("VPN Description: ");
        ui->vpnDescLE->setPlaceholderText("VPN Description");
        ui->rdLbl->setText("Route Distinguisher: ");
        ui->rdLE->setPlaceholderText("VPN RD");
        ui->eRtLbl->setText("Export VPN Target: ");
        ui->eRtLE->setPlaceholderText("Export RT");
        ui->IrtLbl->setText("Import VPN Target: ");
        ui->iRtLE->setPlaceholderText("Import RT");
    }
    else if(os == 1)
    {
        ui->vpnLbl->setText("VRF: ");
        ui->vpnLE->setPlaceholderText("VRF");
        ui->vpnDescLbl->setText("VRF Description: ");
        ui->vpnDescLE->setPlaceholderText("VRF Description");
        ui->rdLbl->setText("Route Distinguisher: ");
        ui->rdLE->setPlaceholderText("VRF RD");
        ui->eRtLbl->setText("Export Route-Target: ");
        ui->eRtLE->setPlaceholderText("Export RT");
        ui->IrtLbl->setText("Import Route-Target: ");
        ui->iRtLE->setPlaceholderText("Import RT");
    }
    else if(os == 2)
    {
        ui->vpnLbl->setText("VRF: ");
        ui->vpnLE->setPlaceholderText("VRF");
        ui->vpnDescLbl->setText("VRF Description: ");
        ui->vpnDescLE->setPlaceholderText("VRF Description");
        ui->rdLbl->setText("Route Distinguisher: ");
        ui->rdLE->setPlaceholderText("VRF RD");
        ui->eRtLbl->setText("Export Route-Target: ");
        ui->eRtLE->setPlaceholderText("Export RT");
        ui->IrtLbl->setText("Import Route-Target: ");
        ui->iRtLE->setPlaceholderText("Import RT");
    }
}

VpnInstanceWidget::~VpnInstanceWidget()
{
    delete ui;
}

void VpnInstanceWidget::on_vpnLE_textChanged(const QString &arg1)
{
    bool desc = ui->vpnDescLE->text().isEmpty();
    bool rd = ui->rdLE->text().isEmpty();
    bool ert = ui->eRtLE->text().isEmpty();
    bool irt = ui->iRtLE->text().isEmpty();

    if( (!arg1.isEmpty()) && (!rd) && (!ert) && (!irt) && (!desc) )
        emit setWriteBtnEnableSignal(true);
    else
        emit setWriteBtnEnableSignal(false);
}

void VpnInstanceWidget::on_vpnDescLE_textChanged(const QString &arg1)
{
    bool vpn = ui->vpnLE->text().isEmpty();
    bool rd = ui->rdLE->text().isEmpty();
    bool ert = ui->eRtLE->text().isEmpty();
    bool irt = ui->iRtLE->text().isEmpty();

    if( (!arg1.isEmpty()) && (!rd) && (!ert) && (!irt) && (!vpn) )
        emit setWriteBtnEnableSignal(true);
    else
        emit setWriteBtnEnableSignal(false);
}

void VpnInstanceWidget::on_rdLE_textChanged(const QString &arg1)
{
    bool desc = ui->vpnDescLE->text().isEmpty();
    bool vpn = ui->vpnLE->text().isEmpty();
    bool ert = ui->eRtLE->text().isEmpty();
    bool irt = ui->iRtLE->text().isEmpty();

    if( (!arg1.isEmpty()) && (!vpn) && (!ert) && (!irt) && (!desc) )
        emit setWriteBtnEnableSignal(true);
    else
        emit setWriteBtnEnableSignal(false);
}

void VpnInstanceWidget::on_eRtLE_textChanged(const QString &arg1)
{
    bool desc = ui->vpnDescLE->text().isEmpty();
    bool rd = ui->rdLE->text().isEmpty();
    bool vpn = ui->vpnLE->text().isEmpty();
    bool irt = ui->iRtLE->text().isEmpty();

    if( (!arg1.isEmpty()) && (!rd) && (!vpn) && (!irt) && (!desc) )
        emit setWriteBtnEnableSignal(true);
    else
        emit setWriteBtnEnableSignal(false);
}

void VpnInstanceWidget::on_iRtLE_textEdited(const QString &arg1)
{
    bool desc = ui->vpnDescLE->text().isEmpty();
    bool rd = ui->rdLE->text().isEmpty();
    bool ert = ui->eRtLE->text().isEmpty();
    bool vpn = ui->vpnLE->text().isEmpty();

    if( (!arg1.isEmpty()) && (!rd) && (!ert) && (!vpn) && (!desc) )
        emit setWriteBtnEnableSignal(true);
    else
        emit setWriteBtnEnableSignal(false);
}

void VpnInstanceWidget::write()
{
    if(os == 0)
    {
        QString vpn = ui->vpnLE->text();
        QString desc = ui->vpnDescLE->text();
        QString rd = ui->rdLE->text();
        QString ert = ui->eRtLE->text();
        QString irt = ui->iRtLE->text();

        QString Config = "ip vpn-instance "+vpn;
        Config += "\n  description "+desc;
        Config += "\n  route-distinguisher "+rd;
        Config += "\n  vpn-target "+ert+" export-extcommunity";
        Config += "\n  vpn-target "+irt+" import-extcommunity";

        te->appendPlainText(Config);
    }
    else if(os == 1)
    {
        QString vpn = ui->vpnLE->text();
        QString desc = ui->vpnDescLE->text();
        QString rd = ui->rdLE->text();
        QString ert = ui->eRtLE->text();
        QString irt = ui->iRtLE->text();

        QString Config = "ip vrf "+vpn;
        Config += "\n  description "+desc;
        Config += "\n  rd "+rd;
        Config += "\n  route-target export "+ert;
        Config += "\n  route-target import "+irt;

        te->appendPlainText(Config);
    }
    else if(os == 2)
    {
        QString vpn = ui->vpnLE->text();
        QString desc = ui->vpnDescLE->text();
        QString rd = ui->rdLE->text();
        QString ert = ui->eRtLE->text();
        QString irt = ui->iRtLE->text();

        QString Config = "vrf definition "+vpn;
        Config += "\n  description "+desc;
        Config += "\n  rd "+rd;
        Config += "\n  address family ipv4";
        Config += "\n  route-target export "+ert;
        Config += "\n  route-target import "+irt;
        Config += "\n  exit-address-family";

        te->appendPlainText(Config);
    }


}
