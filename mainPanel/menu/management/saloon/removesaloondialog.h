#ifndef REMOVESALOONDIALOG_H
#define REMOVESALOONDIALOG_H

#include <QDialog>
class DanetDbMan;

namespace Ui {
class RemoveSaloonDialog;
}

class RemoveSaloonDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RemoveSaloonDialog(QWidget *parent, DanetDbMan *db, const int Id, const QString Sal);
    ~RemoveSaloonDialog();

private slots:
    void on_CancelBtn_clicked();

    void on_okBtn_clicked();

    void on_checkBox_toggled(bool checked);

private:
    Ui::RemoveSaloonDialog *ui;
    DanetDbMan *dbMan;
    const int id;
};

#endif // REMOVEPURCHASEDIALOG_H
