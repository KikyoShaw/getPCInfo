#include "GetPCInfo.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    GetPCInfo w;
    w.show();
    return a.exec();
}
