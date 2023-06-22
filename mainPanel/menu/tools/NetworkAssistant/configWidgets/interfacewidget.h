#ifndef INTERFACEWIDGET_H
#define INTERFACEWIDGET_H

#include <QWidget>
class QPlainTextEdit;
class QPushButton;

namespace Ui {
class InterfaceWidget;
}

class InterfaceWidget : public QWidget
{
    Q_OBJECT

public:
    explicit InterfaceWidget(QWidget *parent,const int OS,QPushButton *Btn, QPlainTextEdit *TE);
    ~InterfaceWidget();

signals:
    void setWriteBtnEnableSignal(bool en);

private slots:
    void on_interfaceLE_textChanged(const QString &arg1);

    void on_interfaceDescLE_textChanged(const QString &arg1);

    void write();

    void on_interfaceTypeCB_currentTextChanged(const QString &arg1);

private:
    Ui::InterfaceWidget *ui;
    QPushButton *btn;
    QPlainTextEdit *te;
    const int os;
};

#endif // INTERFACEWIDGET_H
