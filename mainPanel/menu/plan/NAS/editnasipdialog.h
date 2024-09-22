#ifndef EDITNASIPDIALOG_H
#define EDITNASIPDIALOG_H

#include <QDialog>
class DanetDbMan;
class QIntValidator;
class QRegExpValidator;

namespace Ui {
class EditNasIpDialog;
}

class EditNasIpDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EditNasIpDialog(QWidget *parent, DanetDbMan *db, const int Id, const int DevexId, const int Area, const QString Nas);
    ~EditNasIpDialog();

private slots:
    void on_areaCB_currentIndexChanged(int index);
    void on_brasCB_currentIndexChanged(int index);
    void on_nasLE_textChanged(const QString &arg1);
    void on_cancelBtn_clicked();
    void on_okBtn_clicked();

private:
    Ui::EditNasIpDialog *ui;
    DanetDbMan *dbMan;
    const int id;
    const QString ipRange;
    const QRegExp ipRegex;
    QRegExpValidator *ipValidator;
};

#endif // EDITNASIPDIALOG_H
