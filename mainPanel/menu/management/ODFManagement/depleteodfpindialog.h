#ifndef DEPLETEODFPINDIALOG_H
#define DEPLETEODFPINDIALOG_H

#include <QDialog>
class DanetDbMan;
namespace Ui {
class DepleteOdfPinDialog;
}

class DepleteOdfPinDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DepleteOdfPinDialog(QWidget *parent, DanetDbMan *db, const int PinId, const QString exch, const QString saloon, const QString odf, const QString pos, const QString pin, const QString LocalDevice, const QString LocalLabel, const QString Remote, const QString RemoteLabel);
    ~DepleteOdfPinDialog();

private slots:
    void on_checkBox_toggled(bool checked);

    void on_cancelBtn_clicked();

    void on_okBtn_clicked();

private:
    Ui::DepleteOdfPinDialog *ui;
    DanetDbMan *dbMan;
    const int pinId;
};

#endif // DEPLETEODFPINDIALOG_H
