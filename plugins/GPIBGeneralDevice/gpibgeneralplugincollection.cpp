#include "gpibgeneralplugincollection.h"

#include "gpibdeviceview.h"
#include "gpibgeneraldevice.h"
#include "osciloscope.h"

GPIBGeneralPluginCollection::GPIBGeneralPluginCollection(QObject *parent) :
    QObject(parent)
{
}

QList<QObject *> GPIBGeneralPluginCollection::plugins()
{
    QList<QObject *> plugins;
    plugins.append(new GPIBGeneralDevice);
    plugins.append(new GPIBDeviceView);
    plugins.append(new Osciloscope);

    return plugins;
}
