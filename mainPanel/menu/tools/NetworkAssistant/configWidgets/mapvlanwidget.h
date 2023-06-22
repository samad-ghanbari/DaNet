#ifndef MAPVLANWIDGET_H
#define MAPVLANWIDGET_H

#include <QWidget>
class QPlainTextEdit;
class QPushButton;

namespace Ui {
class MapVlanWidget;
}

class MapVlanWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MapVlanWidget(QWidget *parent, const int OS,QPushButton *Btn, QPlainTextEdit *TE);
    ~MapVlanWidget();

signals:
    void setWriteBtnEnableSignal(bool en);

private slots:
    void on_mapSrcVlanLE_textEdited(const QString &arg1);

    void on_mapVlanDstLE_textChanged(const QString &arg1);

    void write();

private:
    Ui::MapVlanWidget *ui;
    QPushButton *btn;
    QPlainTextEdit *te;
    const int os;

};

#endif // MAPVLANWIDGET_H
