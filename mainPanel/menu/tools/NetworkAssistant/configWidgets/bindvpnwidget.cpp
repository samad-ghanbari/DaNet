#include "bindvpnwidget.h"
#include "ui_bindvpnwidget.h"
#include <QPlainTextEdit>
#include <QPushButton>

BindVpnWidget::BindVpnWidget(QWidget *parent,const int OS,QPushButton *Btn, QPlainTextEdit *TE) :
    QWidget(parent),
    ui(new Ui::BindVpnWidget),
    btn(Btn),
    te(TE),
    os(OS)
{
    ui->setupUi(this);

    connect(btn,SIGNAL(clicked(bool)),this,SLOT(write()));

    if(os == 0)
    {
        ui->intBindLbl->setText("Bind VPN: ");
        ui->intBindVpnLE->setPlaceholderText("VPN Instance");
    }
    else if(os == 1)
    {
        ui->intBindLbl->setText("Bind VRF: ");
        ui->intBindVpnLE->setPlaceholderText("VRF");
    }
    else if(os == 2)
    {
        ui->intBindLbl->setText("Bind VRF: ");
        ui->intBindVpnLE->setPlaceholderText("VRF");
    }
}

BindVpnWidget::~BindVpnWidget()
{
    delete ui;
}

void BindVpnWidget::on_intBindVpnLE_textChanged(const QString &arg1)
{
    if (arg1.isEmpty())
        emit setWriteBtnEnableSignal(false);
    else
        emit setWriteBtnEnableSignal(true);
}

void BindVpnWidget::write()
{
    if(os == 0)
    {
        QString vpn = ui->intBindVpnLE->text();

        QString Config = "  ip binding vpn-instance "+vpn;
        te->appendPlainText(Config);
    }
    else if(os == 1)
    {
        QString vpn = ui->intBindVpnLE->text();

        QString Config = "  ip vrf forwarding "+vpn;
        te->appendPlainText(Config);
    }
    else if(os == 2)
    {
        QString vpn = ui->intBindVpnLE->text();

        QString Config = "  vrf forwarding "+vpn;
        te->appendPlainText(Config);
    }


}
