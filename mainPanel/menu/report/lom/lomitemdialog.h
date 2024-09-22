#ifndef LOMITEMDIALOG_H
#define LOMITEMDIALOG_H

#include <QDialog>
#include <QMap>

namespace Ui {
class LomItemDialog;
}

class DanetDbMan;

class LomItemDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LomItemDialog(DanetDbMan *db);
    ~LomItemDialog();
    QString CommType, CommInfo, Purchase, Location,Code, Desc;
    int Amount;
    bool done;
    void setLabel(QString lbl);
    void setOkBtnText(QString txt);
    void setValues(QString type, QString info, int amount, QString purchase, QString location, QString code, QString desc);

private slots:
    void on_okBtn_clicked();
    void on_commCatCB_currentIndexChanged(int index);
    void on_commCB_currentIndexChanged(int index);

private:
    Ui::LomItemDialog *ui;
    DanetDbMan *dbMan;
    QMap<int, QVariantList> commInfoMap;

};

#endif // LOMITEMDIALOG_H
