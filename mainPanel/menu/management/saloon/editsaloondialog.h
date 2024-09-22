#ifndef EDITSALOONDIALOG_H
#define EDITSALOONDIALOG_H

#include <QDialog>
class DanetDbMan;

namespace Ui {
class EditSaloonDialog;
}

class EditSaloonDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EditSaloonDialog(QWidget *parent, DanetDbMan *db, const int Id,const QString Sal);
    ~EditSaloonDialog();

private slots:
    void on_CancelBtn_clicked();

    void on_okBtn_clicked();
    void on_saloonLE_textChanged(const QString &arg1);

private:
    Ui::EditSaloonDialog *ui;
    DanetDbMan *dbMan;
    const int id;
};

#endif // EDITSALOONDIALOG_H
