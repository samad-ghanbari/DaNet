#ifndef REPORTLOM_H
#define REPORTLOM_H

#include <QObject>
#include <QModelIndex>
#include <QPen>
#include <QFont>

class QPrinter;
class QPainter;
class QAbstractItemModel;

class ReportLom : public QObject
{
    Q_OBJECT
public:
  ReportLom(QObject *Parent, QAbstractItemModel *tableModel, const QList<int> ColumnStretch,const bool ALIGNLEFT, const QString OutputPath, const QString Abbr, const QString Exchange, const QString ReportTitle, const QString CustomInfo="");
  ~ReportLom();
  QString lastError();
  void preparePage();
  bool print();
  bool printTable(QPrinter *p);

private:
  QPainter *painter;
  const QAbstractItemModel* model;
  const QList<int> columnStretch;
  const QString outputPath;
  const QString exchange, abbr, currentDate, reportTitle;
  QList<QString> headers;
  QPen pen; // for table borders
  QFont headersFont;
  QFont contentFont;
  QColor headerColor;
  QColor contentColor;
  // cell margins
  int topMargin;
  int bottomMargin;
  int leftMargin;
  int rightMargin;
  bool newPageFlag;

  // margins for table
  int headerHeight;
  int bottomHeight;
  int leftBlank;
  int rightBlank;

  int maxRowHeight;
  int tableHeaderHight;
  int columnCount;
  int rowCount;
  Qt::AlignmentFlag contentAlign;

  int pageNumber;
  QModelIndex index;

  QString error, customInfo;
  bool CUSTOM_INFO;
};

#endif // REPORTLOM_H
