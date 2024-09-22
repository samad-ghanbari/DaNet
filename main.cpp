#include <QApplication>
#include "backbone/backbone.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    BackBone bb;
    bb.initiate();
    return a.exec();
}
