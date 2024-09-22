#include "removebrasaggdialog.h"
#include "ui_removebrasaggdialog.h"
#include "lib/database/danetdbman.h"
#include <QMessageBox>


RemoveBrasAggDialog::RemoveBrasAggDialog(QWidget *parent, DanetDbMan *db, const int Id,const int BrasId,const QString BrasName, const QString Exch, const QString Agg1, const int Eth1, const QString Agg2, const int Eth2) :
    QDialog(parent),
    ui(new Ui::RemoveBrasAggDialog),
    dbMan(db),
    id(Id),
    brasId(BrasId),
    agg1Eth(Eth1),
    agg2Eth(Eth2)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowMinMaxButtonsHint);

    ui->brasLbl->setText(BrasName);
    ui->exchLbl->setText(Exch);
    ui->agg1Lbl->setText(Agg1);
    ui->agg2Lbl->setText(Agg2);
    ui->eth1Lbl->setText(QString::number(Eth1));
    ui->eth2Lbl->setText(QString::number(Eth2));

    ui->checkBox->setChecked(false);
    ui->okBtn->setEnabled(false);
}

RemoveBrasAggDialog::~RemoveBrasAggDialog()
{
    delete ui;
}

void RemoveBrasAggDialog::on_cancelBtn_clicked()
{
    this->close();
}

void RemoveBrasAggDialog::on_okBtn_clicked()
{
    if(dbMan->deleteBrasConnection(id))
    {
        dbMan->deleteEtherTrunk(brasId,agg1Eth);
        dbMan->deleteEtherTrunk(brasId, agg2Eth);
        this->close();
    }
    else
        QMessageBox::warning(this,"ERROR", "Cannot delete BRAS connection.\n"+dbMan->lastError());
}

void RemoveBrasAggDialog::on_checkBox_toggled(bool checked)
{
    if(checked)
        ui->okBtn->setEnabled(true);
    else
        ui->okBtn->setEnabled(false);
}
