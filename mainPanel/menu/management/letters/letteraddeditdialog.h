#ifndef LETTERADDEDITDIALOG_H
#define LETTERADDEDITDIALOG_H

#include <QDialog>
class DanetDbMan;

namespace Ui {
class LetterAddEditDialog;
}

class LetterAddEditDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LetterAddEditDialog(DanetDbMan *db, QWidget *parent = nullptr);
    ~LetterAddEditDialog();
    void setValues(int Id,QString letter_id, QString letter_no, QString from_dep, QString to_dep, QString desc);

private slots:
    void on_okBtn_clicked();

private:
    Ui::LetterAddEditDialog *ui;
    DanetDbMan *dbMan;
    int id;
};

#endif // LETTERADDEDITDIALOG_H
