#include "editsaloondialog.h"
#include "ui_editsaloondialog.h"
#include "lib/database/danetdbman.h"
#include <QMessageBox>

EditSaloonDialog::EditSaloonDialog(QWidget *parent, DanetDbMan *db, const int Id, const QString Sal) :
    QDialog(parent),
    ui(new Ui::EditSaloonDialog),
    dbMan(db),
    id(Id)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowMinMaxButtonsHint);

    ui->saloonLE->setText(Sal);

    ui->okBtn->setEnabled(false);
}

EditSaloonDialog::~EditSaloonDialog()
{
    delete ui;
}

void EditSaloonDialog::on_CancelBtn_clicked()
{
    this->close();
}

void EditSaloonDialog::on_okBtn_clicked()
{
    QString Sal = ui->saloonLE->text();

    if(dbMan->updateSaloon(id,Sal))
        this->close();
    else
        QMessageBox::warning(this, "ERROR", "Cannot update Saloon.\n"+dbMan->lastError());
}

void EditSaloonDialog::on_saloonLE_textChanged(const QString &arg1)
{
    if(arg1.isEmpty())
        ui->okBtn->setEnabled(false);
    else
        ui->okBtn->setEnabled(true);
}
