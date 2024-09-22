#include "subnet3v4assignmentdialog.h"
#include "ui_subnet3v4assignmentdialog.h"
#include "lib/database/danetdbman.h"
#include "lib/netTools/nettools.h"
#include <QMessageBox>
#include <QSqlQuery>

Subnet3V4AssignmentDialog::Subnet3V4AssignmentDialog(QWidget *parent, DanetDbMan *db, const int SubnetPoolId, const QString subnet, const int prefix) :
    QDialog(parent),
    ui(new Ui::Subnet3V4AssignmentDialog),
    dbMan(db),
    subnetPoolId(SubnetPoolId)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowMinMaxButtonsHint);

    ui->netLbl->setText(subnet);
    ui->prefixLbl->setText(QString::number(prefix));
    ui->maskLbl->setText(NetTools::prefixToSubnetMask(prefix));
    ui->bcLbl->setText(NetTools::broadcast(subnet,prefix));
    ui->hostLbl->setText(QString::number(NetTools::prefixHostCount(prefix)));

    Network = subnet+"/"+QString::number(prefix);

    //area
    ui->areaCB->blockSignals(true);
    ui->areaCB->addItem("General", 1);
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


    //app
    ui->appCB->blockSignals(true);
    ui->appCB->addItem("Custom");
    ui->appCB->addItem("Interconnection");
    ui->appCB->addItem("LoopBack");
    ui->appCB->addItem("DSLAM");
    ui->appCB->addItem("BRAS");
    ui->appCB->addItem("SBC");
    ui->appCB->addItem("OLT");
    ui->appCB->addItem("MSAN");
    ui->appCB->addItem("Reserved");
    ui->appCB->addItem("Free");
    ui->appCB->setCurrentIndex(-1);
    ui->appCB->blockSignals(false);
    ui->appLE->setVisible(false);

    int workingArea = dbMan->getLoggedInDefaultArea();
    ui->areaCB->setCurrentIndex(ui->areaCB->findData(workingArea));
}

Subnet3V4AssignmentDialog::Subnet3V4AssignmentDialog(QWidget *parent, DanetDbMan *db, const int SubnetPoolId, const QString subnet, const int prefix, const int area, const int exchId, const QString ap, const QString desc) :
    QDialog(parent),
    ui(new Ui::Subnet3V4AssignmentDialog),
    dbMan(db),
    subnetPoolId(SubnetPoolId)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowMinMaxButtonsHint);

    ui->netLbl->setText(subnet);
    ui->prefixLbl->setText(QString::number(prefix));
    ui->maskLbl->setText(NetTools::prefixToSubnetMask(prefix));
    ui->bcLbl->setText(NetTools::broadcast(subnet,prefix));
    ui->hostLbl->setText(QString::number(NetTools::prefixHostCount(prefix)));

    Network = subnet+"/"+QString::number(prefix);

// area = 1 > general
//exchId = -2 > general
    //area
    ui->areaCB->blockSignals(true);
    ui->areaCB->addItem("General", 1);
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
    if(area == 1)
    {
        ui->areaCB->setCurrentIndex(0);
        ui->abbrCB->clear();
        ui->abbrCB->setEnabled(false);
    }
    else
    {
        ui->areaCB->setCurrentIndex(ui->areaCB->findData(area));
//exchId == -2 > general
        ui->abbrCB->setCurrentIndex(ui->abbrCB->findData(exchId));

    }

    //ap
    ui->appCB->blockSignals(true);
    ui->appCB->addItem("Custom");
    ui->appCB->addItem("Interconnection");
    ui->appCB->addItem("LoopBack");
    ui->appCB->addItem("DSLAM");
    ui->appCB->addItem("BRAS");
    ui->appCB->addItem("SBC");
    ui->appCB->addItem("OLT");
    ui->appCB->addItem("MSAN");
    ui->appCB->addItem("Reserved");
    ui->appCB->addItem("Free");
    ui->appCB->setCurrentIndex(-1);
    ui->appCB->blockSignals(false);
    ui->appLE->setVisible(false);

    if(ui->appCB->findText(ap.trimmed()) > -1)
        ui->appCB->setCurrentText(ap.trimmed());
    else
    {
        ui->appCB->setCurrentIndex(0);
        ui->appLE->setText(ap);
    }

    ui->descLE->blockSignals(true);
    ui->descLE->setText(desc);
    ui->descLE->blockSignals(false);

    ui->okBtn->setEnabled(false);

}

Subnet3V4AssignmentDialog::~Subnet3V4AssignmentDialog()
{
    delete ui;
}

void Subnet3V4AssignmentDialog::entryCheck()
{
    bool ar = !ui->areaCB->currentText().isEmpty();
    bool ab = !ui->abbrCB->currentText().isEmpty();
    bool des = !ui->descLE->text().isEmpty();
    int area = ui->areaCB->currentData().toInt();
    bool a = !ui->appCB->currentText().isEmpty();
    bool ap = !ui->appLE->text().isEmpty();

    bool con = des  && ar && a;
    if(area > 1)
        con = con && ab;

    if(ui->appCB->currentIndex() == 0)
        con = con && ap;

    if( con )
        ui->okBtn->setEnabled(true);
    else
        ui->okBtn->setEnabled(false);
}

void Subnet3V4AssignmentDialog::on_areaCB_currentIndexChanged(int index)
{
    index++;
    ui->abbrCB->clear();

    int area = ui->areaCB->currentData().toInt();
    if(area > 1)
    {
        QSqlQuery *query = dbMan->selectExchAndSites(area);
        int id;
        QString exch;
        ui->abbrCB->setEnabled(true);
        ui->abbrCB->addItem("General", -2);
        while(query->next())
        {
            id = query->value(0).toInt();
            exch = query->value(1).toString();
            ui->abbrCB->addItem(exch, id);
        }
    }
    else
    {
        ui->abbrCB->setEnabled(false);
    }

    entryCheck();
}

void Subnet3V4AssignmentDialog::on_descLE_textChanged(const QString &arg1)
{
    if(arg1.isEmpty())
        ui->okBtn->setEnabled(false);
    else
        entryCheck();
}

void Subnet3V4AssignmentDialog::on_cancelBtn_clicked()
{
    this->close();
}

void Subnet3V4AssignmentDialog::on_okBtn_clicked()
{
    int area = ui->areaCB->currentData().toInt();
    int exchId = ui->abbrCB->currentData().toInt();
    QString ap = ui->appCB->currentText();
    if(ui->appCB->currentIndex() == 0)
        ap = ui->appLE->text();
    QString desc = ui->descLE->text();
    QString user = dbMan->getLoggedinNameLastname();

    if(area == 1) area = -1; // general
    if(exchId == -2) exchId = -1;//general

    if(dbMan->updateSubnetPool3(subnetPoolId,area,exchId,ap,desc))
    {
        dbMan->insertLog(user,"Assign Subnet3: "+Network+" desc: "+desc);
        this->close();
    }
    else
        QMessageBox::warning(this,"ERROR", "Cannot assign network.\n"+dbMan->lastError());
}

void Subnet3V4AssignmentDialog::on_abbrCB_currentIndexChanged(int index)
{
    index++;
    entryCheck();
}

void Subnet3V4AssignmentDialog::on_appCB_currentIndexChanged(int index)
{
    if(index == 0)
    {
        ui->appLE->setEnabled(true);
        ui->appLE->setVisible(true);
    }
    else
    {
        ui->appLE->setEnabled(false);
        ui->appLE->setVisible(false);
    }
    entryCheck();
}

void Subnet3V4AssignmentDialog::on_appLE_textChanged(const QString &arg1)
{
    if(arg1.isEmpty())
        entryCheck();
    else
        entryCheck();
}
