#ifndef ADDVSIREFDIALOG_H
#define ADDVSIREFDIALOG_H

#include <QDialog>
class DanetDbMan;
namespace Ui {
class AddVsiRefDialog;
}

class AddVsiRefDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddVsiRefDialog(QWidget *parent, DanetDbMan *db);
    ~AddVsiRefDialog();
    void checkEntry();

private:
    Ui::AddVsiRefDialog *ui;
    DanetDbMan *dbMan;


private slots:
    void on_CancelBtn_clicked();
    void on_okBtn_clicked();
    void on_descTE_textChanged();
};

#endif // ADDVSIREFDIALOG_H
