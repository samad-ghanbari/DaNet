#include "newrelease.h"
#include "ui_newrelease.h"

NewRelease::NewRelease(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewRelease)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
}

NewRelease::~NewRelease()
{
    delete ui;
}

void NewRelease::setVersionLabel(QString ver)
{
    ui->versionLbl->setText("Version "+ver+" is released!");
}

void NewRelease::setPathLabel(QString path)
{
    ui->pathLbl->setText(path);
}

void NewRelease::on_pushButton_clicked()
{
    this->close();
    exit(0);
}

