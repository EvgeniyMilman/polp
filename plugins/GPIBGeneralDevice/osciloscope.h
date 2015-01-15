#ifndef OSCILOSCOPE_H
#define OSCILOSCOPE_H

#include "gpibgeneraldevice_global.h"
#include <QObject>
#include "plugins.h"

namespace Ui {
    class OsciloscopeControlPane;
}
class Osciloscope: public QWidget, public Device
{
    Q_OBJECT
    Q_INTERFACES(Device)
public:
    Osciloscope(QWidget* parent=0);
    virtual QStringList devices();
    virtual QWidget * controlPane();

    // Returns non zero value if errors occur(For example: Faild to read device)
    virtual int loadData(Data* data);

    //Returns error message
    virtual QString error();

    virtual QString preferedView();
    virtual QString deviceClass();
private slots:
    void on_pushButton_clicked();
    void on_checkBox_stateChanged(int arg1);

    void on_checkBox_2_stateChanged(int arg1);

    void on_checkBox_3_stateChanged(int arg1);

    void on_checkBox_4_stateChanged(int arg1);

    void on_func1checkBox_stateChanged(int arg1);
    void on_func2checkBox_stateChanged(int arg1);
    void on_func3checkBox_stateChanged(int arg1);
    void on_func4checkBox_stateChanged(int arg1);

private:
    QString error_message;
    Data * data;
    Ui::OsciloscopeControlPane* ui;
};


#endif // OSCILOSCOPE_H
