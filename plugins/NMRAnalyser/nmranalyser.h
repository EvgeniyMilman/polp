#ifndef NMRANALYSER_H
#define NMRANALYSER_H

#include "nmranalyser_global.h"
#include "plugins.h"
#include "nmrcontrolform.h"
#include <QWidget>

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

private:
    Ui::NMRControlForm *ui;
    Data2D * data;
    Data2D * currentdata;
};

#endif // NMRANALYSER_H
