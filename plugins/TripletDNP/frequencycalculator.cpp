#include "frequencycalculator.h"

#include "ui_FrequencyCalculator.h"

FrequencyCalculator::FrequencyCalculator(QWidget *parent) :QWidget(parent){
    ui->setupUi(this);
}

QString FrequencyCalculator::title(){
    return "Triplet frequence calculator";
}

QWidget *FrequencyCalculator::toolPane(){
    return this;
}
