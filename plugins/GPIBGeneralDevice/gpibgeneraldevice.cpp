#include "gpibgeneraldevice.h"
#include "ui_GPIBControlPane.h"
#include "gpibbus.h"

GPIBGeneralDevice::GPIBGeneralDevice(QWidget *parent):QWidget(parent),ui(new Ui::GPIBControlPane)
{
}

QStringList GPIBGeneralDevice::devices()
{
    return GPIBBus::instance()->findDev("");
}

QWidget *GPIBGeneralDevice::controlPane()
{
    return this;
}

int GPIBGeneralDevice::loadData(Data *data)
{
    this->data = data;
    return 0;
}

QString GPIBGeneralDevice::error()
{
    return error_message;
}

QString GPIBGeneralDevice::preferedView()
{
    return "GPIB General Device";
}

QString GPIBGeneralDevice::deviceClass()
{
    return "GPIB General Device";
}
