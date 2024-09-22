#include "mapvlanwidget.h"
#include "ui_mapvlanwidget.h"
#include <QPlainTextEdit>
#include <QPushButton>

MapVlanWidget::MapVlanWidget(QWidget *parent, const int OS, QPushButton *Btn, QPlainTextEdit *TE) :
    QWidget(parent),
    ui(new Ui::MapVlanWidget),
    btn(Btn),
    te(TE),
    os(OS)
{
    ui->setupUi(this);

    connect(btn,SIGNAL(clicked(bool)),this,SLOT(write()));

    if(os == 0)
    {
    }
    else if(os == 1)
    {
    }
    else if(os == 2)
    {
    }
}

MapVlanWidget::~MapVlanWidget()
{
    delete ui;
}

void MapVlanWidget::on_mapSrcVlanLE_textEdited(const QString &arg1)
{

    if(  (!arg1.isEmpty()) && (!ui->mapVlanDstLE->text().isEmpty()) )
        emit setWriteBtnEnableSignal(true);
    else
        emit setWriteBtnEnableSignal(false);
}

void MapVlanWidget::on_mapVlanDstLE_textChanged(const QString &arg1)
{

    if( (!arg1.isEmpty()) && (!ui->mapSrcVlanLE->text().isEmpty()) )
        emit setWriteBtnEnableSignal(true);
    else
        emit setWriteBtnEnableSignal(false);
}

void MapVlanWidget::write()
{
    if(os == 0)
    {
        QString srcVlan = ui->mapSrcVlanLE->text();
        QString dstVlan = ui->mapVlanDstLE->text();

        QString Config = "  qinq vlan-translation enable\n";
        Config += "  port vlan-mapping valn "+srcVlan+" map-vlan "+dstVlan;
        te->appendPlainText(Config);
    }
    else if(os == 1)
    {
        QString srcVlan = ui->mapSrcVlanLE->text();
        QString dstVlan = ui->mapVlanDstLE->text();

        QString Config = "  switchport vlan mapping "+srcVlan+"  "+dstVlan+"\n";
        Config += "  switchport vlan mapping default drop";
        te->appendPlainText(Config);
    }
    else if(os == 2)
    {
    }


}
