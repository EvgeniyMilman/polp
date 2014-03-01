#ifndef FREQUENCYCALCULATOR_H
#define FREQUENCYCALCULATOR_H

#include <QWidget>
#include <plugins.h>

namespace Ui {
class FrequencyCalculatorForm;
}

#define P_TERPHENYL_X 518.54
#define P_TERPHENYL_Y 411.84
#define P_TERPHENYL_Z -930.38

#define NAPHTHALENE_X  503.0
#define NAPHTHALENE_Y  418.0
#define NAPHTHALENE_Z -921.0

#define PROTON_GYROMAGNETIC_RATIO 42.576 // MHz/T

class FrequencyCalculator : public QWidget, public Tool
{
    Q_OBJECT
    Q_INTERFACES(Tool)
public:
    explicit FrequencyCalculator(QWidget *parent = 0);
    virtual QString title();
    virtual QWidget *toolPane();
signals:

public slots:
private slots:
    void on_hostComboBox_currentIndexChanged(int index);

    void on_magneticFieldSpinBox_valueChanged(double arg1);

    void on_nmrSpinBox_valueChanged(double arg1);

    void on_esr1SpinBox_valueChanged(double arg1);

    void on_esr2SpinBox_valueChanged(double arg1);

    void on_phySpinBox_valueChanged(double arg1);

    void on_thetaSpinBox_valueChanged(double arg1);

    void on_orientationcomboBox_currentIndexChanged(int index);

private:
    Ui::FrequencyCalculatorForm * ui;
    bool calc;
    double X ,Y ,Z;
    double phy, theta;
    double esr1, esr2;
    void calculateESR(double magfield, double X, double Y, double Z,int orientation, double phy, double theta);
    double calculateMagneticField(double esr, int transition, double X, double Y, double Z,int orientation, double phy, double theta);
};

#endif // FREQUENCYCALCULATOR_H
