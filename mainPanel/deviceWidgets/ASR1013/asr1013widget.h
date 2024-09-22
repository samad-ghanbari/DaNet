#ifndef ASR1013WIDGET_H
#define ASR1013WIDGET_H

#include <QWidget>
class QSqlQueryModel;

namespace Ui {
class ASR1013Widget;
}

class ASR1013Widget : public QWidget
{
    Q_OBJECT

public:
    explicit ASR1013Widget(QWidget *parent, QSqlQueryModel *model);
    ~ASR1013Widget();
    void hideSlots();
    void setText(const int slot, int subSlot, QString card);

private:
    Ui::ASR1013Widget *ui;
};

#endif // ASR1013WIDGET_H
