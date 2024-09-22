#ifndef REPORTSUBNETV4DIALOG_H
#define REPORTSUBNETV4DIALOG_H

#include <QDialog>

class DanetDbMan;
class QSqlQueryModel;

namespace Ui {
class ReportSubnetV4Dialog;
}

class ReportSubnetV4Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit ReportSubnetV4Dialog(QWidget *parent, DanetDbMan *db);
    ~ReportSubnetV4Dialog();

private slots:
    void on_cancelBtn_clicked();
    void on_prefixChB_toggled(bool checked);
    void on_searchBtn_clicked();
    void on_areaCB_currentIndexChanged(int index);
    void on_okBtn_clicked();

    void on_subnet1RB_toggled(bool checked);

    void on_subnet2RB_toggled(bool checked);

    void on_subnet3RB_toggled(bool checked);

    void on_netLE_textChanged(const QString &arg1);

    void on_abbrCB_currentIndexChanged(int index);

    void on_appLE_textChanged(const QString &arg1);

    void on_descLE_textChanged(const QString &arg1);

    void on_prefixSB_valueChanged(int arg1);

private:
    Ui::ReportSubnetV4Dialog *ui;
    DanetDbMan *dbMan;
    QSqlQueryModel *model;
    int subnetPoolNo;
};

#endif // REPORTSUBNETV4DIALOG_H
