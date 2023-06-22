#ifndef REPORTNETWORKIPV4PLAN_H
#define REPORTNETWORKIPV4PLAN_H

#include <QObject>
#include <QModelIndex>
#include <QPen>
#include <QFont>

class QPrinter;
class QPainter;
class QAbstractItemModel;

class ReportNetworkIpV4Plan : public QObject
{
    Q_OBJECT
public:
  ReportNetworkIpV4Plan(QObject *Parent, const QAbstractItemModel *tableModel, const QString OutputPath, const QString CustomInfo="");
  ~ReportNetworkIpV4Plan();
  QString lastError();
  void preparePage();
  bool print();
  bool printTable(QPrinter *printer);

private:
  QPainter *painter;
  const QAbstractItemModel* model;
  const QString outputPath;
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

  QVector<int> columnStretch;
  QVector<QString> headers;
  QModelIndex index;

  QString error, customInfo;
  bool CUSTOM_INFO;
};

#endif // REPORTNETWORKIPV4PLAN_H
