#ifndef REMOVEPOSDIALOG_H
#define REMOVEPOSDIALOG_H

#include <QDialog>
class DanetDbMan;

namespace Ui {
class RemovePosDialog;
}

class RemovePosDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RemovePosDialog(QWidget *parent, DanetDbMan *db, QString AreaAbbr, int saloon, int PosId, int OdfNo, int PosNo);
    ~RemovePosDialog();

private slots:
    void on_cancelBtn_clicked();
    void on_removeBtn_clicked();

    void on_checkBox_toggled(bool checked);

private:
    Ui::RemovePosDialog *ui;
    DanetDbMan *dbMan;
    int posId;
};

#endif // REMOVEPOSDIALOG_H
