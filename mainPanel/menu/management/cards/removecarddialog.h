#ifndef REMOVECARDDIALOG_H
#define REMOVECARDDIALOG_H

#include <QDialog>
class DanetDbMan;

namespace Ui {
class RemoveCardDialog;
}

class RemoveCardDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RemoveCardDialog(QWidget *parent, DanetDbMan *db, int Id, QString card, QString device, QString type);
    ~RemoveCardDialog();

private slots:
    void on_cancelBtn_clicked();
    void on_removeBtn_clicked();

    void on_checkBox_toggled(bool checked);

private:
    Ui::RemoveCardDialog *ui;
    DanetDbMan *dbMan;
    const int id;
};

#endif // REMOVECARDDIALOG_H
