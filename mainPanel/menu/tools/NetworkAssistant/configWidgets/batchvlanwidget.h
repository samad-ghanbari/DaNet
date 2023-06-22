#ifndef BATCHVLANWIDGET_H
#define BATCHVLANWIDGET_H

#include <QWidget>
class QPlainTextEdit;
class QPushButton;

namespace Ui {
class BatchVlanWidget;
}

class BatchVlanWidget : public QWidget
{
    Q_OBJECT

public:
    explicit BatchVlanWidget(QWidget *parent, const int OS,QPushButton *Btn, QPlainTextEdit *TE);
    ~BatchVlanWidget();

signals:
    void setWriteBtnEnableSignal(bool en);

private slots:
    void on_vlanBatchLE_textChanged(const QString &arg1);
    void write();

private:
    Ui::BatchVlanWidget *ui;
    QPushButton *btn;
    QPlainTextEdit *te;
    const int os;
};

#endif // BATCHVLANWIDGET_H
