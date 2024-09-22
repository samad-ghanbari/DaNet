#ifndef ASR9010WIDGET_H
#define ASR9010WIDGET_H

#include <QWidget>
class QSqlQueryModel;

namespace Ui {
class ASR9010Widget;
}

class ASR9010Widget : public QWidget
{
    Q_OBJECT

public:
    explicit ASR9010Widget(QWidget *parent, QSqlQueryModel *model);
    ~ASR9010Widget();
    void hideSlots();
    QString stringToVerticalString(QString string);
    void setText(const int slot, int subSlot,QString card);

private:
    Ui::ASR9010Widget *ui;
};

#endif // ASR9010WIDGET_H
