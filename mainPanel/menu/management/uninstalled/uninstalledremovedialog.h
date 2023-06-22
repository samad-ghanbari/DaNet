#ifndef UNINSTALLEDREMOVEDIALOG_H
#define UNINSTALLEDREMOVEDIALOG_H

#include <QDialog>
class DanetDbMan;

namespace Ui {
class UninstalledRemoveDialog;
}

class UninstalledRemoveDialog : public QDialog
{
    Q_OBJECT

public:
    explicit UninstalledRemoveDialog(DanetDbMan *db, const int ID, const int area, const QString exch, QString commodity, const int amount, const QString letter, const QString desc, const QString ts, QWidget *parent = nullptr);
    ~UninstalledRemoveDialog();

private slots:
    void on_confirmChb_toggled(bool checked);
    void on_okBtn_clicked();

private:
    Ui::UninstalledRemoveDialog *ui;
    DanetDbMan *dbMan;
    const int id;
};

#endif // UNINSTALLEDREMOVEDIALOG_H
