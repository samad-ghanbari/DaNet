#include "removeaggcxdialog.h"
#include "ui_removeaggcxdialog.h"
#include "lib/database/danetdbman.h"
#include <QMessageBox>


RemoveAggCxDialog::RemoveAggCxDialog(QWidget *parent, DanetDbMan *Db, const int Id, const QString Abbr, const int Agg1Id, const QString Agg, const int AggEth, const int CxId, const QString Cx, const int CxEth) :
    QDialog(parent),
    ui(new Ui::RemoveAggCxDialog),
    dbMan(Db),
    id(Id),
    agg1Id(Agg1Id),
    cxId(CxId),
    agg1Eth(AggEth),
    cxEth(CxEth)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowMinMaxButtonsHint);

    ui->abbrLbl->setText(Abbr);
    ui->aggLbl->setText(Agg);
    ui->aggEthLbl->setText(QString::number(AggEth));
    ui->cxLbl->setText(Cx);
    ui->cxEthLbl->setText(QString::number(CxEth));

    ui->checkBox->setChecked(false);
    ui->okBtn->setEnabled(false);

}

RemoveAggCxDialog::~RemoveAggCxDialog()
{
    delete ui;
}

void RemoveAggCxDialog::on_checkBox_toggled(bool checked)
{
    if(checked)
        ui->okBtn->setEnabled(true);
    else
        ui->okBtn->setEnabled(false);
}

void RemoveAggCxDialog::on_okBtn_clicked()
{
    if(dbMan->deleteAggCx(id))
    {
        dbMan->deleteEtherTrunk(cxId,cxEth);
        dbMan->deleteEtherTrunk(agg1Id, agg1Eth);

        this->close();
    }
    else
        QMessageBox::warning(this,"ERROR", "Cannot Remove Connection.\n"+dbMan->lastError());
}
