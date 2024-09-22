#ifndef ADDNASIPDIALOG_H
#define ADDNASIPDIALOG_H

#include <QDialog>
class DanetDbMan;
class QRegExpValidator;

namespace Ui {
class AddNasIpDialog;
}

class AddNasIpDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddNasIpDialog(QWidget *parent, DanetDbMan *db);
    ~AddNasIpDialog();

private slots:
    void on_areaCB_currentIndexChanged(int index);
    void on_brasCB_currentIndexChanged(int index);
    void on_nasLE_textChanged(const QString &arg1);
    void on_cancelBtn_clicked();
    void on_okBtn_clicked();

private:
    Ui::AddNasIpDialog *ui;
    DanetDbMan *dbMan;
    const QString ipRange;
    const QRegExp ipRegex;
    QRegExpValidator *ipValidator;
};

#endif // ADDNASIPDIALOG_H
