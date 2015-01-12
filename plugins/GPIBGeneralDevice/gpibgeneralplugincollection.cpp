#include "gpibgeneralplugincollection.h"

#include "gpibdeviceview.h"
#include "gpibgeneraldevice.h"
#include "osciloscope.h"
#include "hposciloscope.h"
#include "wavefactory.h"

GPIBGeneralPluginCollection::GPIBGeneralPluginCollection(QObject *parent) :
    QObject(parent)
{
}

QList<QObject *> GPIBGeneralPluginCollection::plugins()
{
    QList<QObject *> plugins;
    plugins.append(new GPIBGeneralDevice);
    //plugins.append(new GPIBDeviceView);
    plugins.append(new Osciloscope);
    plugins.append(new HPOsciloscope);
    plugins.append(new WaveFactory);
    return plugins;
}
