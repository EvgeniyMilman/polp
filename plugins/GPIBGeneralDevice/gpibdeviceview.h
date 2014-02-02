#ifndef GPIBDEVICEVIEW_H
#define GPIBDEVICEVIEW_H

#include <QObject>
#include <plugins.h>

namespace Ui {
    class GPIBDeviceView;
}


class GPIBDeviceView : public QWidget, public View
{
    Q_OBJECT
    Q_INTERFACES(View)
public:
    explicit GPIBDeviceView(QWidget *parent = 0);
    virtual QString title();
    virtual QWidget *viewPane();

    // Returns non zero value if errors occur(For example: Wrong data format)
    virtual int setData(Data* data);

    //Returns error message
    virtual QString error();
signals:

public slots:

private:
    Ui::GPIBDeviceView* ui;
    QString error_message;
};

#endif // GPIBDEVICEVIEW_H
