#ifndef TMPESREXPERIMENT_H
#define TMPESREXPERIMENT_H

#include <QWidget>

#include"plugins.h"
#include "wavefactory.h"
#include "osciloscope.h"
#include "hposciloscope.h"

namespace Ui {
class TmpESRExperiment;
}

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

private slots:
   void  onProjectChanged();
   void on_startpushButton_clicked();

};


#endif // TMPESREXPERIMENT_H
