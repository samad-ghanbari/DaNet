#include "aboutdialog.h"
#include "ui_aboutdialog.h"


AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDialog)
{
    ui->setupUi(this);

    QString ver = QString(DANET_VER);
    ver = "DaNet Version "+ver;
    ui->danetLbl->setText(ver);
}

AboutDialog::~AboutDialog()
{
    delete ui;
}

void AboutDialog::on_okBtn_clicked()
{
    this->close();
}
