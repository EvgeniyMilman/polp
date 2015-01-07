#include "nmranalyser.h"
#include "ui_nmrcontrolform.h"
#include <math.h>
#include <fftw3.h>

#define PI 3.141592

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
    analysis = false;
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
    if(analysis == false){
    multiset = false;
    currentdata = (Data2D*) _data;
    this->data->startEdit();
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
    ui->absFTcheckBox->setEnabled(true);
    ui->realFTcheckBox->setEnabled(true);
    ui->imagFTcheckBox->setEnabled(true);
    this->data->stopEdit();
    }
    return this->data;
}

Data *NMRAnalyser::analyse(QList<Data *> dataset){
    if(analysis == false){
    multiset = true;
    this->dataset = dataset;
    data->startEdit();
    int i=0;
    Q_FOREACH(Data* _data ,dataset){
        currentdata = (Data2D*) _data;
        if(currentdata != 0 && currentdata->curvers().size()==2){
            _analyse();
            double realInt , imagInt,absInt;
            double mult = freqdomain->at(1)-freqdomain->at(0);
            _integrate(&realInt,&imagInt,&absInt);
            qDebug("%f  %f %f ",realInt*mult,imagInt*mult,absInt*mult);
            data->addPoint("real",i,realInt*mult);
            data->addPoint("imag",i,imagInt*mult);
            data->addPoint("abs",i,absInt*mult);
            i++;
        }
    }
    QVariant tmp = "1/1";
    data->setParameter("view_split",tmp);
    tmp = "a.u.";
    data->setParameter("view_0_y",tmp);
    tmp = "Data index";
    data->setParameter("view_0_x",tmp);
    if(!ui->abscheckBox->isChecked()){
        data->removeCurve("abs");
    }
    if(!ui->realcheckBox->isChecked()){
        data->removeCurve("real");
    }
    if(!ui->imagcheckBox->isChecked()){
        data->removeCurve("imag");
    }
    ui->absFTcheckBox->setEnabled(false);
    ui->realFTcheckBox->setEnabled(false);
    ui->imagFTcheckBox->setEnabled(false);
    data->stopEdit();
    }
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
    if(step == 0)
        step =1;
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
    analysis = true;
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
            qDebug("_calcDCoffset");
            _loadData(realX,realY,imagX,imagY,dcreal,dcimag);
            qDebug("_loadData");
            _calcFT();
            qDebug("_calcFT");
        }else {
            //TODO::error
            qDebug("TODO:: error:: Wrond data format");
        }
    }
    analysis = false;
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
    fftw_complex *in, *out;
    fftw_plan p;
    int N = real->size();
    in = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);
    out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);
    p = fftw_plan_dft_1d(N, in, out, FFTW_BACKWARD, FFTW_ESTIMATE);
    for(int i=0 ;i<N;i++){
        int mul = (i%2==0)?1:-1;
        in[i][0] = (*real)[i]*mul;
        in[i][1] = (*imag)[i]*mul;
    }
    fftw_execute(p); /* repeat as needed */
    if(ui->autophaseCheckBox->isChecked()){
        //calc phase
        double intgReal=0;
        double intgImag = 0;
        for(int i=0;i<N;i++){
            intgReal += out[i][0];
            intgImag += out[i][1];
        }
        double phase = atan(-intgImag/intgReal)*180./PI;
        if(phase < 0){
            phase +=360.;
        }
        ui->phaseSpinBox->setValue(phase);
    }
    int n  = (N%2!=0)?-(N - 1)/2:-N/2;
    double dwell = timedomain->at(1)-timedomain->at(0);
    double phase = ui->phaseSpinBox->value();
    for(int i=0 ;i<N;i++){
        double sinPhase = sin(phase*PI/180.);
        double cosPhase = cos(phase*PI/180.);
        double realV =out[i][0]*2.0/N;
        double imagV =out[i][1]*2.0/N;
        (*realFT).append(realV * cosPhase - imagV * sinPhase);
        (*imagFT).append(imagV * cosPhase + realV * sinPhase);
        absFT->append(sqrt(realV*realV + imagV*imagV));
        freqdomain->append(1000/dwell/N*n);
        n++;
    }
    fftw_destroy_plan(p);
    fftw_free(in); fftw_free(out);

}

void NMRAnalyser::_integrate(double *realInt, double *imagInt, double *absInt){
    int N = realFT->size();
    *realInt = (realFT->at(0)+realFT->at(N-1))/2;
    *imagInt = (imagFT->at(0)+imagFT->at(N-1))/2;
    *absInt = (absFT->at(0)+absFT->at(N-1))/2;
       for(int i = 1;i<N-1;i++){
           *realInt = (*realInt) + (*realFT)[i];
           *imagInt = (*imagInt) + (*imagFT)[i];
           *absInt =  (*absInt) + (*absFT)[i];
       }
}


void NMRAnalyser::on_skipSpinBox_valueChanged(double val){
    if(multiset){
        analyse(dataset);
    }else{
        analyse(currentdata);
    }
}

void NMRAnalyser::on_dwtimeSpinBox_valueChanged(double val){
    data->setParameter("dw",qVariantFromValue(val));
    if(multiset){
        analyse(dataset);
    }else{
        analyse(currentdata);
    }
}

void NMRAnalyser::on_offsetSpinBox_valueChanged(double val){
    if(multiset){
        analyse(dataset);
    }else{
        analyse(currentdata);
    }
}

void NMRAnalyser::on_zerofillcheckBox_clicked(){
    if(multiset){
        analyse(dataset);
    }else{
        analyse(currentdata);
    }
}

void NMRAnalyser::on_autophaseCheckBox_clicked(){
    if(multiset){
        analyse(dataset);
    }else{
        analyse(currentdata);
    }
}

void NMRAnalyser::on_phaseSpinBox_valueChanged(double arg1){
    if(!ui->autophaseCheckBox->isChecked()){
        if(multiset){
            analyse(dataset);
        }else{
            analyse(currentdata);
        }
    }
}

void NMRAnalyser::on_windowFunctionBox_currentIndexChanged(int index){
    if(multiset){
        analyse(dataset);
    }else{
        analyse(currentdata);
    }
}

void NMRAnalyser::on_windowTimeConstantSpinBox_valueChanged(double val){
    if(multiset){
        analyse(dataset);
    }else{
        analyse(currentdata);
    }
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
    if(multiset){
        if(ui->realcheckBox->isChecked()){
            analyse(dataset);
        }else{
            data->removeCurve("real");
            emit data->dataChanged();
        }
    }else{
         _displayCheckboxClicked(ui->realcheckBox,"real",timedomain,real);
    }
}

void NMRAnalyser::on_imagcheckBox_clicked(){
    if(multiset){
        if(ui->imagcheckBox->isChecked()){
            analyse(dataset);
        }else{
            data->removeCurve("imag");
            emit data->dataChanged();
        }
    }else{
        _displayCheckboxClicked(ui->imagcheckBox,"imag",timedomain,imag);
    }

}

void NMRAnalyser::on_abscheckBox_clicked(){
    if(multiset){
        if(ui->abscheckBox->isChecked()){
            analyse(dataset);
        }else{
            data->removeCurve("abs");
            emit data->dataChanged();
        }
    }else{
    _displayCheckboxClicked(ui->abscheckBox,"abs",timedomain,abs);
    }
}

void NMRAnalyser::on_showWindowBox_clicked(){
    //_analyse();
}

void NMRAnalyser::on_realFTcheckBox_clicked(){
    if(multiset){
    }else{
    _displayCheckboxClicked(ui->realFTcheckBox,"realFT",freqdomain,realFT);
    }
}

void NMRAnalyser::on_imagFTcheckBox_clicked(){
    if(multiset){

    }else{
    _displayCheckboxClicked(ui->imagFTcheckBox,"imagFT",freqdomain,imagFT);
    }
}

void NMRAnalyser::on_absFTcheckBox_clicked(){
    if(multiset){

    }else{
    _displayCheckboxClicked(ui->absFTcheckBox,"absFT",freqdomain,absFT);
    }
}
