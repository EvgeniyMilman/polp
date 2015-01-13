#include "tmpesrexperiment.h"
#include "ui_tmpesrexperiment.h"
#include "wavefactory.h"
#include "osciloscope.h"
#include "hposciloscope.h"
#include "projectmanager.h"
#include <qthread.h>

TmpESRExperiment::TmpESRExperiment(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TmpESRExperiment)
{
    ui->setupUi(this);
   connect(ProjectManager::instance(),SIGNAL(projectChanged()),this,SLOT(onProjectChanged()));
}

TmpESRExperiment::~TmpESRExperiment(){
    delete ui;
}

QString TmpESRExperiment::title(){
    return "Temp ESR experiment";
}

QWidget *TmpESRExperiment::toolPane()
{
    return this;
}

void TmpESRExperiment::onProjectChanged(){
    ui->wfcomboBox->clear();
    ui->osciloscopecomboBox->clear();
    QList<ProjectItem*> items = ProjectManager::instance()->currentProject()->items();
    Q_FOREACH(ProjectItem* item , items){
        DeviceProjectItem* devitem = qobject_cast<DeviceProjectItem*>(item);
        if(devitem != NULL){
            WaveFactory* wf = qobject_cast<WaveFactory*>((QObject*)devitem->device);
            if(wf!=NULL){
                QVariant var = qVariantFromValue((void*)item->data);
                ui->wfcomboBox->addItem(item->data->parameter("title").toString(),var);
                this->wf = wf;
            }
            HPOsciloscope * osc = qobject_cast<HPOsciloscope*>((QObject*)devitem->device);
            if(osc!=NULL){
                QVariant var = qVariantFromValue((void*)item->data);
                ui->osciloscopecomboBox->addItem(item->data->parameter("title").toString(),var);
                this->hposciloscope = osc;
            }
        }
    }
}

void TmpESRExperiment::on_startpushButton_clicked(){

 if(wf!=NULL && hposciloscope !=NULL){
    disconnect(ProjectManager::instance(),SIGNAL(projectChanged()),this,SLOT(onProjectChanged()));
    Data2D* wfdata = (Data2D*)ui->wfcomboBox->currentData().value<void*>();
    Data2D* hpdata = (Data2D*)ui->osciloscopecomboBox->currentData().value<void*>();
    wf->setSource(wfdata,"1");
    wf->setSourceON(wfdata,true);
    wf->setMode(wfdata,"DC");
    wf->setDC(wfdata, ui->dcstartSpinBox->value());
    double step = ui->dcstepSpinBox->value();
    double max = ui->dcstopSpinBox->value();
    if(step != 0){
        for(double volt=ui->dcstartSpinBox->value(); volt<max; volt=volt+step){
            wf->setDC(wfdata, volt);
            QThread::sleep(ui->delaySpinBox->value());
            hposciloscope->readData(hpdata);
            QString title = ui->datatitlelineEdit->text()+"_"+QString::number(volt);
            ProjectManager::instance()->currentProject()->copyData(hpdata,title);
        }
    }
    connect(ProjectManager::instance(),SIGNAL(projectChanged()),this,SLOT(onProjectChanged()));
 }
}
