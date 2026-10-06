#include "converttoduplex.h"
#include "ui_converttoduplex.h"

ConvertToDuplex::ConvertToDuplex(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ConvertToDuplex)
{
    ui->setupUi(this);
}

ConvertToDuplex::~ConvertToDuplex()
{
    delete ui;
}
