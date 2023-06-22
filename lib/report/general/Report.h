#ifndef REPORTR_H
#define REPORT_H

#include <QObject>
#include <QModelIndex>
#include <QPen>
#include <QFont>

class QPrinter;
class QPainter;
class QAbstractItemModel;

class Report : public QObject
{
    Q_OBJECT
public:
  Report(QObject *Parent, const QAbstractItemModel *tableModel,const QVector<QString> Headers, const QVector<int> ColumnStretch, const QString OutputPath, const QString ReportTopic, const QString CustomInfo="");
  ~Report();
  QString lastError();
  void preparePage(QPainter *painter);
  bool print();
  bool printTable(QPrinter *p);

private:
  QPainter *painter;
  const QAbstractItemModel* model;
  const QVector<QString> headers;
  const QVector<int> columnStretch;
  const QString outputPath, reportTopic;

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

  int pageNumber;
  QString current_date;

  QModelIndex index;

  QString error, customInfo;
  bool CUSTOM_INFO;
};

#endif // Report_H
