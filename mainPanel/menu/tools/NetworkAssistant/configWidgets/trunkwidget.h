#ifndef TRUNKWIDGET_H
#define TRUNKWIDGET_H

#include <QWidget>
class QPlainTextEdit;
class QPushButton;

namespace Ui {
class TrunkWidget;
}

class TrunkWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TrunkWidget(QWidget *parent, const int OS,QPushButton *Btn, QPlainTextEdit *TE);
    ~TrunkWidget();

signals:
    void setWriteBtnEnableSignal(bool en);

private slots:
    void on_intEtherLE_textChanged(const QString &arg1);
    void write();
    void on_trunkProtocolCB_currentTextChanged(const QString &arg1);
    void on_trunkProModeCB_currentTextChanged(const QString &arg1);

private:
    Ui::TrunkWidget *ui;
    QPushButton *btn;
    QPlainTextEdit *te;
    const int os;
};

#endif // TRUNKWIDGET_H
