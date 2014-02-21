#include "simulationcontrolpane.h"
#include "ui_simulationcontrolpane.h"


static SimulationControlPane * controlPane = NULL;

SimulationControlPane::SimulationControlPane(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SimulationControlPane)
{
    ui->setupUi(this);
}

SimulationControlPane *SimulationControlPane::instance(){
    if(controlPane==NULL){
        controlPane= new SimulationControlPane;
    }
    return controlPane;
}

SimulationControlPane::~SimulationControlPane()
{
    delete ui;
}

void SimulationControlPane::setSimulation(Simulation *sim){
    simulation = sim;
}

void SimulationControlPane::setData(Data *data){
 this->data = data;
}
