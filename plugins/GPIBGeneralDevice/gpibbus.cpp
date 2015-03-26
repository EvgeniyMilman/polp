#include "gpibbus.h"
#include <qglobal.h>

static  GPIBBus* bus= NULL;
#ifdef Q_OS_WIN32
#include <visa.h>
static ViSession defaultRM;
#endif

static  char * gpibbuff;

GPIBBus::GPIBBus(){
    busTimeout = DEFAULT_TIMEOUT;
    gpibbuff = new char[BUFFFER_SIZE];
#ifdef Q_OS_WIN32
    viOpenDefaultRM(&defaultRM);
#endif
}

GPIBBus::~GPIBBus(){
#ifdef Q_OS_WIN32
    viClose(defaultRM);
#endif
}


 GPIBBus* GPIBBus::instance()
 {
     if(bus==NULL){
         bus = new GPIBBus;
     }
     return bus;
 }

 unsigned int GPIBBus::openDev(QVariant dev){
#ifdef Q_OS_WIN32
     ViSession vi;
     unsigned int res;
    viOpen(defaultRM,dev.toString().toLocal8Bit().data(),VI_NULL,VI_NULL,&vi);
    res = vi;
    return res;
#endif
 }

 void GPIBBus::closeDev(QVariant dev){
#ifdef Q_OS_WIN32
 ViSession vi;
 unsigned int res = dev.toUInt();
 vi = res;
 viClose(vi);
#endif
 }

QStringList GPIBBus::findDev(QString _str){
#ifdef Q_OS_WIN32
QStringList qlist;
     ViChar buffer [VI_FIND_BUFLEN];
     char buff [1000];
     ViRsrc matches=buffer;
     ViUInt32 nmatches ;
     ViFindList list;
     ViSession vi;
    qDebug((char*)buffer);
     viFindRsrc(defaultRM, "GPIB[0-9]*::?*INSTR", &list, &nmatches, matches);
     for(int i= 0; i< nmatches;i++){
         viOpen(defaultRM,matches,VI_NULL,VI_NULL,&vi);
        // viPrintf(vi,"*RST\n");
         viPrintf (vi, "*IDN?\n");
         viScanf (vi, "%[^\t\n]", buff);
         QString str(buff);
         if((_str.isNull() || _str.isEmpty()) || str.contains(_str)){
             qlist << matches;
         }
        viClose(vi);
        viFindNext(list, matches);
     }
     viClose(list);
    // viClose(defaultRM);
     return qlist;

#endif
 }

 int GPIBBus::write(QVariant dev, void *data, int count, long unsigned int *returnCount){
#ifdef Q_OS_WIN32
     ViSession vi;
     unsigned int res = dev.toUInt();
     vi = res;
     if(vi !=0){
        viPrintf(vi,(char*)data);
     }else{
         //TODO:: handle error
     }
#endif
 }

 int GPIBBus::readByte(QVariant dev, char *data){
#ifdef Q_OS_WIN32
     ViSession vi;
     unsigned int res = dev.toUInt();
      vi = res;
      long unsigned int actual;
      if(vi !=0){
         viRead (vi, (ViBuf)data, 1, &actual);
      } // if vi == 0 do nothing NEED to fix it (handle error)
      return actual;
#endif
      return 0;
 }

 int GPIBBus::readBlock(QVariant dev, void *data, int count, long unsigned int *returnCount){
#ifdef Q_OS_WIN32
     ViSession vi;
     unsigned int res = dev.toUInt();
      vi = res;
      int readres = 0;
      if(vi !=0){
      readres= viRead (vi, (ViBuf)data, count, returnCount);
      } // if vi == 0 do nothing NEED to fix it (handle error)
       return readres;
#endif
       return 0;
 }

 int GPIBBus::gpib_command(QVariant dev, QString command){
     long unsigned int res;
     write(dev,command.toLocal8Bit().data(),command.length(),&res);
     return res;
 }

 QString GPIBBus::gpib_query(QVariant dev, QString command){
      long unsigned int ret = 0;
      queryBlock(dev,command,gpibbuff,BUFFFER_SIZE,&ret);
      gpibbuff[ret] = 0;
      QString res = gpibbuff;
      return res;
 }

 int GPIBBus::queryBlock(QVariant dev, QString command, void *data, int count, long unsigned int *returnCount){
     gpib_command(dev,command);
     return readBlock(dev,data,count,returnCount);
 }

