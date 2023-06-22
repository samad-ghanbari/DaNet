#ifndef LETTERVIEWDIALOG_H
#define LETTERVIEWDIALOG_H

#include <QDialog>

namespace Ui {
class LetterViewDialog;
}

class LetterViewDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LetterViewDialog(const QString letter_id, const QString letter_no, QString from, const QString to, const QString desc, const QString ts, QWidget *parent = nullptr);
    ~LetterViewDialog();

private slots:
    void on_okBtn_clicked();

private:
    Ui::LetterViewDialog *ui;
};

#endif // LETTERVIEWDIALOG_H
