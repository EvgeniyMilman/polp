#include "csvfileformat.h"



#include <QTextStream>
#include <qfile.h>

#include <qdebug.h>

CSVFileFormat::CSVFileFormat(QObject *parent)
    : QObject(parent)
{
}

QString CSVFileFormat::extension(){
    return "*.csv";
}

QString CSVFileFormat::description(){
    return "CSV data file";
}

int CSVFileFormat::loadData(Data *data){

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

     line = stream.readLine();
     QStringList titles = line.split(",");
      for(int i=0;i<titles.length()/2;i++){
          titles[i*2].remove(titles[i*2].length()-2,2);
      }
     line = stream.readLine();
            while(!line.isNull()) {
             QStringList datas = line.split(",");
             for(int i=0;i<titles.length()/2;i++){
                double x = datas[i*2].toDouble();
                double y = datas[i*2+1].toDouble();
                data2d->addPoint(titles[i*2],x, y);
             }
             line = stream.readLine();
            }
            file.close();
     data2d->stopEdit();

    return 0;
}

int CSVFileFormat::saveData(Data *data){
    Data2D *data2d =  qobject_cast<Data2D*>(data);
    if (data2d == NULL) {
        this->error_message = "Error! Invalid data object provided.";
        return -1;
    }

    QString filename = data2d->parameter("filename").toString();

    QFile file(filename);
    if(!file.open(QIODevice::WriteOnly)){
        error_message = "Can not open file"+ filename;
        return -1;
    }
    QTextStream stream(&file);

    QString line;
    QList<QString> curves = data2d->curvers();
    for(int i =0;i<curves.size();i++){
        line.append(curves[i]+".X,");
        line.append(curves[i]+".Y");
        if(i!=curves.size()-1){
            line.append(",");
        }
    }
     stream << line << "\n";
     if(curves.size()>=1){
        int size = data2d->x(curves[0])->size();
        for(int j=0;j<size;j++){
             for(int i =0;i<curves.size();i++){
                 stream <<QString::number( data2d->x(curves[i])->at(j))<<","<<QString::number(data2d->y(curves[i])->at(j));
                 if(i!=curves.size()-1){
                     stream << ",";
                 }else{
                     stream << "\n";
                 }
             }
        }
     }
     file.close();
     data2d->stopEdit();

    return 0;
}

QString CSVFileFormat::error(){
    return error_message;
}

QString CSVFileFormat::preferedView(){
    return "Simple View";
}

int CSVFileFormat::validateFilename(QString filename)
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

bool CSVFileFormat::hasError()
{
    if (!error_message.isNull()) {
        return true;
    } else {
        return false;
    }
}

