#ifndef EDITNETV4DIALOG_H
#define EDITNETV4DIALOG_H

#include <QDialog>
class DanetDbMan;
class QIntValidator;
class QRegExpValidator;

namespace Ui {
class EditNetV4Dialog;
}

class EditNetV4Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit EditNetV4Dialog(QWidget *parent, DanetDbMan *db, const int NetPoolId, const QString net, const int prefix);
    ~EditNetV4Dialog();
    void entryCheck();

private slots:
    void on_cancelBtn_clicked();
    void on_okBtn_clicked();

    void on_netLE_textChanged(const QString &arg1);
    void on_prefixLE_textChanged(const QString &arg1);
    void on_maskLE_textChanged(const QString &arg1);

private:
    Ui::EditNetV4Dialog *ui;
    DanetDbMan *dbMan;
    const int netPoolId;
    const QString ipRange;
    const QRegExp ipRegex;
    QIntValidator *intValidator;
    QRegExpValidator *ipValidator;
    QString oldNet;
};

#endif // EDITNETV4DIALOG_H
