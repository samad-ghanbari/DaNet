#include "s2403hwidget.h"
#include "ui_s2403hwidget.h"

S2403HWidget::S2403HWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::S2403HWidget)
{
    ui->setupUi(this);
}

S2403HWidget::~S2403HWidget()
{
    delete ui;
}
