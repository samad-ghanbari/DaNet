#include "ipassignwidget.h"
#include "ui_ipassignwidget.h"
#include <QPlainTextEdit>
#include <QRegExpValidator>
#include <QPushButton>

IpAssignWidget::IpAssignWidget(QWidget *parent, const int OS, QPushButton *Btn, QPlainTextEdit *TE) :
    QWidget(parent),
    ui(new Ui::IpAssignWidget),
    btn(Btn),
    te(TE),
    os(OS),
    ipRange("(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])"),
    ipRegex("^" + ipRange+ "\\." + ipRange+ "\\." + ipRange+"\\." + ipRange + "$"),
    ipValidator(new QRegExpValidator(ipRegex, this))
{
    ui->setupUi(this);

    connect(btn,SIGNAL(clicked(bool)),this,SLOT(write()));
    ui->ipLE->setValidator(ipValidator);
    ui->maskLE->setValidator(ipValidator);

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

IpAssignWidget::~IpAssignWidget()
{
    delete ui;
}

void IpAssignWidget::on_ipLE_textChanged(const QString &arg1)
{
    bool i = ui->ipLE->hasAcceptableInput();
    bool m = ui->maskLE->hasAcceptableInput();

    if( (!arg1.isEmpty()) && (!ui->maskLE->text().isEmpty()) && i && m )
        emit setWriteBtnEnableSignal(true);
    else
        emit setWriteBtnEnableSignal(false);
}

void IpAssignWidget::on_maskLE_textChanged(const QString &arg1)
{
    bool i = ui->ipLE->hasAcceptableInput();
    bool m = ui->maskLE->hasAcceptableInput();

    if( (!arg1.isEmpty()) && (!ui->ipLE->text().isEmpty()) && i && m )
        emit setWriteBtnEnableSignal(true);
    else
        emit setWriteBtnEnableSignal(false);
}

void IpAssignWidget::write()
{
    if(os == 0)
    {
        QString ip = ui->ipLE->text();
        QString mask = ui->maskLE->text();

        QString Config = "  ip address "+ip+"  "+mask;
        te->appendPlainText(Config);
    }
    else if(os == 1)
    {
        QString ip = ui->ipLE->text();
        QString mask = ui->maskLE->text();

        QString Config = "  ip address "+ip+"  "+mask;
        te->appendPlainText(Config);
    }
    else if(os == 2)
    {
        QString ip = ui->ipLE->text();
        QString mask = ui->maskLE->text();

        QString Config = "  ipv4 address "+ip+"  "+mask;
        te->appendPlainText(Config);
    }


}
