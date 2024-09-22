#ifndef DEFAULTVLANWIDGET_H
#define DEFAULTVLANWIDGET_H

#include <QWidget>
class QPlainTextEdit;
class QPushButton;

namespace Ui {
class DefaultVlanWidget;
}

class DefaultVlanWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DefaultVlanWidget(QWidget *parent, const int OS,QPushButton *Btn, QPlainTextEdit *TE);
    ~DefaultVlanWidget();

signals:
    void setWriteBtnEnableSignal(bool en);

private slots:
    void on_defaultVlanLE_textChanged(const QString &arg1);
    void write();

private:
    Ui::DefaultVlanWidget *ui;
    QPushButton *btn;
    QPlainTextEdit *te;
    const int os;
};

#endif // DEFAULTVLANWIDGET_H
