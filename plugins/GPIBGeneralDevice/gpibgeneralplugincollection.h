#ifndef GPIBGENERALPLUGINCOLLECTION_H
#define GPIBGENERALPLUGINCOLLECTION_H

#include <QObject>
#include "plugins.h"

class GPIBGeneralPluginCollection : public QObject, public PluginCollection
{
    Q_OBJECT
    Q_INTERFACES(PluginCollection)
    Q_PLUGIN_METADATA(IID "GPIBGeneralPluginCollection.Plugin")
public:
    explicit GPIBGeneralPluginCollection(QObject *parent = 0);
    virtual QList<QObject *> plugins();
signals:

public slots:

};

#endif // GPIBGENERALPLUGINCOLLECTION_H
