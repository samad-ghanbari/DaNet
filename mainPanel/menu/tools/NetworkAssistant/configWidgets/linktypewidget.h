#ifndef LINKTYPEWIDGET_H
#define LINKTYPEWIDGET_H

#include <QWidget>
class QPlainTextEdit;
class QPushButton;

namespace Ui {
class LinkTypeWidget;
}

class LinkTypeWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LinkTypeWidget(QWidget *parent, const int OS,QPushButton *Btn, QPlainTextEdit *TE);
    ~LinkTypeWidget();

signals:
    void setWriteBtnEnableSignal(bool en);

private slots:
    void write();

    void on_linkTypeCB_currentTextChanged(const QString &arg1);

private:
    Ui::LinkTypeWidget *ui;
    QPushButton *btn;
    QPlainTextEdit *te;
    const int os;
};

#endif // LINKTYPEWIDGET_H
