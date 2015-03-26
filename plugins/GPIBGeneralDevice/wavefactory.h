#ifndef WAVEFACTORY_H
#define WAVEFACTORY_H

#include <QWidget>
#include "plugins.h"

namespace Ui {
class WaveFactory;
}

class WaveFactory : public QWidget, public Device
{
    Q_OBJECT
    Q_INTERFACES(Device)
public:
    WaveFactory(QWidget* parent=0);
    virtual QStringList devices();
    virtual QWidget * controlPane();

    // Returns non zero value if errors occur(For example: Faild to read device)
    virtual int loadData(Data* data);

    //Returns error message
    virtual QString error();

    virtual QString preferedView();
    virtual QString deviceClass();

    ~WaveFactory();
    void setSource(Data* data,QString src);
    void setSourceON(Data* data,bool on);
    void setMode(Data* data,QString wvmode);
    void setDC(Data* data, double dc);
private slots:
    void on_modeBox_currentIndexChanged(const QString &arg1);

    void on_sourceON_clicked();

    void on_dcdoubleSpinBox_valueChanged(double arg1);

    void on_ampdoubleSpinBox_valueChanged(double arg1);

private:
    Ui::WaveFactory *ui;
    QString error_message;
    Data * data;
};

#endif // WAVEFACTORY_H
