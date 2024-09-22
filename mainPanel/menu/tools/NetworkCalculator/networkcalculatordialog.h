#ifndef NETWORKCALCULATORDIALOG_H
#define NETWORKCALCULATORDIALOG_H

#include <QDialog>
class QRegExpValidator;
class QStandardItemModel;
class QStringListModel;

namespace Ui {
class NetworkCalculatorDialog;
}

class NetworkCalculatorDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NetworkCalculatorDialog(QWidget *parent = 0);
    ~NetworkCalculatorDialog();
    void splitIp(QString ip, int prefix);
    void ipCalculate();
    void emptyIpCalculate();


private slots:
    void on_classCB_currentIndexChanged(int index);
    void on_NIPClassLE_textChanged(const QString &arg1);
    void on_SIPLE_textChanged(const QString &arg1);
    void on_SPrefixSB_valueChanged(int arg1);
    void on_SMaskLE_textChanged(const QString &arg1);
    void on_CIpLE_textChanged(const QString &arg1);
    void on_CPrefixSB_valueChanged(int arg1);
    void on_CMaskLE_textChanged(const QString &arg1);
    void on_C2PrefixSB_valueChanged(int arg1);
    void on_C2MaskLE_textChanged(const QString &arg1);

private:
    Ui::NetworkCalculatorDialog *ui;
    QStandardItemModel *model;
    QStringListModel *LVModel;
    const QString ipRange;
    const QRegExp ipRegex;
    QRegExpValidator *ipValidator;
};

#endif // NETWORKCALCULATORDIALOG_H
