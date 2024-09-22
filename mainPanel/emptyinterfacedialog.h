#ifndef EMPTYINTERFACEDIALOG_H
#define EMPTYINTERFACEDIALOG_H

#include <QDialog>
class DanetDbMan;

namespace Ui {
class EmptyInterfaceDialog;
}

class EmptyInterfaceDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EmptyInterfaceDialog(QWidget *parent, DanetDbMan *db, const int IntId, const int devexId, const QString Device, const QString devInt, const QString Label, const QString module, const QString Eth, const QString PeerExch, const QString PeerDevice, const QString PeerInt, const QString PeerLabel, const QString desc);
    ~EmptyInterfaceDialog();

private slots:
    void on_cancelBtn_clicked();
    void on_okBtn_clicked();
    void on_cb_toggled(bool checked);

private:
    Ui::EmptyInterfaceDialog *ui;
    DanetDbMan *dbMan;
    const int intId;
    const int eth;
    QString devName;
};

#endif // EMPTYINTERFACEDIALOG_H
