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


/*
 * Properties:      Value
 * view_split       String  1/1  (col, row) Default !
 * view_plot_legend Bool    true Default            !
 * view_plot_title  String                          // TODO:: no support
 * view_plot_x      String                          !
 * view_plot_y      String                          !
 * view_plot_type   lin,log,Smith    lin is Default  //TODO ::no support for  Smith
 *
 * view.curve.plot  int     1               Default
 * view.curve.color QColor
 * view.curve.title String
 */


int SimpleView::setData(Data *data){
    if(qobject_cast<Data2D*>(data)){
        Data2D* data2d  = qobject_cast<Data2D*>(data);
        ui->stackedWidget->setCurrentWidget(ui->plot);
        // Prepare Qcustom plot for ploting
        QVector<QCPAxisRect *> rects = _preparePlotWidget(data2d);
        Q_FOREACH(QString curve , data2d->curvers()){
            QVariant tmp;
            QString par = "view."+curve+".plot";
            tmp = data->parameter(par);
            int plot = (!tmp.isNull() && tmp.type()==QVariant::Int)?tmp.toInt():0;
            QCPGraph* graph = ui->plot->addGraph(rects[plot]->axis(QCPAxis::atBottom),rects[plot]->axis(QCPAxis::atLeft));
            graph->setData(*(data2d->x(curve)),*(data2d->y(curve)));

            par = "view."+curve+".color";
            tmp = data->parameter(par);
            QColor col = (!tmp.isNull() && tmp.type()==QVariant::Color)?tmp.value<QColor>():QColor();
            QPen pen;
            pen.setColor(col);
            graph->setPen(pen);

            par = "view."+curve+".title";
            tmp = data->parameter(par);
            QString title = (!tmp.isNull() && tmp.type()==QVariant::String)?tmp.toString():curve;
            graph->setName(title);

            if(legends[plot]!=0){
                legends[plot]->addItem(new QCPPlottableLegendItem(legends[plot], graph));
            }
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

QVector<QCPAxisRect *> SimpleView::_preparePlotWidget(Data2D *data){
    ui->plot->clearGraphs();
    ui->plot->plotLayout()->clear();
    ui->plot->setInteractions(QCP::iRangeDrag |QCP::iRangeZoom);
    legends.clear();

    QVariant tmp;
    QString par = "view_split";
    tmp = data->parameter(par);
    QString plots = (!tmp.isNull() && tmp.type()==QVariant::String)?tmp.toString():"1/1";
    int col,row;
    col = plots.split("/")[0].toInt();
    row = plots.split("/")[1].toInt();
    QVector<QCPAxisRect *> rects;

    for(int i=0; i< col;i++){
        for(int j=0; j< row;j++){
            QCPAxisRect *rect = new QCPAxisRect(ui->plot);
            rect->setRangeDrag(Qt::Horizontal | Qt::Vertical);
            rect->setRangeZoom(Qt::Horizontal | Qt::Vertical);
            ui->plot->plotLayout()->addElement(j,i,rect);
            rects.append(rect);
            Q_FOREACH(QCPAxis *axis,rect->axes()){
                axis->setLayer("axes");
                axis->grid()->setLayer("grid");
            }
        }
    }
    for(int i= 0; i< col*row; i++){
        par = "view_"+QString::number(i)+"_x";
        tmp = data->parameter(par);
        QString x = (!tmp.isNull() && tmp.type()==QVariant::String)?tmp.toString():"X";
        rects[i]->axis(QCPAxis::atBottom)->setLabel(x);

        par = "view_"+QString::number(i)+"_y";
        tmp = data->parameter(par);
        QString y = (!tmp.isNull() && tmp.type()==QVariant::String)?tmp.toString():"Y";
        rects[i]->axis(QCPAxis::atLeft)->setLabel(y);

        par = "view_"+QString::number(i)+"_legend";
        tmp = data->parameter(par);
        bool leg = (!tmp.isNull() && tmp.type()==QVariant::Bool)?tmp.toBool():true;
        if(leg){
            QCPLegend *legend = new QCPLegend;
            rects[i]->insetLayout()->addElement(legend, Qt::AlignTop|Qt::AlignRight);
            legend->setLayer("legend");
            legends.append(legend);
        }else {
            legends.append(0);
        }
        par = "view_"+QString::number(i)+"_type";
        tmp = data->parameter(par);
        QString type = (!tmp.isNull() && tmp.type()==QVariant::String)?tmp.toString():"lin";
        if(type =="lin"){
            rects[i]->axis(QCPAxis::atLeft)->setScaleType(QCPAxis::stLinear);
        } else if(type=="log"){
            rects[i]->axis(QCPAxis::atLeft)->setScaleLogBase(10);
            rects[i]->axis(QCPAxis::atLeft)->setScaleType(QCPAxis::stLogarithmic);
        }
    }
    return rects;
}
