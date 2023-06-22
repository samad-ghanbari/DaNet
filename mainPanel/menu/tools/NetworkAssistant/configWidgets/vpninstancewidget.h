#ifndef VPNINSTANCEWIDGET_H
#define VPNINSTANCEWIDGET_H

#include <QWidget>
class QPlainTextEdit;
class QPushButton;

namespace Ui {
class VpnInstanceWidget;
}

class VpnInstanceWidget : public QWidget
{
    Q_OBJECT

public:
    explicit VpnInstanceWidget(QWidget *parent, const int OS,QPushButton *Btn, QPlainTextEdit *TE);
    ~VpnInstanceWidget();

signals:
    void setWriteBtnEnableSignal(bool en);

private slots:
    void on_vpnLE_textChanged(const QString &arg1);

    void on_vpnDescLE_textChanged(const QString &arg1);

    void on_rdLE_textChanged(const QString &arg1);

    void on_eRtLE_textChanged(const QString &arg1);

    void on_iRtLE_textEdited(const QString &arg1);

    void write();

private:
    Ui::VpnInstanceWidget *ui;
    QPushButton *btn;
    QPlainTextEdit *te;
    const int os;
};

#endif // VPNINSTANCEWIDGET_H
