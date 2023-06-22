#ifndef CARDSTATDIALOG_H
#define CARDSTATDIALOG_H

#include <QDialog>
class DanetDbMan;
class QStandardItemModel;

namespace Ui {
class CardStatDialog;
}

class CardStatDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CardStatDialog(QWidget *parent, DanetDbMan *db);
    ~CardStatDialog();

private slots:
    void on_areaCB_currentIndexChanged(int index);
    void on_abbrCB_currentIndexChanged(int index);
    void on_typeCB_currentIndexChanged(int index);
    void on_siteCB_currentIndexChanged(int index);
    void on_devCB_currentIndexChanged(int index);
    void on_cardCB_currentIndexChanged(int index);

    void on_CancelBtn_clicked();

private:
    Ui::CardStatDialog *ui;
    DanetDbMan *dbMan;
    QStandardItemModel *model;
};

#endif // CARDSTATDIALOG_H
