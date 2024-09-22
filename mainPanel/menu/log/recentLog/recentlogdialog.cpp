#include "recentlogdialog.h"
#include "ui_recentlogdialog.h"
#include "lib/database/danetdbman.h"
#include "lib/headerview.h"
#include <QSqlQuery>
#include <QSqlQueryModel>
#include "lib/sortfilterproxymodel.h"
//#include <QDate>


#define EXCHANGE 0
#define ABBR 1
#define USER 2
#define TIMESTAMP 3
#define DESC 4

RecentLogDialog::RecentLogDialog(QWidget *parent, DanetDbMan *db) :
    QDialog(parent),
    ui(new Ui::RecentLogDialog),
    dbMan(db),
    proxyModel(nullptr),
    header(nullptr),
    query(nullptr),
    model(new QSqlQueryModel(this))
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowMinMaxButtonsHint);

    //area
    ui->areaCB->blockSignals(true);
    ui->areaCB->addItem("2", 2);
    ui->areaCB->addItem("3", 3);
    ui->areaCB->addItem("4", 4);
    ui->areaCB->addItem("5", 5);
    ui->areaCB->addItem("6", 6);
    ui->areaCB->addItem("7", 7);
    ui->areaCB->addItem("8", 8);
    ui->areaCB->addItem("Karaj", 9);
    ui->areaCB->setCurrentIndex(-1);
    ui->areaCB->blockSignals(false);

    //
    proxyModel = new SortFilterProxyModel(this);
    header = new HeaderView(5,ABBR, ui->logTV);
    ui->logTV->setHorizontalHeader(header);

    header->hideComboBox(EXCHANGE);
    header->setLableText(EXCHANGE, "Exch/Site");
    header->setLineEditPlaceHolder(EXCHANGE,"Search Exchange/Site");

    header->hideComboBox(ABBR);
    header->setLableText(ABBR, "Abbr");
    header->setLineEditPlaceHolder(ABBR, "Search Abbr");

    header->hideComboBox(USER);
    header->setLableText(USER, "User");
    header->setLineEditPlaceHolder(USER, "Search User");

    header->hideComboBox(TIMESTAMP);
    header->setLableText(TIMESTAMP, "Time Stamp");
    header->setLineEditPlaceHolder(TIMESTAMP, "2017-12-22");

    header->hideComboBox(DESC);
    header->setLableText(DESC, "Description");
    header->setLineEditPlaceHolder(DESC, "Search Desc");

    ui->logTV->setModel(proxyModel);
    connect(header, SIGNAL(sectionsWidgetsSignal()), this, SLOT(headerWidgetsChagedSlot()));


    ui->logTV->verticalHeader()->hide();
    ui->logTV->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    //ui->logTV->horizontalHeader()->setStretchLastSection(true);
    ui->logTV->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

//    QDate d = QDate::currentDate();
//    int year = d.year();
//    int month = d.month()-1;
//    int day = d.day();
//    d.setDate(year, month, day);

//    ui->startDE->setDate(d);
//    ui->startDE->setEnabled(false);

    ui->recentLogChB->blockSignals(true);
    ui->recentLogChB->setChecked(false);
    ui->recentLogChB->blockSignals(false);
    ui->recentLogChB->setChecked(true);

}

RecentLogDialog::~RecentLogDialog()
{
    delete ui;
}

void RecentLogDialog::fillRecentLog(bool RESET)
{
    proxyModel->invalidate();
    query = dbMan->selectRecentLog();

    //`exchange`, `abbr`, `user`, `time_stamp`, `description`

    header->blockSignals(true);
    model->setQuery(*query);
    proxyModel->setSourceModel(model);
    proxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);

    if(RESET)
    {
        header->resetHeaderWidgets();
    }

    //ui->logTV->horizontalHeader()->setStretchLastSection(true);
    header->blockSignals(false);
}

void RecentLogDialog::fillLog(bool RESET)
{
    proxyModel->invalidate();
    int exchId = ui->abbrCB->currentData().toInt();
    //QDate d = ui->startDE->date();
    //int year = d.year();
    //int month = d.month();
    //int day = d.day();
    //QString startDate = QString::number(year)+"-"+QString::number(month)+"-"+QString::number(day);
    //QString endDate = QString::number(year)+"-"+QString::number(month+1)+"-"+QString::number(day+1);

    //query = dbMan->selectLog(exchId, startDate, endDate);
    query = dbMan->selectLog(exchId);

    header->blockSignals(true);
    model->setQuery(*query);
    proxyModel->setSourceModel(model);
    proxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);

    if(RESET)
    {
        header->resetHeaderWidgets();
    }

    header->blockSignals(false);

    ui->logTV->verticalHeader()->hide();
    ui->logTV->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    //ui->logTV->horizontalHeader()->setStretchLastSection(true);
    ui->logTV->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
}

void RecentLogDialog::on_recentLogChB_toggled(bool checked)
{
    if(checked)
    {
        ui->abbrCB->blockSignals(true);
        ui->abbrCB->setCurrentIndex(-1);
        ui->abbrCB->blockSignals(false);

        int area = dbMan->getLoggedInDefaultArea();
        ui->areaCB->setCurrentIndex(ui->areaCB->findData(area));

        ui->exchFrame->hide();
        fillRecentLog(true);
    }
    else
    {
        ui->exchFrame->show();
        ui->abbrCB->setCurrentIndex(-1);
        fillLog(true);
    }
}

void RecentLogDialog::on_areaCB_currentIndexChanged(int index)
{
    proxyModel->invalidate();
    ui->abbrCB->blockSignals(true);
    ui->abbrCB->clear();

    int area = index + 2;
    QSqlQuery *query = dbMan->selectExchAndSites(area);
    int id;
    QString abbr;
    while(query->next())
    {
        id = query->value(0).toInt();
        abbr = query->value(1).toString();
        ui->abbrCB->addItem(abbr, id);
    }

    ui->abbrCB->setCurrentIndex(-1);
    ui->abbrCB->blockSignals(false);
    fillLog(true);
}

void RecentLogDialog::on_abbrCB_currentIndexChanged(int index)
{
    index++;
    if(!ui->abbrCB->currentText().isEmpty())
    {
//        ui->startDE->setEnabled(true);
        fillLog(true);
    }
}

void RecentLogDialog::headerWidgetsChagedSlot()
{
    QString exch = header->sections[EXCHANGE]->lineEdit->text();
    QString abbr = header->sections[ABBR]->lineEdit->text();
    QString user = header->sections[USER]->lineEdit->text();
    QString time_stamp = header->sections[TIMESTAMP]->lineEdit->text();
    QString desc = header->sections[DESC]->lineEdit->text();


    QMap<int, QRegExp> mapRegExp;
    QRegExp reg;
    reg.setCaseSensitivity(Qt::CaseInsensitive);

    if(!exch.isEmpty())
    {
        reg.setPattern(exch);
        mapRegExp.insert(EXCHANGE, reg);
    }
    if(!abbr.isEmpty())
    {
        reg.setPattern(abbr);
        mapRegExp.insert(ABBR, reg);
    }
    if(!user.isEmpty())
    {
        reg.setPattern(user);
        mapRegExp.insert(USER, reg);
    }
    if(!time_stamp.isEmpty())
    {
        reg.setPattern(time_stamp);
        mapRegExp.insert(TIMESTAMP, reg);
    }
    if(!desc.isEmpty())
    {
        reg.setPattern(desc);
        mapRegExp.insert(DESC, reg);
    }


    proxyModel->setColumnsFilterRegExp(mapRegExp);


    ui->logTV->verticalHeader()->hide();
    ui->logTV->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->logTV->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

}


//void RecentLogDialog::on_startDE_dateChanged(const QDate &date)
//{
//    int year = date.year();
//    int month = date.month();
//    int day = date.day();

//    ui->endDE->setText(QString::number(year)+"/"+QString::number(month+1)+"/"+QString::number(day));

//    fillLog(true);
//}











