#include "editnasipdialog.h"
#include "ui_editnasipdialog.h"
#include "lib/database/danetdbman.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QRegExpValidator>
#include <QMessageBox>

EditNasIpDialog::EditNasIpDialog(QWidget *parent, DanetDbMan *db, const int Id, const int DevexId, const int Area, const QString Nas) :
    QDialog(parent),
    ui(new Ui::EditNasIpDialog),
    dbMan(db),
    id(Id),
    ipRange("(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])"),
    ipRegex("^" + ipRange+ "\\." + ipRange+ "\\." + ipRange+"\\." + ipRange + "$"),
    ipValidator(new QRegExpValidator(ipRegex, this))
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
    ui->areaCB->setCurrentIndex(ui->areaCB->findData(Area));


    ui->brasCB->setCurrentIndex(ui->brasCB->findData(DevexId));
    ui->nasLE->setText(Nas);
    ui->nasLE->setValidator(ipValidator);

    ui->okBtn->setEnabled(false);
}

EditNasIpDialog::~EditNasIpDialog()
{
    delete ui;
}

void EditNasIpDialog::on_areaCB_currentIndexChanged(int index)
{
    index++;
    ui->nasLE->clear();
    int area = ui->areaCB->currentData().toInt();
    QSqlQuery *query = dbMan->selectBras(area);
    //id, deviceName
    int id;
    QString name;
    ui->brasCB->blockSignals(true);
    ui->brasCB->clear();
    while(query->next())
    {
        id = query->value(0).toInt();
        name = query->value(1).toString();
        ui->brasCB->addItem(name, id);
    }
    ui->brasCB->setCurrentIndex(-1);
    ui->brasCB->blockSignals(false);

    ui->nasLE->setEnabled(false);
    ui->okBtn->setEnabled(false);
}

void EditNasIpDialog::on_brasCB_currentIndexChanged(int index)
{
    index++;
    ui->nasLE->clear();
    if(ui->brasCB->currentText().isEmpty())
    {
        ui->nasLE->setEnabled(false);
        ui->okBtn->setEnabled(false);
    }
    else
        ui->nasLE->setEnabled(true);
}

void EditNasIpDialog::on_nasLE_textChanged(const QString &arg1)
{
    if(arg1.isEmpty())
        ui->okBtn->setEnabled(false);
    else
    {
        if(ui->nasLE->hasAcceptableInput())
            ui->okBtn->setEnabled(true);
        else
            ui->okBtn->setEnabled(false);
    }

}

void EditNasIpDialog::on_cancelBtn_clicked()
{
    this->close();
}

void EditNasIpDialog::on_okBtn_clicked()
{
    QString nasIp = ui->nasLE->text();

    if(dbMan->updateBrasNasIp(id, nasIp))
        this->close();
    else
        QMessageBox::warning(this, "Error", "Cannot update NAS IP.\n"+dbMan->lastError());
}
