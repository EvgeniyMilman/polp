#ifndef HPOSCILOSCOPE_H
#define HPOSCILOSCOPE_H

#include "gpibgeneraldevice_global.h"
#include <QObject>
#include "plugins.h"

namespace Ui {
    class OsciloscopeControlPane;
}
class HPOsciloscope:public QWidget, public Device
{
    Q_OBJECT
    Q_INTERFACES(Device)
public:
    HPOsciloscope(QWidget* parent=0);
    virtual QStringList devices();
    virtual QWidget * controlPane();

    // Returns non zero value if errors occur(For example: Faild to read device)
    virtual int loadData(Data* data);

    //Returns error message
    virtual QString error();

    virtual QString preferedView();
    virtual QString deviceClass();
    void readData(Data* data);

signals:
   void labeNewText(QString);
   void newProgress(int);

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

    void on_labeNewText(QString text);
    void on_newProgress(int pr);



private:
    QString error_message;
    Data * data;
    Ui::OsciloscopeControlPane* ui;
};

#endif // HPOSCILOSCOPE_H
