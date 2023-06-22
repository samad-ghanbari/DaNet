#ifndef CENTERALIGNEDMODEL_H
#define CENTERALIGNEDMODEL_H

#include <QSqlQueryModel>
#include <QFont>

#include <QDebug>
class CenterAlignedModel : public QSqlQueryModel
{
public:
    CenterAlignedModel(QObject *parent = 0):
        QSqlQueryModel(parent)
    {
        specificColumn = -1;
    }
    CenterAlignedModel(QObject *parent, const int SpecificCol):
        QSqlQueryModel(parent)
    {
        specificColumn = SpecificCol;
    }

    void setSpecificColumn(int col){specificColumn = col;}

    QVariant data(const QModelIndex &item, int role = Qt::DisplayRole) const
    {
        if(role == Qt::TextAlignmentRole)
            return Qt::AlignCenter;

//        if (role == Qt::BackgroundRole)
//        {
//           int col = item.column();
//           if(col == specificColumn)
//               return QVariant(QBrush(QColor(200, 200, 230)));
//        }

        if (role == Qt::FontRole)
        {
            int col = item.column();
            if(col == specificColumn)
            {
                QFont font;
                font.setBold(true);
                font.setPixelSize(16);
                return font;
            }
        }

        if (role == Qt::ForegroundRole)
        {
            int col = item.column();
            if(col == specificColumn)
            {
                return QVariant(QColor(0, 0, 150));
            }

        }


        return QSqlQueryModel::data(item, role);
    }


private:
    int specificColumn;
};


#endif // CENTERALIGNEDMODEL_H
