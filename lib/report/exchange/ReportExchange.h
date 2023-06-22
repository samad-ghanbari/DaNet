#ifndef REPORTEXCHANGE_H
#define REPORTEXCHANGE_H

#include <QObject>
#include <QModelIndex>
#include <QPen>
#include <QFont>

class QPrinter;
class QPainter;
class QAbstractItemModel;

class ReportExchange : public QObject
{
    Q_OBJECT
public:
  ReportExchange(QObject *Parent, const QAbstractItemModel *tableModel, const QString OutputPath);
  ~ReportExchange();
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

  int pageNumber;
  QString current_date;

  QVector<int> columnStretch;
  QVector<QString> headers;
  QModelIndex index;

  QString error;
};

#endif // REPORTEXCHANGE_H
