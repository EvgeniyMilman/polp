#ifndef PLUGINS_H
#define PLUGINS_H

#include "data.h"
#include <QWidget>
#include <QStringList>

class View{
public:
    virtual QString title()=0;
    virtual QWidget * viewPane()=0;

    // Returns non zero value if errors occur(For example: Wrong data format)
    virtual int setData(Data* data)=0;

    //Returns error message
    virtual QString error() = 0;

};

class Analyser{
public:
    virtual QString title()=0;
    virtual QWidget * controlPane()=0;
    virtual Data * analyse(Data* data)=0;
    virtual Data * analyse(QList<Data*> dataset)=0;
};

class Device{
public:
    virtual QStringList devices()=0;
    virtual QWidget * controlPane()=0;

    // Returns non zero value if errors occur(For example: Faild to read device)
    virtual int loadData(Data* data)=0;

    //Returns error message
    virtual QString error() = 0;

    virtual QString preferedView()=0;
    virtual QString deviceClass()=0;
};

class FileFormat{
public:
    virtual QString extension()=0;
    virtual QString description()=0;

    // Returns non zero value if errors occur(For example: Faild to open file)
    virtual int loadData(Data* data)=0;
    virtual int saveData(Data* data)=0;

    //Returns error message
    virtual QString error() = 0;

    virtual QString preferedView()=0;
};

class Simulation{
public:
    virtual QString title()=0;
    virtual Data * simulate(Data* data)=0;
    virtual QString preferedView()=0;
    virtual void predefineDataParametres(Data* data)=0;
    virtual int progress()=0;
};

class Tool{
public:
    virtual QString title()=0;
    virtual QWidget* toolPane()=0;
};

class Settings{
public:
    virtual QString title()=0;
    virtual QWidget* settingsPane()=0;
    virtual void saveSettings()=0;
};

class PluginCollection {
public:
    virtual QList<QObject*> plugins()=0;
};

Q_DECLARE_INTERFACE(View,"jp.riken.ribf.polp.View/1.0")
Q_DECLARE_INTERFACE(Analyser,"jp.riken.ribf.polp.Analyser/1.0")
Q_DECLARE_INTERFACE(Device,"jp.riken.ribf.polp.Device/1.0")
Q_DECLARE_INTERFACE(FileFormat,"jp.riken.ribf.polp.FileFormat/1.0")
Q_DECLARE_INTERFACE(Simulation,"jp.riken.ribf.polp.Simulation/1.0")
Q_DECLARE_INTERFACE(Tool,"jp.riken.ribf.polp.Tool/1.0")
Q_DECLARE_INTERFACE(Settings,"jp.riken.ribf.polp.Settings/1.0")
Q_DECLARE_INTERFACE(PluginCollection,"jp.riken.ribf.polp.PluginCollection/1.0")


#endif // PLUGINS_H
