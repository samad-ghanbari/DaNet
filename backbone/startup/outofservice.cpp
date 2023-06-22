#include "outofservice.h"
#include "ui_outofservice.h"

OutOfService::OutOfService(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OutOfService)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
}

OutOfService::~OutOfService()
{
    delete ui;
}

void OutOfService::on_pushButton_clicked()
{
    this->close();
    exit(0);
}

