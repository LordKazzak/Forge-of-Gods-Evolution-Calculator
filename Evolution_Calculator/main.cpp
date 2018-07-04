#include "evolution.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Evolution w;
    w.show();

    return a.exec();
}
