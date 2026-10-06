#ifndef CONVERTTODUPLEX_H
#define CONVERTTODUPLEX_H

#include <QDialog>

namespace Ui {
class ConvertToDuplex;
}

class ConvertToDuplex : public QDialog
{
    Q_OBJECT

public:
    explicit ConvertToDuplex(QWidget *parent = nullptr);
    ~ConvertToDuplex();

private:
    Ui::ConvertToDuplex *ui;
};

#endif // CONVERTTODUPLEX_H
