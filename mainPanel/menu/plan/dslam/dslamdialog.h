#ifndef DSLAMDIALOG_H
#define DSLAMDIALOG_H

#include <QDialog>
#include <QMenu>

class DanetDbMan;
class QSqlQueryModel;
class QStringListModel;

namespace Ui {
class DslamDialog;
}

class DslamDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DslamDialog(QWidget *parent, DanetDbMan *Db);
    explicit DslamDialog(QWidget *parent, DanetDbMan *Db, const int area,const int ExchId,const int type, const int SiteId, const int DevexId);
    ~DslamDialog();
    void init();

private slots:
    void on_areaCB_currentIndexChanged(int index);
    void on_abbrCB_currentIndexChanged(int index);
    void on_typeCB_currentIndexChanged(int index);
    void on_siteCB_currentIndexChanged(int index);
    void on_dslamCB_currentIndexChanged(int index);
    void updateDslamCB(int i);

private:
    Ui::DslamDialog *ui;
    DanetDbMan *dbMan;
    int dslamId,area, exchId;
    QString dslamName;
};

#endif // DSLAMDIALOG_H
