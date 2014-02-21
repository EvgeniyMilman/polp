#ifndef SIMULATIONCONTROLPANE_H
#define SIMULATIONCONTROLPANE_H

#include <QWidget>
#include <plugins.h>

namespace Ui {
class SimulationControlPane;
}

class SimulationControlPane : public QWidget
{
    Q_OBJECT
    explicit SimulationControlPane(QWidget *parent = 0);
public:
    static SimulationControlPane* instance();
    ~SimulationControlPane();
    void setSimulation(Simulation* sim);
    void setData(Data* data);
private:
    Ui::SimulationControlPane *ui;
    Simulation * simulation;
    Data* data;
};

#endif // SIMULATIONCONTROLPANE_H
