#ifndef ADDRDREFDIALOG_H
#define ADDRDREFDIALOG_H

#include <QDialog>
class QRegExpValidator;

class DanetDbMan;
namespace Ui {
class AddRdRefDialog;
}

class AddRdRefDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddRdRefDialog(QWidget *parent, DanetDbMan *db);
    ~AddRdRefDialog();
    void checkEntry();

private:
    Ui::AddRdRefDialog *ui;
    DanetDbMan *dbMan;
    const QRegExp regex;
    QRegExpValidator *validator;


private slots:
    void on_CancelBtn_clicked();
    void on_okBtn_clicked();
    void on_rdLE_textEdited(const QString &arg1);
    void on_descTE_textChanged();
};

#endif // ADDRDREFDIALOG_H
