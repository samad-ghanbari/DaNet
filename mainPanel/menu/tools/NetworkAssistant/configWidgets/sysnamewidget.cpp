#include "sysnamewidget.h"
#include "ui_sysnamewidget.h"
#include <QPlainTextEdit>
#include <QPushButton>

SysnameWidget::SysnameWidget(QWidget *parent,const int OS,QPushButton *Btn, QPlainTextEdit *TE) :
    QWidget(parent),
    ui(new Ui::SysnameWidget),
    btn(Btn),
    te(TE),
    os(OS)
{
    ui->setupUi(this);

    connect(btn,SIGNAL(clicked(bool)),this,SLOT(write()));

    if(os == 0)
    {
        ui->sysnameLbl->setText("Sysname: ");
    }
    else if(os == 1)
    {
        ui->sysnameLbl->setText("Hostname: ");
    }
    else if(os == 2)
    {
        ui->sysnameLbl->setText("Hostname: ");
    }

}

SysnameWidget::~SysnameWidget()
{
    delete ui;
}

void SysnameWidget::on_sysnameLE_textChanged(const QString &arg1)
{
    if(arg1.isEmpty())
        emit setWriteBtnEnableSignal(false);
    else
        emit setWriteBtnEnableSignal(true);
}

void SysnameWidget::write()
{

    if(os == 0)
    {
        QString sysname = ui->sysnameLE->text();
        QString Config = "sysname "+sysname;
        te->appendPlainText(Config);
    }
    else if(os == 1)
    {
        QString hostname = ui->sysnameLE->text();
        QString Config = "hostname "+hostname;
        te->appendPlainText(Config);
    }
    else if(os == 2)
    {
        QString hostname = ui->sysnameLE->text();
        QString Config = "hostname "+hostname;
        te->appendPlainText(Config);
    }
}
