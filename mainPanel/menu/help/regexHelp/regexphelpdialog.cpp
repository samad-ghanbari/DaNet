#include "regexphelpdialog.h"
#include "ui_regexphelpdialog.h"
#include <QStandardItemModel>

RegexpHelpDialog::RegexpHelpDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RegexpHelpDialog),
    model(new QStandardItemModel(this))
{
    ui->setupUi(this);

    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowMinMaxButtonsHint);

    ui->tabWidget->setCurrentIndex(0);

    model->setColumnCount(2);
    model->setHorizontalHeaderLabels(QStringList()<<"RegExp"<<"Meaning");

    ui->TV->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->TV->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->TV->verticalHeader()->hide();

    QModelIndex index;
    int row = 0;
    model->insertRow(row);

    index = model->index(row,0);
    model->setData(index,"^2/");
    index = model->index(row,1);
    model->setData(index,"anything starts with 2/");

    row++;
    model->insertRow(row);
    index = model->index(row,0);
    model->setData(index,"/2$");
    index = model->index(row,1);
    model->setData(index,"anything ends by /2");

    row++;
    model->insertRow(row);
    index = model->index(row,0);
    model->setData(index,"^2/.*/0$");
    index = model->index(row,1);
    model->setData(index,"anything starts with 2/ and ends by /0");

    row++;
    model->insertRow(row);
    index = model->index(row,0);
    model->setData(index,"^[2,5]/0");
    index = model->index(row,1);
    model->setData(index,"anything starts with either 2/0 or 5/0");

    row++;
    model->insertRow(row);
    index = model->index(row,0);
    model->setData(index,"^2/0/[4-6]$");
    index = model->index(row,1);
    model->setData(index,"anything starts with 2/0/ and ends by 4, 5 or 6");

    row++;
    model->insertRow(row);
    index = model->index(row,0);
    model->setData(index,"^[2,3]");
    index = model->index(row,1);
    model->setData(index,"anything starts with either 2 or 3");

    row++;
    model->insertRow(row);
    index = model->index(row,0);
    model->setData(index,"^[2-5]");
    index = model->index(row,1);
    model->setData(index,"anything starts with 2, 3, 4 or 5");


    row++;
    model->insertRow(row);
    index = model->index(row,0);
    model->setData(index,"^2[0,2]$");
    index = model->index(row,1);
    model->setData(index,"anything starts with 2 and ends by either 0 or 2");

    row++;
    model->insertRow(row);
    index = model->index(row,0);
    model->setData(index,"^[3,4][2,3]$");
    index = model->index(row,1);
    model->setData(index,"anything starts with either 3 or 4 and ends by either 2 or 3");

    row++;
    model->insertRow(row);
    index = model->index(row,0);
    model->setData(index,"^(90|11)");
    index = model->index(row,1);
    model->setData(index,"anything starts with either 90 or 11");

    row++;
    model->insertRow(row);
    index = model->index(row,0);
    model->setData(index,"^(?!AGG)");
    index = model->index(row,1);
    model->setData(index,"anything not starts with AGG");
    ui->TV->setModel(model);

    row++;
    model->insertRow(row);
    index = model->index(row,0);
    model->setData(index,"^\\s*$  OR  ^ *$");
    index = model->index(row,1);
    model->setData(index,"matches space(empty)");

    row++;
    model->insertRow(row);
    index = model->index(row,0);
    model->setData(index,"^\\d+:\\d+");
    index = model->index(row,1);
    model->setData(index,"matches N1:N2 like 65021:3000");

    row++;
    model->insertRow(row);
    index = model->index(row,0);
    model->setData(index,"^\\d+:\\d+(\\s*\\d+:\\d+)*");
    index = model->index(row,1);
    model->setData(index,"matches one or more space separated rd like [65021:10 65021:100]");


    ui->TV->setModel(model);
}

RegexpHelpDialog::~RegexpHelpDialog()
{
    delete ui;
}

void RegexpHelpDialog::on_okBtn_clicked()
{
    this->close();
}

