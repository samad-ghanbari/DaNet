#include "removerecommendationdialog.h"
#include "ui_removerecommendationdialog.h"
#include <QMessageBox>
#include "lib/database/danetdbman.h"

RemoveRecommendationDialog::RemoveRecommendationDialog(QWidget *parent, DanetDbMan *db, const int Id, const QString lastname, const QString recom) :
    QDialog(parent),
    ui(new Ui::RemoveRecommendationDialog),
    dbMan(db),
    id(Id)
{
    ui->setupUi(this);
    ui->checkBox->setChecked(false);
    ui->okBtn->setEnabled(false);

    ui->userLbl->setText(lastname);
    ui->recomTE->setText(recom);
}

RemoveRecommendationDialog::~RemoveRecommendationDialog()
{
    delete ui;
}

void RemoveRecommendationDialog::on_checkBox_toggled(bool checked)
{
    if(checked)
        ui->okBtn->setEnabled(true);
    else
        ui->okBtn->setEnabled(false);
}

void RemoveRecommendationDialog::on_cancelBtn_clicked()
{
    this->close();
}

void RemoveRecommendationDialog::on_okBtn_clicked()
{
    if(dbMan->deleteRecommendation(id))
        this->close();
    else
        QMessageBox::warning(this, "ERROR", "Cannot remove recommendation.\n"+dbMan->lastError());
}


