#include "removeodfdialog.h"
#include "ui_removeodfdialog.h"
#include "lib/database/danetdbman.h"
#include <QMessageBox>

RemoveOdfDialog::RemoveOdfDialog(QWidget *parent,DanetDbMan *db, int ExchId, QString AreaAbbr, int Saloon, int OdfId, int OdfNo, int PosCount, int ppp) :
    QDialog(parent),
    ui(new Ui::RemoveOdfDialog),
    dbMan(db)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowMinMaxButtonsHint);


    ui->abbrLbl->setText(AreaAbbr);
    exchId = ExchId;
    saloon = Saloon;
    odfId = OdfId;
    ui->saloonLbl->setText(dbMan->getSaloonName(saloon));
    ui->odfNoLbl->setText(QString::number(OdfNo));
    ui->posCountLbl->setText(QString::number(PosCount));
    ui->pppLbl->setText(QString::number(ppp));


    ui->infoLbl->setVisible(true);
    ui->okBtn->setEnabled(false);

    if(dbMan->odfHasPOS(odfId))
    {
        ui->okBtn->setEnabled(false);
        ui->checkBox->setEnabled(false);
        ui->checkBox->setChecked(false);
        ui->infoLbl->setVisible(true);
    }
    else
    {
        ui->infoLbl->setVisible(false);
        ui->checkBox->setEnabled(true);
        ui->checkBox->setChecked(false);
    }


}

RemoveOdfDialog::~RemoveOdfDialog()
{
    delete ui;
}

void RemoveOdfDialog::on_cancelBtn_clicked()
{
    this->close();
}

void RemoveOdfDialog::on_okBtn_clicked()
{
    QString username = dbMan->getLoggedinNameLastname();
    QString saloon = ui->saloonLbl->text();
    QString odf = ui->odfNoLbl->text();

    if(dbMan->deleteOdf(odfId))
    {
        dbMan->insertLog(exchId, username, "Remove ODF "+odf+" In Saloon "+saloon);

        this->close();
    }
    else
        QMessageBox::information(this, "Error", "You can only remove ODF with no POS.\n"+dbMan->lastError());
}

void RemoveOdfDialog::on_checkBox_toggled(bool checked)
{
    if(checked)
        ui->okBtn->setEnabled(true);
    else
        ui->okBtn->setEnabled(false);
}


