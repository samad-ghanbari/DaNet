#ifndef REMOVEMODULEDIALOG_H
#define REMOVEMODULEDIALOG_H

#include <QDialog>
class DanetDbMan;

namespace Ui {
class RemoveModuleDialog;
}

class RemoveModuleDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RemoveModuleDialog(QWidget *parent, DanetDbMan *db, const int Id, const QString module, const QString vendor, const double km);
    ~RemoveModuleDialog();

private slots:
    void on_checkBox_toggled(bool checked);

    void on_CancelBtn_clicked();

    void on_okBtn_clicked();

private:
    Ui::RemoveModuleDialog *ui;
    DanetDbMan *dbMan;
    const int id;

};

#endif // REMOVEMODULEDIALOG_H
