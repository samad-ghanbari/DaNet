#ifndef REPORTINTERFACE_H
#define REPORTINTERFACE_H

#include <QObject>
#include <QModelIndex>
#include <QPen>
#include <QFont>

class QPrinter;
class QPainter;
class QAbstractItemModel;

class ReportInterface : public QObject
{
    Q_OBJECT
public:
  ReportInterface(QObject *Parent, const QAbstractItemModel *tableModel, const QString OutputPath);
  ReportInterface(QObject *Parent, const QAbstractItemModel *tableModel, const QString OutputPath, const QString CustomInfo);
  ~ReportInterface();
  void initiate();
  QString lastError();
  void preparePage(QPainter *painter);
  bool print();
  bool printTable(QPrinter *p);

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

  int pageNumber, area;
  QString header_info, exchange, abbr, current_date, customInfo;
  bool CUSTOM_INFO;

  QVector<int> columnStretch;
  QVector<QString> headers;
  QModelIndex index;

  QString error;
};

#endif // REPORTINTERFACE_H
