#ifndef PORTSWITCHWIDGET_H
#define PORTSWITCHWIDGET_H

#include <QWidget>
class QPlainTextEdit;
class QPushButton;

namespace Ui {
class PortSwitchWidget;
}

class PortSwitchWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PortSwitchWidget(QWidget *parent, const int OS, QPushButton *Btn, QPlainTextEdit *TE);
    ~PortSwitchWidget();

private slots:
    void write();

private:
    Ui::PortSwitchWidget *ui;
    QPushButton *btn;
    QPlainTextEdit *te;
    const int os;
};

#endif // PORTSWITCHWIDGET_H
