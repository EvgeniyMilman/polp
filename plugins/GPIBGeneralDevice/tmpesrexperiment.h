#ifndef TMPESREXPERIMENT_H
#define TMPESREXPERIMENT_H

#include <QWidget>

#include"plugins.h"
#include "wavefactory.h"
#include "osciloscope.h"
#include "hposciloscope.h"
#include "qthread.h"
#include "qtimer.h"

namespace Ui {
class TmpESRExperiment;
}

class TmpESRExperimentThread;

class TmpESRExperiment : public QWidget,Tool
{
    Q_OBJECT
    Q_INTERFACES(Tool)
public:
    explicit TmpESRExperiment(QWidget *parent = 0);
    ~TmpESRExperiment();
     QString title();
     QWidget* toolPane();
private:
    Ui::TmpESRExperiment *ui;
    WaveFactory* wf;
    Osciloscope* osciloscope;
    HPOsciloscope* hposciloscope;
    QTimer* timer;
    void startEXPrun();
    double max;
    double step;
    double volt;
    double min;
    double delay;
    Data2D* wfdata;
    Data2D* hpdata;
    QString datatitle;
    double progress;
    bool abort_me;

private slots:
   void  onProjectChanged();
   void on_startpushButton_clicked();
   void on_timer();

};




#endif // TMPESREXPERIMENT_H
