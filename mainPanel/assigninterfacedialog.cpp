#include "assigninterfacedialog.h"
#include "ui_assigninterfacedialog.h"
#include "lib/database/danetdbman.h"
#include <QMessageBox>
#include <QIntValidator>

AssignInterfaceDialog::AssignInterfaceDialog(QWidget *parent, DanetDbMan *db, int IntId, QString Device, QString Int, QString Label, QString module) :
    QDialog(parent),
    ui(new Ui::AssignInterfaceDialog),
    query(NULL),
    dbMan(db),
    intId(IntId),
    ethValidator(new QIntValidator(this))
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowMinMaxButtonsHint);

    ui->intLbl->setText(Int);
    ui->labelLbl->setText(Label);
    devName = Device;

    query = dbMan->selectModules();
    int i;
    QString m;
    while(query->next())
    {
        i = query->value(0).toInt();
        m = query->value(1).toString();

        ui->moduleCB->addItem(m, i);
    }

    ui->moduleCB->setCurrentText(module);

    ui->areaCB->blockSignals(true);
    ui->areaCB->addItem("Custom", 1);
    ui->areaCB->addItem("2", 2);
    ui->areaCB->addItem("3", 3);
    ui->areaCB->addItem("4", 4);
    ui->areaCB->addItem("5", 5);
    ui->areaCB->addItem("6", 6);
    ui->areaCB->addItem("7", 7);
    ui->areaCB->addItem("8", 8);
    ui->areaCB->addItem( "Karaj", 9);
    ui->areaCB->setCurrentIndex(-1);
    ui->areaCB->blockSignals(false);

    ui->peerExchTypeCB->addItem("Exchange", 1);
    ui->peerExchTypeCB->addItem("Site", 2);

    ui->peerDevLE->setVisible(false);
    ui->peerExchLE->setVisible(false);

    ui->etherLE->setValidator(ethValidator);
}

AssignInterfaceDialog::~AssignInterfaceDialog()
{
    delete ui;
}

QString AssignInterfaceDialog::getLable()
{
    QString dev = "";

    if(ui->areaCB->currentIndex() == 0)
    {
            if(!ui->peerExchLE->text().isEmpty())
                dev = ui->peerExchLE->text();

        if(!ui->peerDevLE->text().isEmpty())
            dev += "- "+ui->peerDevLE->text();

        if(!ui->peerIntLE->text().isEmpty())
            dev += "- "+ui->peerIntLE->text();
    }
    else
    {
        if(ui->peerDevCB->isVisible())
        {
            if(ui->peerDevCB->currentIndex() == 0) //custom
            {
                ui->peerDevLE->setVisible(true);
                if(ui->areaCB->currentIndex() == 0)
                    dev = "";
                else
                    dev = ui->areaCB->currentData().toString();

                if(ui->peerExchCB->currentIndex() == 0)
                {
                    if(!ui->peerExchLE->text().isEmpty())
                        dev += "- "+ui->peerExchLE->text();
                }
                else
                {
                    dev += "- "+ui->peerExchCB->currentText();
                    if(ui->peerExchTypeCB->currentIndex() == 1)
                    {
                        if(!ui->peerSiteCB->currentText().isEmpty())
                        dev += "- "+ui->peerSiteCB->currentText();
                    }
                }

                if(!ui->peerDevLE->text().isEmpty())
                    dev += "- "+ui->peerDevLE->text();

                if(!ui->peerIntLE->text().isEmpty())
                    dev += "- "+ui->peerIntLE->text();

            }
            else if(ui->peerDevCB->currentIndex() > 0)
            {
                ui->peerDevLE->clear();
                ui->peerDevLE->setVisible(false);

                dev = ui->peerDevCB->currentText();

                if(!dev.trimmed().isEmpty())
                {
                    if(!ui->peerIntLE->text().isEmpty())
                        dev += "- "+ui->peerIntLE->text();
                }
            }
            else
            {
                dev = "";
            }
        }
        else
        {
            ui->peerDevLE->setVisible(true);
            if(ui->areaCB->currentIndex() == 0)
                dev = "";
            else
                dev = ui->areaCB->currentData().toString();

            if(ui->peerExchCB->currentIndex() == 0)
            {
                if(!ui->peerExchLE->text().isEmpty())
                    dev += "- "+ui->peerExchLE->text();
            }
            else
            {
                dev += "- "+ui->peerExchCB->currentText();
                if(ui->peerExchTypeCB->currentIndex() == 1)
                {
                    if(!ui->peerSiteCB->currentText().isEmpty())
                    dev += "- "+ui->peerSiteCB->currentText();
                }
            }

            if(!ui->peerDevLE->text().isEmpty())
                dev += "- "+ui->peerDevLE->text();

            if(!ui->peerIntLE->text().isEmpty())
                dev += "- "+ui->peerIntLE->text();
        }
    }


    return dev;
}

void AssignInterfaceDialog::checkEntry()
{
    bool E, D;
    if(ui->etherLE->text().trimmed().isEmpty())
        E = true;
    else
        E = ui->etherLE->hasAcceptableInput();

    bool l = !ui->peerLblLE->text().isEmpty();
    bool i = !ui->peerIntLE->text().isEmpty();


    if(ui->peerDevCB->currentText().isEmpty())
        D = false;
    else
    {
        if(ui->peerDevCB->currentIndex() == 0)
            {
                if(ui->peerDevLE->text().isEmpty())
                    D = false;
                else
                    D = true;
            }
            else
                D = true;
    }

    if(!ui->peerDevCB->isVisible())
    {
        if(ui->peerDevLE->text().isEmpty())
            D = false;
        else
            D = true;
    }

    if(l && i &&  E && D)
        ui->okBtn->setEnabled(true);
    else
        ui->okBtn->setEnabled(false);
}

void AssignInterfaceDialog::on_cancelBtn_clicked()
{
    this->close();
}

void AssignInterfaceDialog::on_okBtn_clicked()
{
    QString module = ui->moduleCB->currentText();
    QString peerExch = "", peerDev="", peerInt = "", peerLbl="";
    QString desc = ui->descTE->toPlainText().trimmed();

    if(ui->areaCB->currentIndex() == 0)
    {
        peerExch = ui->peerExchLE->text();
        peerDev = ui->peerDevLE->text();
    }
    else
    {
        if(ui->peerExchCB->currentIndex() > 0)
        {
            peerExch = ui->areaCB->currentData().toString()+ "- "+ui->peerExchCB->currentText();
            if(ui->peerExchTypeCB->currentIndex() == 1)
            {
                //site
                peerExch += "- "+ui->peerSiteCB->currentText();
            }

            if(ui->peerDevCB->currentIndex() == 0)
                peerDev = ui->peerDevLE->text();
            else
                peerDev = ui->peerDevCB->currentText();

        }
        else
        {
            peerExch = ui->peerExchLE->text();
            peerDev = ui->peerDevLE->text();
        }
    }

    peerInt = ui->peerIntLE->text();
    peerLbl = ui->peerLblLE->text();

    int eth;
    if(ui->etherLE->text().trimmed().isEmpty())
        eth = -1;
    else
        eth = ui->etherLE->text().toInt();

    //log
    QString localDevice = devName;
    QString Interface = ui->intLbl->text();
    int exchId = dbMan->getInterfaceExchangeId(intId);
    QString username = dbMan->getLoggedinNameLastname();

    if(dbMan->updateInterface(intId, module, peerExch, peerDev,peerInt, peerLbl,eth, desc))
    {
        dbMan->insertLog(exchId, username, "Assign Interface of "+localDevice+" "+Interface+" To "+peerLbl);
        this->close();
    }
    else
        QMessageBox::warning(this, "Error","Can not assign interface.\n"+dbMan->lastError());
}

void AssignInterfaceDialog::on_areaCB_currentIndexChanged(int index)
{
    ui->peerExchCB->clear();
    ui->peerExchLE->clear();
    ui->peerSiteCB->clear();
    ui->peerExchTypeCB->setCurrentIndex(0);
    ui->peerDevCB->clear();
    ui->peerDevLE->clear();
    ui->peerLblLE->clear();
    ui->peerIntLE->clear();


    if(index == 0)
    {
        ui->peerExchCB->setVisible(false);
        ui->peerExchTypeCB->setVisible(false);
        ui->peerExchLE->setVisible(true);
        ui->peerExchTypeCB->setCurrentIndex(0);
        ui->siteFrame->setVisible(false);
        ui->peerDevCB->setVisible(false);
        ui->peerDevLE->setVisible(true);
    }
    else
    {

        ui->peerExchCB->setVisible(true);
        ui->peerExchTypeCB->setVisible(true);
        ui->peerExchLE->setVisible(false);
        ui->peerExchTypeCB->setCurrentIndex(0);
        ui->siteFrame->setVisible(false);
        ui->peerDevCB->setVisible(true);
        ui->peerDevLE->setVisible(false);

        int area = ui->areaCB->currentData().toInt();
        ui->peerExchCB->blockSignals(true);
        ui->peerExchCB->addItem("Custom",0);
        query = dbMan->selectAreaAbbreviations(area, true);
        QString abbr;
        int id;
        while(query->next())
        {
            id = query->value(0).toInt();
            abbr = query->value(1).toString();
            ui->peerExchCB->addItem(abbr,id);
        }
        ui->peerExchCB->setCurrentIndex(-1);
        ui->peerExchCB->blockSignals(false);
    }


}

void AssignInterfaceDialog::on_peerExchCB_currentIndexChanged(int index)
{
    ui->peerExchLE->clear();
    ui->peerDevLE->clear();
    ui->peerExchTypeCB->setCurrentIndex(0);
    ui->peerSiteCB->clear();
    ui->peerIntLE->clear();
    ui->peerLblLE->clear();


    if(index == 0)
    {
        ui->peerExchTypeCB->setVisible(false);
        ui->peerExchLE->setVisible(true);
        ui->peerDevCB->setVisible(false);
        ui->peerDevLE->setVisible(true);
    }
    else
    {
        ui->peerExchTypeCB->setVisible(true);
        ui->peerExchLE->setVisible(false);
        ui->peerDevCB->setVisible(true);
        ui->peerDevLE->setVisible(false);

        int exchId = ui->peerExchCB->currentData().toInt();
        query = dbMan->selectBriefDevex(exchId);
        //`id`, `device_name`
        int id;
        ui->peerDevCB->blockSignals(true);
        ui->peerDevCB->clear();
        ui->peerDevCB->addItem("Custom", 0);
        QString dev, pur, t,sn, dvType0, dvType1;
        while(query->next())
        {
            id = query->value(0).toInt();
            dev = query->value(1).toString();
            pur = query->value(2).toString();
            dvType1 = query->value(5).toString();
            if(dvType1.contains("AGG", Qt::CaseInsensitive))
                dvType1 = "AGG";
            if(dvType1.contains("BRAS", Qt::CaseInsensitive))
                dvType1 = "BRAS";
            if(dvType0.isEmpty())
                dvType0 = dvType1;
            if(dvType1.compare(dvType0) != 0)
            {
                ui->peerDevCB->insertSeparator(ui->peerDevCB->count());
                dvType0 = dvType1;
            }

            if(query->value(3).isNull() || query->value(4).isNull())
            {
                ui->peerDevCB->addItem(dev,id);
            }
            else
            {
                t = query->value(3).toString();
                sn = query->value(4).toString();
                dev += "-"+t+"-"+pur+"-"+sn;
                ui->peerDevCB->addItem(dev,id);
            }
        }


        ui->peerDevCB->setCurrentIndex(-1);
        ui->peerDevCB->blockSignals(false);
        ui->peerDevCB->setEnabled(true);

    }

}

void AssignInterfaceDialog::on_peerExchLE_textChanged(const QString &arg1)
{
    ui->peerLblLE->clear();

    if(arg1.isEmpty())
    {
        ui->okBtn->setEnabled(false);
    }
    else
    {
        ui->peerLblLE->setText(getLable());
        checkEntry();
    }
}

void AssignInterfaceDialog::on_peerDevCB_currentIndexChanged(int index)
{
    index++;
    ui->peerLblLE->clear();
    ui->peerDevLE->clear();
    ui->peerLblLE->setText(getLable());
    checkEntry();

}

void AssignInterfaceDialog::on_peerDevLE_textChanged(const QString &arg1)
{
    if(arg1.isEmpty())
        ui->peerLblLE->setText(getLable());
    else
        ui->peerLblLE->setText(getLable());

    checkEntry();

}

void AssignInterfaceDialog::on_peerIntLE_textChanged(const QString &arg1)
{
    ui->peerLblLE->setText(getLable());

    if(arg1.isEmpty())
        ui->okBtn->setEnabled(false);
    else
    {
        checkEntry();
    }

}

void AssignInterfaceDialog::on_peerLblLE_textChanged(const QString &arg1)
{
    if(arg1.isEmpty())
        ui->okBtn->setEnabled(false);
    else
        checkEntry();
}

void AssignInterfaceDialog::on_peerExchTypeCB_currentIndexChanged(int index)
{
    ui->peerSiteCB->blockSignals(true);
    ui->peerSiteCB->clear();

    ui->peerIntLE->blockSignals(true);
    ui->peerLblLE->blockSignals(true);
    ui->peerDevLE->blockSignals(true);
    ui->peerIntLE->clear();
    ui->peerLblLE->clear();
    ui->peerDevLE->clear();
    ui->peerIntLE->blockSignals(false);
    ui->peerLblLE->blockSignals(false);
    ui->peerDevLE->blockSignals(false);

    if(index == 1) // site
    {
        ui->siteFrame->setEnabled(true);
        ui->siteFrame->setVisible(true);


        int area = ui->areaCB->currentData().toInt();
        QString mother = ui->peerExchCB->currentText();

        QSqlQuery *query = dbMan->selectSites(area, mother);
        //`id`,`abbr`, `exchange`, `site_cascade`, `site_node`
        int i;
        QString a;
        while(query->next())
        {
            i = query->value(0).toInt();
            a = query->value(1).toString();
            ui->peerSiteCB->addItem(a, i);
        }
        ui->peerSiteCB->setCurrentIndex(-1);
        ui->peerSiteCB->blockSignals(false);
        ui->peerSiteCB->setEnabled(true);

        ui->peerDevCB->blockSignals(true);
        ui->peerDevCB->clear();
        ui->peerDevCB->blockSignals(false);

    }
    else // exchange
    {
        ui->peerSiteCB->blockSignals(true);
        ui->peerSiteCB->clear();
        ui->peerSiteCB->blockSignals(false);
        ui->siteFrame->setEnabled(false);
        ui->siteFrame->setVisible(false);

        ui->peerDevCB->blockSignals(true);
        ui->peerDevCB->clear();
        ui->peerDevCB->addItem("Custom", 0);

        int exchId = ui->peerExchCB->currentData().toInt();
        query = dbMan->selectBriefDevex(exchId);
        int id;
        QString device;
    //`id`, `device_name`
        while(query->next())
        {
            id = query->value(0).toInt();
            device = query->value(1).toString();
            ui->peerDevCB->addItem(device, id);
        }

        ui->peerDevCB->setCurrentIndex(-1);
        ui->peerDevCB->blockSignals(false);

        ui->peerDevLE->setVisible(false);

    }

    ui->peerSiteCB->blockSignals(false);
}

void AssignInterfaceDialog::on_peerSiteCB_currentIndexChanged(int index)
{
    index++;

    ui->peerDevCB->blockSignals(true);
    ui->peerDevCB->clear();
    ui->peerDevCB->addItem("Custom", 0);

    int exchId = ui->peerSiteCB->currentData().toInt();
    query = dbMan->selectBriefDevex(exchId);
    int id;
    QString device;
//`id`, `device_name`
    while(query->next())
    {
        id = query->value(0).toInt();
        device = query->value(1).toString();
        ui->peerDevCB->addItem(device, id);
    }

    ui->peerDevCB->setCurrentIndex(-1);
    ui->peerDevCB->blockSignals(false);

    ui->peerIntLE->clear();
    ui->peerLblLE->clear();

    ui->peerDevLE->clear();
    ui->peerDevLE->setVisible(false);
}

