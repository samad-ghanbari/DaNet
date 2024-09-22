#ifndef LETTERREMOVEDIALOG_H
#define LETTERREMOVEDIALOG_H

#include <QDialog>
class DanetDbMan;

namespace Ui {
class LetterRemoveDialog;
}

class LetterRemoveDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LetterRemoveDialog(DanetDbMan *db, const int ID, const QString letter_id, const QString letter_no, QString from, const QString to, const QString desc, const QString ts, QWidget *parent = nullptr);
    ~LetterRemoveDialog();

private slots:
    void on_confirmChb_toggled(bool checked);

    void on_okBtn_clicked();

private:
    Ui::LetterRemoveDialog *ui;
    DanetDbMan *dbMan;
    const int id;
};

#endif // LETTERREMOVEDIALOG_H
