#include "defaultvlanwidget.h"
#include "ui_defaultvlanwidget.h"
#include <QPlainTextEdit>
#include <QPushButton>

DefaultVlanWidget::DefaultVlanWidget(QWidget *parent, const int OS, QPushButton *Btn, QPlainTextEdit *TE) :
    QWidget(parent),
    ui(new Ui::DefaultVlanWidget),
    btn(Btn),
    te(TE),
    os(OS)
{
    ui->setupUi(this);

    connect(btn,SIGNAL(clicked(bool)),this,SLOT(write()));

    if(os == 0)
    {
        ui->defaultVlanLbl->setText("Default VLAN: ");
        ui->defaultVlanLE->setPlaceholderText("Default VLAN");
    }
    else if(os == 1)
    {
        ui->defaultVlanLbl->setText("Default VLAN: ");
        ui->defaultVlanLE->setPlaceholderText("Default VLAN");
    }
    else if(os == 2)
    {
    }
}

DefaultVlanWidget::~DefaultVlanWidget()
{
    delete ui;
}

void DefaultVlanWidget::on_defaultVlanLE_textChanged(const QString &arg1)
{
    if(arg1.isEmpty())
        emit setWriteBtnEnableSignal(false);
    else
        emit setWriteBtnEnableSignal(true);
}

void DefaultVlanWidget::write()
{
    if(os == 0)
    {
        QString vlan = ui->defaultVlanLE->text();
        QString Config = "  port default valn "+vlan;

        te->appendPlainText(Config);
    }
    else if(os == 1)
    {
        QString vlan = ui->defaultVlanLE->text();
        QString Config = "  switchport access vlan "+vlan;

        te->appendPlainText(Config);
    }
    else if(os == 2)
    {
    }


}
