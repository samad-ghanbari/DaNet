#include "ReportInterface.h"
#include <QAbstractItemModel>
#include <QDate>
#include <QPainter>
#include <QPrinter>
#include <QFontDatabase>

ReportInterface::ReportInterface(QObject *Parent, const QAbstractItemModel* tableModel, const QString OutputPath) :
    QObject(Parent),
    painter(new QPainter()),
    model(tableModel),
    outputPath(OutputPath)
{
    CUSTOM_INFO = false;
    customInfo = "";

    initiate();
}

ReportInterface::ReportInterface(QObject *Parent, const QAbstractItemModel *tableModel, const QString OutputPath, const QString CustomInfo):
    QObject(Parent),
    painter(new QPainter()),
    model(tableModel),
    outputPath(OutputPath)
{
    if(CustomInfo.trimmed().isEmpty())
    {
        CUSTOM_INFO = false;
        customInfo = "";
    }
    else
    {
        CUSTOM_INFO = true;
        customInfo = CustomInfo;
    }

    initiate();
}

ReportInterface::~ReportInterface()
{
    delete painter;
    painter = NULL;
}

void ReportInterface::initiate()
{
    QFontDatabase::addApplicationFont(":/font/tahoma.ttf");
    QFontDatabase::addApplicationFont(":/font/tahomabd.ttf");

    topMargin = 10;
    bottomMargin = 10;
    leftMargin = 5;
    rightMargin = 5;
    if(CUSTOM_INFO)
        headerHeight = 120;
    else
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
    //`area`,`abbr`,`exchange`,`saloon_name`,
    //`device`, `interface_type`,`interface`,`module`,`label`,
    //`peer_abbr`,`peer_device`,`peer_interface`,`peer_label`,`ether_trunk`
    columnStretch = QVector<int>() <<0<<0<<0<<7
                                  <<10<<0<<6<<7<<20
                                 <<8<<10<<8<<20<<5;

    headers = QVector<QString>() << "Area" << "Abbr"<<"Exchange"<< "Saloon"
                                 << "Device"<< "Interface\nType"<< "Interface"<<"Module"<<"Local Label"
                                 <<"Peer\nAbbr"<<"Peer\nDevice"<<"Peer\nInterface"<<"Peer Label"<<"Ether\nTrunk";

    index = model->index(0,0);
    area = model->data(index).toInt();
    index = index.sibling(0, 1);
    abbr = model->data(index).toString();
    index = index.sibling(0, 2);
    exchange = model->data(index).toString();

    pageNumber = 1;
    current_date = QDate::currentDate().toString("yyyy/MM/dd");
    error = "No error";
}

void ReportInterface::preparePage(QPainter *painter)
{ // print a border on each page

    header_info = QString::number(area) + " - " + abbr;

    painter->setPen(Qt::darkMagenta);
    painter->setFont(QFont("tahoma", 16, QFont::Bold));
    QRect rec(10,10,painter->viewport().width()-20,30);
    painter->drawText(rec,Qt::AlignLeft, current_date);
    painter->drawText(rec,Qt::AlignCenter,"DaNet Interface Report");
    rec.setRect(10,40,painter->viewport().width()-20,30);
    painter->drawText(rec, Qt::AlignLeft,header_info);
    painter->drawText(rec, Qt::AlignCenter,exchange);
    QPixmap im(":/Image/tct.jpg");
    painter->drawPixmap(painter->viewport().width()-50, 0,im);

    if(CUSTOM_INFO)
    {
        painter->setFont(QFont("tahoma", 14, QFont::Bold));
        rec.setRect(10,80,painter->viewport().width()-20,30);
        painter->setPen(Qt::darkCyan);
        painter->drawText(rec, Qt::AlignLeft,customInfo);
        painter->setPen(Qt::darkMagenta);
        painter->setFont(QFont("tahoma", 16, QFont::Bold));
    }

    painter->setFont(QFont("tahoma", 12));
    painter->translate(10, painter->viewport().height() );
    painter->setPen(QPen(QColor(0, 0, 0), 2));
    painter->drawText(0, 0, QString("Page %1").arg(pageNumber));
    pageNumber += 1;
}

bool ReportInterface::print()
{
    QPrinter *pr = new QPrinter(QPrinter::ScreenResolution);
    pr->setOutputFormat(QPrinter::PdfFormat);
    pr->setOrientation(QPrinter::Landscape);
    pr->setPaperSize(QPrinter::A3);
    pr->setOutputFileName(outputPath);
    pr->setCreator("DaNet Application");
    pr->setDocName("Interfaces Report");
    pr->setPrintProgram("DaNet Application");

    if(printTable(pr))
        return true;
    else
        return false;
}

bool ReportInterface::printTable(QPrinter *p)
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
        int AREA = model->data(index).toInt();
        index = index.sibling(j, 1);
        QString ABBR = model->data(index).toString();
        index = index.sibling(j,2);
        QString EXCH = model->data(index).toString();

        if(abbr.compare(ABBR, Qt::CaseInsensitive) != 0)
        {
            area = AREA;
            abbr = ABBR;
            exchange = EXCH;

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

QString ReportInterface::lastError()
{
    return error;
}
