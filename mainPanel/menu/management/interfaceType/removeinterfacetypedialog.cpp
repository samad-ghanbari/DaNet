#include "removeinterfacetypedialog.h"
#include "ui_removeinterfacetypedialog.h"
#include "lib/database/danetdbman.h"
#include <QMessageBox>

RemoveInterfaceTypeDialog::RemoveInterfaceTypeDialog(QWidget *parent, DanetDbMan *db, const int Id, const QString intType, const double speed, const QString media) :
    QDialog(parent),
    ui(new Ui::RemoveInterfaceTypeDialog),
    dbMan(db),
    id(Id)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowMinMaxButtonsHint);

    ui->intTypeLE->setText(intType);
    ui->speedLE->setText(QString::number(speed));
    ui->mediaLE->setText(media);
    ui->okBtn->setEnabled(false);
    ui->checkBox->setChecked(false);
}

RemoveInterfaceTypeDialog::~RemoveInterfaceTypeDialog()
{
    delete ui;
}

void RemoveInterfaceTypeDialog::on_checkBox_toggled(bool checked)
{
    if(checked)
        ui->okBtn->setEnabled(true);
    else
        ui->okBtn->setEnabled(false);
}

void RemoveInterfaceTypeDialog::on_CancelBtn_clicked()
{
    this->close();
}

void RemoveInterfaceTypeDialog::on_okBtn_clicked()
{
    if(dbMan->deleteInterfaceType(id))
        this->close();
    else
        QMessageBox::warning(this,"ERROR", "Cannot remove Interface Type.\n"+dbMan->lastError());
}
