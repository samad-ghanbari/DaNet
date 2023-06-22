#ifndef REPORTODF_H
#define REPORTODF_H

#include <QObject>
#include <QPen>
#include <QFont>
#include <QModelIndex>

class QPrinter;
class QPainter;
class QAbstractItemModel;

class ReportOdf : public QObject
{
    Q_OBJECT
public:
  ReportOdf(QObject *parent, const QAbstractItemModel *tableModel, const QString OutputPath, const QString CustomInfo = "");
  ReportOdf(QObject *parent, const QAbstractItemModel *tableModel, const QList<int> HighlightPinIds, const QString OutputPath, const QString CustomInfo = "");
  ~ReportOdf();
  void init();
  QString lastError();
  void preparePage(QPainter *painter);
  bool print();

public slots:
  bool printTable(QPrinter *p);

private:
  QPainter *painter;
  const QAbstractItemModel* model;
  const QString outputPath;
  bool highlightFlag;
  bool doHighlight;
  QList<int> highlightPinIds;
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
  QString headerAbbr, currentDate, exchange, saloon;
  int currentPosId, ppp, odfNo, posNo;

  QVector<int> columnStretch;
  QVector<QString> headers;

  QString error;
  bool CUSTOM_INFO;
  QString customInfo;
};

#endif // REPORTODF_H
