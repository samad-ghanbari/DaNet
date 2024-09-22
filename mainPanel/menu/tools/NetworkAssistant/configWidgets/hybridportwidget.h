#ifndef HYBRIDPORTWIDGET_H
#define HYBRIDPORTWIDGET_H

#include <QWidget>
class QPlainTextEdit;
class QPushButton;

namespace Ui {
class HybridPortWidget;
}

class HybridPortWidget : public QWidget
{
    Q_OBJECT

public:
    explicit HybridPortWidget(QWidget *parent, const int OS,QPushButton *Btn, QPlainTextEdit *TE);
    ~HybridPortWidget();

signals:
    void setWriteBtnEnableSignal(bool en);

private slots:
    void on_taggedLE_textChanged(const QString &arg1);

    void on_untaggedLE_textChanged(const QString &arg1);

    void write();

private:
    Ui::HybridPortWidget *ui;
    QPushButton *btn;
    QPlainTextEdit *te;
    const int os;
};

#endif // HYBRIDPORTWIDGET_H
