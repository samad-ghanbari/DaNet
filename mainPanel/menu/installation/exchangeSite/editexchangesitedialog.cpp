#include "editexchangesitedialog.h"
#include "ui_editexchangesitedialog.h"
#include "lib/database/danetdbman.h"
#include <QSqlQuery>
#include <QMessageBox>

EditExchangeSiteDialog::EditExchangeSiteDialog(QWidget *parent, DanetDbMan *db, const int Id, const int area, const QString exch, const QString abbr, const int type_no,const QString dataSaloon, const QString switchSaloon, const QString pcmSaloon, const int motherId, const int uplinkId,  const int cascade, const int node, const QString address) :
    QDialog(parent),
    ui(new Ui::EditExchangeSiteDialog),
    dbMan(db),
    oldAbbr(abbr)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowMinMaxButtonsHint);

    ui->tabWidget->tabBar()->hide();
    ui->tabWidget->setCurrentIndex(0);

    // area CB
    ui->areaCB->blockSignals(true);
    ui->areaCB->addItem("2", 2);
    ui->areaCB->addItem("3", 3);
    ui->areaCB->addItem("4", 4);
    ui->areaCB->addItem("5", 5);
    ui->areaCB->addItem("6", 6);
    ui->areaCB->addItem("7", 7);
    ui->areaCB->addItem("8", 8);
    ui->areaCB->addItem("کرج", 9);
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

    id = Id;
    ui->areaCB->setCurrentIndex(ui->areaCB->findData(area));
    ui->exchLE->setText(exch);
    ui->abbrLE->setText(abbr);
    ui->typeCB->setCurrentIndex(ui->typeCB->findData(type_no));
    ui->dataSaloonCB->setCurrentIndex(ui->dataSaloonCB->findData(dataSaloon));
    ui->switchSaloonCB->setCurrentIndex(ui->switchSaloonCB->findData(switchSaloon));
    ui->pcmSaloonCB->setCurrentIndex(ui->pcmSaloonCB->findData(pcmSaloon));

    if(type_no == 3)
    {
        ui->saloonGB->setEnabled(false);
        ui->motherCB->setCurrentIndex(ui->motherCB->findData(motherId));
        ui->uplinkExchCB->setCurrentIndex(ui->uplinkExchCB->findData(uplinkId));
        ui->cascadeSB->setValue(cascade);
        ui->nodeSB->setValue(node);
    }
    else if(type_no == 2) // exch
    {
        ui->siteGB->setEnabled(false);
        ui->saloonGB->setEnabled(true);
    }
    else
    {
        ui->motherCB->setCurrentIndex(-1);
        ui->uplinkExchCB->setCurrentIndex(-1);
        ui->siteGB->setEnabled(false);
        ui->saloonGB->setEnabled(false);
    }
    ui->addressTE->setText(address);

    ui->okBtn->setEnabled(false);
}

EditExchangeSiteDialog::~EditExchangeSiteDialog()
{
    delete ui;
}

void EditExchangeSiteDialog::checkEntry()
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
                if(dbMan->exchangeDefinability(ab,mothId) || (ab.compare(oldAbbr,Qt::CaseInsensitive)==0))
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
            if(dbMan->exchangeDefinability(ab) || (ab.compare(oldAbbr,Qt::CaseInsensitive)==0))
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

void EditExchangeSiteDialog::on_areaCB_currentIndexChanged(int index)
{
    int area = index + 2 ;
    QSqlQuery *query = dbMan->selectAreaAbbreviations(area, true); //just exchanges
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

    ui->uplinkExchCB->clear();
    ui->uplinkExchCB->blockSignals(true);
    query = dbMan->selectAreaAbbreviations(area); //all
    while(query->next())
    {
        ID = query->value(0).toInt();
        ABBR = query->value(1).toString();
        ui->uplinkExchCB->addItem(ABBR, ID);
    }
    ui->uplinkExchCB->setCurrentIndex(-1);
    ui->uplinkExchCB->blockSignals(false);

    checkEntry();
}

void EditExchangeSiteDialog::on_exchLE_textChanged(const QString &arg1)
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

void EditExchangeSiteDialog::on_abbrLE_textChanged(const QString &arg1)
{

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

void EditExchangeSiteDialog::on_typeCB_currentIndexChanged(int index)
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

void EditExchangeSiteDialog::on_addressTE_textChanged()
{
    checkEntry();
}

void EditExchangeSiteDialog::on_okBtn_clicked()
{
    int area, type,motherId = -1,uplinkId = -1, cascade = -1, node = -1;
    QString exch, abbr, mother = "",uplink="", address, data, sw, pcm;
    data = ui->dataSaloonCB->currentData().toString();
    sw = ui->switchSaloonCB->currentData().toString();
    pcm  = ui->pcmSaloonCB->currentData().toString();

    bool ok = true;

    area = ui->areaCB->currentData().toInt();
    type = ui->typeCB->currentData().toInt();
    if(type == 3)
    {
        motherId = ui->motherCB->currentData().toInt();
        mother = ui->motherCB->currentText();
        uplinkId = ui->uplinkExchCB->currentData().toInt();
        uplink = dbMan->getExchangeAbbr(uplinkId);
        cascade = ui->cascadeSB->value();
        node = ui->nodeSB->value();
        if(node == 1)
        {
            if(motherId == uplinkId)
                ok = true;
            else
                ok = false;
        }
    }

    exch = ui->exchLE->text();
    abbr = ui->abbrLE->text();
    address = ui->addressTE->toPlainText();

    QString username = dbMan->getLoggedinNameLastname();
    dbMan->insertLog(id, username, "Change Exchange To "+exch+" "+QString::number(area)+"-"+abbr);

    if(ok)
    {
        if(dbMan->updateExchange(id,area, exch, abbr, type, data, sw, pcm,motherId, mother,uplinkId, uplink, cascade, node,address))
        {
            //in case abbr changed update device names
            if(abbr.compare(oldAbbr, Qt::CaseInsensitive) != 0)
            {
                //modify device names
                dbMan->updateExchangeDevexName(id);

            }
            this->close();
        }
        else
            QMessageBox::information(this, "Error", "Can not edit exchange/site.\n"+dbMan->lastError());
    }
    else
        QMessageBox::information(this, "Error", "Mother exchange and uplink exchange should be the same in the first node.");

}

void EditExchangeSiteDialog::on_dataSaloonCB_currentIndexChanged(int index)
{
    index++;
    checkEntry();
}

void EditExchangeSiteDialog::on_switchSaloonCB_currentIndexChanged(int index)
{
    index++;
    checkEntry();
}

void EditExchangeSiteDialog::on_pcmSaloonCB_currentIndexChanged(int index)
{
    index++;
    checkEntry();
}

void EditExchangeSiteDialog::on_cascadeSB_valueChanged(int arg1)
{
    arg1++;
    checkEntry();
}

void EditExchangeSiteDialog::on_nodeSB_valueChanged(int arg1)
{
    arg1++;
    checkEntry();
}

void EditExchangeSiteDialog::on_uplinkExchCB_currentIndexChanged(int index)
{
    index++;
    int u = ui->uplinkExchCB->currentData().toInt();
    int m = ui->motherCB->currentData().toInt();
    if(m == u)
    {
        ui->nodeSB->setMinimum(1);
        ui->nodeSB->setMaximum(1);
        ui->cascadeSB->setMinimum(1);
        ui->cascadeSB->setMaximum(500);
        int c = dbMan->getExchangeMaxCascase(u);
        if(c > -1)
            ui->cascadeSB->setValue(c+1);
        ui->cascadeSB->setValue(c+1);
    }
    else
    {
        QList<int> cn = dbMan->getSiteCascadeNode(u);
        if(cn.length() == 2)
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

        ui->nodeSB->setMaximum(500);
    }
    checkEntry();
}

void EditExchangeSiteDialog::on_motherCB_currentIndexChanged(int index)
{
    index++;
    ui->uplinkExchCB->clear();

    int area = ui->areaCB->currentData().toInt();
    int ExchId = ui->motherCB->currentData().toInt();
    QSqlQuery *query = dbMan->selectAreaAbbreviations(area,ExchId);
    //`id`,`abbr`, exchange
    int ID;
    QString ABBR, EXCH;
    ui->uplinkExchCB->clear();
    ui->uplinkExchCB->blockSignals(true);
    while(query->next())
    {
        ID = query->value(0).toInt();
        ABBR = query->value(1).toString();
        EXCH = query->value(2).toString();
        ABBR = ABBR + " - "+EXCH;
        ui->uplinkExchCB->addItem(ABBR, ID);
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

void EditExchangeSiteDialog::on_exchNextBtn_clicked()
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

void EditExchangeSiteDialog::on_siteBackBtn_clicked()
{
    ui->tabWidget->setCurrentIndex(0);
}

void EditExchangeSiteDialog::on_siteNextBtn_clicked()
{
    ui->tabWidget->setCurrentIndex(2);
}

void EditExchangeSiteDialog::on_editBackBtn_clicked()
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
