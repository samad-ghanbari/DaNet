#ifndef EDITINTERFACEDIALOG_H
#define EDITINTERFACEDIALOG_H

#include <QDialog>
#include <QSqlQuery>
class DanetDbMan;
class QIntValidator;

namespace Ui {
class EditInterfaceDialog;
}

class EditInterfaceDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EditInterfaceDialog(QWidget *parent, DanetDbMan *db, const int IntId, const int devexId, const QString Device, const QString Int, const QString Label, const QString module, const QString eth, QString PeerAbbr, const QString PeerDevice, const QString PeerInt, const QString PeerLabel, const QString desc);
    ~EditInterfaceDialog();
    QString getLable();
    void checkEntry();

private slots:
    void on_cancelBtn_clicked();
    void on_okBtn_clicked();
    void on_peerIntLE_textChanged(const QString &arg1);
    void on_peerLblLE_textChanged(const QString &arg1);
    void on_peerDevLE_textChanged(const QString &arg1);
    void on_peerExchLE_textChanged(const QString &arg1);
    void on_etherLE_textChanged(const QString &arg1);
    void on_moduleCB_currentIndexChanged(int index);

    void on_descTE_textChanged();

private:
    Ui::EditInterfaceDialog *ui;
    QSqlQuery *query;
    DanetDbMan *dbMan;
    const int intId;
    QIntValidator *ethValidator;
    QString devName;

};

#endif // EDITINTERFACEDIALOG_H
