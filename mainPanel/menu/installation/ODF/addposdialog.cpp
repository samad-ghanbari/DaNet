#include "addposdialog.h"
#include "ui_addposdialog.h"
#include "lib/database/danetdbman.h"
#include <QMessageBox>

AddPosDialog::AddPosDialog(QWidget *parent, DanetDbMan *db, QString AreaAbbr, int saloon, int OdfId, int OdfNo) :
    QDialog(parent),
    ui(new Ui::AddPosDialog),
    dbMan(db),
    odfId(OdfId)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowMinMaxButtonsHint);


    ui->abbrLbl->setText(AreaAbbr);
    ui->odfNoLbl->setText(QString::number(OdfNo));

    ui->okBtn->setEnabled(false);
    ui->warningLbl->setVisible(true);

    //saloon
    ui->saloonLbl->setText(dbMan->getSaloonName(saloon));

    if(dbMan->posInsertAbility(OdfId))
    {
        ui->okBtn->setEnabled(true);
        ui->warningLbl->setVisible(false);
    }
    else
    {
        ui->okBtn->setEnabled(false);
        ui->warningLbl->setVisible(true);
    }

    ui->posNoSB->setMinimum(dbMan->getOdfMaxPosCount(OdfId)+1);
}

AddPosDialog::~AddPosDialog()
{
    delete ui;
}

void AddPosDialog::on_cancelBtn_clicked()
{
    this->close();
}

void AddPosDialog::on_okBtn_clicked()
{
    int posNo = ui->posNoSB->value();

    QString username = dbMan->getLoggedinNameLastname();
    QString saloon = ui->saloonLbl->text();
    QString odfNo = ui->odfNoLbl->text();

    int exchId = dbMan->getOdfExchangeId(odfId);

    if(dbMan->insertPos(odfId, posNo))
    {
        dbMan->insertLog(exchId, username, "Add POS "+QString::number(posNo)+" To ODF "+odfNo+" In Saloon "+saloon);
        this->close();
    }
    else
        QMessageBox::information(this, "Error", "Can not Insert New POS.\n"+dbMan->lastError());

}



