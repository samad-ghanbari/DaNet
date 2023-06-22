#ifndef EDITODFPINDIALOG_H
#define EDITODFPINDIALOG_H

#include <QDialog>
class DanetDbMan;

namespace Ui {
class EditOdfPinDialog;
}

class EditOdfPinDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EditOdfPinDialog(QWidget *parent, DanetDbMan *db, const int PinId, const QString exch, const QString saloon, const QString odf, const QString pos, const QString pin,
                              const QString LocalDevice,const QString LocalLabel,const QString Remote,const QString RemoteLabel);
    ~EditOdfPinDialog();
    bool checkFields();

private slots:
    void on_localDevLE_textChanged(const QString &arg1);
    void on_localLblLE_textChanged(const QString &arg1);
    void on_remoteLblLE_textChanged(const QString &arg1);
    void on_cancelBtn_clicked();
    void on_okBtn_clicked();

    void on_remoteLE_textChanged(const QString &arg1);

private:
    Ui::EditOdfPinDialog *ui;
    DanetDbMan *dbMan;
    const int pinId;
};

#endif // EDITODFPINDIALOG_H
