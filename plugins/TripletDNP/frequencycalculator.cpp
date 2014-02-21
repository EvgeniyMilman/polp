#include "frequencycalculator.h"

FrequencyCalculator::FrequencyCalculator(QWidget *parent) :
    QWidget(parent)
{
}

QString FrequencyCalculator::title(){
    return "Triplet frequence calculator";
}

QWidget *FrequencyCalculator::toolPane(){
    return this;
}
