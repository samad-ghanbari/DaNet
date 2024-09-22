#ifndef SUBNETV4SEARCHDIALOG_H
#define SUBNETV4SEARCHDIALOG_H

#include <QDialog>

class DanetDbMan;
class QStandardItemModel;

namespace Ui {
class SubnetV4SearchDialog;
}

class SubnetV4SearchDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SubnetV4SearchDialog( QWidget *parent, DanetDbMan *DbMan);
    ~SubnetV4SearchDialog();
    void checkEntry();

private slots:
    void on_subnetLE_textChanged(const QString &arg1);
    void on_areaCB_currentIndexChanged(int index);
    void on_abbrCB_currentIndexChanged(int index);
    void on_appLE_textChanged(const QString &arg1);
    void on_descLE_textChanged(const QString &arg1);
    void on_searchBtn_clicked();

    void on_exportBtn_clicked();

private:
    Ui::SubnetV4SearchDialog *ui;
    DanetDbMan *dbMan;
    QStandardItemModel *model;
};

#endif // SUBNETV4SEARCHDIALOG_H
