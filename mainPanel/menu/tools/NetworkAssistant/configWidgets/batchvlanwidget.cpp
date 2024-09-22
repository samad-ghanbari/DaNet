#include "batchvlanwidget.h"
#include "ui_batchvlanwidget.h"
#include <QPlainTextEdit>
#include <QPushButton>

BatchVlanWidget::BatchVlanWidget(QWidget *parent, const int OS, QPushButton *Btn, QPlainTextEdit *TE) :
    QWidget(parent),
    ui(new Ui::BatchVlanWidget),
    btn(Btn),
    te(TE),
    os(OS)
{
    ui->setupUi(this);

    connect(btn,SIGNAL(clicked(bool)),this,SLOT(write()));

    if(os == 0)
    {
        ui->vlanBatchLbl->setText("VLAN Batch: ");
        ui->vlanBatchLE->setPlaceholderText("Like: 10 11 100 TO 200");
    }
    else if(os == 1)
    {
        ui->vlanBatchLbl->setText("VLAN: ");
        ui->vlanBatchLE->setPlaceholderText("Like: 10, 11, 100-200");
    }
    else if(os == 2)
    {
    }
}

BatchVlanWidget::~BatchVlanWidget()
{
    delete ui;
}

void BatchVlanWidget::on_vlanBatchLE_textChanged(const QString &arg1)
{
    if(arg1.isEmpty())
        emit setWriteBtnEnableSignal(false);
    else
        emit setWriteBtnEnableSignal(true);
}

void BatchVlanWidget::write()
{
    if(os == 0)
    {
        QString batchVlan = ui->vlanBatchLE->text();
        QString Config = "vlan batch "+batchVlan;
        te->appendPlainText(Config);
    }
    else if(os == 1)
    {
        QString batchVlan = ui->vlanBatchLE->text();
        QString Config = "vlan "+batchVlan;
        te->appendPlainText(Config);
    }
    else if(os == 2)
    {
    }


}
