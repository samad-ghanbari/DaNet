#ifndef LOADBALACEWIDGET_H
#define LOADBALACEWIDGET_H

#include <QWidget>
class QPlainTextEdit;
class QPushButton;

namespace Ui {
class LoadBalaceWidget;
}

class LoadBalaceWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LoadBalaceWidget(QWidget *parent , const int OS,QPushButton *Btn, QPlainTextEdit *TE);
    ~LoadBalaceWidget();

signals:
    void setWriteBtnEnableSignal(bool en);

private slots:
    void on_loadBCB_currentTextChanged(const QString &arg1);
    void write();

private:
    Ui::LoadBalaceWidget *ui;
    QPushButton *btn;
    QPlainTextEdit *te;
    const int os;
};

#endif // LOADBALACEWIDGET_H
