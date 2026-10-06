#ifndef CONVERTTOBIDI_H
#define CONVERTTOBIDI_H

class DanetDbMan;

#include <QDialog>

namespace Ui {
class ConvertToBiDi;
}

class ConvertToBiDi : public QDialog
{
    Q_OBJECT

public:
    explicit ConvertToBiDi(QWidget *parent, DanetDbMan *db, const int PinId, const QString exch, const QString saloon, const QString odf, const QString pos, const QString pinNO);
    ~ConvertToBiDi();

private slots:
    void on_cancelBtn_clicked();

    void on_confirmChB_toggled(bool checked);

    void on_okBtn_clicked();

private:
    Ui::ConvertToBiDi *ui;
    DanetDbMan *dbMan;
    const int pinId;
};

#endif // CONVERTTOBIDI_H
