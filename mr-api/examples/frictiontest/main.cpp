#include "frictiontest.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    FrictionTest w;
    w.show();

    return a.exec();
}
