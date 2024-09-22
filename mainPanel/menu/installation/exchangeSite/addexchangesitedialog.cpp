#include "addexchangesitedialog.h"
#include "ui_addexchangesitedialog.h"
#include "lib/database/danetdbman.h"
#include <QSqlQuery>
#include <QMessageBox>

AddExchangeSiteDialog::AddExchangeSiteDialog(QWidget *parent, DanetDbMan *db, bool JustSite) :
    QDialog(parent),
    ui(new Ui::AddExchangeSiteDialog),
    dbMan(db),
    justSite(JustSite)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowMinMaxButtonsHint);


    ui->tabWidget->tabBar()->hide();

    ui->siteAbbrErrorLbl->hide();

    ADDED = false;

    // area CB
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


    //type CB
    ui->typeCB->blockSignals(true);
    ui->typeCB->addItem("اداره کل", 1);
    ui->typeCB->addItem("مرکز", 2);
    ui->typeCB->addItem("سایت", 3);
    ui->typeCB->setCurrentIndex(-1);
    ui->typeCB->blockSignals(false);
    //saloon orientation
    ui->dataSaloonCB->addItem("Top-Left", "TL");
    ui->dataSaloonCB->addItem("Top-Right", "TR");
    ui->dataSaloonCB->addItem("Bottom-Left", "BL");
    ui->dataSaloonCB->addItem("Bottom-Right", "BR");

    ui->switchSaloonCB->addItem("Top-Left", "TL");
    ui->switchSaloonCB->addItem("Top-Right", "TR");
    ui->switchSaloonCB->addItem("Bottom-Left", "BL");
    ui->switchSaloonCB->addItem("Bottom-Right", "BR");

    ui->pcmSaloonCB->addItem("Top-Left", "TL");
    ui->pcmSaloonCB->addItem("Top-Right", "TR");
    ui->pcmSaloonCB->addItem("Bottom-Left", "BL");
    ui->pcmSaloonCB->addItem("Bottom-Right", "BR");


    int workingArea = dbMan->getLoggedInDefaultArea();
    ui->areaCB->setCurrentIndex(ui->areaCB->findData(workingArea));

    ui->typeCB->setCurrentIndex(1);

    if(JustSite)
    {
        ui->typeCB->setEnabled(false);
        ui->typeCB->setCurrentIndex(2);
    }

}

AddExchangeSiteDialog::~AddExchangeSiteDialog()
{
    delete ui;
}

void AddExchangeSiteDialog::checkEntry()
{
    bool Area = !ui->areaCB->currentText().isEmpty();
    bool Exch = !ui->exchLE->text().isEmpty();
    bool Abbr = !ui->abbrLE->text().isEmpty();
    bool Type = !ui->typeCB->currentText().isEmpty();
    int typeVal = ui->typeCB->currentData().toInt();
    bool Mother = !ui->motherCB->currentText().isEmpty();
    bool Up = !ui->uplinkExchCB->currentText().isEmpty();

    bool ok = Area && Exch && Abbr && Type;
    bool exchDefinable = false;
    ui->siteAbbrErrorLbl->hide();


    if(ok)
    {
        QString ab = ui->abbrLE->text().trimmed();
        if(ui->typeCB->currentData().toInt() == 3)
        {
            //site
            ui->abbrLE->setStyleSheet("QLineEdit{color: rgb(85,0,127)}");
            int mothId = ui->motherCB->currentData().toInt();
            if(ui->motherCB->currentText().isEmpty())
            {
                ui->exchNextBtn->setEnabled(true);
                exchDefinable = true;
            }
            else
            {
                if(dbMan->exchangeDefinability(ab,mothId))
                {
                    ui->abbrLE->setStyleSheet("QLineEdit{color: rgb(0,200,0)}");
                    ui->exchNextBtn->setEnabled(true);
                    exchDefinable = true;
                }
                else
                {
                    ui->abbrLE->setStyleSheet("QLineEdit{color: rgb(200,0,0)}");
                    exchDefinable = false;
                    ui->siteAbbrErrorLbl->show();

                }
            }
        }
        else
        {
            //exch
            if(dbMan->exchangeDefinability(ab))
            {
                ui->abbrLE->setStyleSheet("QLineEdit{color: rgb(0,200,0)}");
                ui->exchNextBtn->setEnabled(true);
                exchDefinable = true;
            }
            else
            {
                ui->abbrLE->setStyleSheet("QLineEdit{color: rgb(200,0,0)}");
                exchDefinable = false;
            }
        }
    }

    if(exchDefinable)
    {
        if(Mother && Up)
            ui->siteNextBtn->setEnabled(true);
        else
            ui->siteNextBtn->setEnabled(false);

        if(typeVal == 3)
            ok = ok && Mother && Up;

        if(ok)
            ui->okBtn->setEnabled(true);
        else
            ui->okBtn->setEnabled(false);;
    }
    else
    {
        ui->exchNextBtn->setEnabled(false);
        ui->siteNextBtn->setEnabled(false);
        ui->okBtn->setEnabled(false);
    }
}

QString AddExchangeSiteDialog::getAbbr()
{
    return (ui->abbrLE->text());
}

int AddExchangeSiteDialog::getMotherId()
{
    if(ui->typeCB->currentData().toInt() == 3)
        return (ui->motherCB->currentData().toInt());
    else
        return -1;
}

bool AddExchangeSiteDialog::isDone()
{
    return ADDED;
}

void AddExchangeSiteDialog::on_areaCB_currentIndexChanged(int index)
{
    int area = index + 2;
    ui->typeCB->setCurrentIndex(1);
    QSqlQuery *query = dbMan->selectAreaAbbreviations(area, true);
    //`id`,`abbr`
    int ID;
    QString ABBR;
    ui->motherCB->clear();
    ui->motherCB->blockSignals(true);
    while(query->next())
    {
        ID = query->value(0).toInt();
        ABBR = query->value(1).toString();
        ui->motherCB->addItem(ABBR, ID);
    }
    ui->motherCB->setCurrentIndex(-1);
    ui->motherCB->blockSignals(false);

    checkEntry();
    if(justSite)
        ui->typeCB->setCurrentIndex(2);
}

void AddExchangeSiteDialog::on_typeCB_currentIndexChanged(int index)
{
    ui->siteGB->setEnabled(false);
    ui->saloonGB->setEnabled(false);
    ui->motherCB->setCurrentIndex(-1);
    ui->uplinkExchCB->setCurrentIndex(-1);

    if(index == 0)
    {
        ui->siteGB->setEnabled(false);
        ui->saloonGB->setEnabled(false);
    }else if(index == 1)// exch
    {
        ui->siteGB->setEnabled(false);
        ui->saloonGB->setEnabled(true);
    }else if(index == 2)//site
    {
        ui->siteGB->setEnabled(true);
        ui->saloonGB->setEnabled(false);
    }

    checkEntry();
}

void AddExchangeSiteDialog::on_exchLE_textChanged(const QString &arg1)
{
    if(arg1.isEmpty())
    {
        ui->exchLE->setStyleSheet("QLineEdit{color: rgb(200,0,0)}");
        ui->exchNextBtn->setEnabled(false);
    }
    else
    {
        ui->exchLE->setStyleSheet("QLineEdit{color: rgb(85,0,127)}");
        checkEntry();
    }
}

void AddExchangeSiteDialog::on_abbrLE_textChanged(const QString &arg1)
{
    //    QString str = arg1;
    //    str = str.replace(" ", "");
    //    str = str.trimmed();
    //    ui->abbrLE->setText(str);

    if(arg1.isEmpty())
    {
        ui->abbrLE->setStyleSheet("QLineEdit{color: rgb(200,0,0)}");
        ui->exchNextBtn->setEnabled(false);

    }
    else
    {
        QString ab = ui->abbrLE->text().trimmed();
        if(ab.contains("/"))
            ab.replace("/", "-");
        if(ab.contains("\\"))
            ab.replace("\\", "-");
        ui->abbrLE->setText(ab);
        checkEntry();
    }
}

void AddExchangeSiteDialog::on_addressTE_textChanged()
{
    checkEntry();
}

void AddExchangeSiteDialog::on_okBtn_clicked()
{
    int area, type,motherId,uplinkId, cascade,node;
    QString exch, abbr, address, mother, uplink, data, sw, pcm;
    area = ui->areaCB->currentData().toInt();
    exch = ui->exchLE->text();
    abbr = ui->abbrLE->text();
    type = ui->typeCB->currentData().toInt();
    data = ui->dataSaloonCB->currentData().toString();
    sw = ui->switchSaloonCB->currentData().toString();
    pcm = ui->pcmSaloonCB->currentData().toString();

    int typeNo = ui->typeCB->currentData().toInt();

    if( typeNo == 3) // site
    {
        motherId = ui->motherCB->currentData().toInt();
        mother = ui->motherCB->currentText();
        uplinkId = ui->uplinkExchCB->currentData().toInt();
        uplink = dbMan->getExchangeAbbr(uplinkId);
        cascade = ui->cascadeSB->value();
        node = ui->nodeSB->value();
        address = ui->addressTE->toPlainText();
        if(node == 1)
        {
            if(motherId == uplinkId)
            {
                if(dbMan->insertSite(area, exch, abbr, type,motherId, mother,uplinkId, uplink, cascade, node, address))
                {
                    ADDED = true;
                    this->close();
                }
                else
                    QMessageBox::information(this, "Error", "Can not Insert Site.\n"+dbMan->lastError());
            }
            else
            {
                QMessageBox::information(this, "Error", "Mother exchange and uplink exchange should be the same in the first node.");
            }
        }
        else
        {
            if(dbMan->insertSite(area, exch, abbr, type,motherId, mother,uplinkId, uplink, cascade, node, address))
            {
                ADDED = true;
                this->close();
            }
            else
                QMessageBox::information(this, "Error", "Can not Insert Site.\n"+dbMan->lastError());
        }
    }
    else if(typeNo == 2)//exch
    {
        address = ui->addressTE->toPlainText();

        if(dbMan->insertExchange(area, exch, abbr, type,data, sw,pcm, address))
        {
            ADDED = true;
            this->close();
        }
        else
            QMessageBox::information(this, "Error", "Can not Insert Exchange.\n"+dbMan->lastError());
    }
    else if(typeNo == 1)
    {
        address = ui->addressTE->toPlainText();

        if(dbMan->insertExchange(area, exch, abbr, type, address))
        {
            ADDED = true;
            this->close();
        }
        else
            QMessageBox::information(this, "Error", "Can not Insert Exchange.\n"+dbMan->lastError());
    }
}

void AddExchangeSiteDialog::on_dataSaloonCB_currentIndexChanged(int index)
{
    index++;
    checkEntry();
}

void AddExchangeSiteDialog::on_switchSaloonCB_currentIndexChanged(int index)
{
    index++;
    checkEntry();
}

void AddExchangeSiteDialog::on_pcmSaloonCB_currentIndexChanged(int index)
{
    index++;
    checkEntry();
}

void AddExchangeSiteDialog::on_uplinkExchCB_currentIndexChanged(int index)
{
    index++;
    int u = ui->uplinkExchCB->currentData().toInt();
    int m = ui->motherCB->currentData().toInt();
    if(m == u)
    {
        ui->nodeSB->setValue(1);
        ui->nodeSB->setMinimum(1);
        ui->nodeSB->setMaximum(1);
        ui->cascadeSB->setMinimum(1);
        ui->cascadeSB->setMaximum(500);
        int c = dbMan->getExchangeMaxCascase(u);
        if(c > -1)
            ui->cascadeSB->setValue(c+1);
        else
            ui->cascadeSB->setValue(1);
    }
    else
    {
        ui->nodeSB->setMaximum(500);

        QList<int> cn = dbMan->getSiteCascadeNode(u);
        if(cn.at(0) > -1)
        {
            ui->cascadeSB->setValue(cn.at(0));
            ui->cascadeSB->setMinimum(cn.at(0));
            ui->cascadeSB->setMaximum(cn.at(0));

            ui->nodeSB->setMinimum(cn.at(1)+1);
            ui->nodeSB->setValue(cn.at(1)+1);
        }
        else
        {
            ui->cascadeSB->setValue(1);
            ui->cascadeSB->setMinimum(1);
            ui->cascadeSB->setMaximum(500);

            ui->nodeSB->setMinimum(2);
        }


    }
    checkEntry();
}

void AddExchangeSiteDialog::on_motherCB_currentIndexChanged(int index)
{
    index++;
    ui->uplinkExchCB->clear();

    int ExchId = ui->motherCB->currentData().toInt();
    QString mother = ui->motherCB->currentText();
    QSqlQuery *query = dbMan->selectSites(ExchId);
    //`id`,`abbr`, `exchange`, `site_cascade`, `site_node`
    int i;
    QString a, s,c,n;
    ui->uplinkExchCB->clear();
    ui->uplinkExchCB->blockSignals(true);
    ui->uplinkExchCB->addItem(mother, ExchId);
    while(query->next())
    {
        i = query->value(0).toInt();
        a = query->value(1).toString();
        s = query->value(2).toString();
        c = query->value(3).toString();
        n = query->value(4).toString();
        ui->uplinkExchCB->addItem(c+"-"+n+"*"+a+"*"+s, i);
    }
    ui->uplinkExchCB->setCurrentIndex(-1);
    ui->uplinkExchCB->blockSignals(false);


    int u = ui->uplinkExchCB->currentData().toInt();
    int m = ui->motherCB->currentData().toInt();
    if(m == u)
    {
        ui->nodeSB->setMinimum(1);
        ui->nodeSB->setMaximum(1);
    }
    else
    {
        ui->nodeSB->setMinimum(2);
        ui->nodeSB->setMaximum(99);
    }
    checkEntry();
}

void AddExchangeSiteDialog::on_cascadeSB_valueChanged(int arg1)
{
    arg1++;
    checkEntry();
}

void AddExchangeSiteDialog::on_nodeSB_valueChanged(int arg1)
{
    arg1++;
    checkEntry();
}

void AddExchangeSiteDialog::on_addBackBtn_clicked()
{
    int siteData = ui->typeCB->currentData().toInt();
    if(siteData == 3)
    {
        //site
        ui->tabWidget->setCurrentIndex(1);
    }
    else
        ui->tabWidget->setCurrentIndex(0);
}

void AddExchangeSiteDialog::on_siteNextBtn_clicked()
{
    ui->tabWidget->setCurrentIndex(2);
}

void AddExchangeSiteDialog::on_siteBackBtn_clicked()
{
    ui->tabWidget->setCurrentIndex(0);
}

void AddExchangeSiteDialog::on_exchNextBtn_clicked()
{
    int siteData = ui->typeCB->currentData().toInt();
    if(siteData == 3)
    {
        //site
        ui->tabWidget->setCurrentIndex(1);
    }
    else
        ui->tabWidget->setCurrentIndex(2);
}
