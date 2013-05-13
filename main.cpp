#include "MacDock.h"
#include <QtGui/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MacDock w;
    w.show();
    return a.exec();
}
