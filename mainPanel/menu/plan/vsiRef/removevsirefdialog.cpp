#include "removevsirefdialog.h"
#include "ui_removevsirefdialog.h"
#include "lib/database/danetdbman.h"
#include <QMessageBox>

RemoveVsiRefDialog::RemoveVsiRefDialog(QWidget *parent, DanetDbMan *db, int Id, const int Vsi, const QString Desc) :
    QDialog(parent),
    ui(new Ui::RemoveVsiRefDialog),
    dbMan(db),
    id(Id),
    vsi(QString::number(Vsi)),
    desc(Desc)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowMinMaxButtonsHint);

    ui->vsiLbl->setText(vsi);
    ui->descTE->setText(Desc);

    ui->removeBtn->setEnabled(false);
    ui->checkBox->setChecked(false);
}

RemoveVsiRefDialog::~RemoveVsiRefDialog()
{
    delete ui;
}

void RemoveVsiRefDialog::on_cancelBtn_clicked()
{
    this->close();
}

void RemoveVsiRefDialog::on_removeBtn_clicked()
{
    QString username = dbMan->getLoggedinNameLastname();
    dbMan->insertLog(username, "Remove VSI Reference: "+vsi +" Desc: "+desc);

    if(dbMan->deleteVsiRef(id))
        this->close();
    else
        QMessageBox::information(this, "Error", "Can not remove VSI Reference.\n"+dbMan->lastError());
}

void RemoveVsiRefDialog::on_checkBox_toggled(bool checked)
{
    if(checked)
        ui->removeBtn->setEnabled(true);
    else
        ui->removeBtn->setEnabled(false);
}
