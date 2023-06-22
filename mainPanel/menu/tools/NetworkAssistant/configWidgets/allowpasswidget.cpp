#include "allowpasswidget.h"
#include "ui_allowpasswidget.h"
#include <QPlainTextEdit>
#include <QPushButton>

AllowPassWidget::AllowPassWidget(QWidget *parent, const int OS, QPushButton *Btn, QPlainTextEdit *TE) :
    QWidget(parent),
    ui(new Ui::AllowPassWidget),
    btn(Btn),
    te(TE),
    os(OS)
{
    ui->setupUi(this);
    connect(btn,SIGNAL(clicked(bool)),this,SLOT(write()));

    if(os == 0)
    {
        ui->allowPassLbl->setText("Allow-Pass VLAN : ");
        ui->allowPassLE->setPlaceholderText("Allow-Pass VLAN");
        ui->undoVlan1ChB->setText("Undo Allow-Pass VLAN 1");
    }
    else if(os == 1)
    {
        ui->allowPassLbl->setText("Allowed VLAN : ");
        ui->allowPassLE->setPlaceholderText("Allowed VLAN");
        ui->undoVlan1ChB->setText("Remove VLAN 1");
    }
    else if(os == 2)
    {
    }
}

AllowPassWidget::~AllowPassWidget()
{
    delete ui;
}

void AllowPassWidget::on_allowPassLE_textChanged(const QString &arg1)
{
    if(arg1.isEmpty())
        emit setWriteBtnEnableSignal(false);
    else
        emit setWriteBtnEnableSignal(true);
}

void AllowPassWidget::write()
{

    if(os == 0)
    {
        QString allowPassValn = ui->allowPassLE->text();
        bool v1 = ui->undoVlan1ChB->isChecked();

        QString Config = "  port trunk allow-pass vlan "+allowPassValn;
        if(v1)
            Config += "\n  undo port trunk allow-pass vlan 1";

        te->appendPlainText(Config);
    }
    else if(os == 1)
    {
        QString allowPassValn = ui->allowPassLE->text();
        bool v1 = ui->undoVlan1ChB->isChecked();

        QString Config = "  switchport trunk allowed vlan "+allowPassValn;
        if(v1)
            Config += "\n  switchport trunk allowed vlan remove 1";

        te->appendPlainText(Config);
    }
    else if(os == 2)
    {
    }


}
