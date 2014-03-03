#ifndef PROJECTMANAGER_H
#define PROJECTMANAGER_H


#include "project.h"

#define POLP_PROJECT_SIGN "polpcontrol"
#define POLP_PROJECT_VERSION  1

#define PROJECT_DEFAULT_NAME "untitled"

class ProjectManager :public QObject {
    Q_OBJECT
private:
     ProjectManager(QObject *parent=0);
public:
     static ProjectManager *instance();
     int loadFromFile(QString file);
     Project* currentProject();
     Project* newProject();
     int saveCurrentProject(QString filename);
     QString error();
private:
     Project* _currentProject;
    QString errormessage;
public:
signals:
     void projectChanged();

private slots:
     void onProjectChanged();

private:
     void loadProjectV1(QDataStream &in);
};

#endif // PROJECTMANAGER_H
