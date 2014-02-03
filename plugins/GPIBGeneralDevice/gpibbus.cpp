#include "gpibbus.h"

static  GPIBBus* bus=NULL;
GPIBBus::GPIBBus()
{
}

 GPIBBus* GPIBBus::innstance()
{
     if(bus==NULL){
         bus = new GPIBBus;
     }
    return bus;
}
