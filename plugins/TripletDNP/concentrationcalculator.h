#ifndef CONCENTRATIONCALCULATOR_H
#define CONCENTRATIONCALCULATOR_H

#include <QWidget>
#include <plugins.h>

namespace Ui {
class ConcentrationCalculatorForm;
}

class ConcentrationCalculator : public QWidget, public Tool
{
    Q_OBJECT
    Q_INTERFACES(Tool)
public:
    explicit ConcentrationCalculator(QWidget *parent = 0);
    virtual QString title();
    virtual QWidget* toolPane();
signals:

public slots:

private:
    Ui::ConcentrationCalculatorForm * ui;
};

#endif // CONCENTRATIONCALCULATOR_H
