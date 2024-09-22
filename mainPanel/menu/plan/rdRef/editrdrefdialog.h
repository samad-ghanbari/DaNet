#ifndef EDITRDREFDIALOG_H
#define EDITRDREFDIALOG_H

#include <QDialog>
class DanetDbMan;
class QRegExpValidator;

namespace Ui {
class EditRdRefDialog;
}

class EditRdRefDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EditRdRefDialog(QWidget *parent, DanetDbMan *db, const int Id, const QString rd, const QString desc);
    ~EditRdRefDialog();
    void checkEntry();

private slots:
    void on_CancelBtn_clicked();
    void on_okBtn_clicked();

    void on_rdLE_textEdited(const QString &arg1);

    void on_descTE_textChanged();

private:
    Ui::EditRdRefDialog *ui;
    DanetDbMan *dbMan;
    const int id;
    const QRegExp regex;
    QRegExpValidator *validator;

    QString oldRd;
};

#endif // EDITRDREFDIALOG_H
