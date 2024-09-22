#ifndef CLEARLOGDIALOG_H
#define CLEARLOGDIALOG_H

#include <QDialog>
class DanetDbMan;

namespace Ui {
class ClearLogDialog;
}

class ClearLogDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ClearLogDialog(QWidget *parent, DanetDbMan *db);
    ~ClearLogDialog();

private slots:
    void on_cancelBtn_clicked();

    void on_okBtn_clicked();

    void on_checkBox_toggled(bool checked);

private:
    Ui::ClearLogDialog *ui;
    DanetDbMan *dbMan;
};

#endif // CLEARLOGDIALOG_H
