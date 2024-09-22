#include "addinterfacetypedialog.h"
#include "ui_addinterfacetypedialog.h"
#include "lib/database/danetdbman.h"
#include <QMessageBox>

AddInterfaceTypeDialog::AddInterfaceTypeDialog(QWidget *parent, DanetDbMan *db) :
    QDialog(parent),
    ui(new Ui::AddInterfaceTypeDialog),
    dbMan(db)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowMinMaxButtonsHint);

    ui->okBtn->setEnabled(false);
}

AddInterfaceTypeDialog::~AddInterfaceTypeDialog()
{
    delete ui;
}

void AddInterfaceTypeDialog::entryCheck()
{
    bool i = !ui->intTypeLE->text().isEmpty();
    bool m = !ui->mediaLE->text().isEmpty();
    bool s = (ui->speedSB->value() > 0)? true : false;
    if(i && m && s)
        ui->okBtn->setEnabled(true);
    else
        ui->okBtn->setEnabled(false);
}

void AddInterfaceTypeDialog::on_CancelBtn_clicked()
{
    this->close();
}

void AddInterfaceTypeDialog::on_okBtn_clicked()
{
    QString intType = ui->intTypeLE->text().trimmed();
    QString media = ui->mediaLE->text().trimmed();
    double speed = ui->speedSB->value();

    if(dbMan->insertInterfaceType(intType, speed, media))
        this->close();
    else
        QMessageBox::warning(this,"ERROR", "Cannot insert Interface Type.\n"+dbMan->lastError());
}

void AddInterfaceTypeDialog::on_intTypeLE_textChanged(const QString &arg1)
{
    if(arg1.isEmpty())
        ui->okBtn->setEnabled(false);
    else
        entryCheck();
}

void AddInterfaceTypeDialog::on_speedSB_valueChanged(double arg1)
{
    if(arg1 == 0)
        ui->okBtn->setEnabled(false);
    else
        entryCheck();
}

void AddInterfaceTypeDialog::on_mediaLE_textChanged(const QString &arg1)
{
    if(arg1.isEmpty())
        ui->okBtn->setEnabled(false);
    else
        entryCheck();
}
