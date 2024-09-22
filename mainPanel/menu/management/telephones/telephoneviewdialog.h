#ifndef TELEPHONEVIEWDIALOG_H
#define TELEPHONEVIEWDIALOG_H

#include <QDialog>
namespace Ui {
class TelephoneViewDialog;
}

class TelephoneViewDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TelephoneViewDialog(const QString name, const QString dep, const QString pos, const QString tel, QWidget *parent = nullptr);
    ~TelephoneViewDialog();

private slots:
    void on_okBtn_clicked();

private:
    Ui::TelephoneViewDialog *ui;
};

#endif // TELEPHONEVIEWDIALOG_H
