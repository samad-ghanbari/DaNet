#ifndef MAPMODEL_H
#define MAPMODEL_H

#include <QAbstractTableModel>
#include <QMap>

class MapModel : public QAbstractTableModel
{
    Q_OBJECT
public:

    enum MapRoles {
        KeyRole = Qt::UserRole + 1,
        ValueRole
    };

    explicit MapModel(QObject *parent, QStringList HeaderList) :
        QAbstractTableModel(parent),
        map(NULL)
    {
        headerList = HeaderList;
    }

    int rowCount(const QModelIndex& parent = QModelIndex()) const
    {
        {
            if(parent.isValid())
            {
                if (map)
                    return map->count();
            }

            if (map)
                return map->count();
            return 0;

        }
    }

    int columnCount(const QModelIndex& parent = QModelIndex()) const
    {
        if(parent.isValid())
        {
            return 2;
        }

        return 2;
    }

    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const
    {
        if (!map)
            return QVariant();
        if (index.row() < 0 ||
                index.row() >= map->count() ||
                role != Qt::DisplayRole) {
            return QVariant();
        }
        if (index.column() == 0)
            return map->keys().at(index.row());
        if (index.column() == 1)
            return map->values().at(index.row());
        return QVariant();
    }

    QVariant headerData(int section, Qt::Orientation orientation,int role = Qt::DisplayRole) const
    {
        if (role != Qt::DisplayRole)
            return QVariant();

        if (orientation == Qt::Horizontal)
            return headerList[section];
        else
            return QString("%1").arg(section);
    }

    inline void setMap(QMap<int, QString>* Map) { map = Map; }

private:
    QMap<int, QString>* map;
    QStringList headerList;
};

#endif // MAPMODEL_H
