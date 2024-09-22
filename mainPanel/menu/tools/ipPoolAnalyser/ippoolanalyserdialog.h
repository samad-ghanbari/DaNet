#ifndef IPPOOLANALYSERDIALOG_H
#define IPPOOLANALYSERDIALOG_H

#include <QDialog>
#include <QFile>
#include <QTextStream>

namespace Ui {
class IpPoolAnalyserDialog;
}

class IpPoolAnalyserDialog : public QDialog
{
    Q_OBJECT

public:
    explicit IpPoolAnalyserDialog(QWidget *parent = 0);
    ~IpPoolAnalyserDialog();
    void emptyParameters();
    void huaweiIpPoolAnalyse();
    void IOSIpPoolAnalyse();
    void IOSXRIpPoolAnalyse();

private slots:
    void on_osCB_currentIndexChanged(int index);
    void on_btn_clicked();

private:
    Ui::IpPoolAnalyserDialog *ui;
    QFile configFile;
};

#endif // IPPOOLANALYSERDIALOG_H
