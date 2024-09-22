#include "interfacewidget.h"
#include "ui_interfacewidget.h"
#include <QPlainTextEdit>
#include <QPushButton>

InterfaceWidget::InterfaceWidget(QWidget *parent, const int OS, QPushButton *Btn, QPlainTextEdit *TE) :
    QWidget(parent),
    ui(new Ui::InterfaceWidget),
    btn(Btn),
    te(TE),
    os(OS)
{
    ui->setupUi(this);

    connect(btn,SIGNAL(clicked(bool)),this,SLOT(write()));

    if(os == 0)
    {
        ui->undoShutChB->setText("Undo Shutdown");
    }
    else if(os == 1)
    {
        ui->undoShutChB->setText("No Shutdown");
    }
    else if(os == 2)
    {
        ui->undoShutChB->setText("No Shutdown");
    }


    ui->interfaceTypeCB->addItem("Vlanif");
    ui->interfaceTypeCB->addItem("Eth-Trunk");
    ui->interfaceTypeCB->addItem("GigabitEthernet");
    ui->interfaceTypeCB->addItem("XGigabitEthernet");
    ui->interfaceTypeCB->addItem("Serial");
    ui->interfaceTypeCB->addItem("Loopback");
}

InterfaceWidget::~InterfaceWidget()
{
    delete ui;
}

void InterfaceWidget::on_interfaceTypeCB_currentTextChanged(const QString &arg1)
{
    bool interface = ui->interfaceLE->text().isEmpty();
    bool desc = ui->interfaceDescLE->text().isEmpty();

    if( (!arg1.isEmpty()) && (!interface) && (!desc) )
        emit setWriteBtnEnableSignal(true);
    else
        emit setWriteBtnEnableSignal(false);
}

void InterfaceWidget::on_interfaceLE_textChanged(const QString &arg1)
{
    bool interfaceType = ui->interfaceTypeCB->currentText().isEmpty();
    bool desc = ui->interfaceDescLE->text().isEmpty();

    if( (!arg1.isEmpty()) && (!interfaceType) && (!desc) )
        emit setWriteBtnEnableSignal(true);
    else
        emit setWriteBtnEnableSignal(false);
}

void InterfaceWidget::on_interfaceDescLE_textChanged(const QString &arg1)
{
    bool interface = ui->interfaceLE->text().isEmpty();
    bool interfaceType = ui->interfaceTypeCB->currentText().isEmpty();

    if( (!arg1.isEmpty()) && (!interface) && (!interfaceType) )
        emit setWriteBtnEnableSignal(true);
    else
        emit setWriteBtnEnableSignal(false);
}

void InterfaceWidget::write()
{
    if(os == 0)
    {
        QString interfaceType = ui->interfaceTypeCB->currentText();
        QString interface = ui->interfaceLE->text();
        QString desc = ui->interfaceDescLE->text();
        bool undoShut = ui->undoShutChB->isChecked();

        QString Config = "interface "+interfaceType +" "+interface+"\n";
        Config += "  description "+desc+"\n";
        if(undoShut)
            Config += "  undo shutdown";
        else
            Config += "  shutdown";
        te->appendPlainText(Config);
    }
    else if(os == 1)
    {
        QString interfaceType = ui->interfaceTypeCB->currentText();
        QString interface = ui->interfaceLE->text();
        QString desc = ui->interfaceDescLE->text();
        bool undoShut = ui->undoShutChB->isChecked();

        QString Config = "interface "+interfaceType +" "+interface+"\n";
        Config += "  description "+desc+"\n";
        if(undoShut)
            Config += "  no shutdown";
        else
            Config += "  shutdown";
        te->appendPlainText(Config);
    }
    else if(os == 2)
    {
        QString interfaceType = ui->interfaceTypeCB->currentText();
        QString interface = ui->interfaceLE->text();
        QString desc = ui->interfaceDescLE->text();
        bool undoShut = ui->undoShutChB->isChecked();

        QString Config = "interface "+interfaceType +" "+interface+"\n";
        Config += "  description "+desc+"\n";
        if(undoShut)
            Config += "  no shutdown";
        else
            Config += "  shutdown";
        te->appendPlainText(Config);
    }


}

