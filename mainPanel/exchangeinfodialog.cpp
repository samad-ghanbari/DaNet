#include "exchangeinfodialog.h"
#include "ui_exchangeinfodialog.h"
#include "lib/database/danetdbman.h"
#include <QSqlQuery>

ExchangeInfoDialog::ExchangeInfoDialog(QWidget *parent, DanetDbMan *db, const int exchId) :
    QDialog(parent),
    ui(new Ui::ExchangeInfoDialog)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowMinMaxButtonsHint);


    QSqlQuery *query = db->selectViewExchange(exchId);
    //`id`,`area_no`,`area`,`exchange`,`abbr`,`type_no`,`type`,`mother_exchange_id`,`mother_exchange_abbr`,
    //`uplink_exchange_id`,`uplink_exchange_abbr`,`site_cascade`,`site_node`,`address`
    if(query->next())
    {
        ui->areaLbl->setText(query->value(2).toString());
        ui->exchLbl->setText(query->value(3).toString());
        ui->abbrLbl->setText(query->value(4).toString());
        ui->typeLbl->setText(query->value(6).toString());
        int type = query->value(5).toInt();
        if(type == 3)
        {
            ui->siteGB->setVisible(true);
            ui->motherLbl->setText(query->value(8).toString());
            ui->uplinkLbl->setText(query->value(10).toString());
            ui->cascadeLbl->setText(query->value(11).toString());
            ui->nodeLbl->setText(query->value(12).toString());
        }
        else
        {
            ui->siteGB->setVisible(false);
            this->setMinimumHeight(280);
            this->setMaximumHeight(281);
        }

        ui->addressLbl->setText(query->value(13).toString());
    }
}

ExchangeInfoDialog::~ExchangeInfoDialog()
{
    delete ui;
}

void ExchangeInfoDialog::on_okBtn_clicked()
{
    this->close();
}
