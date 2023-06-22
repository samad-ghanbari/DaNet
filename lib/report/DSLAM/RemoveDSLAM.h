#ifndef REMOVEDSLAM_H
#define REMOVEDSLAM_H

#include <QObject>
#include <QModelIndex>
#include <QPen>
#include <QFont>

class QPrinter;
class QPainter;
class QAbstractItemModel;

class RemoveDSLAM : public QObject
{
    Q_OBJECT
public:
  RemoveDSLAM(QObject *Parent, const QString OutputPath, const QString Area, const QString Mother, const QString Exch, const QString Dslam, const QString IPm, const QString Vlan, const QStringList Agg1, const QStringList Agg2, const QStringList Cx, const QStringList Bras, const QString Desc);
  ~RemoveDSLAM();
  QString lastError();
  void preparePage();
  bool print();
  bool printTable(QPrinter *printer);
private:
  QPainter *painter;
  const QString outputPath, dslamName, area, mother, exch, ipm, vlan;
  const QStringList agg1, agg2, cx, bras;

  QPen pen; // for table borders
  QFont headersFont, contentFont, topicFont;

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

  int tableHeaderHight;

  int pageNumber;
  QString current_date;
  QModelIndex index;
  QString error, customInfo;
};

#endif // REMOVEDSLAM_H
