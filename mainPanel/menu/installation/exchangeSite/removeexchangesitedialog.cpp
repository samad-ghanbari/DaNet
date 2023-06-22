#include "removeexchangesitedialog.h"
#include "ui_removeexchangesitedialog.h"
#include "lib/database/danetdbman.h"
#include <QMessageBox>

RemoveExchangeSiteDialog::RemoveExchangeSiteDialog(QWidget *parent, DanetDbMan *db, int Id, int area, QString exch, QString abbr, QString type) :
    QDialog(parent),
    ui(new Ui::RemoveExchangeSiteDialog),
    dbMan(db)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowMinMaxButtonsHint);

    id = Id;
    ui->areaLbl->setText(QString::number(area));
    ui->exchLbl->setText(exch);
    ui->abbrLbl->setText(abbr);
    ui->typeLbl->setText(type);

    ui->removeBtn->setEnabled(false);
    ui->checkBox->setChecked(false);
}

RemoveExchangeSiteDialog::~RemoveExchangeSiteDialog()
{
    delete ui;
}

void RemoveExchangeSiteDialog::on_cancelBtn_clicked()
{
    this->close();
}

void RemoveExchangeSiteDialog::on_removeBtn_clicked()
{
    QString abbr = ui->areaLbl->text();
    abbr += "-" + ui->abbrLbl->text();

    QString username = dbMan->getLoggedinNameLastname();
    dbMan->insertLog(username, "Remove Exchange "+abbr);

    if(dbMan->deleteExchange(id))
    {
        this->close();
    }
    else
        QMessageBox::information(this, "Error", "Can not remove exchange/site.\n"+dbMan->lastError());
}

void RemoveExchangeSiteDialog::on_checkBox_toggled(bool checked)
{
    if(checked)
        ui->removeBtn->setEnabled(true);
    else
        ui->removeBtn->setEnabled(false);
}
