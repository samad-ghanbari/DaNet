#include "loadbalacewidget.h"
#include "ui_loadbalacewidget.h"
#include <QPlainTextEdit>
#include <QPushButton>

LoadBalaceWidget::LoadBalaceWidget(QWidget *parent, const int OS, QPushButton *Btn, QPlainTextEdit *TE) :
    QWidget(parent),
    ui(new Ui::LoadBalaceWidget),
    btn(Btn),
    te(TE),
    os(OS)
{
    ui->setupUi(this);

    connect(btn,SIGNAL(clicked(bool)),this,SLOT(write()));

    if(os == 0)
    {
        ui->LoadBLbl->setText("Trunk Load Balance: ");
        ui->loadBCB->addItem("packet-all");
        ui->loadBCB->addItem("dst-ip");
        ui->loadBCB->addItem("dst-mac");
        ui->loadBCB->addItem("src-ip");
        ui->loadBCB->addItem("src-mac");
        ui->loadBCB->addItem("enhanced");
        ui->loadBCB->addItem("src-dst-ip");
        ui->loadBCB->addItem("src-dst-mac");
    }
    else if(os == 1)
    {
        ui->LoadBLbl->setText("Port-Channel Load Balance: ");
        ui->loadBCB->addItem("src-mac");
        ui->loadBCB->addItem("dst-mac");
        ui->loadBCB->addItem("src-dst-mac");
        ui->loadBCB->addItem("src-ip");
        ui->loadBCB->addItem("dst-ip");
        ui->loadBCB->addItem("src-dst-ip");
        ui->loadBCB->addItem("src-port");
        ui->loadBCB->addItem("dst-port");
        ui->loadBCB->addItem("src-dst-port");
    }
    else if(os == 2)
    {
        ui->LoadBLbl->setText("Bundle-Ether Load Balance: ");
    }

}

LoadBalaceWidget::~LoadBalaceWidget()
{
    delete ui;
}

void LoadBalaceWidget::on_loadBCB_currentTextChanged(const QString &arg1)
{
    if(arg1.isEmpty())
        emit setWriteBtnEnableSignal(false);
    else
        emit setWriteBtnEnableSignal(true);
}

void LoadBalaceWidget::write()
{
    if(os == 0)
    {
        QString LB = ui->loadBCB->currentText();
        QString Config = "  load-balance "+LB;
        te->appendPlainText(Config);
    }
    else if(os == 1)
    {
        QString LB = ui->loadBCB->currentText();
        QString Config = "port-channel load-balance "+LB;
        te->appendPlainText(Config);
    }
    else if(os == 2)
    {
    }


}
