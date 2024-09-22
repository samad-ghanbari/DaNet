#ifndef ME60X16WIDGET_H
#define ME60X16WIDGET_H

#include <QWidget>
class QSqlQueryModel;

namespace Ui {
class ME60X16Widget;
}

class ME60X16Widget : public QWidget
{
    Q_OBJECT

public:
    explicit ME60X16Widget(QWidget *parent, QSqlQueryModel *model);
    ~ME60X16Widget();
    void hideSlots();
    QString stringToVerticalString(QString string);
    void setText(const int slot, int subSlot, QString card);

private:
    Ui::ME60X16Widget *ui;
};

#endif // ME60X16WIDGET_H
