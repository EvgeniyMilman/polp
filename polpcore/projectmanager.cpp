#include "projectmanager.h"
#include <QFile>
#include <QDataStream>
#include "pluginmanager.h"

static ProjectManager* manager=NULL;

ProjectManager::ProjectManager(QObject *parent) :QObject(parent){
    _currentProject=NULL;
    newProject();
}

ProjectManager* ProjectManager::instance(){
    if(manager == NULL){
        manager = new ProjectManager();
    }
    return manager;
}


Project *ProjectManager::currentProject(){
    return _currentProject;
}

Project *ProjectManager::newProject(){
    //TODO:: Add checking for need of saving
    // Save project
    if(_currentProject!=NULL){
        disconnect(_currentProject,SIGNAL(projectChanged()),this,SLOT(onProjectChanged()));
        delete _currentProject;
    }
    _currentProject = new Project();
    connect(_currentProject,SIGNAL(projectChanged()),this,SLOT(onProjectChanged()));
    _currentProject->filename = PROJECT_DEFAULT_NAME;
    emit projectChanged();
    return _currentProject;
}

// Save
int ProjectManager::saveCurrentProject(QString filename){
    QFile file(filename);
    file.open(QIODevice::WriteOnly);
    QDataStream out(&file);
    out.setFloatingPointPrecision(QDataStream::SinglePrecision);
    out << QString(POLP_PROJECT_SIGN); // Save project sig
    out << qint8(POLP_PROJECT_VERSION);
    Project * prj = currentProject();
    out << prj->items().size();
    Q_FOREACH(ProjectItem* item,prj->items()){
        //Save item type
        if(qobject_cast<DeviceProjectItem*>(item)!=NULL){
            out << qint8(ProjectItem::DeviceItem);
            out << qobject_cast<DeviceProjectItem*>(item)->device->deviceClass();
        }else if(qobject_cast<SimulationProjectItem*>(item)!=NULL){
            out << qint8(ProjectItem::SimulationItem);
            out << qobject_cast<SimulationProjectItem*>(item)->simulation->title();
        }else{
            out << qint8(ProjectItem::Item);
        }
        //save item data preferences
        QList<QString> dataparameters = item->data->parameterList();
        out << qint32(dataparameters.size());
        for(int i=0;i < dataparameters.size();i++){
            out << dataparameters[i];
            out << item->data->parameter(dataparameters[i]);
        }
        // save data curves
        Data2D* data = (Data2D*)item->data;
        QList<QString> curves = data->curvers();
        out << qint32(curves.size());
        Q_FOREACH(QString curve, curves){
            out << curve ;
            int curvesize = qint32(data->x(curve)->size());
            out << curvesize;
            QVector<double>* x = data->x(curve);
            QVector<double>* y = data->y(curve);
            for(int i =0;i<curvesize ; i++){
                float xi = (float)(x->at(i));
                float yi = (float)(y->at(i));
                out << xi << yi;
            }
        }
    }
    currentProject()->filename = filename;
    file.close();
    return 0;
}

QString ProjectManager::error(){
    return errormessage;
}

void ProjectManager::onProjectChanged(){
    emit projectChanged();
}


int ProjectManager::loadFromFile(QString filename){
    //newProject();
    if(_currentProject!=NULL){
        disconnect(_currentProject,SIGNAL(projectChanged()),this,SLOT(onProjectChanged()));
        delete _currentProject;
    }
    _currentProject = new Project();
    connect(_currentProject,SIGNAL(projectChanged()),this,SLOT(onProjectChanged()));
    // Version 1 load
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly)){
        errormessage = "File can not be opened";
        return -1;
    };
    QDataStream in(&file);
    QString id;
    qint8 version;
    in >>id;
    if(id != POLP_PROJECT_SIGN){
        errormessage = "File format is wrong";
        file.close();
        return -1;
    }
    in >> version;
    if(version ==1){
        loadProjectV1(in);
    }else{
        errormessage = "Polpcontrol does not support this project version.";
        file.close();
        return -1;
    }
    file.close();
    currentProject()->filename  = filename;
    emit projectChanged();
    return 0;
}


void ProjectManager::loadProjectV1(QDataStream &in){
    QString tmp;
    qint8 tmpi8;
    qint32 tmpi32;
    float xi,yi;
    in.setFloatingPointPrecision(QDataStream::SinglePrecision);
    in >> tmpi32;
    int size = tmpi32;
    for(int i =0;i<size;i++){
        Data2D* data = new Data2D(currentProject());
        in >> tmpi8;
        int type = tmpi8;
        if(type>0){
            in >> tmp;
        }
        QString providerClass = tmp;

        //load data preferences
        in >> tmpi32;
        for(int i=0;i < tmpi32;i++){
            in >>tmp;
            QVariant v;
            in >> v;
            data->setParameter(tmp,v);
        }
        //Add data to project
        if(type==ProjectItem::DeviceItem){
            Device* dev = PluginManager::instance()->findDevice(providerClass);
            if(dev){
                currentProject()->addItem(data,dev);
            }else {
                currentProject()->addItem(data,"NONE");
            }
        }else if(type==ProjectItem::SimulationItem){
            Simulation* sim = PluginManager::instance()->findSimulation(providerClass);
            if(sim){
                 currentProject()->addItem(data,sim);
            }else {
                 currentProject()->addItem(data,"NONE");
            }
        }else{
            currentProject()->addItem(data,"NONE");
        }

        // load data curves
        in >> tmpi32;
        int curves = tmpi32;
        for(int i=0;i < curves;i++){
            in >> tmp; //curve name
            in >> tmpi32; //data points
            for(int i =0;i<tmpi32; i++){
                in >> xi >>yi;
                data->addPoint(tmp,xi,yi);
            }
        }
    }
}
