#ifndef NMRANALYSER_H
#define NMRANALYSER_H

#include "nmranalyser_global.h"
#include "plugins.h"
#include <QWidget>
#include <QCheckBox>

namespace Ui {
class NMRControlForm;
}

class NMRANALYSERSHARED_EXPORT NMRAnalyser : public QWidget , public Analyser
{
    Q_OBJECT
    Q_INTERFACES(Analyser)
    Q_PLUGIN_METADATA(IID "NMRAnalyser.Plugin")
public:
     NMRAnalyser(QWidget *parent = 0);
     ~NMRAnalyser();
     QString title();
     QWidget * controlPane();
     Data * analyse(Data* data);
     Data * analyse(QList<Data*> dataset);

private slots:
     void on_skipSpinBox_valueChanged(double arg1);

     void on_dwtimeSpinBox_valueChanged(double arg1);

     void on_offsetSpinBox_valueChanged(double arg1);

     void on_zerofillcheckBox_clicked();

     void on_autophaseCheckBox_clicked();

     void on_phaseSpinBox_valueChanged(double arg1);

     void on_windowFunctionBox_currentIndexChanged(int index);

     void on_windowTimeConstantSpinBox_valueChanged(double arg1);

     void on_realcheckBox_clicked();

     void on_imagcheckBox_clicked();

     void on_abscheckBox_clicked();

     void on_showWindowBox_clicked();

     void on_realFTcheckBox_clicked();

     void on_imagFTcheckBox_clicked();

     void on_absFTcheckBox_clicked();

private:
    bool analysis;
    Ui::NMRControlForm *ui;
    Data2D * data;
    Data2D * currentdata;
    QList<Data *> dataset;
    bool multiset;
    QVector<double> * real;
    QVector<double> * imag;
    QVector<double> * abs;
    QVector<double> * timedomain;
    QVector<double> * freqdomain;
    QVector<double> * realFT;
    QVector<double> * imagFT;
    QVector<double> * absFT;

private:
    void _analyse();
    void _calcDCoffset(QVector<double>* y1,QVector<double>* y2, double& dc1, double& dc2 );
    void _loadData(QVector<double>* realX,QVector<double>* realY,QVector<double>*imagX,QVector<double>*imagY,double dcreal, double dcimag);
    double windowFunction(int index, double time,double timeconst);
    void  _displayCheckboxClicked(QCheckBox * checkbox,QString curve, QVector<double> *x,QVector<double>* y );
    void _calcFT();
    void _integrate(double* realInt,double *imagInt,double* absInt);
};

#endif // NMRANALYSER_H
