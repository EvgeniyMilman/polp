#include "gpibbus.h"

static  GPIBBus* bus=NULL;
GPIBBus::GPIBBus()
{
}

 GPIBBus* GPIBBus::instance()
{
     if(bus==NULL){
         bus = new GPIBBus;
     }
    return bus;
}
