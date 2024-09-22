#include "hybridportwidget.h"
#include "ui_hybridportwidget.h"
#include <QPlainTextEdit>
#include <QPushButton>

HybridPortWidget::HybridPortWidget(QWidget *parent, const int OS, QPushButton *Btn, QPlainTextEdit *TE ) :
    QWidget(parent),
    ui(new Ui::HybridPortWidget),
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

HybridPortWidget::~HybridPortWidget()
{
    delete ui;
}

void HybridPortWidget::on_taggedLE_textChanged(const QString &arg1)
{
    if( (!arg1.isEmpty()) && (!ui->untaggedLE->text().isEmpty()) )
        emit setWriteBtnEnableSignal(true);
    else
        emit setWriteBtnEnableSignal(false);
}

void HybridPortWidget::on_untaggedLE_textChanged(const QString &arg1)
{
    if((!arg1.isEmpty()) && (!ui->taggedLE->text().isEmpty()) )
        emit setWriteBtnEnableSignal(true);
    else
        emit setWriteBtnEnableSignal(false);
}

void HybridPortWidget::write()
{
    if(os == 0)
    {
        QString tag = ui->taggedLE->text();
        QString untag = ui->untaggedLE->text();

        QString Config = "  port hybrid tagged vlan "+tag+"\n";
        Config += "  port hybrid untagged vlan "+untag;

        te->appendPlainText(Config);
    }
    else if(os == 1)
    {
    }
    else if(os == 2)
    {
    }


}
