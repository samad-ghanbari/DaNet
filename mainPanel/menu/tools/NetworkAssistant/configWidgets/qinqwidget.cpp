#include "qinqwidget.h"
#include "ui_qinqwidget.h"
#include <QPlainTextEdit>
#include <QPushButton>

QinQWidget::QinQWidget(QWidget *parent, const int OS, QPushButton *Btn, QPlainTextEdit *TE) :
    QWidget(parent),
    ui(new Ui::QinQWidget),
    btn(Btn),
    te(TE),
    os(OS)
{
    ui->setupUi(this);

    connect(btn,SIGNAL(clicked(bool)),this,SLOT(write()));

    if(os == 0)
    {
        ui->userVlanLbl->setText("User VLAN: ");
        ui->userVlanLE->setPlaceholderText("User VLAN");
        ui->qinqLbl->setText("QinQ VLAN: ");
        ui->qinqLE->setPlaceholderText("QinQ VLAN");
    }
    else if(os == 1)
    {
        ui->userVlanLbl->setText("Inner VLAN: ");
        ui->userVlanLE->setPlaceholderText("[any] | 100,300-400");
        ui->qinqLbl->setText("Outer VLAN: ");
        ui->qinqLE->setPlaceholderText("Outer");
    }
    else if(os == 2)
    {
        ui->userVlanLbl->setText("Inner VLAN: ");
        ui->userVlanLE->setPlaceholderText("[any] | 100,300-400");
        ui->qinqLbl->setText("Outer VLAN: ");
        ui->qinqLE->setPlaceholderText("Outer");
    }
}

QinQWidget::~QinQWidget()
{
    delete ui;
}

void QinQWidget::on_userVlanLE_textChanged(const QString &arg1)
{
    if( (!arg1.isEmpty()) && (!ui->qinqLE->text().isEmpty()) )
        emit setWriteBtnEnableSignal(true);
    else
        emit setWriteBtnEnableSignal(false);
}

void QinQWidget::on_qinqLE_textChanged(const QString &arg1)
{
    if( (!arg1.isEmpty()) && (!ui->userVlanLE->text().isEmpty()) )
        emit setWriteBtnEnableSignal(true);
    else
        emit setWriteBtnEnableSignal(false);
}

void QinQWidget::write()
{
    if(os == 0)
    {
        QString userVlan = ui->userVlanLE->text();
        QString qinqVlan = ui->qinqLE->text();
        QString Config = "  port vlan-stacking vlan "+userVlan+" stack-vlan "+qinqVlan;
        te->appendPlainText(Config);
    }
    else if(os == 1)
    {
        QString innerVlan = ui->userVlanLE->text();
        QString outerVlan = ui->qinqLE->text();

        QString Config = "  encapsulation dot1q "+outerVlan+" second-dot1q "+innerVlan;
        te->appendPlainText(Config);
    }
    else if(os == 2)
    {
        QString innerVlan = ui->userVlanLE->text();
        QString outerVlan = ui->qinqLE->text();
        QString Config = "  l2transport\n";
        Config += "  encapsulation dot1q "+outerVlan+" second-dot1q "+innerVlan;
        te->appendPlainText(Config);
    }


}
