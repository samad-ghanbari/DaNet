#ifndef SBCIPLOOPBACKDIALOG_H
#define SBCIPLOOPBACKDIALOG_H

#include <QDialog>
class DanetDbMan;
class QRegExpValidator;

namespace Ui {
class SbcIpLoopBackDialog;
}

class SbcIpLoopBackDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SbcIpLoopBackDialog(QWidget *parent, DanetDbMan *db);
    ~SbcIpLoopBackDialog();
    void checkEntry();

private slots:
    void on_areaCB_currentIndexChanged(int index);
    void on_abbrCB_currentTextChanged(const QString &arg1);
    void on_remoteIpLE_textChanged(const QString &arg1);
    void on_ospfCB_toggled(bool checked);
    void on_ospfLE_textChanged(const QString &arg1);
    void on_lbNetLE_textChanged(const QString &arg1);
    void on_lbMaskLE_textChanged(const QString &arg1);
    void on_cancelBtn_clicked();
    void on_okBtn_clicked();
    void on_directCB_toggled(bool checked);

private:
    Ui::SbcIpLoopBackDialog *ui;
    DanetDbMan *dbMan;
    const QString ipRange;
    const QRegExp ipRegex;
    QRegExpValidator *ipValidator;
};

#endif // SBCIPLOOPBACKDIALOG_H
