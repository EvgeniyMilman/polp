#include "frequencycalculator.h"

#include "ui_FrequencyCalculator.h"
#include <math.h>

#define PI 3.14159265359

#define sng(x) ((x < 0)?-1:1)

FrequencyCalculator::FrequencyCalculator(QWidget *parent) :QWidget(parent),ui(new Ui::FrequencyCalculatorForm){
    ui->setupUi(this);
    calc = false;
    ui->hostComboBox->setCurrentIndex(1);
    ui->hostComboBox->setCurrentIndex(0);
    ui->orientationcomboBox->setCurrentIndex(1);
    ui->orientationcomboBox->setCurrentIndex(0);
}

QString FrequencyCalculator::title(){
    return "Triplet frequence calculator";
}

QWidget *FrequencyCalculator::toolPane(){
    return this;
}

void FrequencyCalculator::on_hostComboBox_currentIndexChanged(int index){
    double x[2] = {P_TERPHENYL_X,NAPHTHALENE_X};
    double y[2] = {P_TERPHENYL_Y,NAPHTHALENE_Y};
    double z[2] = {P_TERPHENYL_Z,NAPHTHALENE_Z};
    X = x[index], Y = y[index], Z = z[index];
    calculateESR(ui->magneticFieldSpinBox->value(),X,Y,Z,ui->orientationcomboBox->currentIndex(),phy,theta);
    ui->nmrSpinBox->setValue(ui->magneticFieldSpinBox->value()*PROTON_GYROMAGNETIC_RATIO);
    calc = true;
        ui->esr1SpinBox->setValue(esr1);
        ui->esr2SpinBox->setValue(esr2);
    calc = false;
}

void FrequencyCalculator::on_magneticFieldSpinBox_valueChanged(double magfield){  
    if(!calc){
        calc = true;
        ui->nmrSpinBox->setValue(magfield*PROTON_GYROMAGNETIC_RATIO);
        calculateESR(magfield,X,Y,Z,ui->orientationcomboBox->currentIndex(),phy,theta);
        ui->esr1SpinBox->setValue(esr1);
        ui->esr2SpinBox->setValue(esr2);
        calc = false;
    }
}

void FrequencyCalculator::on_nmrSpinBox_valueChanged(double nmr){
    if(!calc){
        calc = true;
        double magfield = nmr/PROTON_GYROMAGNETIC_RATIO;
        ui->magneticFieldSpinBox->setValue(magfield);
        calculateESR(magfield,X,Y,Z,ui->orientationcomboBox->currentIndex(),phy,theta);
        ui->esr1SpinBox->setValue(esr1);
        ui->esr2SpinBox->setValue(esr2);
        calc = false;
    }
}

void FrequencyCalculator::on_esr1SpinBox_valueChanged(double esr){
    if(!calc){
        calc = true;
        double magfield = calculateMagneticField(esr,1,X,Y,Z,ui->orientationcomboBox->currentIndex(),phy,theta);
        ui->nmrSpinBox->setValue(magfield*PROTON_GYROMAGNETIC_RATIO);
        ui->magneticFieldSpinBox->setValue(magfield);
        calculateESR(magfield,X,Y,Z,ui->orientationcomboBox->currentIndex(),phy,theta);
        ui->esr2SpinBox->setValue(esr2);
        calc = false;
    }
}

void FrequencyCalculator::on_esr2SpinBox_valueChanged(double esr){
    if(!calc){
        calc = true;
        double magfield = calculateMagneticField(esr,-1,X,Y,Z,ui->orientationcomboBox->currentIndex(),phy,theta);
        ui->nmrSpinBox->setValue(magfield*PROTON_GYROMAGNETIC_RATIO);
        ui->magneticFieldSpinBox->setValue(magfield);
        calculateESR(magfield,X,Y,Z,ui->orientationcomboBox->currentIndex(),phy,theta);
        ui->esr1SpinBox->setValue(esr1);
        calc = false;
    }
}

void FrequencyCalculator::on_phySpinBox_valueChanged(double arg1){
    if(!calc){
        calc = true;
        phy = arg1;
        calculateESR(ui->magneticFieldSpinBox->value(),X,Y,Z,ui->orientationcomboBox->currentIndex(),phy,theta);
        ui->esr1SpinBox->setValue(esr1);
        ui->esr2SpinBox->setValue(esr2);
        calc = false;
    }
}

void FrequencyCalculator::on_thetaSpinBox_valueChanged(double arg1){
    if(!calc){
        calc = true;
        theta = arg1;
        calculateESR(ui->magneticFieldSpinBox->value(),X,Y,Z,ui->orientationcomboBox->currentIndex(),phy,theta);
        ui->esr1SpinBox->setValue(esr1);
        ui->esr2SpinBox->setValue(esr2);
        calc = false;
    }
}

void FrequencyCalculator::on_orientationcomboBox_currentIndexChanged(int index){
    ui->phySpinBox->setEnabled(index==3);
    ui->thetaSpinBox->setEnabled(index==3);
    double tethas[4] = {90.,90.,0,theta};
    double phys[4] = {0.,90.,90.,phy};
    calc = true;
    ui->phySpinBox->setValue(phys[index]);
    ui->thetaSpinBox->setValue(tethas[index]);
    calculateESR(ui->magneticFieldSpinBox->value(),X,Y,Z,index,phys[index],tethas[index]);
    ui->esr1SpinBox->setValue(esr1);
    ui->esr2SpinBox->setValue(esr2);
    calc = false;
}

void FrequencyCalculator::calculateESR(double magfield, double X, double Y, double Z, int orientation, double phy, double theta){
    double ws = 2* magfield * 13996.24604;
    double D = (X+Y)/2 - Z;
    double E = -(X-Y)/2;
    double w0,w1,w2;
    double x[3] = {X,Y,Z};
    double y[3] = {Y,X,X};
    double z[3] = {Z,Z,Y};
    if(orientation<3){
        w0 = x[orientation];
        double tmp = sqrt( pow(y[orientation]-z[orientation],2)/4. + ws*ws);
        w1 = (y[orientation]+z[orientation])/2. + tmp;
        w2 = (y[orientation]+z[orientation])/2. - tmp;
    }else{
        double wzfs = D/2.*(1-3.*cos(theta*PI/180.)) + 3./2.*E*sin(theta*PI/180.)*sin(theta*PI/180.)* cos(2*phy*PI/180.);
        w1 = -ws + wzfs/3.;
        w0 = -2./3.*wzfs;
        w2 = ws + wzfs/3.;
    }
    esr1 = fabs(w0 - w1);
    esr2 = fabs(w0 - w2);
}

double FrequencyCalculator::calculateMagneticField(double esr, int transition, double X, double Y, double Z, int orient, double phy, double theta){
    double res,tmp;
    double x[3] = {X,Y,Z};
    double y[3] = {Y,X,X};
    double z[3] = {Z,Z,Y};
    double D = (X+Y)/2 - Z;
    double E = -(X-Y)/2;
    if(orient<3){
            tmp = esr+x[orient]-(y[orient]+z[orient])/2.;
            tmp = (sng(transition) == sng(tmp))?
                      tmp:(-esr+x[orient]-(y[orient]+z[orient])/2.);
        res = sqrt(tmp*tmp - (y[orient]-z[orient])*(y[orient]-z[orient])/4.);
    }else{
        double wzfs = D/2.*(1-3.*cos(theta*PI/180.)) + 3./2.*E*sin(theta*PI/180.)*sin(theta*PI/180.)* cos(2*phy*PI/180.);
        tmp = (wzfs -esr)* sng(transition);
            res = (tmp<0)?(wzfs+esr)*transition:tmp;
    }
    return res / 2. /13996.24604;
}
