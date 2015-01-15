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
    wf=0;
    hposciloscope=0;
    timer = new QTimer(this);
   connect(ProjectManager::instance(),SIGNAL(projectChanged()),this,SLOT(onProjectChanged()));
   connect(timer,SIGNAL(timeout()),this,SLOT(on_timer()));
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

void TmpESRExperiment::startEXPrun(){
    if(!abort_me){
        wf->setDC(wfdata,volt);
        timer->start(delay);
    }
}

void TmpESRExperiment::onProjectChanged(){
    ui->wfcomboBox->clear();
    ui->osciloscopecomboBox->clear();
    QList<ProjectItem*> items = ProjectManager::instance()->currentProject()->items();
    Q_FOREACH(ProjectItem* item , items){
        DeviceProjectItem* devitem = qobject_cast<DeviceProjectItem*>(item);
        if(devitem != NULL){
            Device * dev =  devitem->device;
            QObject* obj = dynamic_cast<QObject*>(dev);
            WaveFactory* wf = qobject_cast<WaveFactory*>(obj);
            if(wf!=NULL){
                QVariant var = qVariantFromValue((void*)item->data);
                ui->wfcomboBox->addItem(item->data->parameter("title").toString(),var);
                this->wf = wf;
            }
            HPOsciloscope * osc = qobject_cast<HPOsciloscope*>(obj);
            if(osc!=NULL){
                QVariant var = qVariantFromValue((void*)item->data);
                ui->osciloscopecomboBox->addItem(item->data->parameter("title").toString(),var);
                this->hposciloscope = osc;
            }
        }
    }
}

void TmpESRExperiment::on_startpushButton_clicked(){
    progress = 0;
    if(wf!=NULL && hposciloscope !=NULL){

        if(ui->startpushButton->text() == "Start"){
            abort_me = false;
            disconnect(ProjectManager::instance(),SIGNAL(projectChanged()),this,SLOT(onProjectChanged()));
            ui->startpushButton->setText("Stop");
            delay = ui->delaySpinBox->value()*1000;
            min = ui->dcstartSpinBox->value();
            step = ui->dcstepSpinBox->value();
            max = ui->dcstopSpinBox->value();
            volt = min;
            wfdata = (Data2D*)ui->wfcomboBox->currentData().value<void*>();
            hpdata = (Data2D*)ui->osciloscopecomboBox->currentData().value<void*>();
            wf->setSource(wfdata,"1");
            wf->setSourceON(wfdata,true);
            wf->setMode(wfdata,"DC");
            datatitle = ui->datatitlelineEdit->text();
            startEXPrun();
        }else{
            abort_me = true;
            ui->startpushButton->setText("Start");
            timer->stop();
            connect(ProjectManager::instance(),SIGNAL(projectChanged()),this,SLOT(onProjectChanged()));
        }
    }
}

void TmpESRExperiment::on_timer(){
    if(!abort_me){
        double steps = (max - min)/step;
        hposciloscope->readData(hpdata);
        ProjectManager::instance()->currentProject()->copyData(hpdata,datatitle+"_"+QString::number(volt));
        progress = progress + 100./steps-1;
        ui->progressBar->setValue(progress);
        volt = volt + step;
        if(volt> max){
            progress = 100;
            ui->progressBar->setValue(progress);
            on_startpushButton_clicked();
        }else{
             startEXPrun();
        }
    }
}

