#ifndef SYSNAMEWIDGET_H
#define SYSNAMEWIDGET_H

#include <QWidget>
class QPlainTextEdit;
class QPushButton;

namespace Ui {
class SysnameWidget;
}

class SysnameWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SysnameWidget(QWidget *parent, const int OS,QPushButton *Btn, QPlainTextEdit *TE);
    ~SysnameWidget();

signals:
    void setWriteBtnEnableSignal(bool en);


private slots:
    void on_sysnameLE_textChanged(const QString &arg1);
    void write();

private:
    Ui::SysnameWidget *ui;
    QPushButton *btn;
    QPlainTextEdit *te;
    const int os;
};

#endif // SYSNAMEWIDGET_H
