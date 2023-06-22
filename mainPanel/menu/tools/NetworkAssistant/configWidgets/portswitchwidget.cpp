#include "portswitchwidget.h"
#include "ui_portswitchwidget.h"
#include <QPlainTextEdit>
#include <QPushButton>

PortSwitchWidget::PortSwitchWidget(QWidget *parent, const int OS, QPushButton *Btn, QPlainTextEdit *TE) :
    QWidget(parent),
    ui(new Ui::PortSwitchWidget),
    btn(Btn),
    te(TE),
    os(OS)
{
    ui->setupUi(this);

    connect(btn,SIGNAL(clicked(bool)),this,SLOT(write()));

    if(os == 0)
    {
        ui->portSwChB->setText("PortSwitch");
    }
    else if(os == 1)
    {
        ui->portSwChB->setText("SwitchPort");
    }
    else if(os == 2)
    {
    }
}

PortSwitchWidget::~PortSwitchWidget()
{
    delete ui;
}

void PortSwitchWidget::write()
{
    if(os == 0)
    {
        bool ps = ui->portSwChB->isChecked();
        QString Config;
        if(ps)
        {
            Config = "  portswitch";
        }
        else
        {
            Config = "  undo portswitch";
        }
        te->appendPlainText(Config);
    }
    else if(os == 1)
    {
        bool ps = ui->portSwChB->isChecked();
        QString Config;
        if(ps)
        {
            Config = "  switchport";
        }
        else
        {
            Config = "  no switchport";
        }
        te->appendPlainText(Config);
    }
    else if(os == 2)
    {
    }


}
