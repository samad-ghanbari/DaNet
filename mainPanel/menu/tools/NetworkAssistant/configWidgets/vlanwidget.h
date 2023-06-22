#ifndef VLANWIDGET_H
#define VLANWIDGET_H

#include <QWidget>
class QPlainTextEdit;
class QPushButton;

namespace Ui {
class VlanWidget;
}

class VlanWidget : public QWidget
{
    Q_OBJECT

public:
    explicit VlanWidget(QWidget *parent, const int OS,QPushButton *Btn, QPlainTextEdit *TE);
    ~VlanWidget();

signals:
    void setWriteBtnEnableSignal(bool en);

private slots:
    void on_vlanLE_textChanged(const QString &arg1);
    void on_vlanDescLE_textChanged(const QString &arg1);
    void write();

private:
    Ui::VlanWidget *ui;
    QPushButton *btn;
    QPlainTextEdit *te;
    const int os;
};

#endif // VLANWIDGET_H
