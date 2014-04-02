#include "simpleview.h"
#include <QtGlobal>
#include "ui_simpleviewform.h"

SimpleView::SimpleView(QWidget *parent) :QWidget(parent),ui(new Ui::SimpleViewForm){
    ui->setupUi(this);
}

SimpleView::~SimpleView(){
}

QString SimpleView::title(){
    return "Simple view";
}

QWidget *SimpleView::viewPane(){
    return this;
}
int randInt(int low, int high){
    // Random number between low and high
    return qrand() % ((high + 1) - low) + low;
}

int SimpleView::setData(Data *data){
    if(qobject_cast<Data2D*>(data)){
        qsrand(2);
        ui->stackedWidget->setCurrentWidget(ui->plot);
        Data2D* data2d  = qobject_cast<Data2D*>(data);
        ui->plot->clearGraphs();
        QList<QString> curves = data2d->curvers();
        Q_FOREACH(QString curve , curves){
            QCPGraph* graph = ui->plot->addGraph();
            graph->setData(*(data2d->x(curve)),*(data2d->y(curve)));
            QPen pen;
            pen.setColor(QColor(randInt(0,255), randInt(0,255), randInt(0,255), 255));
            graph->setPen(pen);
        }
        ui->plot->rescaleAxes();
        ui->plot->replot();
    }else if(qobject_cast<DataBin*>(data)){
        DataBin* databin = qobject_cast<DataBin*>(data);
        //TODO:: Implement SimpleView for dataBin
    }else{
        error_message = "Unknown data format";
        return 1;
    }
    return 0;
}

QString SimpleView::error(){
    return error_message;
}
