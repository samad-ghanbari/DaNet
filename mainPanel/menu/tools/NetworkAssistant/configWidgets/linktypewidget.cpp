#include "linktypewidget.h"
#include "ui_linktypewidget.h"
#include <QPlainTextEdit>
#include <QPushButton>

LinkTypeWidget::LinkTypeWidget(QWidget *parent, const int OS, QPushButton *Btn, QPlainTextEdit *TE) :
    QWidget(parent),
    ui(new Ui::LinkTypeWidget),
    btn(Btn),
    te(TE),
    os(OS)
{
    ui->setupUi(this);

    connect(btn,SIGNAL(clicked(bool)),this,SLOT(write()));
    ui->linkTypeCB->addItem("Access");
    ui->linkTypeCB->addItem("Trunk");
    ui->linkTypeCB->addItem("Hybrid");

    if(os == 0)
    {
        ui->linkTypeLbl->setText("Port Link-Type: ");
        ui->linkTypeCB->addItem("Access");
        ui->linkTypeCB->addItem("Trunk");
    }
    else if(os == 1)
    {
        ui->linkTypeLbl->setText("SwitchPort Mode: ");
        ui->linkTypeCB->addItem("Access");
        ui->linkTypeCB->addItem("Trunk");
    }
    else if(os == 2)
    {
    }

}

LinkTypeWidget::~LinkTypeWidget()
{
    delete ui;
}

void LinkTypeWidget::write()
{
    if(os == 0)
    {
        QString linkType = ui->linkTypeCB->currentText();
        QString Config = "  port link-type "+linkType;
        te->appendPlainText(Config);
    }
    else if(os == 1)
    {
        QString linkType = ui->linkTypeCB->currentText();
        QString Config;
        if(linkType.compare("Trunk", Qt::CaseInsensitive) == 0)
            Config = "  switchport trunk encapsulation dot1q \n";
        Config += "  switchport mode "+linkType;
        te->appendPlainText(Config);
    }
    else if(os == 2)
    {
    }


}

void LinkTypeWidget::on_linkTypeCB_currentTextChanged(const QString &arg1)
{
    if( arg1.isEmpty() )
        emit setWriteBtnEnableSignal(false);
    else
        emit setWriteBtnEnableSignal(true);
}
