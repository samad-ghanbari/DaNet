#ifndef DOT1QTAGWIDGET_H
#define DOT1QTAGWIDGET_H

#include <QWidget>
class QPlainTextEdit;
class QPushButton;

namespace Ui {
class Dot1QTagWidget;
}

class Dot1QTagWidget : public QWidget
{
    Q_OBJECT

public:
    explicit Dot1QTagWidget(QWidget *parent, const int OS,QPushButton *Btn, QPlainTextEdit *TE);
    ~Dot1QTagWidget();

signals:
    void setWriteBtnEnableSignal(bool en);

private slots:
    void on_dot1qLE_textChanged(const QString &arg1);

    void write();

private:
    Ui::Dot1QTagWidget *ui;
    QPushButton *btn;
    QPlainTextEdit *te;
    const int os;
};

#endif // DOT1QTAGWIDGET_H
