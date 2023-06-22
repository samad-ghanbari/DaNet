#ifndef ADDNETV4DIALOG_H
#define ADDNETV4DIALOG_H

#include <QDialog>

class DanetDbMan;
class QIntValidator;
class QRegExpValidator;

namespace Ui {
class AddNetV4Dialog;
}

class AddNetV4Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddNetV4Dialog(QWidget *parent, DanetDbMan *db);
    ~AddNetV4Dialog();
    void entryCheck();

private slots:
    void on_cancelBtn_clicked();
    void on_okBtn_clicked();
    void on_netLE_textChanged(const QString &arg1);
    void on_prefixLE_textChanged(const QString &arg1);
    void on_maskLE_textChanged(const QString &arg1);
    void on_descLE_textChanged(const QString &arg1);

private:
    Ui::AddNetV4Dialog *ui;
    DanetDbMan *dbMan;
    const QString ipRange;
    const QRegExp ipRegex;
    QIntValidator *intValidator;
    QRegExpValidator *ipValidator;
};

#endif // ADDNETV4DIALOG_H
