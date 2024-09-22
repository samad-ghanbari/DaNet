#include "sbciploopbackdialog.h"
#include "ui_sbciploopbackdialog.h"
#include "lib/database/danetdbman.h"
#include "lib/netTools/nettools.h"
#include <QMessageBox>
#include <QRegExpValidator>
#include <QSqlQuery>
#include <QFileDialog>
#include <QPrinter>
#include <QDateTime>
#include <QTextEdit>
#include <QFile>
#include <QDir>

SbcIpLoopBackDialog::SbcIpLoopBackDialog(QWidget *parent, DanetDbMan *db) :
    QDialog(parent),
    ui(new Ui::SbcIpLoopBackDialog),
    dbMan(db),
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

    ui->remoteIpLE->setValidator(ipValidator);
    ui->lbNetLE->setValidator(ipValidator);
    ui->lbMaskLE->setValidator(ipValidator);

    int workingArea = dbMan->getLoggedInDefaultArea();
    ui->areaCB->setCurrentIndex(ui->areaCB->findData(workingArea));

}

SbcIpLoopBackDialog::~SbcIpLoopBackDialog()
{
    delete ui;
}

void SbcIpLoopBackDialog::checkEntry()
{
    bool a = !ui->areaCB->currentText().isEmpty();
    bool e = !ui->abbrCB->currentText().isEmpty();
    bool r = ui->remoteIpLE->hasAcceptableInput();
    bool ln = ui->lbNetLE->hasAcceptableInput();
    bool lm = ui->lbMaskLE->hasAcceptableInput();
    bool m = NetTools::subnetMaskValidate(ui->lbMaskLE->text());
    bool o;
    if(ui->ospfCB->isChecked())
        o = !ui->ospfLE->text().isEmpty();
    else
        o = true;

    if(a && e && r && ln && lm && o && m)
        ui->okBtn->setEnabled(true);
    else
        ui->okBtn->setEnabled(false);
}

void SbcIpLoopBackDialog::on_areaCB_currentIndexChanged(int index)
{
    int area = index + 2;
    QSqlQuery *query = dbMan->selectAreaAbbreviations(area,true);
    int exchId;
    QString abbr;
    ui->abbrCB->blockSignals(false);
    ui->abbrCB->clear();
    while(query->next())
    {
        exchId = query->value(0).toInt();
        abbr = query->value(1).toString();

        ui->abbrCB->addItem(abbr, exchId);
    }
    ui->abbrCB->setCurrentIndex(-1);
    ui->abbrCB->blockSignals(false);
}

void SbcIpLoopBackDialog::on_abbrCB_currentTextChanged(const QString &arg1)
{
    if(arg1.isEmpty())
        ui->okBtn->setEnabled(false);
    else
        checkEntry();
}

void SbcIpLoopBackDialog::on_remoteIpLE_textChanged(const QString &arg1)
{
    if(arg1.isEmpty())
        ui->okBtn->setEnabled(false);
    else
        checkEntry();
}

void SbcIpLoopBackDialog::on_ospfCB_toggled(bool checked)
{
    ui->ospfLE->clear();
    if(checked)
    {
        ui->ospfLE->setEnabled(true);
        ui->okBtn->setEnabled(false);
    }
    else
        ui->ospfLE->setEnabled(false);

    checkEntry();


}

void SbcIpLoopBackDialog::on_ospfLE_textChanged(const QString &arg1)
{
    if(arg1.isEmpty() && ui->ospfCB->isChecked())
        ui->okBtn->setEnabled(false);
    else
        checkEntry();
}

void SbcIpLoopBackDialog::on_lbNetLE_textChanged(const QString &arg1)
{
    if(arg1.isEmpty() && ui->ospfCB->isChecked())
        ui->okBtn->setEnabled(false);
    else
        checkEntry();
}

void SbcIpLoopBackDialog::on_lbMaskLE_textChanged(const QString &arg1)
{
    if(arg1.isEmpty() && ui->ospfCB->isChecked())
        ui->okBtn->setEnabled(false);
    else
        checkEntry();
}

void SbcIpLoopBackDialog::on_cancelBtn_clicked()
{
    this->close();
}

void SbcIpLoopBackDialog::on_directCB_toggled(bool checked)
{
    if(checked)
        checked = true;

    checkEntry();
}


void SbcIpLoopBackDialog::on_okBtn_clicked()
{
    QString net = ui->lbNetLE->text();
    QString mask = ui->lbMaskLE->text();
    int prefix = NetTools::subnetMaskToPrefix(mask);
    net = NetTools::netId(net, prefix);
    ui->lbNetLE->setText(net);

    QString now = QDateTime::currentDateTime().toString("yyyy/MM/dd  hh:mm");
    QString name = dbMan->getLoggedinLastname();

    QString exchParam = QString::number(ui->areaCB->currentData().toInt()) +"-"+ui->abbrCB->currentText();
    QString netParam = ui->lbNetLE->text();
    QString maskParam = ui->lbMaskLE->text();
    QString remoteIpParam = ui->remoteIpLE->text();
    bool Direct = ui->directCB->isChecked();
    bool OSPF = ui->ospfCB->isChecked();

    QString str = exchParam +"-SBC IP Loopback Plan";
    QString path = QFileDialog::getExistingDirectory(this, "Select Output Directory");
    if(!path.isEmpty())
    {
        QString reportPath; //for pdfs
        if(!path.endsWith('/'))
            path = path+"/";
        path += str;

        if(QDir(path).exists())
        {
            int i=2;
            while(QDir(path+QString::number(i)).exists())
                i++;

            path += QString::number(i);
        }


        if(QDir().mkdir(path))
        {
            reportPath = path + "/"+"Data Plan-"+ exchParam +"-SBC IP Loopback";
            if(QDir().mkdir(reportPath))
            {
                QTextEdit doc;
                bool done = false;

                QString Path = path+"/"+exchParam+"-SBC IP Loobback.txt";
                QFile Source(":/configurations/HUAWEI/SwitchDep/SBC-IP-LB");
                if(Source.open(QIODevice::ReadOnly | QIODevice::Text))
                {
                    QString text = QString(Source.readAll());
                    Source.close();
                    QFile sbc(Path);

                    if(sbc.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate))
                    {
                        text.replace("exchParam", exchParam);
                        text.replace("netParam", netParam);
                        text.replace("maskParam", maskParam);
                        text.replace("remoteIpParam",remoteIpParam);

                        sbc.seek(0); // go to the beginning of the file
                        text = "\t\tDaNet Configuration Report\n\nReported By "  +name + "\n" +now+"\n_______________________________________\n\n" + text;

                        QString param;

                        if(Direct)
                            param = "import-route direct\n  ";

                        if(OSPF)
                        {
                            QString pid = ui->ospfLE->text().trimmed();
                            QStringList pidList = pid.split(" ",QString::SkipEmptyParts);
                            foreach (QString s, pidList)
                            {
                                if(s.toInt() > 0)
                                {
                                    param += "import-route ospf "+s+"\n  ";
                                }
                            }
                        }


                        text.replace("param",param);

                        sbc.write(text.toUtf8()); // write the new text back to the file

                        sbc.close(); // close the file handle.
                        done = true;

                        doc.setText(text);

                    }
                }

                if(done)
                {
                    //print pdfs
                    //print config
                    QString PdfPath = reportPath+"/"+exchParam+"-SBC IP Loobback.pdf";
                    QPrinter printer(QPrinter::PrinterResolution);
                    printer.setOutputFormat(QPrinter::PdfFormat);
                    printer.setPaperSize(QPrinter::A4);
                    printer.setOutputFileName(PdfPath);
                    doc.print(&printer);
                    QMessageBox::information(this, "Succesfully Done.", "Check the exported file in path:\n"+path);
                    this->close();
                }
                else
                    QMessageBox::warning(this, "ERROR", "Try again later. There were some problems.");

            }
            else
                QMessageBox::warning(this,"error", "Please try again.");
        }
        else
            QMessageBox::warning(this,"error", "Please try again.");
    }

}

