#include "nmranalyser.h"
#include "ui_nmrcontrolform.h"

NMRAnalyser::NMRAnalyser(QWidget *parent) : QWidget(parent), ui(new Ui::NMRControlForm),data(new Data2D){
    ui->setupUi(this);
}

NMRAnalyser::~NMRAnalyser(){
    delete data;
    delete ui;
}

QString NMRAnalyser::title(){
    return "NMR Analyser";
}

QWidget *NMRAnalyser::controlPane(){
    return this;
}

Data *NMRAnalyser::analyse(Data *data){
    currentdata = (Data2D*) data;

    return this->data;
}

Data *NMRAnalyser::analyse(QList<Data *> dataset)
{
    return this->data;
}


