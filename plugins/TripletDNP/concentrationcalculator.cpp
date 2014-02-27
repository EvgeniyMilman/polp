#include "concentrationcalculator.h"

#include "ui_ConcentrationCalculator.h"

ConcentrationCalculator::ConcentrationCalculator(QWidget *parent) :QWidget(parent),ui(new Ui::ConcentrationCalculatorForm()){
    ui->setupUi(this);
    ui->comboBox->setCurrentIndex(1);
    ui->comboBox->setCurrentIndex(0);
}

QString ConcentrationCalculator::title(){
    return "Dopping mass calcualtor";
}

QWidget *ConcentrationCalculator::toolPane(){
    return this;
}

void ConcentrationCalculator::on_comboBox_currentIndexChanged(int index){
   double molarMass[3] ={p_TERPHENYL_MOLAR_MASS,NAPHTHALENE_MOLAR_MASS, customMolarMass};
    ui->molarmassSpinBox->setEnabled((index == 2));
    ui->molarmassSpinBox->setValue(molarMass[index]);
    ui->massPentaceneSpinBox->setValue(
                calculatePentaceneMass(ui->molarmassSpinBox->value(),
                                       ui->massDoubleSpinBox->value(),
                                       ui->concentrationSpinBox->value()
                ));

}

void ConcentrationCalculator::on_molarmassSpinBox_valueChanged(double arg1){
    if(ui->molarmassSpinBox->isEnabled()){
        customMolarMass = arg1;
        ui->massPentaceneSpinBox->setValue(
                    calculatePentaceneMass(ui->molarmassSpinBox->value(),
                                           ui->massDoubleSpinBox->value(),
                                           ui->concentrationSpinBox->value()
                    ));
    }
}


void ConcentrationCalculator::on_massDoubleSpinBox_valueChanged(double arg1){
    ui->massPentaceneSpinBox->setValue(
                calculatePentaceneMass(ui->molarmassSpinBox->value(),
                                       ui->massDoubleSpinBox->value(),
                                       ui->concentrationSpinBox->value()
                ));
}

void ConcentrationCalculator::on_concentrationSpinBox_valueChanged(double arg1){
    ui->massPentaceneSpinBox->setValue(
                calculatePentaceneMass(ui->molarmassSpinBox->value(),
                                       ui->massDoubleSpinBox->value(),
                                       ui->concentrationSpinBox->value()
                ));
}

void ConcentrationCalculator::on_massPentaceneSpinBox_valueChanged(double arg1){
    ui->massDoubleSpinBox->setValue(
                calculateHostMass(ui->molarmassSpinBox->value(),
                                       ui->massPentaceneSpinBox->value(),
                                       ui->concentrationSpinBox->value()
                ));
}

double ConcentrationCalculator::calculatePentaceneMass(double hostMolarMass, double hostMass, double concentration){
    double result;
    double hostSubstanceAmount = hostMass / hostMolarMass;
    concentration = concentration /100;
    double pentaceneSubstanceAmount = concentration * hostSubstanceAmount / (1-concentration);
    result = pentaceneSubstanceAmount * PENTACENE_MOLAR_MASS*1000;
    return result;
}

double ConcentrationCalculator::calculateHostMass(double hostMolarMass, double pentaceneMass, double concentration){
    double result;
    concentration = concentration /100;
    double pentaceneSubstanceAmount = pentaceneMass /PENTACENE_MOLAR_MASS /1000;
    double hostSubstanceAmount = pentaceneSubstanceAmount *(1 - concentration)/concentration;
    result = hostSubstanceAmount * hostMolarMass;
    return result;
}
