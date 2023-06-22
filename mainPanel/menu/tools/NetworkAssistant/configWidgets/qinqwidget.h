#ifndef QINQWIDGET_H
#define QINQWIDGET_H

#include <QWidget>
class QPlainTextEdit;
class QPushButton;

namespace Ui {
class QinQWidget;
}

class QinQWidget : public QWidget
{
    Q_OBJECT

public:
    explicit QinQWidget(QWidget *parent, const int OS,QPushButton *Btn, QPlainTextEdit *TE);
    ~QinQWidget();

signals:
    void setWriteBtnEnableSignal(bool en);

private slots:
    void on_userVlanLE_textChanged(const QString &arg1);

    void on_qinqLE_textChanged(const QString &arg1);

    void write();

private:
    Ui::QinQWidget *ui;
    QPushButton *btn;
    QPlainTextEdit *te;
    const int os;
};

#endif // QINQWIDGET_H
