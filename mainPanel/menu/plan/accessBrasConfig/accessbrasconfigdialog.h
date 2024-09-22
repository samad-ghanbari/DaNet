#ifndef ACCESSBRASCONFIGDIALOG_H
#define ACCESSBRASCONFIGDIALOG_H

#include <QDialog>
class DanetDbMan;

namespace Ui {
class AccessBRASConfigDialog;
}

class AccessBRASConfigDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AccessBRASConfigDialog(QWidget *parent, DanetDbMan *db);
    ~AccessBRASConfigDialog();
    void checkEntry();

private slots:
    void on_areaCB_currentIndexChanged(int index);
    void on_abbrCB_currentIndexChanged(int index);
    void on_okBtn_clicked();
    void on_bras2EnCB_toggled(bool checked);
    void on_bras1CB_currentIndexChanged(int index);
    void on_bras2CB_currentIndexChanged(int index);

private:
    Ui::AccessBRASConfigDialog *ui;
    DanetDbMan *dbMan;
};

#endif // ACCESSBRASCONFIGDIALOG_H
