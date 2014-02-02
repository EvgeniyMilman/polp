#include "gpibgeneralplugincollection.h"

#include "gpibdeviceview.h"
#include "gpibgeneraldevice.h"

GPIBGeneralPluginCollection::GPIBGeneralPluginCollection(QObject *parent) :
    QObject(parent)
{
}

QList<QObject *> GPIBGeneralPluginCollection::plugins()
{
    QList<QObject *> plugins;
    plugins.append(new GPIBGeneralDevice);
    plugins.append(new GPIBDeviceView);
    return plugins;
}
