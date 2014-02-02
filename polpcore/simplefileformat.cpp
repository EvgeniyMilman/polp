#include "simplefileformat.h"

SimpleFileFormat::SimpleFileFormat(QObject *parent) :
    QObject(parent)
{
}

QString SimpleFileFormat::extension()
{
    return "*.txt *.dat";
}

QString SimpleFileFormat::description()
{
    return "Simple data format";
}

int SimpleFileFormat::loadData(Data *data)
{
    Data2D * d = (Data2D*)data;
    for(int i=-50;i<100;i++){
        d->addPoint("test curve",i,i*i*i);
    }
    return 0;
}

int SimpleFileFormat::saveData(Data *data)
{
    return 0;
}

QString SimpleFileFormat::error()
{
    return "Test error form SimpleFileFormat";
}

QString SimpleFileFormat::preferedView()
{
    return "NONE";
}
