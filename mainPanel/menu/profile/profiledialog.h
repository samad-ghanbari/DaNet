#ifndef PROFILEDIALOG_H
#define PROFILEDIALOG_H

#include <QDialog>
class DanetDbMan;

namespace Ui {
class ProfileDialog;
}

class ProfileDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ProfileDialog(QWidget *parent, DanetDbMan *db);
    ~ProfileDialog();
    void checkEntry();

private slots:
    void on_cancelBtn_clicked();
    void on_okBtn_clicked();
    void on_usernameLE_textChanged(const QString &arg1);
    void on_areaCB_currentIndexChanged(int index);
    void on_passChB_toggled(bool checked);
    void on_passLE_textChanged(const QString &arg1);
    void on_passConfirmLE_textChanged(const QString &arg1);

    void on_currentPassLE_textChanged(const QString &arg1);

private:
    Ui::ProfileDialog *ui;
    DanetDbMan *dbMan;
    QString name, username,currentPass;
    int userId;
    bool permitFlag;

};

#endif // PROFILEDIALOG_H
