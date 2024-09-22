#include "changeinterfacetypedialog.h"
#include "ui_changeinterfacetypedialog.h"
#include "lib/database/danetdbman.h"
#include <QMessageBox>
#include <QSqlQuery>

ChangeInterfaceTypeDialog::ChangeInterfaceTypeDialog(QWidget *parent, DanetDbMan *db, const int ExchId, const int IntId, const QString abbr, const QString saloon, const QString device, const QString type, const QString interface, const QString module) :
    QDialog(parent),
    ui(new Ui::ChangeInterfaceTypeDialog),
    dbMan(db),
    intId(IntId),
    exchId(ExchId),
    INTERFACE(interface),
    TYPE(type),
    ABBDEV(abbr+"-"+device)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowMinMaxButtonsHint);

    ui->abbrLbl->setText(abbr);
    ui->saloonLbl->setText(saloon);
    ui->deviceLbl->setText(device);
    ui->currTypeLbl->setText(type);
    ui->intLbl->setText(interface);
    ui->modLbl->setText(module);
    confirm = false;

    QSqlQuery *query = dbMan->selectInterfaceTypes();
    QString intT;
    ui->newTypeCB->blockSignals(true);
    while(query->next())
    {
        intT = query->value(1).toString();
        if(intT.compare(type,Qt::CaseInsensitive) != 0)
            ui->newTypeCB->addItem(intT);
    }
    ui->newTypeCB->setCurrentIndex(-1);
    ui->newTypeCB->blockSignals(false);
}

ChangeInterfaceTypeDialog::~ChangeInterfaceTypeDialog()
{
    delete ui;
}

void ChangeInterfaceTypeDialog::on_cancelBtn_clicked()
{
    this->close();
}

void ChangeInterfaceTypeDialog::on_okBtn_clicked()
{
    QString intT = ui->newTypeCB->currentText();

    if(dbMan->updateIntType(intId, intT))
    {
        //insert Log
        QString username = dbMan->getLoggedinNameLastname();
        dbMan->insertLog(exchId,username,"Change interface type of "+ABBDEV+" "+TYPE+" "+INTERFACE+" to new type "+intT);
        this->close();
    }
    else
        QMessageBox::warning(this, "ERROR", "Cannot update interface type.\n"+dbMan->lastError());
}

void ChangeInterfaceTypeDialog::on_newTypeCB_currentIndexChanged(int index)
{
    index++;
    checkInput();
}


void ChangeInterfaceTypeDialog::on_confirmChB_toggled(bool checked)
{
    if(checked)
        confirm = true;
    else
        confirm = false;

    checkInput();
}

void ChangeInterfaceTypeDialog::checkInput()
{
    if(confirm && (!ui->newTypeCB->currentText().isEmpty()))
        ui->okBtn->setEnabled(true);
    else
        ui->okBtn->setEnabled(false);
}
