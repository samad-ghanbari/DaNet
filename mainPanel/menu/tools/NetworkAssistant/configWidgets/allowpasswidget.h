#ifndef ALLOWPASSWIDGET_H
#define ALLOWPASSWIDGET_H

#include <QWidget>
class QPlainTextEdit;
class QPushButton;

namespace Ui {
class AllowPassWidget;
}

class AllowPassWidget : public QWidget
{
    Q_OBJECT

public:
    explicit AllowPassWidget(QWidget *parent, const int OS,QPushButton *Btn, QPlainTextEdit *TE);
    ~AllowPassWidget();

signals:
    void setWriteBtnEnableSignal(bool en);

private slots:
    void on_allowPassLE_textChanged(const QString &arg1);
    void write();

private:
    Ui::AllowPassWidget *ui;
    QPushButton *btn;
    QPlainTextEdit *te;
    const int os;
};

#endif // ALLOWPASSWIDGET_H
