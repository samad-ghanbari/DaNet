#ifndef NETWORKASSISTANTDIALOG_H
#define NETWORKASSISTANTDIALOG_H

#include <QDialog>

namespace Ui {
class NetworkAssistantDialog;
}

class NetworkAssistantDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NetworkAssistantDialog(QWidget *parent);
    ~NetworkAssistantDialog();


private slots:
    void on_osCB_currentIndexChanged(int index);
    void on_configCB_currentIndexChanged(int index);
    void on_exportBtn_clicked();
    void on_clearBtn_clicked();
    void on_configTE_textChanged();
    void on_sepBtn_clicked();
    void setWriteBtnEnableSlot(bool en);

private:
    Ui::NetworkAssistantDialog *ui;
};

#endif // NETWORKASSISTANTDIALOG_H
