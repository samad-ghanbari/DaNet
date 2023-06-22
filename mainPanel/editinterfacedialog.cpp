#include "editinterfacedialog.h"
#include "ui_editinterfacedialog.h"
#include "lib/database/danetdbman.h"
#include <QMessageBox>
#include <QIntValidator>

EditInterfaceDialog::EditInterfaceDialog(QWidget *parent, DanetDbMan *db, const int IntId, const int devexId, const QString Device, const QString Int, const QString Label, const QString module, const QString eth, QString PeerAbbr, const QString PeerDevice, const QString PeerInt, const QString PeerLabel, const QString desc) :
    QDialog(parent),
    ui(new Ui::EditInterfaceDialog),
    query(NULL),
    dbMan(db),
    intId(IntId),
    ethValidator(new QIntValidator(this))
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowMinMaxButtonsHint);


    devName = Device;
    ui->intLbl->setText(Int);
    ui->labelLbl->setText(Label);
    ui->etherLE->setText(eth);
    ui->descTE->setText(desc);

    query = dbMan->selectModules();
    int i;
    QString m;
    while(query->next())
    {
        i = query->value(0).toInt();
        m = query->value(1).toString();

        ui->moduleCB->addItem(m, i);
    }

    ui->moduleCB->setCurrentText(module);


    ui->peerExchLE->setText(PeerAbbr.trimmed());
    ui->peerDevLE->setText(PeerDevice.trimmed());
    ui->peerIntLE->setText(PeerInt.trimmed());
    ui->peerLblLE->setText(PeerLabel.trimmed());
    ui->etherLE->setValidator(ethValidator);

    ui->okBtn->setEnabled(false);

    if(dbMan->isDslamPlannedInterface(intId, devexId))//(clickedPeerDevice.compare("DSLAM MA5600", Qt::CaseInsensitive) == 0) || (clickedPeerDevice.compare("DSLAM MA5616", Qt::CaseInsensitive) == 0) || (clickedPeerDevice.compare("DSLAM MXK819", Qt::CaseInsensitive) == 0)
    {
        ui->GB1->setEnabled(false);
        ui->GB2->setEnabled(false);
    }
}

EditInterfaceDialog::~EditInterfaceDialog()
{
    delete ui;
}

QString EditInterfaceDialog::getLable()
{
    QString dev = "";
    QString ex,d;
    ex = ui->peerExchLE->text().trimmed();
    d = ui->peerDevLE->text().trimmed();

    if(!ui->peerExchLE->text().isEmpty())
        if(!d.contains(ex, Qt::CaseInsensitive))
            dev = ui->peerExchLE->text();

    if(!ui->peerDevLE->text().isEmpty())
    {
        if(dev.isEmpty())
            dev = ui->peerDevLE->text();
        else
            dev += "- "+ui->peerDevLE->text();
    }

    if(!ui->peerIntLE->text().isEmpty())
        dev += "- "+ui->peerIntLE->text();

    return dev;
}

void EditInterfaceDialog::checkEntry()
{
    bool E;
    if(ui->etherLE->text().trimmed().isEmpty())
        E = true;
    else
        E = ui->etherLE->hasAcceptableInput();

    bool e = !ui->peerExchLE->text().isEmpty();
    bool D = !ui->peerDevLE->text().isEmpty();
    bool i = !ui->peerIntLE->text().isEmpty();
    bool l = !ui->peerLblLE->text().isEmpty();



    if(l && i &&  E && D && e)
        ui->okBtn->setEnabled(true);
    else
        ui->okBtn->setEnabled(false);
}

void EditInterfaceDialog::on_cancelBtn_clicked()
{
    this->close();
}

void EditInterfaceDialog::on_okBtn_clicked()
{
    QString module = ui->moduleCB->currentText();
    QString peerExch = "", peerDev="", peerInt = "", peerLbl="", desc = "";
    peerExch = ui->peerExchLE->text();
    peerDev = ui->peerDevLE->text();
    peerInt = ui->peerIntLE->text();
    peerLbl = ui->peerLblLE->text();

    int eth;
    if(ui->etherLE->text().trimmed().isEmpty())
        eth = -1;
    else
        eth = ui->etherLE->text().toInt();

     desc = ui->descTE->toPlainText().trimmed();

    //log
    QString localDevice = devName;
    QString Interface = ui->intLbl->text();
    int exchId = dbMan->getInterfaceExchangeId(intId);
    QString username = dbMan->getLoggedinNameLastname();



    if(dbMan->updateInterface(intId, module, peerExch, peerDev,peerInt, peerLbl,eth, desc))
    {
        dbMan->insertLog(exchId, username, "Edit Interface of "+localDevice+" "+Interface+" To "+peerLbl);
        this->close();
    }
    else
        QMessageBox::warning(this, "Error","Can not assign interface.\n"+dbMan->lastError());
}

void EditInterfaceDialog::on_peerExchLE_textChanged(const QString &arg1)
{
    ui->peerLblLE->clear();

    if(arg1.isEmpty())
    {
        ui->okBtn->setEnabled(false);
    }
    else
    {
        ui->peerLblLE->setText(getLable());
        checkEntry();
    }
}

void EditInterfaceDialog::on_peerDevLE_textChanged(const QString &arg1)
{
    if(arg1.isEmpty())
        ui->peerLblLE->setText(getLable());
    else
        ui->peerLblLE->setText(getLable());

    checkEntry();
}

void EditInterfaceDialog::on_peerIntLE_textChanged(const QString &arg1)
{
    ui->peerLblLE->setText(getLable());

    if(arg1.isEmpty())
        ui->okBtn->setEnabled(false);
    else
    {
        checkEntry();
    }
}

void EditInterfaceDialog::on_peerLblLE_textChanged(const QString &arg1)
{
    if(arg1.isEmpty())
        ui->okBtn->setEnabled(false);
    else
        checkEntry();
}

void EditInterfaceDialog::on_etherLE_textChanged(const QString &arg1)
{
    if(arg1.isEmpty())
        checkEntry();
    else
        checkEntry();
}

void EditInterfaceDialog::on_moduleCB_currentIndexChanged(int index)
{
    index++;
    checkEntry();
}

void EditInterfaceDialog::on_descTE_textChanged()
{
    checkEntry();
}
