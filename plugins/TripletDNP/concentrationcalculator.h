#ifndef CONCENTRATIONCALCULATOR_H
#define CONCENTRATIONCALCULATOR_H

#include <QWidget>
#include <plugins.h>

namespace Ui {
class ConcentrationCalculatorForm;
}

#define PENTACENE_MOLAR_MASS 278.36 // g/mol

#define p_TERPHENYL_MOLAR_MASS 230.30 // g/mol

#define NAPHTHALENE_MOLAR_MASS 128.17 // g/mol


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

private slots:
    void on_comboBox_currentIndexChanged(int index);

    void on_molarmassSpinBox_valueChanged(double arg1);

    void on_massDoubleSpinBox_valueChanged(double arg1);

    void on_concentrationSpinBox_valueChanged(double arg1);

    void on_massPentaceneSpinBox_valueChanged(double arg1);

private:
    Ui::ConcentrationCalculatorForm * ui;
    double customMolarMass;

    double calculatePentaceneMass(double hostMolarMass, double hostMass, double concentration);
    double calculateHostMass(double hostMolarMass, double pentaceneMass, double concentration);
};

#endif // CONCENTRATIONCALCULATOR_H
