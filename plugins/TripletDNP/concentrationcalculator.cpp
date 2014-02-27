#include "concentrationcalculator.h"

#include "ui_ConcentrationCalculator.h"

ConcentrationCalculator::ConcentrationCalculator(QWidget *parent) :QWidget(parent){
    ui->setupUi(this);
}

QString ConcentrationCalculator::title(){
    return "Dopping mass calcualtor";
}

QWidget *ConcentrationCalculator::toolPane(){
    return this;
}
