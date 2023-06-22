#ifndef BINDVPNWIDGET_H
#define BINDVPNWIDGET_H

#include <QWidget>
class QPlainTextEdit;
class QPushButton;

namespace Ui {
class BindVpnWidget;
}

class BindVpnWidget : public QWidget
{
    Q_OBJECT

public:
    explicit BindVpnWidget(QWidget *parent, const int OS,QPushButton *Btn, QPlainTextEdit *TE);
    ~BindVpnWidget();

signals:
    void setWriteBtnEnableSignal(bool en);

private slots:
    void on_intBindVpnLE_textChanged(const QString &arg1);

    void write();

private:
    Ui::BindVpnWidget *ui;
    QPushButton *btn;
    QPlainTextEdit *te;
    const int os;
};

#endif // BINDVPNWIDGET_H
