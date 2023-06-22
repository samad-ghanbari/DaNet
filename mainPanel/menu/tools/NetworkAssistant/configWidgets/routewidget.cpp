#include "routewidget.h"
#include "ui_routewidget.h"
#include <QPlainTextEdit>
#include <QRegExpValidator>
#include <QPushButton>


RouteWidget::RouteWidget(QWidget *parent, const int OS, const int RouteFlag , QPushButton *Btn, QPlainTextEdit *TE ) :
    QWidget(parent),
    ui(new Ui::RouteWidget),
    btn(Btn),
    te(TE),
    os(OS),
    d12(RouteFlag),
    ipRange("(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])"),
    ipRegex("^" + ipRange+ "\\." + ipRange+ "\\." + ipRange+"\\." + ipRange + "$"),
    ipValidator(new QRegExpValidator(ipRegex, this))
{
    ui->setupUi(this);

    connect(btn,SIGNAL(clicked(bool)),this,SLOT(write()));

    ui->defaultNextHopLE->setValidator(ipValidator);
    ui->static1IpLE->setValidator(ipValidator);
    ui->static1MaskLE->setValidator(ipValidator);
    ui->static1NextHopLE->setValidator(ipValidator);
    ui->static2IpLE->setValidator(ipValidator);
    ui->static2MaskLE->setValidator(ipValidator);
    ui->static2NextHopLE->setValidator(ipValidator);

    ui->defaultRouteFrame->hide();
    ui->static1Frame->hide();
    ui->static2Frame->hide();

    if(os == 0)
    {
        ui->defaultNextHopLbl->setText("Next Hop: ");
        ui->defaultPreferenceLbl->setText("Preference: ");
        ui->defaultPreferenceLE->setPlaceholderText("Preference");
        ui->static1NextHopLbl->setText("Next Hop: ");
        ui->static1NextHopLE->setPlaceholderText("Next Hop");
        ui->static1PreferenceLbl->setText("Preference: ");
        ui->static1PreferenceLE->setPlaceholderText("Preference");
        ui->static2VpnLbl->setText("VPN Instance: ");
        ui->static2VpnLE->setPlaceholderText("VPN Instance");
        ui->static2NextHopLbl->setText("Next Hop: ");
        ui->static2PreferenceLbl->setText("Preference: ");
        ui->static2PreferenceLE->setPlaceholderText("Preference");
    }
    else if(os == 1)
    {
        ui->defaultNextHopLbl->setText("Default Route: ");
        ui->defaultPreferenceLbl->setText("Preference: ");
        ui->defaultPreferenceLE->setPlaceholderText("Preference");
        ui->static1NextHopLbl->setText("Next Hop: ");
        ui->static1NextHopLE->setPlaceholderText("Next Hop");
        ui->static1PreferenceLbl->setText("Preference: ");
        ui->static1PreferenceLE->setPlaceholderText("Preference");
        ui->static2VpnLbl->setText("VRF: ");
        ui->static2VpnLE->setPlaceholderText("VRF");
        ui->static2NextHopLbl->setText("Next Hop: ");
        ui->static2PreferenceLbl->setText("Preference: ");
        ui->static2PreferenceLE->setPlaceholderText("Preference");
    }
    else if(os == 2)
    {
        ui->defaultNextHopLbl->setText("Default Route: ");
        ui->defaultPreferenceLbl->setText("Preference: ");
        ui->defaultPreferenceLE->setPlaceholderText("Preference");
        ui->static1NextHopLbl->setText("Next Hop: ");
        ui->static1NextHopLE->setPlaceholderText("Next Hop");
        ui->static1PreferenceLbl->setText("Preference: ");
        ui->static1PreferenceLE->setPlaceholderText("Preference");
        ui->static2VpnLbl->setText("VRF: ");
        ui->static2VpnLE->setPlaceholderText("VRF");
        ui->static2NextHopLbl->setText("Next Hop: ");
        ui->static2PreferenceLbl->setText("Preference: ");
        ui->static2PreferenceLE->setPlaceholderText("Preference");
    }


    if(d12 == 0)
    {
        ui->defaultRouteFrame->show();
        ui->static1Frame->hide();
        ui->static2Frame->hide();

        this->setMinimumHeight(100);
        this->setMaximumHeight(100);
    }
    else if(d12 == 1)
    {
        ui->defaultRouteFrame->hide();
        ui->static1Frame->show();
        ui->static2Frame->hide();

        this->setMinimumHeight(150);
        this->setMaximumHeight(150);
    }
    else if(d12 == 2)
    {
        ui->defaultRouteFrame->hide();
        ui->static1Frame->hide();
        ui->static2Frame->show();

        this->setMinimumHeight(150);
        this->setMaximumHeight(150);
    }
}

RouteWidget::~RouteWidget()
{
    delete ui;
}

void RouteWidget::write()
{
    if(os == 0)
    {
        if(d12 == 0)
        {// default route
            QString nextHop = ui->defaultNextHopLE->text();
            QString pre = ui->defaultPreferenceLE->text();

            QString Config = "ip route-static 0.0.0.0  0.0.0.0  "+nextHop;
            if(!pre.trimmed().isEmpty())
                Config += "  "+pre;

            te->appendPlainText(Config);
        }
        else if(d12 == 1)
        {// static
            QString net = ui->static1IpLE->text();
            QString mask = ui->static1MaskLE->text();

            QString nextHop = ui->static1NextHopLE->text();
            QString pre = ui->static1PreferenceLE->text();

            QString Config = "ip route-static "+net+" "+mask+" "+nextHop;
            if(!pre.trimmed().isEmpty())
                Config += "  "+pre;

            te->appendPlainText(Config);
        }
        else if(d12 == 2)
        {// vpn static
            QString net = ui->static2IpLE->text();
            QString mask = ui->static2MaskLE->text();
            QString vpn = ui->static2VpnLE->text();
            QString nextHop = ui->static2NextHopLE->text();
            QString pre = ui->static2PreferenceLE->text();

            QString Config = "ip route-static vpn-instance  "+vpn+" "+net+" "+mask+" "+nextHop;
            if(!pre.trimmed().isEmpty())
                Config += "  "+pre;

            te->appendPlainText(Config);
        }
    }
    else if(os == 1)
    {
        if(d12 == 0)
        {// default route
            QString nextHop = ui->defaultNextHopLE->text();
            QString pre = ui->defaultPreferenceLE->text();

            QString Config = "ip route 0.0.0.0  0.0.0.0  "+nextHop;
            if(!pre.trimmed().isEmpty())
                Config += "  "+pre;

            te->appendPlainText(Config);
        }
        else if(d12 == 1)
        {// static
            QString net = ui->static1IpLE->text();
            QString mask = ui->static1MaskLE->text();

            QString nextHop = ui->static1NextHopLE->text();
            QString pre = ui->static1PreferenceLE->text();

            QString Config = "ip route "+net+" "+mask+" "+nextHop;
            if(!pre.trimmed().isEmpty())
                Config += "  "+pre;

            te->appendPlainText(Config);
        }
        else if(d12 == 2)
        {// vpn static
            QString net = ui->static2IpLE->text();
            QString mask = ui->static2MaskLE->text();
            QString vpn = ui->static2VpnLE->text();
            QString nextHop = ui->static2NextHopLE->text();
            QString pre = ui->static2PreferenceLE->text();

            QString Config = "ip route vrf  "+vpn+" "+net+" "+mask+" "+nextHop;
            if(!pre.trimmed().isEmpty())
                Config += "  "+pre;

            te->appendPlainText(Config);
        }
    }
    else if(os == 2)
    {
        if(d12 == 0)
        {// default route
            QString nextHop = ui->defaultNextHopLE->text();
            QString pre = ui->defaultPreferenceLE->text();

            QString Config = "router static";
            Config += "\n  address-family ipv4 unicast ";
            Config += "\n  0.0.0.0  0.0.0.0  "+nextHop;
            if(!pre.trimmed().isEmpty())
                Config += "  "+pre;

            te->appendPlainText(Config);
        }
        else if(d12 == 1)
        {// static
            QString net = ui->static1IpLE->text();
            QString mask = ui->static1MaskLE->text();

            QString nextHop = ui->static1NextHopLE->text();
            QString pre = ui->static1PreferenceLE->text();

            QString Config = "router static";
            Config += "\n  address-family ipv4 unicast";
            Config += "\n  " + net +"  "+mask+" "+ nextHop;
            if(!pre.trimmed().isEmpty())
                Config += "  "+pre;

            te->appendPlainText(Config);
        }
        else if(d12 == 2)
        {// vpn static
            QString net = ui->static2IpLE->text();
            QString mask = ui->static2MaskLE->text();
            QString vpn = ui->static2VpnLE->text();
            QString nextHop = ui->static2NextHopLE->text();
            QString pre = ui->static2PreferenceLE->text();

            QString Config = "router static";
            Config += "\n  vrf "+vpn;
            Config += "\n  address-family ipv4 unicast";
            Config += "\n  "+net +"  "+mask+" "+ nextHop;
            if(!pre.trimmed().isEmpty())
                Config += "  "+pre;

            te->appendPlainText(Config);
        }
    }



}
//
void RouteWidget::on_defaultNextHopLE_textChanged(const QString &arg1)
{
    bool i = ui->defaultNextHopLE->hasAcceptableInput();

    if( (!arg1.isEmpty()) && i && (d12 == 0))
        emit setWriteBtnEnableSignal(true);
    else
        emit setWriteBtnEnableSignal(false);
}
//

void RouteWidget::on_static1IpLE_textChanged(const QString &arg1)
{
    bool mask = ui->static1MaskLE->text().isEmpty();
    bool hop = ui->static1NextHopLE->text().isEmpty();

    bool i = ui->static1IpLE->hasAcceptableInput();
    bool m = ui->static1MaskLE->hasAcceptableInput();
    bool h = ui->static1NextHopLE->hasAcceptableInput();

    if( (!arg1.isEmpty()) && (!mask) & (!hop) && i && m && h && (d12 == 1) )
        emit setWriteBtnEnableSignal(true);
    else
        emit setWriteBtnEnableSignal(false);
}

void RouteWidget::on_static1MaskLE_textChanged(const QString &arg1)
{
    bool ip = ui->static1IpLE->text().isEmpty();
    bool hop = ui->static1NextHopLE->text().isEmpty();

    bool i = ui->static1IpLE->hasAcceptableInput();
    bool m = ui->static1MaskLE->hasAcceptableInput();
    bool h = ui->static1NextHopLE->hasAcceptableInput();

    if( (!arg1.isEmpty()) && (!ip) && (!hop) && i && m && h && (d12 == 1))
        emit setWriteBtnEnableSignal(true);
    else
        emit setWriteBtnEnableSignal(false);
}

void RouteWidget::on_static1NextHopLE_textChanged(const QString &arg1)
{
    bool ip = ui->static1IpLE->text().isEmpty();
    bool mask = ui->static1MaskLE->text().isEmpty();

    bool i = ui->static1IpLE->hasAcceptableInput();
    bool m = ui->static1MaskLE->hasAcceptableInput();
    bool h = ui->static1NextHopLE->hasAcceptableInput();

    if( (!arg1.isEmpty()) && (!mask) && (!ip) && i && m && h && (d12 == 1))
        emit setWriteBtnEnableSignal(true);
    else
        emit setWriteBtnEnableSignal(false);
}
//

void RouteWidget::on_static2IpLE_textChanged(const QString &arg1)
{
    bool mask = ui->static2MaskLE->text().isEmpty();
    bool hop = ui->static2NextHopLE->text().isEmpty();
    bool vpn = ui->static2VpnLE->text().isEmpty();

    bool i = ui->static2IpLE->hasAcceptableInput();
    bool m = ui->static2MaskLE->hasAcceptableInput();
    bool h = ui->static2NextHopLE->hasAcceptableInput();

    if( (!arg1.isEmpty()) && (!mask) && (!hop) && (!vpn) && i && m && h && (d12 == 2) )
        emit setWriteBtnEnableSignal(true);
    else
        emit setWriteBtnEnableSignal(false);
}

void RouteWidget::on_static2MaskLE_textChanged(const QString &arg1)
{
    bool ip = ui->static2IpLE->text().isEmpty();
    bool hop = ui->static2NextHopLE->text().isEmpty();
    bool vpn = ui->static2VpnLE->text().isEmpty();

    bool i = ui->static2IpLE->hasAcceptableInput();
    bool m = ui->static2MaskLE->hasAcceptableInput();
    bool h = ui->static2NextHopLE->hasAcceptableInput();

    if( (!arg1.isEmpty()) && (!ip) && (!hop) && (!vpn) && i && m && h && (d12 == 2) )
        emit setWriteBtnEnableSignal(true);
    else
        emit setWriteBtnEnableSignal(false);
}

void RouteWidget::on_static2VpnLE_textChanged(const QString &arg1)
{
    bool ip = ui->static2IpLE->text().isEmpty();
    bool mask = ui->static2MaskLE->text().isEmpty();
    bool hop = ui->static2NextHopLE->text().isEmpty();

    bool i = ui->static2IpLE->hasAcceptableInput();
    bool m = ui->static2MaskLE->hasAcceptableInput();
    bool h = ui->static2NextHopLE->hasAcceptableInput();

    if( (!arg1.isEmpty()) && (!mask) && (!ip) && (!hop) && i && m && h && (d12 == 2))
        emit setWriteBtnEnableSignal(true);
    else
        emit setWriteBtnEnableSignal(false);
}

void RouteWidget::on_static2NextHopLE_textChanged(const QString &arg1)
{
    bool ip = ui->static2IpLE->text().isEmpty();
    bool mask = ui->static2MaskLE->text().isEmpty();
    bool vpn = ui->static2VpnLE->text().isEmpty();

    if( (!arg1.isEmpty()) && (!mask) && (!ip) && (!vpn) && (d12 == 2))
        emit setWriteBtnEnableSignal(true);
    else
        emit setWriteBtnEnableSignal(false);
}
