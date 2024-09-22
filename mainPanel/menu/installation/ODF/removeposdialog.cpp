#include "removeposdialog.h"
#include "ui_removeposdialog.h"
#include "lib/database/danetdbman.h"
#include <QMessageBox>

RemovePosDialog::RemovePosDialog(QWidget *parent, DanetDbMan *db, QString AreaAbbr,int saloon, int PosId, int OdfNo, int PosNo) :
    QDialog(parent),
    ui(new Ui::RemovePosDialog),
    dbMan(db)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowMinMaxButtonsHint);


    ui->removeBtn->setEnabled(false);
    ui->infoLbl->setVisible(true);

    posId = PosId;

    ui->abbrLbl->setText(AreaAbbr);
    ui->odfNoLbl->setText(QString::number(OdfNo));
    ui->posNoLbl->setText(QString::number(PosNo));

    //saloon
    ui->saloonLbl->setText(dbMan->getSaloonName(saloon));

    if(dbMan->isPosEmpty(PosId))
    {
        ui->checkBox->setEnabled(true);
        ui->checkBox->setChecked(false);
        ui->infoLbl->setVisible(false);
    }
    else
    {
        ui->removeBtn->setEnabled(false);
        ui->checkBox->setEnabled(false);
        ui->checkBox->setChecked(false);
        ui->infoLbl->setVisible(true);
    }

}

RemovePosDialog::~RemovePosDialog()
{
    delete ui;
}

void RemovePosDialog::on_cancelBtn_clicked()
{
    this->close();
}

void RemovePosDialog::on_removeBtn_clicked()
{
    if(dbMan->isPosEmpty(posId)) // can remove
    {

        QString username = dbMan->getLoggedinNameLastname();
        QString saloon = ui->saloonLbl->text();
        QString odf = ui->odfNoLbl->text();
        QString Pos = ui->posNoLbl->text();
        int exchId = dbMan->getPosExchangeId(posId);

        if(dbMan->deletePosAllPin(posId))
        {
            if(dbMan->deletePos(posId))
            {
                dbMan->insertLog(exchId, username, "Remove POS "+Pos+" From ODF "+odf+" In Saloon "+saloon);

                this->close();
            }
            else     QMessageBox::information(this, "Error", "You can only remove empty POS.\n"+dbMan->lastError());
        }
        else     QMessageBox::information(this, "Error", "You can only remove empty POS.\n"+dbMan->lastError());
    }else
        QMessageBox::information(this, "Error", "You can only remove empty POS.\n"+dbMan->lastError());


}

void RemovePosDialog::on_checkBox_toggled(bool checked)
{
    if(checked)
        ui->removeBtn->setEnabled(true);
    else
        ui->removeBtn->setEnabled(false);

}
