#include "s5300widget.h"
#include "ui_s5300widget.h"

S5300Widget::S5300Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::S5300Widget)
{
    ui->setupUi(this);
}

S5300Widget::~S5300Widget()
{
    delete ui;
}
