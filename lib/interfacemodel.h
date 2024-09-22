#ifndef INTERFACEMODEL_H
#define INTERFACEMODEL_H

#include <QSqlQueryModel>
#include <QFont>
#include <QVariant>
#include <QColor>
#include <QBrush>

class InterfaceModel : public QSqlQueryModel
{
public:
    InterfaceModel(QObject *parent = 0):
        QSqlQueryModel(parent)
    {
        specificColumn = -1;
    }
    InterfaceModel(QObject *parent, const int SpecificCol):
        QSqlQueryModel(parent)
    {
        specificColumn = SpecificCol;
    }

    void setSpecificColumn(int col){specificColumn = col; }

    QVariant data(const QModelIndex &item, int role = Qt::DisplayRole) const
    {

        int col = item.column();
        if (role == Qt::BackgroundRole)
        {
           if(col == specificColumn)
           {
              QModelIndex ind = item.sibling(item.row(), 7);
              if(QSqlQueryModel::data(ind).isNull())
                return QVariant(QBrush(QColor(250, 220, 220)));
              else
                  return QVariant(QBrush(QColor(220, 250, 220)));
           }

        }

        return QSqlQueryModel::data(item, role);
    }

private:
    int specificColumn;
};


#endif // INTERFACEMODEL_H
