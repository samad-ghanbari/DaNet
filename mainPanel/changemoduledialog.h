#ifndef CHANGEMODULEDIALOG_H
#define CHANGEMODULEDIALOG_H

#include <QDialog>
class DanetDbMan;

namespace Ui {
class ChangeModuleDialog;
}

class ChangeModuleDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ChangeModuleDialog(QWidget *parent, DanetDbMan *db, const int IntId, const QString abbr,const QString saloon, const QString device, const QString type, const QString interface, const QString module);
    ~ChangeModuleDialog();

private slots:
    void on_moduleCB_currentIndexChanged(int index);

    void on_cancelBtn_clicked();

    void on_okBtn_clicked();

private:
    Ui::ChangeModuleDialog *ui;
    DanetDbMan *dbMan;
    const int intId;
};

#endif // CHANGEMODULEDIALOG_H
