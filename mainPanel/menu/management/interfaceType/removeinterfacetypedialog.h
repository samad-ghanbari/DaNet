#ifndef REMOVEINTERFACETYPEDIALOG_H
#define REMOVEINTERFACETYPEDIALOG_H

#include <QDialog>
class DanetDbMan;

namespace Ui {
class RemoveInterfaceTypeDialog;
}

class RemoveInterfaceTypeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RemoveInterfaceTypeDialog(QWidget *parent, DanetDbMan *db, const int Id, const QString intType, const double speed, const QString media);
    ~RemoveInterfaceTypeDialog();

private slots:
    void on_checkBox_toggled(bool checked);

    void on_CancelBtn_clicked();

    void on_okBtn_clicked();

private:
    Ui::RemoveInterfaceTypeDialog *ui;
    DanetDbMan *dbMan;
    const int id;

};

#endif // REMOVEINTERFACETYPEDIALOG_H
