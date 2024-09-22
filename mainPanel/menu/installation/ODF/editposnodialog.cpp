#include "editposnodialog.h"
#include "ui_editposnodialog.h"
#include "lib/database/danetdbman.h"
#include <QMessageBox>

EditPosNoDialog::EditPosNoDialog(QWidget *parent, DanetDbMan *db, QString AreaAbbr,int saloon, int PosId, int OdfNo, int PosNo) :
    QDialog(parent),
    ui(new Ui::EditPosNoDialog),
    dbMan(db)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowMinMaxButtonsHint);

    ui->saloonLbl->setText(dbMan->getSaloonName(saloon));

    ui->abbrLbl->setText(AreaAbbr);
    posId = PosId;
    ui->odfNoLbl->setText(QString::number(OdfNo));
    ui->posNoSB->setValue(PosNo);
}

EditPosNoDialog::~EditPosNoDialog()
{
    delete ui;
}

void EditPosNoDialog::on_cancelbtn_clicked()
{
    this->close();
}

void EditPosNoDialog::on_modifyBtn_clicked()
{

    int posNo = ui->posNoSB->value();

    QString username = dbMan->getLoggedinNameLastname();
    QString saloon = ui->saloonLbl->text();
    int exchangeId = dbMan->getPosExchangeId(posId);

    if(dbMan->updatePosNo(posId, posNo))
    {
        dbMan->insertLog(exchangeId, username, "Edit POS "+QString::number(posNo)+" In Saloon "+saloon);

        this->close();
    }
    else
        QMessageBox::information(this, "Error", "Can not modify POS No.\n"+dbMan->lastError());
}



