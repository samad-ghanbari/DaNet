#ifndef ASSIGNODFPINDIALOG_H
#define ASSIGNODFPINDIALOG_H

#include <QDialog>
class DanetDbMan;

namespace Ui {
class AssignOdfPinDialog;
}

class AssignOdfPinDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AssignOdfPinDialog(QWidget *parent, DanetDbMan *db, const int PinId, const QString exch, const QString saloon, const QString odf, const QString pos, const QString pin);
    ~AssignOdfPinDialog();
    bool checkFields();

private slots:
    void on_localDevLE_textChanged(const QString &arg1);
    void on_localLblLE_textChanged(const QString &arg1);
    void on_remoteLblLE_textChanged(const QString &arg1);
    void on_cancelBtn_clicked();
    void on_okBtn_clicked();
    void on_remoteLE_textChanged(const QString &arg1);

private:
    Ui::AssignOdfPinDialog *ui;
    DanetDbMan *dbMan;
    const int pinId;
};

#endif // ASSIGNODFPINDIALOG_H
