#ifndef EXCHANGESSTATDIALOG_H
#define EXCHANGESSTATDIALOG_H

#include <QDialog>
class DanetDbMan;

namespace Ui {
class ExchangesStatDialog;
}

class ExchangesStatDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ExchangesStatDialog(QWidget *parent, DanetDbMan *db);
    ~ExchangesStatDialog();

private slots:
    void on_CancelBtn_clicked();
    void on_areaCB_currentIndexChanged(int index);

private:
    Ui::ExchangesStatDialog *ui;
    DanetDbMan *dbMan;
};

#endif // EXCHANGESSTATDIALOG_H
