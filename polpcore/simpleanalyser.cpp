#include "simpleanalyser.h"

SimpleAnalyser::SimpleAnalyser(QObject *parent) :QObject(parent){
    data = new Data2D(this);
}

QString SimpleAnalyser::title(){
    return "<None>";
}

QWidget *SimpleAnalyser::controlPane(){
    return NULL;
}

Data *SimpleAnalyser::analyse(Data *data){
    return data;
}

Data *SimpleAnalyser::analyse(QList<Data *> dataset){
    if(dataset.isEmpty())
        return data;
    data->startEdit();
    Q_FOREACH(Data* tmp,dataset){
        if(qobject_cast<Data2D*>(tmp)){
            Data2D* data2d = (Data2D*)tmp;
            Q_FOREACH(QString curve,data2d->curvers()){
                QString newcurve = data2d->parameter("title").toString()+curve;
                data->addCurve(newcurve,data2d->x(curve),data2d->y(curve));
            }
        }
    }
    data->stopEdit();
    return data;
}
