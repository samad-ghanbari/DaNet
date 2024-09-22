#ifndef SORTFILTERPROXYMODEL
#define SORTFILTERPROXYMODEL

#include <QSortFilterProxyModel>

// in case we want to filter model in more than one column

class SortFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit SortFilterProxyModel(QObject *parent):QSortFilterProxyModel(parent){}
    void setColumnsFilterFixedString(QMap<int, QString> map)
    {
        columnsString = map;
        invalidate();
    }
    void setColumnsFilterRegExp(QMap<int, QRegExp> map)
    {
        columnsPattern = map;
        invalidate();
    }
    void setColumnsFilter(QMap<int, QString> StringMap,QMap<int, QRegExp> RegexpMap )
    {
        columnsString = StringMap;
        columnsPattern = RegexpMap;
        invalidate();
    }

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
    {
        if(columnsString.isEmpty() && columnsPattern.isEmpty())
            return true;

        bool ret=false;
        if(!columnsString.isEmpty() && columnsPattern.isEmpty()) // must check fixed strings
        {
            for(QMap<int, QString>::const_iterator iter = columnsString.constBegin();
                iter != columnsString.constEnd();
                ++iter)
            {
                QModelIndex index = sourceModel()->index(sourceRow, iter.key(), sourceParent);
                ret = (index.data().toString() == iter.value());

                if(!ret)
                    return ret;
            }

            return ret;
        }
        if(columnsString.isEmpty() && !columnsPattern.isEmpty())//must check regexp
        {
            for(QMap<int, QRegExp>::const_iterator iter = columnsPattern.constBegin();
                iter != columnsPattern.constEnd();
                ++iter)
            {
                QModelIndex index = sourceModel()->index(sourceRow, iter.key(), sourceParent);
                if(iter.value().indexIn(index.data().toString()) == -1)
                    ret = false;
                else
                    ret = true;

                if(!ret)
                    return ret;
            }

            return ret;
        }
        if(!columnsString.isEmpty() && !columnsPattern.isEmpty())//must check fixed strings and regexp
        {
            for(QMap<int, QRegExp>::const_iterator iter = columnsPattern.constBegin();
                iter != columnsPattern.constEnd();
                ++iter)
            {
                QModelIndex index = sourceModel()->index(sourceRow, iter.key(), sourceParent);
                if(iter.value().indexIn(index.data().toString()) == -1)
                    ret = false;
                else
                    ret = true;

                if(!ret)
                    return ret;
            }

            for(QMap<int, QString>::const_iterator iter = columnsString.constBegin();
                iter != columnsString.constEnd();
                ++iter)
            {
                QModelIndex index = sourceModel()->index(sourceRow, iter.key(), sourceParent);
                ret = (index.data().toString() == iter.value());

                if(!ret)
                    return ret;
            }

            return ret;
        }

        return ret;
    }

private:
    QMap<int, QString> columnsString;
    QMap<int, QRegExp> columnsPattern;
};

#endif // SORTFILTERPROXYMODEL
