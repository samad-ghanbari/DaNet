#include "cx200widget.h"
#include "ui_cx200widget.h"

CX200Widget::CX200Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CX200Widget)
{
    ui->setupUi(this);
}

CX200Widget::~CX200Widget()
{
    delete ui;
}
