#ifndef MODULEANALYSERDIALOG_H
#define MODULEANALYSERDIALOG_H

#include <QDialog>
#include <QFile>
#include <QBrush>
class QStandardItemModel;


namespace Ui {
class ModuleAnalyserDialog;
}

class ModuleAnalyserDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ModuleAnalyserDialog(QWidget *parent = 0);
    ~ModuleAnalyserDialog();
    void huaweiModulesAnalyser();
    void IOSModulesAnalyser();
    void IOSXRModulesAnalyser();

private slots:
    void on_osCB_currentIndexChanged(int index);
    void on_btn_clicked();

    void on_exportBtn_clicked();

private:
    Ui::ModuleAnalyserDialog *ui;
    QStandardItemModel *model;
    QFile configFile;
    QBrush redBrush;
    bool INFO_FLAG;
};

#endif // MODULEANALYSERDIALOG_H
