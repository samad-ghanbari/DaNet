#ifndef ME60X8WIDGET_H
#define ME60X8WIDGET_H

#include <QWidget>
class QSqlQueryModel;

namespace Ui {
class ME60X8Widget;
}

class ME60X8Widget : public QWidget
{
    Q_OBJECT

public:
    explicit ME60X8Widget(QWidget *parent, QSqlQueryModel *model);
    ~ME60X8Widget();
    void hideSlots();
    QString stringToVerticalString(QString string);
    void setText(const int slot, int subSlot, QString card);

private:
    Ui::ME60X8Widget *ui;
};

#endif // ME60X8WIDGET_H
