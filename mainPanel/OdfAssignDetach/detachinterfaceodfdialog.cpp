#include "detachinterfaceodfdialog.h"
#include "ui_detachinterfaceodfdialog.h"
#include "lib/database/danetdbman.h"
#include <QSqlQuery>
#include <QMessageBox>

DetachInterfaceOdfDialog::DetachInterfaceOdfDialog(QWidget *parent, DanetDbMan *DbMan,  const int ExchId, const QString clickedLabel, const int OdfId, const int PosId, const int clickedPinId, const int IntId) :
    QDialog(parent),
    ui(new Ui::DetachInterfaceOdfDialog),
    dbMan(DbMan),
    exchId(ExchId),
    odfId(OdfId),
    posId(PosId),
    pinId(clickedPinId),
    intId(IntId)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowMinMaxButtonsHint);

    ui->devLbl->setText(clickedLabel);
    QString currentOdf = dbMan->getOdfPosPinString(pinId);
    ui->currentOdfLbl->setText(currentOdf);

    ui->okBtn->setEnabled(false);
    ui->confirmChB->setChecked(false);

}

DetachInterfaceOdfDialog::~DetachInterfaceOdfDialog()
{
    delete ui;
}


void DetachInterfaceOdfDialog::on_cancelBtn_clicked()
{
    this->close();
}

void DetachInterfaceOdfDialog::on_okBtn_clicked()
{

    if(dbMan->detachInterfaceOdfPosition(intId, pinId))
    {
        //log
        QString odf = ui->currentOdfLbl->text();
        QString dev = ui->devLbl->text();
        int exchId = dbMan->getInterfaceExchangeId(intId);
        QString username = dbMan->getLoggedinNameLastname();
        dbMan->insertLog(exchId, username, "Unlink and detach ODF "+odf+" of "+dev);

        this->close();
    }
    else
    {
        QMessageBox::warning(this,"Error", "Cannot unlink the ODF.");
    }

}

void DetachInterfaceOdfDialog::on_confirmChB_toggled(bool checked)
{
    if(checked)
        ui->okBtn->setEnabled(true);
    else
        ui->okBtn->setEnabled(false);
}
