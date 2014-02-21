#ifndef TRIPLETDNP_H
#define TRIPLETDNP_H


#include "tripletdnp_global.h"

#include "plugins.h"

class TRIPLETDNPSHARED_EXPORT TripletDNP : public QObject , public PluginCollection
{
    Q_OBJECT
    Q_INTERFACES(PluginCollection)
    Q_PLUGIN_METADATA(IID "TripletDNPPluginCollection.Plugin")
public:
    TripletDNP();
    virtual QList<QObject *> plugins();
};

#endif // TRIPLETDNP_H
