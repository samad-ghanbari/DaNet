#ifndef ADDINTERFACETYPEDIALOG_H
#define ADDINTERFACETYPEDIALOG_H

#include <QDialog>
class DanetDbMan;

namespace Ui {
class AddInterfaceTypeDialog;
}

class AddInterfaceTypeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddInterfaceTypeDialog(QWidget *parent, DanetDbMan *db);
    ~AddInterfaceTypeDialog();
    void entryCheck();

private slots:
    void on_CancelBtn_clicked();
    void on_okBtn_clicked();

    void on_intTypeLE_textChanged(const QString &arg1);

    void on_speedSB_valueChanged(double arg1);

    void on_mediaLE_textChanged(const QString &arg1);

private:
    Ui::AddInterfaceTypeDialog *ui;
    DanetDbMan *dbMan;
};

#endif // ADDINTERFACETYPEDIALOG_H
