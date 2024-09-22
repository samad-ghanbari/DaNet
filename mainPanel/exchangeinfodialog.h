#ifndef EXCHANGEINFODIALOG_H
#define EXCHANGEINFODIALOG_H

#include <QDialog>
class DanetDbMan;

namespace Ui {
class ExchangeInfoDialog;
}

class ExchangeInfoDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ExchangeInfoDialog(QWidget *parent, DanetDbMan *db, const int exchId);
    ~ExchangeInfoDialog();

private slots:
    void on_okBtn_clicked();

private:
    Ui::ExchangeInfoDialog *ui;
};

#endif // EXCHANGEINFODIALOG_H
