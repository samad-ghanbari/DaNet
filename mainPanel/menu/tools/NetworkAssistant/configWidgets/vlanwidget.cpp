#include "vlanwidget.h"
#include "ui_vlanwidget.h"
#include <QPlainTextEdit>
#include <QPushButton>

VlanWidget::VlanWidget(QWidget *parent, const int OS, QPushButton *Btn, QPlainTextEdit *TE) :
    QWidget(parent),
    ui(new Ui::VlanWidget),
    btn(Btn),
    te(TE),
    os(OS)
{
    ui->setupUi(this);

    connect(btn,SIGNAL(clicked(bool)),this,SLOT(write()));

    if(os == 0)
    {
        ui->vlanDescLbl->setText("Description: ");
        ui->vlanDescLE->setPlaceholderText("VLAN Description");
    }
    else if(os == 1)
    {
        ui->vlanDescLbl->setText("Name: ");
        ui->vlanDescLE->setPlaceholderText("VLAN Name");
    }
    else if(os == 2)
    {
    }
}

VlanWidget::~VlanWidget()
{
    delete ui;
}

void VlanWidget::on_vlanLE_textChanged(const QString &arg1)
{
    if(arg1.isEmpty() || ui->vlanDescLE->text().isEmpty())
        emit setWriteBtnEnableSignal(false);
    else
        emit setWriteBtnEnableSignal(true);
}

void VlanWidget::on_vlanDescLE_textChanged(const QString &arg1)
{
    if(arg1.isEmpty() || ui->vlanLE->text().isEmpty())
        emit setWriteBtnEnableSignal(false);
    else
        emit setWriteBtnEnableSignal(true);
}

void VlanWidget::write()
{
    if(os == 0)
    {
        QString vlan = ui->vlanLE->text();
        QString desc = ui->vlanDescLE->text();
        QString Config = "vlan "+vlan +"\n"+"  description "+desc;
        te->appendPlainText(Config);
    }
    else if(os == 1)
    {
        QString vlan = ui->vlanLE->text();
        QString desc = ui->vlanDescLE->text();
        QString Config = "vlan "+vlan +"\n"+"  name "+desc;
        te->appendPlainText(Config);
    }
    else if(os == 2)
    {
    }


}
