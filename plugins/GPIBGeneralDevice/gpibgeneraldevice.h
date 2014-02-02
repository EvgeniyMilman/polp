#ifndef GPIBGENERALDEVICE_H
#define GPIBGENERALDEVICE_H

#include "gpibgeneraldevice_global.h"
#include <QObject>
#include "plugins.h"

namespace Ui {
    class GPIBControlPane;
}

class GPIBGeneralDevice: public QWidget, public Device
{
    Q_OBJECT
    Q_INTERFACES(Device)
public:
    GPIBGeneralDevice(QWidget* parent=0);
    virtual QStringList devices();
    virtual QWidget * controlPane();

    // Returns non zero value if errors occur(For example: Faild to read device)
    virtual int loadData(Data* data);

    //Returns error message
    virtual QString error();

    virtual QString preferedView();
    virtual QString deviceClass();
private:
    QString error_message;
    Data * data;
    Ui::GPIBControlPane* ui;
};

#endif // GPIBGENERALDEVICE_H
