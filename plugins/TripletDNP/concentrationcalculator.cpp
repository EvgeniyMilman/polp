#include "concentrationcalculator.h"

ConcentrationCalculator::ConcentrationCalculator(QWidget *parent) :
    QWidget(parent)
{
}

QString ConcentrationCalculator::title(){
    return "Dopping mass calcualtor";
}

QWidget *ConcentrationCalculator::toolPane(){
    return this;
}
