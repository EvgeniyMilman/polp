#include "project.h"
#include "pluginmanager.h"
#include "projectmanager.h"
#include "simulationcontrolpane.h"

Project::Project(QObject *parent) :
    QObject(parent)
{
}

Project::~Project(){
    _items.clear();
}

QList<ProjectItem *> Project::items(){
    return _items;
}

void Project::addItem(Data *data, Simulation *sim){
    SimulationProjectItem * item = new SimulationProjectItem(this);
    item->title = data->parameter("title").toString();
    item->data = data;
    item->simulation = sim;
    item->view = PluginManager::instance()->findView(sim->preferedView());
    _items.append(item);
    _status = Unsaved;
    emit projectChanged();
}

void Project::addItem(Data *data, Device *dev){
    DeviceProjectItem * item = new DeviceProjectItem(this);
    item->data = data;
    item->device = dev;
    item->title = data->parameter("title").toString();
    item->view = PluginManager::instance()->findView(dev->preferedView());
    _items.append(item);
    _status = Unsaved;
    emit projectChanged();
}

void Project::addItem(Data *data,QString preferedview){
    ProjectItem * item = new ProjectItem(this);
    item->data = data;
    item->title = data->parameter("title").toString();
    item->view = PluginManager::instance()->findView(preferedview);
    _items.append(item);
    _status = Unsaved;
    emit projectChanged();
}

void Project::deleteItems(QList<ProjectItem *> datalist){
    Q_FOREACH(ProjectItem * item, datalist){
        _items.removeOne(item);
    }
    _status = Unsaved;
    emit projectChanged();
}

void Project::copyData(Data *_data, QString title){
    Data2D * data = new Data2D;
        Data2D * _currentData =(Data2D *)_data;
        if(_currentData!=NULL){
            Q_FOREACH(QString curve, _currentData->curvers()){
                data->addCurve(curve,_currentData->x(curve),_currentData->y(curve));
            }
            Q_FOREACH(QString param, _currentData->parameterList()){
                data->setParameter(param,_currentData->parameter(param));
            }
        }
    data->setParameter("title",title);
    addItem(data,"SimpleView");
}

Project::Status Project::status(){
    return Unsaved;
}

QString Project::getFilename(){
    return filename;
}


ProjectItem::ProjectItem(QObject *parent):QObject(parent){
   analyser =NULL;
   data = NULL;
   view =NULL;
}

ProjectItem::~ProjectItem(){
   // delete data;
   // data = NULL;
}

void ProjectItem::itemSelected(){
}

QWidget *ProjectItem::control()
{
    return NULL;
}


SimulationProjectItem::SimulationProjectItem(QObject *parent):ProjectItem(parent){
    simulation=NULL;
}

void SimulationProjectItem::itemSelected(){
}

QWidget *SimulationProjectItem::control() {
    SimulationControlPane::instance()->setSimulation(this->simulation);
    SimulationControlPane::instance()->setData(this->data);
    return SimulationControlPane::instance();
}


DeviceProjectItem::DeviceProjectItem(QObject *parent):ProjectItem(parent){
    device = NULL;
}

void DeviceProjectItem::itemSelected(){
    if(device!=NULL)
        device->loadData(data);
}

QWidget *DeviceProjectItem::control(){
    if(device!=NULL){
        return device->controlPane();
    }
}


ProjectModel::ProjectModel(QObject *parent):QAbstractListModel(parent){
    connect(ProjectManager::instance(),SIGNAL(projectChanged()),this,SLOT(onProjectChanged()));
}

ProjectModel::~ProjectModel(){
}

QVariant ProjectModel::data(const QModelIndex &index, int role) const{
    Project* project = ProjectManager::instance()->currentProject();
    if(!index.isValid())
            return QVariant();
    if (index.row() >= project->items().size())
            return QVariant();
    if ( role == Qt::DisplayRole || role == Qt::EditRole ) {
        return project->items().at(index.row())->title;
    }else if(role ==Qt::UserRole){
        Project* project = ProjectManager::instance()->currentProject();
        QVariant v;
        v.setValue<void*>(project->items().at(index.row()));
        return v;
    }
    return QVariant();
}

QVariant ProjectModel::headerData(int selection, Qt::Orientation orientation, int role) const{
    return "Project";
}

int ProjectModel::rowCount(const QModelIndex &parent) const{
    Project* project = ProjectManager::instance()->currentProject();  
    return project->items().size();
}

bool ProjectModel::insertRows(int position, int rows, const QModelIndex &parent){
    return false; //TODO::
}

bool ProjectModel::removeRows(int position, int rows, const QModelIndex &parent){
    return false;
}

bool ProjectModel::setData(const QModelIndex &index, const QVariant &value, int role){
        if(role == Qt::EditRole){
            Project* project = ProjectManager::instance()->currentProject();
            Data * data = project->items().at(index.row())->data;
            data->setParameter("title",value);
            project->items().at(index.row())->title = value.toString();
            return true;
        }
    return false; //TODO::
}

Qt::ItemFlags ProjectModel::flags(const QModelIndex &index) const{
    if(!index.isValid())
            return Qt::ItemIsEditable;
    return QAbstractListModel::flags(index) |  Qt::ItemIsSelectable |Qt::ItemIsEnabled | Qt::ItemIsEditable;
}

void ProjectModel::onProjectChanged(){
    QModelIndex _left = createIndex(0,0);
    QModelIndex _right = createIndex(ProjectManager::instance()->currentProject()->items().size()-1,0);
    emit dataChanged(_left,_right);
}


