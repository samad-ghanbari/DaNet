#include "trunkwidget.h"
#include "ui_trunkwidget.h"
#include <QPlainTextEdit>
#include <QPushButton>

TrunkWidget::TrunkWidget(QWidget *parent, const int OS, QPushButton *Btn, QPlainTextEdit *TE) :
    QWidget(parent),
    ui(new Ui::TrunkWidget),
    btn(Btn),
    te(TE),
    os(OS)
{
    ui->setupUi(this);

    connect(btn,SIGNAL(clicked(bool)),this,SLOT(write()));

    if(os == 0)
    {
        ui->intEthLbl->setText("Eth-Trunk : ");
        ui->intEtherLE->setPlaceholderText("Eth-Trunk No");

        ui->trunkProtocolCB->addItem("LACP");
    }
    else if(os == 1)
    {
        ui->intEthLbl->setText("Channel-Group: ");
        ui->intEtherLE->setPlaceholderText("Port-Channel No");

        ui->trunkProtocolCB->addItem("LACP");
        ui->trunkProtocolCB->addItem("PAgP");
    }
    else if(os == 2)
    {
        ui->intEthLbl->setText("Bundle-Ether: ");
        ui->intEtherLE->setPlaceholderText("Bundle-Ether ID");
        ui->trunkProtocolCB->addItem("LACP");
    }
}

TrunkWidget::~TrunkWidget()
{
    delete ui;
}

void TrunkWidget::on_intEtherLE_textChanged(const QString &arg1)
{
    if( (!arg1.isEmpty()) && (!ui->trunkProtocolCB->currentText().isEmpty()) && (!ui->trunkProModeCB->currentText().isEmpty()) )
        emit setWriteBtnEnableSignal(true);
    else
        emit setWriteBtnEnableSignal(false);
}

void TrunkWidget::write()
{
    if(os == 0)
    {
        QString trunk = ui->intEtherLE->text();
        QString mode = ui->trunkProModeCB->currentText();

        QString Config =  "  eth-trunk "+trunk+"\n";
        Config += "  mode lacp-"+mode;
        te->appendPlainText(Config);
    }
    else if(os == 1)
    {
        QString trunk = ui->intEtherLE->text();
        QString mode = ui->trunkProModeCB->currentText();
        QString pro = ui->trunkProtocolCB->currentText();

        QString Config =  "  channel-protocol "+pro+"\n";
        Config += "  channel-group "+trunk+" mode "+mode;
        te->appendPlainText(Config);
    }
    else if(os == 2)
    {
        QString trunk = ui->intEtherLE->text();
        QString mode = ui->trunkProModeCB->currentText();

        QString Config = "  bundle id "+trunk+" mode "+mode;
        te->appendPlainText(Config);
    }


}

void TrunkWidget::on_trunkProtocolCB_currentTextChanged(const QString &arg1)
{
    ui->trunkProModeCB->clear();

    if(os == 0)
    {
        if(arg1.compare("LACP", Qt::CaseInsensitive) == 0)
        {
            ui->trunkProModeCB->addItem("static");
            ui->trunkProModeCB->addItem("dynamic");
        }
    }
    else if((os == 1) || (os == 2) )
    {
        if(arg1.compare("LACP", Qt::CaseInsensitive) == 0)
        {
            ui->trunkProModeCB->addItem("On");
            ui->trunkProModeCB->addItem("Active");
            ui->trunkProModeCB->addItem("Passive");
        }
        else if(arg1.compare("PAgP", Qt::CaseInsensitive) == 0)
        {
            ui->trunkProModeCB->addItem("On");
            ui->trunkProModeCB->addItem("Desirable");
            ui->trunkProModeCB->addItem("Auto");
        }
    }


    if(  (!arg1.isEmpty()) && (!ui->intEtherLE->text().isEmpty()) && (!ui->trunkProModeCB->currentText().isEmpty()) )
        emit setWriteBtnEnableSignal(true);
    else
        emit setWriteBtnEnableSignal(false);
}

void TrunkWidget::on_trunkProModeCB_currentTextChanged(const QString &arg1)
{
    if( (!arg1.isEmpty()) && (!ui->trunkProtocolCB->currentText().isEmpty()) && (!ui->intEtherLE->text().isEmpty()) )
        emit setWriteBtnEnableSignal(true);
    else
        emit setWriteBtnEnableSignal(false);
}
