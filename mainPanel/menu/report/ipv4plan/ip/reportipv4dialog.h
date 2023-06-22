#ifndef REPORTIPV4DIALOG_H
#define REPORTIPV4DIALOG_H

#include <QDialog>

class DanetDbMan;
class QSqlQueryModel;
class QRegExpValidator;

namespace Ui {
class ReportIpV4Dialog;
}

class ReportIpV4Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit ReportIpV4Dialog(QWidget *parent, DanetDbMan *db);
    ~ReportIpV4Dialog();
    void checkEntry();

private slots:
    void on_searchBtn_clicked();
    void on_okBtn_clicked();
    void on_cancelBtn_clicked();
    void on_netLE_textChanged(const QString &arg1);
    void on_ipLE_textChanged(const QString &arg1);
    void on_descLE_textChanged(const QString &arg1);
    void on_prefixSB_valueChanged(int arg1);

private:
    Ui::ReportIpV4Dialog *ui;
    DanetDbMan *dbMan;
    QSqlQueryModel *model;
    const QString ipRange;
    const QRegExp ipRegex;
    QRegExpValidator *ipValidator;
    QList<QString> subnetInfo;
};

#endif // REPORTIPV4DIALOG_H
