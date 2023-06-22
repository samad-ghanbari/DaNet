#ifndef TELEPHONEREMOVEDIALOG_H
#define TELEPHONEREMOVEDIALOG_H

#include <QDialog>
class DanetDbMan;

namespace Ui {
class TelephoneRemoveDialog;
}

class TelephoneRemoveDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TelephoneRemoveDialog(DanetDbMan *db, const int ID, const QString name, const QString dep, const QString pos, const QString tel, QWidget *parent = nullptr);
    ~TelephoneRemoveDialog();

private slots:
    void on_confirmChb_toggled(bool checked);

    void on_okBtn_clicked();

private:
    Ui::TelephoneRemoveDialog *ui;
    DanetDbMan *dbMan;
    const int id;
};

#endif // TELEPHONEREMOVEDIALOG_H
