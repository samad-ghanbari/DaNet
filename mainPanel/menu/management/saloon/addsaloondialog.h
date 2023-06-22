#ifndef ADDSALOONDIALOG_H
#define ADDSALOONDIALOG_H

#include <QDialog>
class DanetDbMan;

namespace Ui {
class AddSaloonDialog;
}

class AddSaloonDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddSaloonDialog(QWidget *parent, DanetDbMan *db);
    ~AddSaloonDialog();

private slots:
    void on_saloonLE_textChanged(const QString &arg1);

    void on_CancelBtn_clicked();

    void on_okBtn_clicked();

private:
    Ui::AddSaloonDialog *ui;
    DanetDbMan *dbMan;
};

#endif // ADDSALOONDIALOG_H
