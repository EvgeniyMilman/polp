#include "gpibdeviceview.h"
#include "ui_GPIBDeviceView.h"

GPIBDeviceView::GPIBDeviceView(QWidget *parent) :
    QWidget(parent),ui(new Ui::GPIBDeviceView)
{
}

QString GPIBDeviceView::title()
{
    return "GPIB General Device";
}

QWidget *GPIBDeviceView::viewPane()
{
    return this;
}

int GPIBDeviceView::setData(Data *data)
{
    return 0;
}

QString GPIBDeviceView::error()
{
    return error_message;
}
