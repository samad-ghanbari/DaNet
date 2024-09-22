#include "dot1qtagwidget.h"
#include "ui_dot1qtagwidget.h"
#include <QPlainTextEdit>
#include <QPushButton>

Dot1QTagWidget::Dot1QTagWidget(QWidget *parent, const int OS, QPushButton *Btn, QPlainTextEdit *TE) :
    QWidget(parent),
    ui(new Ui::Dot1QTagWidget),
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

Dot1QTagWidget::~Dot1QTagWidget()
{
    delete ui;
}

void Dot1QTagWidget::on_dot1qLE_textChanged(const QString &arg1)
{
    if(arg1.isEmpty())
        emit setWriteBtnEnableSignal(false);
    else
        emit setWriteBtnEnableSignal(true);
}

void Dot1QTagWidget::write()
{
    if(os == 0)
    {
        QString dot1q = ui->dot1qLE->text();

        QString Config = "  vlan-type dot1q "+dot1q;
        te->appendPlainText(Config);
    }
    else if(os == 1)
    {
        QString dot1q = ui->dot1qLE->text();

        QString Config = "  encapsulation dot1q "+dot1q;
        te->appendPlainText(Config);
    }
    else if(os == 2)
    {
        QString dot1q = ui->dot1qLE->text();
        QString Config = "  l2transport\n";
        Config += "  encapsulation dot1q "+dot1q;
        te->appendPlainText(Config);
    }


}
