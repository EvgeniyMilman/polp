#include "data.h"

Data::Data(QObject *parent) :QObject(parent){
}

Data::~Data(){
}

QVariant Data::parameter(QString key){
    return parameters.value(key);
}

void Data::setParameter(QString key, QVariant value){
    parameters.insert(key,value);
}

QList<QString> Data::parameterList(){
    return parameters.keys();
}



Data2D::Data2D(QObject *parent) :Data(parent){
}

Data2D::~Data2D(){
;
    xdata.clear();
    ydata.clear();
}

void Data2D::addPoint(QString curve, double x, double y){
    if(!xdata.contains(curve)){
        QVector<double>* tmp = new QVector<double>();
        xdata.insert(curve,tmp);
        tmp = new QVector<double>();
        ydata.insert(curve,tmp);
    }
    QVector<double>* tmp = xdata.value(curve);
    tmp->append(x);
    tmp = ydata.value(curve);
    tmp->append(y);
}

QVector<double>* Data2D::x(QString curve){
    return (xdata.contains(curve))?xdata.value(curve):NULL;
}

QVector<double> *Data2D::y(QString curve){
    return (ydata.contains(curve))?ydata.value(curve):NULL;
}

QList<QString> Data2D::curvers(){
    return xdata.keys();
}

void Data2D::startEdit(){
    QList<QVector<double>*> values = xdata.values();
    for (int i=0 ; i < values.size();i++){
        delete values.at(i);
    }
    values = ydata.values();
    for (int i=0 ; i < values.size();i++){
        delete values.at(i);
    }
    xdata.clear();
    ydata.clear();
}

void Data2D::stopEdit(){
    emit dataChanged();
}

void Data2D::addCurve(QString curve, QVector<double> *x, QVector<double> *y){
    xdata.insert(curve, new QVector<double>(*x));
    ydata.insert(curve, new QVector<double>(*y));
}

void Data2D::removeCurve(QString curve){
  QVector<double> * cur =   xdata.value(curve);
  xdata.remove(curve);
  if(cur !=0){
      delete cur;
  }
  cur = ydata.value(curve);
  ydata.remove(curve);
  if(cur !=0){
      delete cur;
  }
}


DataBin::DataBin(QObject *parent):Data(parent){
    _size =0;
    _data= NULL;
}

ulong DataBin::size(){
    return _size;
}

char *DataBin::data(){
    return _data;
}

void DataBin::setData(ulong size, char *data){
    if(data !=_data){
        delete[] _data;
    }
    _size = size;
    _data = data;
}

void DataBin::startEdit(){
//TODO:: ????
}

void DataBin::stopEdit(){
    emit dataChanged();
}
