#include "isefileformat.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ISEFileFormat w;
    w.show();

    return a.exec();
}
