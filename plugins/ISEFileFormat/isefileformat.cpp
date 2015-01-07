#include "isefileformat.h"

#include <QTextStream>
#include <qfile.h>

#include <qdebug.h>

ISEFileFormat::ISEFileFormat(QObject *parent)
    : QObject(parent)
{
}

QString ISEFileFormat::extension(){
    return "*.dat";
}

QString ISEFileFormat::description(){
    return "ISE data file";
}

int ISEFileFormat::loadData(Data *data){

    Data2D *data2d =  qobject_cast<Data2D*>(data);
    if (data2d == NULL) {
        this->error_message = "Error! Invalid data object provided.";
        return -1;
    }

    QString filename = data2d->parameter("filename").toString();
    if (validateFilename(filename) != 0) {
        return -1;
    }

    QFile file(filename);
    if(!file.open(QIODevice::ReadOnly)){
        error_message = "Can not open file"+ filename;
        return -1;
    }
    QTextStream stream(&file);

    QString line;
     data2d->startEdit();
     double time, re, im , dummy;
     line = stream.readLine();
            while(!line.isNull()) {
             qDebug() << line;
             if(!line.isEmpty() && !line.startsWith("#")){
                QStringList list = line.split(QRegExp("\\s+"),QString::SkipEmptyParts);
                time = list[0].toDouble() * 1e6;
                re = list[1].toDouble();
                im = list[2].toDouble();
                data2d->addPoint("real", time, re);
                data2d->addPoint("imag", time, im);
             }
             line = stream.readLine();
            }

            file.close();
     data2d->stopEdit();

    return 0;
}

int ISEFileFormat::saveData(Data *data){
// if !qobject_cast<Data2D*>(data) else error "Wrong data format"
    // create file to open data.parameter("filename")
    // if QVariant not QString error "wronf file name"
    //  take first two curves from data2D(error if no 2curves)

    return 0;
}

QString ISEFileFormat::error(){
    return error_message;
}

QString ISEFileFormat::preferedView(){
    return "NMRView";
}

int ISEFileFormat::validateFilename(QString filename)
{
    if (filename == NULL || filename.isEmpty()) {
        this->error_message = "'filename' parameter cannot be empty!";
        return -1;
    }

    QFile file(filename);
    if (!file.exists()) {
        this->error_message = "Cannot open provided input file: " + filename;
        return -1;
    }

    return 0;
}

bool ISEFileFormat::hasError()
{
    if (!error_message.isNull()) {
        return true;
    } else {
        return false;
    }
}

