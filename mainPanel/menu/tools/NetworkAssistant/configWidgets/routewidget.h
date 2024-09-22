#ifndef ROUTEWIDGET_H
#define ROUTEWIDGET_H

#include <QWidget>
class QPlainTextEdit;
class QRegExpValidator;
class QPushButton;

namespace Ui {
class RouteWidget;
}

class RouteWidget : public QWidget
{
    Q_OBJECT

public:
    explicit RouteWidget(QWidget *parent,const int OS, const int RouteFlag ,QPushButton *Btn,QPlainTextEdit *TE );
    ~RouteWidget();

signals:
    void setWriteBtnEnableSignal(bool en);

private slots:
    void write();
    void on_defaultNextHopLE_textChanged(const QString &arg1);
    void on_static1IpLE_textChanged(const QString &arg1);
    void on_static1MaskLE_textChanged(const QString &arg1);
    void on_static1NextHopLE_textChanged(const QString &arg1);
    void on_static2IpLE_textChanged(const QString &arg1);
    void on_static2MaskLE_textChanged(const QString &arg1);
    void on_static2VpnLE_textChanged(const QString &arg1);
    void on_static2NextHopLE_textChanged(const QString &arg1);

private:
    Ui::RouteWidget *ui;
    QPushButton *btn;
    QPlainTextEdit *te;
    const int os;
    const int d12;//0:D, 1:static1, 2: static2
    const QString ipRange;
    const QRegExp ipRegex;
    QRegExpValidator *ipValidator;
};

#endif // ROUTEWIDGET_H
