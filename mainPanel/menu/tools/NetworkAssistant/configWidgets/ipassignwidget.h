#ifndef IPASSIGNWIDGET_H
#define IPASSIGNWIDGET_H

#include <QWidget>
class QRegExpValidator;
class QPlainTextEdit;
class QPushButton;

namespace Ui {
class IpAssignWidget;
}

class IpAssignWidget : public QWidget
{
    Q_OBJECT

public:
    explicit IpAssignWidget(QWidget *parent, const int OS,QPushButton *Btn, QPlainTextEdit *TE);
    ~IpAssignWidget();

signals:
    void setWriteBtnEnableSignal(bool en);

private slots:
    void on_ipLE_textChanged(const QString &arg1);
    void on_maskLE_textChanged(const QString &arg1);
    void write();

private:
    Ui::IpAssignWidget *ui;
    QPushButton *btn;
    QPlainTextEdit *te;
    const int os;
    const QString ipRange;
    const QRegExp ipRegex;
    QRegExpValidator *ipValidator;
};

#endif // IPASSIGNWIDGET_H
