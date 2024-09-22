#include "exportpdfdialog.h"
#include "ui_exportpdfdialog.h"
#include "lib/database/danetdbman.h"
#include "lib/report/interface/ReportInterface.h"
#include "lib/report/odf/ReportOdf.h"
#include <QSqlQueryModel>
#include <QStandardItemModel>
#include <QSqlQuery>
#include <QMessageBox>
#include <QFileDialog>
#include <QFile>
#include <QDesktopServices>

ExportPdfDialog::ExportPdfDialog(QWidget *parent, DanetDbMan *db, const QList<int> intIds, const  QList<int> posIds, const QList<int> PinIds) :
    QDialog(parent),
    ui(new Ui::ExportPdfDialog),
    dbMan(db),
    pinIds(PinIds),
    intModel(new QSqlQueryModel(this)),
    odfModel(new QSqlQueryModel(this))
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowMinMaxButtonsHint);

    ui->toolBox->setCurrentIndex(0);

    //interfaces
    QString condition = "int_id IN(";
    bool start = true;
    for(int i=0; i < intIds.count(); i++)
    {
        if(start)
        {
            condition += QString::number(intIds.at(i));
            start = false;
        }
        else
        {
            condition += ", "+QString::number(intIds.at(i));
        }
    }
    condition += ");";

    QSqlQuery *query = dbMan->selectViewInterfacesWhere(condition);
    //`area`,`abbr`,`exchange`,`saloon_name`,`device`, `interface_type`,`interface`,`module`,`label`, `peer_abbr`,`peer_device`,`peer_interface`,`peer_label`,`ether_trunk`

    intModel->setQuery(*query);
    intModel->setHeaderData(0,Qt::Horizontal,"Area");
    intModel->setHeaderData(1,Qt::Horizontal,"Abbr");
    intModel->setHeaderData(2,Qt::Horizontal,"Exchange");
    intModel->setHeaderData(3,Qt::Horizontal,"Saloon\nName");
    intModel->setHeaderData(4,Qt::Horizontal,"Device");
    intModel->setHeaderData(5,Qt::Horizontal,"Interface\nType");
    intModel->setHeaderData(6,Qt::Horizontal,"Interface");
    intModel->setHeaderData(7,Qt::Horizontal,"Module");
    intModel->setHeaderData(8,Qt::Horizontal,"Label");
    intModel->setHeaderData(9,Qt::Horizontal,"Peer\nAbbr");
    intModel->setHeaderData(10,Qt::Horizontal,"Peer\nDevice");
    intModel->setHeaderData(11,Qt::Horizontal,"Peer\nInterface");
    intModel->setHeaderData(12,Qt::Horizontal,"Peer\nLabel");
    intModel->setHeaderData(13,Qt::Horizontal,"Ether\nTrunk");

    ui->tv->setModel(intModel);
    ui->tv->verticalHeader()->hide();
    ui->tv->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tv->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);


    //odf
    condition = "pos_id IN(";
    start = true;
    for(int i=0; i < posIds.count(); i++)
    {
        if(start)
        {
            condition += QString::number(posIds.at(i));
            start = false;
        }
        else
        {
            condition += ", "+QString::number(posIds.at(i));
        }
    }
    condition += ");";

    query = dbMan->selectViewOdfPosPin(condition);
    //`odf_id`, `pos_id`, `pin_id`, `exchange_id`, `saloon_no`, `area`, `exchange`, `abbr`, `saloon_name`, `pin_per_pos`,
    //`odf_no`, `pos_no`, `local_device`, `local_label`, `pin_no`, `remote`, `remote_label`

    odfModel->setQuery(*query);


    int row = intModel->rowCount();
    if(row > 500)
    {
        ui->confirmChB->setVisible(true);
        ui->confirmChB->setChecked(false);
        ui->confirmLbl->setVisible(true);
        ui->okBtn->setEnabled(false);
    }
    else
    {
        ui->confirmChB->setVisible(false);
        ui->confirmLbl->setVisible(false);
        ui->okBtn->setEnabled(true);
    }

    odfExport = false;
    ui->odfChB->setChecked(false);
}

ExportPdfDialog::~ExportPdfDialog()
{
    delete ui;
}

void ExportPdfDialog::on_cancelBtn_clicked()
{
    this->close();
}

void ExportPdfDialog::on_okBtn_clicked()
{

    bool intDone=false, odfDone=false;
    QString customInfo = ui->customInfoLE->text().trimmed();

    QString intPath, odfPath;
    intPath = QFileDialog::getSaveFileName(this, "Save DaNet Interface Report As", "","pdf (*.pdf)");
    if(!intPath.isEmpty())
    {
        if(!intPath.endsWith(".pdf", Qt::CaseInsensitive)) intPath += ".pdf";

        if(customInfo.isEmpty())
        {
            ReportInterface reportInterface(this,intModel, intPath);
            if(reportInterface.print())
                intDone = true;
            else
                QMessageBox::warning(this, "ERROR", reportInterface.lastError());
        }
        else
        {
            ReportInterface reportInterface(this,intModel, intPath, customInfo);
            if(reportInterface.print())
                intDone = true;
            else
                QMessageBox::warning(this, "ERROR", reportInterface.lastError());
        }
    }


    //odf
    if(odfExport)
    {

        odfPath = QFileDialog::getSaveFileName(this, "Save DaNet ODF Report AS", "","pdf (*.pdf)");
        if(!odfPath.isEmpty())
        {
            if(!odfPath.endsWith(".pdf", Qt::CaseInsensitive)) odfPath += ".pdf";

            if(ui->customInfoLE->text().isEmpty())
            {
                ReportOdf reportOdf(this, odfModel, pinIds, odfPath);
                if(reportOdf.print())
                    odfDone = true;

                else
                    QMessageBox::warning(this, "ERROR", reportOdf.lastError());
            }
            else
            {
                ReportOdf reportOdf(this, odfModel, pinIds, odfPath, customInfo);
                if(reportOdf.print())
                    odfDone = true;

                else
                    QMessageBox::warning(this, "ERROR", reportOdf.lastError());
            }


        }
    }


    if(intDone)
        QDesktopServices::openUrl(intPath);

    if(odfDone)
        QDesktopServices::openUrl(odfPath);

    this->close();

}

void ExportPdfDialog::on_odfChB_toggled(bool checked)
{
    if(checked)
    {
        odfExport = true;
    }
    else
    {
        odfExport = false;
    }
}

void ExportPdfDialog::on_confirmChB_toggled(bool checked)
{
    if(checked)
        ui->okBtn->setEnabled(true);
    else
        ui->okBtn->setEnabled(false);
}
