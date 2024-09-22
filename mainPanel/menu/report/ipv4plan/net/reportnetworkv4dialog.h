#ifndef REPORTNETWORKV4DIALOG_H
#define REPORTNETWORKV4DIALOG_H

#include <QDialog>

class DanetDbMan;
class QSqlQueryModel;

namespace Ui {
class ReportNetworkV4Dialog;
}

class ReportNetworkV4Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit ReportNetworkV4Dialog(QWidget *parent, DanetDbMan *db);
    ~ReportNetworkV4Dialog();

private slots:
    void on_okBtn_clicked();
    void on_cancelBtn_clicked();
    void on_prefixChB_toggled(bool checked);
    void on_searchBtn_clicked();
    void on_netLE_textChanged(const QString &arg1);
    void on_prefixSB_valueChanged(int arg1);
    void on_descLE_textChanged(const QString &arg1);

private:
    Ui::ReportNetworkV4Dialog *ui;
    DanetDbMan *dbMan;
    QSqlQueryModel *model;
};

#endif // REPORTNETWORKV4DIALOG_H
