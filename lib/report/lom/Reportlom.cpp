#include "Reportlom.h"
#include <QAbstractItemModel>
#include <QDate>
#include <QPainter>
#include <QPrinter>

ReportLom::ReportLom(QObject *Parent, QAbstractItemModel* tableModel, const QList<int> ColumnStretch, const bool ALIGNLEFT, const QString OutputPath, const QString Abbr, const QString Exchange, const QString ReportTitle, const QString CustomInfo) :
    QObject(Parent),
    painter(new QPainter()),
    model(tableModel),
    columnStretch(ColumnStretch),
    outputPath(OutputPath),
    exchange(Exchange),
    abbr(Abbr),
    currentDate(QDate::currentDate().toString("yyyy/MM/dd")),
    reportTitle(ReportTitle)
{

    if(CustomInfo.trimmed().isEmpty())
    {
        CUSTOM_INFO = false;
        customInfo = "";
        headerHeight = 105;
    }
    else
    {
        CUSTOM_INFO = true;
        customInfo = CustomInfo;
        headerHeight = 165;
    }

    topMargin = 10;
    bottomMargin = 10;
    leftMargin = 5;
    rightMargin = 5;

    bottomHeight = 20;
    leftBlank = 10;
    rightBlank = 10;
    maxRowHeight = 1000;
    tableHeaderHight = 50;
    pen = QPen(QColor(0, 0, 80), 1, Qt::SolidLine); //pen for borders
    //    pen.setWidth(2);
    QFont font1; // font for headers
    font1.setPixelSize(14);
    font1.setBold(true);
    headersFont = font1;
    QFont font2; // font for content
    font2.setPixelSize(14);
    font2.setBold(false);
    contentFont = font2;
    headerColor = QColor(60, 1, 80);
    contentColor = QColor(80,1,50);

    if(ALIGNLEFT)
        contentAlign = Qt::AlignLeft;
    else
        contentAlign = Qt::AlignRight;

    columnCount = model->columnCount();
    rowCount = model->rowCount();
    for(int i = 0; i < columnCount; i++)
    {
        headers.insert(i, model->headerData(i,Qt::Horizontal).toString());
    }

    pageNumber = 1;
    error = "No error";
}

ReportLom::~ReportLom()
{
    delete painter;
    painter = NULL;
}

void ReportLom::preparePage()
{ // print a border on each page

    painter->resetTransform();
    painter->setPen(Qt::darkMagenta);
    painter->setFont(QFont("tahoma", 12, QFont::Bold));
    QRect rec(10,10,painter->viewport().width()-20,30);
    painter->drawText(rec,Qt::AlignLeft, currentDate);
    painter->drawText(rec,Qt::AlignCenter,reportTitle);

    rec.setRect(10,40,painter->viewport().width()-20,30);
    painter->setPen(QColor(80, 1, 50));
    painter->drawText(rec, Qt::AlignCenter,abbr);

    rec.setRect(10,70,painter->viewport().width()-20,30);
    painter->setPen(QColor(80, 1, 50));
    painter->drawText(rec, Qt::AlignCenter,exchange);

    painter->setPen(Qt::darkMagenta);
    QPixmap im(":/Image/tct.jpg");
    painter->drawPixmap(painter->viewport().width()-50, 0,im);

    if(CUSTOM_INFO)
    {
        rec.setRect(10,100,painter->viewport().width()-20,60);
        painter->setPen(QColor(50,1,80));
        painter->setFont(QFont("tahoma", 12, QFont::Normal));
        painter->drawText(rec, Qt::AlignRight,customInfo);
        painter->setPen(Qt::darkMagenta);
        painter->setFont(QFont("tahoma", 12, QFont::Bold));
    }

    painter->setFont(QFont("tahoma", 10));
    painter->translate(10, painter->viewport().height() );
    painter->setPen(QPen(QColor(0, 0, 0), 2));
    painter->drawText(0, 0, QString("Page %1").arg(pageNumber));
    painter->translate(-painter->transform().dx() + leftBlank, -painter->transform().dy() + headerHeight);
    pageNumber += 1;
    newPageFlag = true;
}

bool ReportLom::print()
{
    QPrinter *printer = new QPrinter(QPrinter::ScreenResolution);
    printer->setOutputFormat(QPrinter::PdfFormat);
    printer->setOrientation(QPrinter::Landscape);
    printer->setPaperSize(QPrinter::A4);
    printer->setOutputFileName(outputPath);

    printer->setCreator("DaNet Application");
    printer->setDocName("LOM Report");
    printer->setPrintProgram("DaNet Application");


    if(!painter->begin(printer))
    {
        error = "painter->begin error.";
        return false;
    }

    //--------------------------------- error checking -------------------------------------

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

    if(columnCount < 1) {error = "No Column"; return false;}

    if(columnCount != columnStretch.count())
    {
        error = QString("number of Stretches and model columns must the same.");
        return false ;
    }

    QList<double> columnWidth;
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

    for (int i = 0; i < columnStretch.count(); i++)
    {
        columnWidth.append(tableWidth / totalStretch * columnStretch[i]);
    }

    preparePage();

    // to know row height before printing
    // at first print to test image
    QPainter testSize;
    QImage* image = new QImage(10, 10, QImage::Format_RGB32);
    image->setDotsPerMeterX(printer->logicalDpiX() * 100 / 2.54); // 2.54 cm = 1 inch   dot/inch = dot/2.54cm = dot*100/2.54m=100/2.54 dot/m
    image->setDotsPerMeterY(printer->logicalDpiY() * 100 / 2.54);
    testSize.begin(image);

    painter->setPen(pen);
    painter->setFont(contentFont);
    testSize.setFont(contentFont);
    painter->save();

    painter->drawLine(0, 0, tableWidth, 0); // first horizontal line
    //###############################################################

    int row = 0;
    while(row < rowCount)
    { // for each row

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
                str = model->data(model->index(row,i), Qt::DisplayRole).toString();

                QRect rect(0, 0, columnWidth[i] - rightMargin - leftMargin, maxRowHeight);
                QRect realRect;
                testSize.drawText(rect, Qt::AlignLeft | Qt::TextWordWrap, str, &realRect);

                if (realRect.height() > maxHeight && columnStretch[i] != 0)
                {
                    realRect.height() > maxRowHeight ? maxHeight = maxRowHeight : maxHeight = realRect.height();
                }

            }
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
                painter->fillRect(fillRec, QColor(210, 210, 210));
                painter->drawText(rec, Qt::AlignCenter | Qt::TextWordWrap, str);
            } else
            {
                if(model->data(model->index(row,i)).isNull())
                    str = "";
                else
                    str = model->data(model->index(row,i), Qt::DisplayRole).toString();

                QRect rec(leftMargin, topMargin, columnWidth[i] - rightMargin - leftMargin, maxHeight);
                painter->drawText(rec, contentAlign | Qt::AlignVCenter | Qt::TextWordWrap, str);
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
            row++;

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

    painter->end();

    return true;
}

QString ReportLom::lastError()
{
    return error;
}
