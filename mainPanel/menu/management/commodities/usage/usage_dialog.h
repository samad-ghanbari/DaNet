#ifndef USAGE_DIALOG_H
#define USAGE_DIALOG_H

#include <QDialog>
#include <QMenu>
#include <QModelIndex>

class SortFilterProxyModel;
class HeaderView;
class DanetDbMan;
class QSqlQueryModel;

namespace Commodity {
namespace Usage {

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(DanetDbMan *db, const int COMM_ID, const QString COMM, const int TOTAL, const int LEFT, QWidget *parent = nullptr);
    ~Dialog();

signals:
    void changeSignal();

private slots:
    void on_actionEdit_Commodity_Usage_triggered();
    void on_actionRemove_Commodity_Usage_triggered();
    void on_okBtn_clicked();
    void fillTV();
    void contextMenuSlot(QPoint ptr);
    void changeSlot();
    void headerWidgetsChagedSlot();

private:
    Ui::Dialog *ui;
    DanetDbMan *dbMan;
    SortFilterProxyModel *proxyModel;
    HeaderView *header;
    const int comm_id;
    int total, left, row, usage_id, user_id, amount, loggedIdUserId;
    QString letter_id, desc, commodity, user, ts;
    QMenu contextMenu;
};


} // namespace Usage
}
#endif // USAGE_DIALOG_H
