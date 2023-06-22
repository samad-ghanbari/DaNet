#include "addsaloondialog.h"
#include "ui_addsaloondialog.h"
#include "lib/database/danetdbman.h"
#include <QMessageBox>

AddSaloonDialog::AddSaloonDialog(QWidget *parent, DanetDbMan *db) :
    QDialog(parent),
    ui(new Ui::AddSaloonDialog),
    dbMan(db)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowMinMaxButtonsHint);

    ui->okBtn->setEnabled(false);
}

AddSaloonDialog::~AddSaloonDialog()
{
    delete ui;
}

void AddSaloonDialog::on_saloonLE_textChanged(const QString &arg1)
{
    if(arg1.isEmpty())
        ui->okBtn->setEnabled(false);
    else
        ui->okBtn->setEnabled(true);
}

void AddSaloonDialog::on_CancelBtn_clicked()
{
    this->close();
}

void AddSaloonDialog::on_okBtn_clicked()
{
    QString sal = ui->saloonLE->text().trimmed();

    if(dbMan->insertSaloon(sal))
        this->close();
    else
        QMessageBox::warning(this, "ERROR", "Cannot add Saloon.\n"+dbMan->lastError());

}
