#include "changemoduledialog.h"
#include "ui_changemoduledialog.h"
#include "lib/database/danetdbman.h"
#include <QMessageBox>
#include <QSqlQuery>

ChangeModuleDialog::ChangeModuleDialog(QWidget *parent, DanetDbMan *db, const int IntId, const QString abbr, const QString saloon, const QString device, const QString type, const QString interface, const QString module) :
    QDialog(parent),
    ui(new Ui::ChangeModuleDialog),
    dbMan(db),
    intId(IntId)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowMinMaxButtonsHint);

    ui->abbrLbl->setText(abbr);
    ui->saloonLbl->setText(saloon);
    ui->deviceLbl->setText(device);
    ui->typeLbl->setText(type);
    ui->intLbl->setText(interface);
    ui->currModLbl->setText(module);

    QSqlQuery *query = dbMan->selectModules();
    QString mod;
    ui->moduleCB->blockSignals(true);
    ui->moduleCB->addItem("");
    while(query->next())
    {
        mod = query->value(1).toString();
        if(mod.compare(module,Qt::CaseInsensitive) != 0)
            ui->moduleCB->addItem(mod);
    }
    ui->moduleCB->setCurrentIndex(-1);
    ui->moduleCB->blockSignals(false);
}

ChangeModuleDialog::~ChangeModuleDialog()
{
    delete ui;
}

void ChangeModuleDialog::on_moduleCB_currentIndexChanged(int index)
{
    if(index > -1)
        ui->okBtn->setEnabled(true);
    else
        ui->okBtn->setEnabled(false);
}

void ChangeModuleDialog::on_cancelBtn_clicked()
{
    this->close();
}

void ChangeModuleDialog::on_okBtn_clicked()
{
    QString mod = ui->moduleCB->currentText();

    if(dbMan->updateInterfaceModule(intId, mod))
        this->close();
    else
        QMessageBox::warning(this, "ERROR", "Cannot update module.\n"+dbMan->lastError());
}
