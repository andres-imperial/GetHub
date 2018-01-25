#include "gethub.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    GetHub w;
    w.show();

    return a.exec();
}
