#include "emptyinterfacedialog.h"
#include "ui_emptyinterfacedialog.h"
#include "lib/database/danetdbman.h"
#include <QMessageBox>

EmptyInterfaceDialog::EmptyInterfaceDialog(QWidget *parent, DanetDbMan *db, const int IntId, const int devexId, const QString Device, const QString devInt, const QString Label, const QString module, const QString Eth, const QString PeerExch, const QString PeerDevice, const QString PeerInt, const QString PeerLabel, const QString desc) :
    QDialog(parent),
    ui(new Ui::EmptyInterfaceDialog),
    dbMan(db),
    intId(IntId),
    eth(Eth.toInt())
{
    ui->setupUi(this);
    devName = Device;

    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowMinMaxButtonsHint);

    ui->cb->setChecked(false);
    ui->okBtn->setEnabled(false);

    ui->intLbl->setText(devInt);
    ui->labelLbl->setText(Label);
    ui->moduleLbl->setText(module);
    ui->etherLbl->setText(Eth);
    ui->descTE->setText(desc);
    ui->peerExchLbl->setText(PeerExch);
    ui->peerDevLbl->setText(PeerDevice);
    ui->peerIntLbl->setText(PeerInt);
    ui->peerLbl->setText(PeerLabel);

    if(dbMan->isDslamPlannedInterface(intId, devexId))//(clickedPeerDevice.compare("DSLAM MA5600", Qt::CaseInsensitive) == 0) || (clickedPeerDevice.compare("DSLAM MA5616", Qt::CaseInsensitive) == 0) || (clickedPeerDevice.compare("DSLAM MXK819", Qt::CaseInsensitive) == 0)
    {
        ui->cb->setEnabled(false);
        ui->cb->setChecked(false);
        ui->okBtn->setEnabled(false);
    }

}

EmptyInterfaceDialog::~EmptyInterfaceDialog()
{
    delete ui;
}

void EmptyInterfaceDialog::on_cancelBtn_clicked()
{
    this->close();
}

void EmptyInterfaceDialog::on_okBtn_clicked()
{

    if(dbMan->depleteInterface(intId))
    {
        //log
        QString localDevice = devName;
        QString Interface = ui->intLbl->text();
        int exchId = dbMan->getInterfaceExchangeId(intId);
        QString username = dbMan->getLoggedinNameLastname();
        dbMan->insertLog(exchId, username, "Deplete Interface of "+localDevice+" "+Interface);

        this->close();
    }
    else
        QMessageBox::warning(this,"Error", "Cannot deplete port.\n"+dbMan->lastError());
}

void EmptyInterfaceDialog::on_cb_toggled(bool checked)
{
    if(checked)
        ui->okBtn->setEnabled(true);
    else
        ui->okBtn->setEnabled(false);
}
