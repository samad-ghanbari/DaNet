#ifndef ASSIGNINTERFACEDIALOG_H
#define ASSIGNINTERFACEDIALOG_H

#include <QDialog>
#include <QSqlQuery>
class QIntValidator;

class DanetDbMan;
namespace Ui {
class AssignInterfaceDialog;
}

class AssignInterfaceDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AssignInterfaceDialog(QWidget *parent, DanetDbMan *db, int IntId, QString Device, QString Int, QString Label, QString module);
    ~AssignInterfaceDialog();
    QString getLable();
    void checkEntry();

private slots:
    void on_cancelBtn_clicked();
    void on_okBtn_clicked();
    void on_areaCB_currentIndexChanged(int index);
    void on_peerExchCB_currentIndexChanged(int index);
    void on_peerDevCB_currentIndexChanged(int index);
    void on_peerIntLE_textChanged(const QString &arg1);
    void on_peerLblLE_textChanged(const QString &arg1);
    void on_peerDevLE_textChanged(const QString &arg1);
    void on_peerExchTypeCB_currentIndexChanged(int index);
    void on_peerSiteCB_currentIndexChanged(int index);
    void on_peerExchLE_textChanged(const QString &arg1);

private:
    Ui::AssignInterfaceDialog *ui;
    QSqlQuery *query;
    DanetDbMan *dbMan;
    const int intId;
    QIntValidator *ethValidator;
    QString devName;

};

#endif // ASSIGNINTERFACEDIALOG_H
