#ifndef CHANGEINTERFACETYPEDIALOG_H
#define CHANGEINTERFACETYPEDIALOG_H

#include <QDialog>
class DanetDbMan;

namespace Ui {
class ChangeInterfaceTypeDialog;
}

class ChangeInterfaceTypeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ChangeInterfaceTypeDialog(QWidget *parent, DanetDbMan *db, const int ExchId, const int IntId, const QString abbr, const QString saloon, const QString device, const QString type, const QString interface, const QString module);
    ~ChangeInterfaceTypeDialog();

private slots:
    void on_cancelBtn_clicked();
    void on_okBtn_clicked();
    void on_newTypeCB_currentIndexChanged(int index);
    void on_confirmChB_toggled(bool checked);
    void checkInput();

private:
    Ui::ChangeInterfaceTypeDialog *ui;
    DanetDbMan *dbMan;
    const int intId, exchId;
    const QString INTERFACE, TYPE, ABBDEV;
    bool confirm;
};

#endif // CHANGEINTERFACETYPEDIALOG_H
