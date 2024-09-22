#include "editinterfacetypedialog.h"
#include "ui_editinterfacetypedialog.h"
#include "lib/database/danetdbman.h"
#include <QMessageBox>

EditInterfaceTypeDialog::EditInterfaceTypeDialog(QWidget *parent, DanetDbMan *db, const int Id, const QString IntType,const double speed, const QString media) :
    QDialog(parent),
    ui(new Ui::EditInterfaceTypeDialog),
    dbMan(db),
    id(Id)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowMinMaxButtonsHint);

    ui->intTypeLE->setText(IntType);
    ui->mediaLE->setText(media);
    ui->speedSB->setValue(speed);
    ui->okBtn->setEnabled(false);
}

EditInterfaceTypeDialog::~EditInterfaceTypeDialog()
{
    delete ui;
}

void EditInterfaceTypeDialog::entryCheck()
{
    bool i = !ui->intTypeLE->text().isEmpty();
    bool m = !ui->mediaLE->text().isEmpty();
    bool s = (ui->speedSB->value() > 0)? true : false;

    if(i && m && s)
        ui->okBtn->setEnabled(true);
    else
        ui->okBtn->setEnabled(false);
}

void EditInterfaceTypeDialog::on_CancelBtn_clicked()
{
    this->close();
}

void EditInterfaceTypeDialog::on_okBtn_clicked()
{
    QString intType = ui->intTypeLE->text().trimmed();
    QString media = ui->mediaLE->text().trimmed();
    double speed = ui->speedSB->value();

    if(dbMan->updateInterfaceType(id,intType,speed,media))
        this->close();
    else
        QMessageBox::warning(this,"ERROR", "Cannot update Interface Type.\n"+dbMan->lastError());
}

void EditInterfaceTypeDialog::on_intTypeLE_textChanged(const QString &arg1)
{
    if(arg1.isEmpty())
        ui->okBtn->setEnabled(false);
    else
        entryCheck();
}

void EditInterfaceTypeDialog::on_speedSB_valueChanged(double arg1)
{
    if(arg1 == 0)
        ui->okBtn->setEnabled(false);
    else
        entryCheck();
}

void EditInterfaceTypeDialog::on_mediaLE_textChanged(const QString &arg1)
{
    if(arg1.isEmpty())
        ui->okBtn->setEnabled(false);
    else
        entryCheck();
}
