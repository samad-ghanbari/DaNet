#ifndef CONVERTTODUPLEX_H
#define CONVERTTODUPLEX_H

#include <QDialog>

class DanetDbMan;

namespace Ui {
class ConvertToDuplex;
}

class ConvertToDuplex : public QDialog
{
    Q_OBJECT

public:
    explicit ConvertToDuplex(QWidget *parent, DanetDbMan *db, const int PinId, const QString exch, const QString saloon, const QString odf, const QString pos, const QString pinNo);
    ~ConvertToDuplex();

private slots:
    void on_confirmChB_toggled(bool checked);

    void on_okBtn_clicked();

private:
    Ui::ConvertToDuplex *ui;
    DanetDbMan *dbMan;
    const int pinId;
};

#endif // CONVERTTODUPLEX_H
