#include "ReportExchange.h"
#include <QAbstractItemModel>
#include <QDate>
#include <QPainter>
#include <QPrinter>

ReportExchange::ReportExchange(QObject *Parent, const QAbstractItemModel* tableModel, const QString OutputPath) :
    QObject(Parent),
    painter(new QPainter()),
    model(tableModel),
    outputPath(OutputPath)
{

    topMargin = 10;
    bottomMargin = 10;
    leftMargin = 5;
    rightMargin = 5;
    headerHeight = 80;
    bottomHeight = 20;
    leftBlank = 0;
    rightBlank = 0;
    maxRowHeight = 1000;
    tableHeaderHight = 50;
    pen = QPen(QColor(0, 0, 80), 1, Qt::SolidLine); //pen for borders
    //    pen.setWidth(2);
    QFont font1; // font for headers
    font1.setPixelSize(16);
    font1.setBold(true);
    headersFont = font1;
    QFont font2; // font for content
    font2.setPixelSize(14);
    contentFont = font2;
    headerColor = QColor(Qt::darkRed);
    contentColor = QColor(Qt::darkBlue);
//`id`,`area_no`,`area`,`exchange`,
//`abbr`,`type_no`,`type`,`mother_exchange_id`,
//`mother_exchange_abbr`,`uplink_exchange_id`,`uplink_exchange_abbr`,`site_cascade`,
//`site_node`,`data_saloon`,`switch_saloon`, `pcm_saloon`,`address`

    columnStretch = QVector<int>() <<0<<0<<7<<15
                                  <<10<<0<<10<<0
                                  <<10<<0<<0<<0
                                  <<0<<0<<0<<0<<20;

    headers = QVector<QString>() << "ID" << "Area No"<<"Area"<< "Exchange"
                                 << "Abbr"<< "Type No"<< "Type"<<"Mother Id"
                                 <<"Mother Abbr"<<"Uplink ID"<<"Uplink Abbr"<<"Cascade"
                                 <<"Node"<<"Data"<<"Switch"<<"PCM"<<"Address";


    pageNumber = 1;
    current_date = QDate::currentDate().toString("yyyy/MM/dd");
    error = "No error";
}

ReportExchange::~ReportExchange()
{
    delete painter;
    painter = NULL;
}

void ReportExchange::preparePage(QPainter *painter)
{ // print a border on each page

    painter->setPen(Qt::darkMagenta);
    painter->setFont(QFont("Times", 16, QFont::Bold));
    QRect rec(10,10,painter->viewport().width()-20,30);
    painter->drawText(rec,Qt::AlignLeft, current_date);
    painter->drawText(rec,Qt::AlignCenter,"DaNet Exchange Report");
    rec.setRect(10,40,painter->viewport().width()-20,30);
    QPixmap im(":/Image/tct.jpg");
    painter->drawPixmap(painter->viewport().width()-50, 0,im);

    painter->setFont(QFont("Times", 10));
    painter->translate(10, painter->viewport().height() );
    painter->setPen(QPen(QColor(0, 0, 0), 2));
    painter->drawText(0, 0, QString("Page %1").arg(pageNumber));
    pageNumber += 1;
}

bool ReportExchange::print()
{
    QPrinter *pr = new QPrinter(QPrinter::ScreenResolution);
    pr->setOutputFormat(QPrinter::PdfFormat);
    pr->setOrientation(QPrinter::Landscape);
    pr->setPaperSize(QPrinter::A3);
    pr->setOutputFileName(outputPath);
    pr->setCreator("DaNet Application");
    pr->setDocName("Exchange Report");
    pr->setPrintProgram("DaNet Application");

    if(printTable(pr))
        return true;
    else
        return false;
}

bool ReportExchange::printTable(QPrinter *p)
{
    QPrinter *printer = p;

    if(!painter->begin(printer))
    {
        error = "painter->begin error.";
        return false;
    }

    painter->translate(0, headerHeight); // start print point


    //--------------------------------- error checking -------------------------------------

    int columnCount = model->columnCount();
    int count = columnStretch.count();
    if(count != columnCount)
    {
        error = "Different columns count in model and columnStretch";
        return false;
    }
    count = headers.count();
    if(count != columnCount && count != 0)
    {
        error = "Different columns count in model and in headers";
        return false;
    }
    if(!printer->isValid())
    {
        error = "printer.isValid() == false";
        return false;
    }
    if(!painter->isActive())
    {
        error = "painter.isActive() == false";
        return false;
    }
    double tableWidth = painter->viewport().width() - leftBlank - rightBlank;
    if(tableWidth <= 0)
    {
        error = "wrong table width";
        return false;
    }
    int totalStretch = 0;
    for (int i = 0; i < columnStretch.count(); i++)
    {
        if(columnStretch[i] < 0)
        {
            error = QString("wrong column stretch, column: %1 stretch: %2").arg(i).arg(columnStretch[i]);
            return false ;
        }
        totalStretch += columnStretch[i];
    }
    if(totalStretch <= 0)
    {
        error = QString("wrong stretch");
        return false;
    }
    QVector<double> columnWidth;
    for (int i = 0; i < columnStretch.count(); i++)
    {
        columnWidth.append(tableWidth / totalStretch * columnStretch[i]);
    }


    headers.isEmpty() ? newPageFlag = false : newPageFlag = true;


    //----------------------------------------------------------------------------

    painter->save(); // before table print

    // to know row height before printing
    // at first print to test image
    QPainter testSize;
    QImage* image = new QImage(10, 10, QImage::Format_RGB32);
    image->setDotsPerMeterX(printer->logicalDpiX() * 100 / 2.54); // 2.54 cm = 1 inch   dot/inch = dot/2.54cm = dot*100/2.54m=100/2.54 dot/m
    image->setDotsPerMeterY(printer->logicalDpiY() * 100 / 2.54);
    testSize.begin(image);

    painter->save(); //push setting
    painter->translate(-painter->transform().dx(), -painter->transform().dy());
    preparePage(painter);
    painter->restore(); // pop setting

    painter->setPen(pen);
    painter->setFont(contentFont);
    testSize.setFont(contentFont);
    painter->translate(-painter->transform().dx() + leftBlank, 0);
    painter->save();

    painter->drawLine(0, 0, tableWidth, 0); // first horizontal line
    //###############################################################

    int j =0;
    while(j < model->rowCount())
    { // for each row

        index = model->index(j,0);

        if(newPageFlag)
        {
            painter->setFont(headersFont);
            testSize.setFont(headersFont);
        }
        else
        {
            painter->setFont(contentFont);
            testSize.setFont(contentFont);
        }

        // --------------------------- row height counting ----------------------------

        int maxHeight = 0; // max row Height
        for(int i = 0; i < columnCount; i++)
        { // for each column
            QString str;
            if(newPageFlag)
            {
                str = headers.at(i);
                maxHeight = tableHeaderHight;
            } else
            {
                str = model->data(model->index(j,i), Qt::DisplayRole).toString();

                QRect rect(0, 0, columnWidth[i] - rightMargin - leftMargin, maxRowHeight);
                QRect realRect;
                testSize.drawText(rect, Qt::AlignLeft | Qt::TextWordWrap, str, &realRect);

                if (realRect.height() > maxHeight && columnStretch[i] != 0)
                {
                    realRect.height() > maxRowHeight ? maxHeight = maxRowHeight : maxHeight = realRect.height();
                }

            }
        }

        if(painter->transform().dy() + maxHeight + topMargin + bottomMargin > painter->viewport().height() -  bottomHeight)
        { // begin from new page
            int y = painter->transform().dy();
            painter->restore();
            painter->save();
            for(int i = 0; i < columnCount; i++)
            { // vertical lines
                painter->drawLine(0, 0, 0, - painter->transform().dy() + y);
                painter->translate(columnWidth[i], 0);
            }
            painter->drawLine(0, 0, 0, - painter->transform().dy() + y); // last vertical line
            painter->restore();
            printer->newPage();
            newPageFlag = true;

            painter->save();
            painter->translate(-painter->transform().dx(), -painter->transform().dy());
            preparePage(painter);
            painter->restore();

            painter->translate(-painter->transform().dx() + leftBlank, -painter->transform().dy() + headerHeight);
            painter->save();
            painter->drawLine(0, 0, tableWidth, 0); // first horizontal line
        }

        //------------------------------ content printing -------------------------------------------

        painter->save();
        if(newPageFlag)
        {
            painter->setFont(headersFont);
            testSize.setFont(headersFont);
            painter->setPen(QPen(headerColor));
        }
        else
        {
            painter->setFont(contentFont);
            testSize.setFont(contentFont);
            painter->setPen(QPen(contentColor));
        }
        //####
        for(int i = 0; i < columnCount; i++)
        { // for each column
            QString str;
            if(newPageFlag)
            {
                str = headers.at(i);
                maxHeight = tableHeaderHight;
                QRect rec(leftMargin, topMargin, columnWidth[i] - rightMargin - leftMargin, maxHeight);
                QRect fillRec(0, 0, columnWidth[i] , tableHeaderHight+topMargin+bottomMargin);
                painter->fillRect(fillRec, QColor(250, 240, 230));
                painter->drawText(rec, Qt::AlignCenter | Qt::TextWordWrap, str);
            } else
            {
                if(model->data(model->index(j,i)).isNull())
                    str = "";
                else
                    str = model->data(model->index(j,i), Qt::DisplayRole).toString();

                QRect rec(leftMargin, topMargin, columnWidth[i] - rightMargin - leftMargin, maxHeight);
                painter->drawText(rec, Qt::AlignLeft | Qt::AlignVCenter | Qt::TextWordWrap, str);
            }

            painter->translate(columnWidth[i], 0);
        }
        //####
        painter->restore();

        painter->drawLine(0, maxHeight + topMargin + bottomMargin, tableWidth, maxHeight + topMargin + bottomMargin); // last horizontal line
        painter->translate(0, maxHeight + topMargin + bottomMargin);

        if(newPageFlag)
            newPageFlag = false;
        else
            j++;

    }

    //###############################################################
    int y = painter->transform().dy();
    painter->restore();
    painter->save();
    for(int i = 0; i < columnCount; i++)
    { // vertical lines
        painter->drawLine(0, 0, 0, - painter->transform().dy() + y);
        painter->translate(columnWidth[i], 0);
    }
    painter->drawLine(0, 0, 0, - painter->transform().dy() + y); // last vertical line
    painter->restore();

    testSize.end();
    delete image;

    painter->restore(); // before table print

    painter->end();

    return true;
}

QString ReportExchange::lastError()
{
    return error;
}
