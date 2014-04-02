#include "nmranalyser.h"
#include "ui_nmrcontrolform.h"
#include <math.h>

NMRAnalyser::NMRAnalyser(QWidget *parent) : QWidget(parent), ui(new Ui::NMRControlForm),data(new Data2D){
    ui->setupUi(this);
    real = new QVector<double>;
    imag = new QVector<double>;
    realFT = new QVector<double>;
    imagFT = new QVector<double>;
    timedomain = new QVector<double>;
    freqdomain = new QVector<double>;
    abs = new QVector<double>;
    absFT = new QVector<double>;
}

NMRAnalyser::~NMRAnalyser(){
    delete data;
    delete ui;
    delete real;
    delete imag;
    delete realFT;
    delete imagFT;
    delete timedomain;
    delete freqdomain;
}

QString NMRAnalyser::title(){
    return "NMR Analyser";
}

QWidget *NMRAnalyser::controlPane(){
    return this;
}

Data *NMRAnalyser::analyse(Data *_data){
    currentdata = (Data2D*) _data;
    _analyse();
    QVariant tmp = "1/2";
    data->setParameter("view_split",tmp);
    tmp = "Voltage (V)";
    data->setParameter("view_0_y",tmp);
    tmp = "Time (us)";
    data->setParameter("view_0_x",tmp);
    tmp = "a.u.";
    data->setParameter("view_1_y",tmp);
    tmp = "Frequency (kHz)";
    data->setParameter("view_1_x",tmp);
    tmp = 0;
    data->setParameter("view.real.plot",tmp);
    data->setParameter("view.imag.plot",tmp);
    data->setParameter("view.abs.plot",tmp);

    tmp=1;
    data->setParameter("view.realFT.plot",tmp);
    data->setParameter("view.imagFT.plot",tmp);
    data->setParameter("view.absFT.plot",tmp);

    tmp = qVariantFromValue(QColor(Qt::red));
    data->setParameter("view.realFT.color",tmp);
    data->setParameter("view.real.color",tmp);

    tmp = qVariantFromValue(QColor(Qt::green));
    data->setParameter("view.imagFT.color",tmp);
    data->setParameter("view.imag.color",tmp);

    tmp = qVariantFromValue(QColor(Qt::blue));
    data->setParameter("view.absFT.color",tmp);
    data->setParameter("view.abs.color",tmp);

    tmp = "Real";
    data->setParameter("view.realFT.title",tmp);
    data->setParameter("view.real.title",tmp);

    tmp = "Imaginary";
    data->setParameter("view.imagFT.title",tmp);
    data->setParameter("view.imag.title",tmp);

    tmp = "Absolute";
    data->setParameter("view.absFT.title",tmp);
    data->setParameter("view.abs.title",tmp);
    return this->data;
}

Data *NMRAnalyser::analyse(QList<Data *> dataset){
    return this->data;
}

void NMRAnalyser::_calcDCoffset(QVector<double>* y1,QVector<double>* y2, double& dc1, double& dc2 ){
    double dcoffset = ui->offsetSpinBox->value();
    double datapoints = y1->size();
    dc1 = 0.;
    dc2 = 0.;
    for(int i =datapoints*(1. - dcoffset/100.);i<datapoints;i++){
        dc1 += (*y1)[i];
        dc2 += (*y2)[i];
    }
    if(((int)datapoints*dcoffset/100.)!= 0){
        dc1 /= datapoints * dcoffset/100.;
        dc2 /= datapoints * dcoffset/100.;
    }
}

void NMRAnalyser::_loadData(QVector<double>* realX,QVector<double>* realY,QVector<double>*imagX,QVector<double>*imagY,double dcreal, double dcimag){
    double datapoints = realX->size();
    double datadw = realX->at(1)- realX->at(0);
    ui->dwtimeSpinBox->setMinimum(datadw);
    ui->dwtimeSpinBox->setSingleStep(datadw);
    int step = data->parameter("dw").toDouble() / datadw;
    double skip = ui->skipSpinBox->value();
    double timeconst = ui->windowTimeConstantSpinBox->value();
    int wfindex = ui->windowFunctionBox->currentIndex();
    double timeoffset = (*realX)[datapoints * skip/100.];
    for(int i = datapoints * skip/100.;i < datapoints; i=i+step){
        double time = (*realX)[i]-timeoffset;
        double windowVal = windowFunction(wfindex,time,timeconst);
        double realVal = ((*realY)[i]-dcreal)*windowVal;
        double imagVal = ((*imagY)[i]-dcimag)*windowVal;
        double absVal = sqrt(realVal*realVal + imagVal* imagVal);
        real->append(realVal);
        imag->append(imagVal);
        abs->append(absVal);
        timedomain->append(time);
    }
    if(ui->zerofillcheckBox->isChecked()){
        int size = real->size();
        double time = timedomain->at(size-1);
        double timestep = timedomain->at(1)-timedomain->at(0);
        for(int i = 0; i<size*4;i++){
            real->append(0.);
            imag->append(0.);
            abs->append(0.);
            timedomain->append(time + timestep*i);
        }
    }
}

void NMRAnalyser::_analyse(){
    this->data->startEdit();
    real->clear();
    imag->clear();
    realFT->clear();
    imagFT->clear();
    timedomain->clear();
    freqdomain->clear();
    abs->clear();
    absFT->clear();
    if(currentdata != 0 && currentdata->curvers().size()==2){
        QString real_title = currentdata->curvers()[0];
        QString imag_title = currentdata->curvers()[1];
        QVector<double>* realX = currentdata->x(real_title);
        QVector<double>* realY = currentdata->y(real_title);
        QVector<double>* imagX = currentdata->x(imag_title);
        QVector<double>* imagY = currentdata->y(imag_title);
        if(realX->size()>2 && realX->size()==imagX->size()){
            //calc DC offset
            double dcreal = 0.;
            double dcimag = 0.;
            _calcDCoffset(realY,imagY,dcreal,dcimag);
            _loadData(realX,realY,imagX,imagY,dcreal,dcimag);
            _calcFT();
            if(ui->abscheckBox->isChecked()){
                data->addCurve("abs",timedomain,abs);
            }
            if(ui->realcheckBox->isChecked()){
                data->addCurve("real",timedomain,real);
            }
            if(ui->imagcheckBox->isChecked()){
                data->addCurve("imag",timedomain,imag);
            }
            if(ui->absFTcheckBox->isChecked()){
                data->addCurve("absFT",freqdomain,absFT);
            }
            if(ui->realFTcheckBox->isChecked()){
                data->addCurve("realFT",freqdomain,realFT);
            }
            if(ui->imagFTcheckBox->isChecked()){
                data->addCurve("imagFT",freqdomain,imagFT);
            }
        }else {
            //TODO::error
             qDebug("TODO:: error:: Wrond data format");
        }
    }
    this->data->stopEdit();
}

double NMRAnalyser::windowFunction(int index, double time, double timeconst){
    switch (index){
    case 0:
        return 1.;
    case 1:
        return exp(-time/timeconst);
    }
}

void NMRAnalyser::_calcFT()
{

}


void NMRAnalyser::on_skipSpinBox_valueChanged(double val){
    _analyse();
}

void NMRAnalyser::on_dwtimeSpinBox_valueChanged(double val){
    data->setParameter("dw",qVariantFromValue(val));
    _analyse();
}

void NMRAnalyser::on_offsetSpinBox_valueChanged(double val){
    _analyse();
}

void NMRAnalyser::on_zerofillcheckBox_clicked(){
    _analyse();
}

void NMRAnalyser::on_autophaseCheckBox_clicked(){

}

void NMRAnalyser::on_phaseSpinBox_valueChanged(double arg1){

}

void NMRAnalyser::on_windowFunctionBox_currentIndexChanged(int index){
    _analyse();
}

void NMRAnalyser::on_windowTimeConstantSpinBox_valueChanged(double val){
    _analyse();
}

void NMRAnalyser::_displayCheckboxClicked(QCheckBox * checkbox,QString curve, QVector<double> *x,QVector<double>* y ){
    if(checkbox->isChecked()){
        data->addCurve(curve,x,y);
    }else {
        data->removeCurve(curve);
    }
    emit data->dataChanged();
}

void NMRAnalyser::on_realcheckBox_clicked(){
    _displayCheckboxClicked(ui->realcheckBox,"real",timedomain,real);
}

void NMRAnalyser::on_imagcheckBox_clicked(){
    _displayCheckboxClicked(ui->imagcheckBox,"imag",timedomain,imag);
}

void NMRAnalyser::on_abscheckBox_clicked(){
    _displayCheckboxClicked(ui->abscheckBox,"abs",timedomain,abs);
}

void NMRAnalyser::on_showWindowBox_clicked(){
    //_analyse();
}

void NMRAnalyser::on_realFTcheckBox_clicked(){
    _displayCheckboxClicked(ui->realFTcheckBox,"realFT",freqdomain,realFT);
}

void NMRAnalyser::on_imagFTcheckBox_clicked(){
    _displayCheckboxClicked(ui->imagFTcheckBox,"imagFT",freqdomain,imagFT);
}

void NMRAnalyser::on_absFTcheckBox_clicked(){
    _displayCheckboxClicked(ui->absFTcheckBox,"absFT",freqdomain,absFT);
}
