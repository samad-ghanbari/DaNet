#include "exchangesstatdialog.h"
#include "ui_exchangesstatdialog.h"
#include "lib/database/danetdbman.h"

ExchangesStatDialog::ExchangesStatDialog(QWidget *parent, DanetDbMan *db) :
    QDialog(parent),
    ui(new Ui::ExchangesStatDialog),
    dbMan(db)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowMinMaxButtonsHint);


    ui->areaCB->blockSignals(true);
    ui->areaCB->addItem("2", 2);
    ui->areaCB->addItem("3", 3);
    ui->areaCB->addItem("4", 4);
    ui->areaCB->addItem("5", 5);
    ui->areaCB->addItem("6", 6);
    ui->areaCB->addItem("7", 7);
    ui->areaCB->addItem("8", 8);
    ui->areaCB->addItem("Karaj", 9);
    ui->areaCB->addItem("All", 1);
    ui->areaCB->setCurrentIndex(-1);
    ui->areaCB->blockSignals(false);
}

ExchangesStatDialog::~ExchangesStatDialog()
{
    delete ui;
}

void ExchangesStatDialog::on_CancelBtn_clicked()
{
    this->close();
}

void ExchangesStatDialog::on_areaCB_currentIndexChanged(int index)
{
    index++;
    int area = ui->areaCB->currentData().toInt();

    int exch = dbMan->getAreaExchangesCount(area);
    int site = dbMan->getAreaSitesCount(area);

    ui->exchLbl->setText(QString::number(exch));
    ui->siteLbl->setText(QString::number(site));
}

