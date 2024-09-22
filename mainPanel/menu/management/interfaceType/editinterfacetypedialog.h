#ifndef EDITINTERFACETYPEDIALOG_H
#define EDITINTERFACETYPEDIALOG_H

#include <QDialog>
class DanetDbMan;

namespace Ui {
class EditInterfaceTypeDialog;
}

class EditInterfaceTypeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EditInterfaceTypeDialog(QWidget *parent, DanetDbMan *db, const int Id, const QString intType, const double speed, const QString media);
    ~EditInterfaceTypeDialog();
    void entryCheck();
private slots:
    void on_CancelBtn_clicked();
    void on_okBtn_clicked();

    void on_intTypeLE_textChanged(const QString &arg1);

    void on_speedSB_valueChanged(double arg1);

    void on_mediaLE_textChanged(const QString &arg1);

private:
    Ui::EditInterfaceTypeDialog *ui;
    DanetDbMan *dbMan;
    const int id;
};

#endif // EDITINTERFACETYPEDIALOG_H
