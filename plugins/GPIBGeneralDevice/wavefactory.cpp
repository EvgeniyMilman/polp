#include "wavefactory.h"
#include "ui_wavefactory.h"

#include "gpibbus.h"

WaveFactory::WaveFactory(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WaveFactory)
{
    ui->setupUi(this);
}

QStringList WaveFactory::devices()
{
return GPIBBus::instance()->findDev("NF-ELECTRONIC-INSTRUMENTS");
}

QWidget *WaveFactory::controlPane()
{
    return this;
}

int WaveFactory::loadData(Data *data)
{
    this->data = data;
    data->setParameter("wf_source", ui->sourceBox->currentText());
    setSource(data,ui->sourceBox->currentText());
 return 0;
}

QString WaveFactory::error()
{
    return error_message;
}

QString WaveFactory::preferedView()
{
    return "SimpleView";
}

QString WaveFactory::deviceClass()
{
    return "Wave Factory";
}

WaveFactory::~WaveFactory()
{
    delete ui;
}

void WaveFactory::setSource(Data *data, QString src)
{
    QVariant vi;
    GPIBBus *bus = GPIBBus::instance();
    QVariant devId = data->parameter("deviceID");
    if(devId.isNull()){
        error_message = "Wrong data object";
        return;
    }
    vi =bus->openDev(devId);
    bus->gpib_command(vi,":*CLS\n");
    bus->gpib_command(vi,":CHAN "+src+"\n");
    bus->closeDev(vi);
}

void WaveFactory::setSourceON(Data *data, bool on)
{
    QVariant vi;
    GPIBBus *bus = GPIBBus::instance();
    QVariant devId = data->parameter("deviceID");
    QString output = data->parameter("wf_source").toString();
    if(devId.isNull()||output.isNull()||output.isEmpty()){
        error_message = "Wrong data object";
        return;
    }
    vi =bus->openDev(devId);
    QString ON = (on)?"1":"0";
    bus->gpib_command(vi,":*CLS\n");
    bus->gpib_command(vi,":OUTP:STAT "+ON+"\n");
    bus->closeDev(vi);
}

void WaveFactory::setMode(Data *data, QString wvmode)
{
    QVariant vi;
    GPIBBus *bus = GPIBBus::instance();
    QVariant devId = data->parameter("deviceID");
    QString output = data->parameter("wf_source").toString();
    if(devId.isNull()||output.isNull()||output.isEmpty()){
        error_message = "Wrong data object";
        return;
    }
    vi =bus->openDev(devId);
    bus->gpib_command(vi,":*CLS\n");
    bus->gpib_command(vi,":MODE "+wvmode+"\n");
    bus->closeDev(vi);
}

void WaveFactory::setDC(Data *data, double dc){
    QVariant vi;
    GPIBBus *bus = GPIBBus::instance();
    QVariant devId = data->parameter("deviceID");
    QString output = data->parameter("wf_source").toString();
    if(devId.isNull()||output.isNull()||output.isEmpty()){
        error_message = "Wrong data object";
        return;
    }
    vi =bus->openDev(devId);
    bus->gpib_command(vi,":*CLS\n");
    bus->gpib_command(vi,":VOLT "+QString::number(dc)+"\n");
    bus->closeDev(vi);
}

void WaveFactory::on_modeBox_currentIndexChanged(const QString &arg1){
    if(arg1 == "DC"){
        ui->dcdoubleSpinBox->setVisible(true);
    }else{
        ui->dcdoubleSpinBox->setVisible(false);
    }
    setMode(data,ui->modeBox->currentText());
}

void WaveFactory::on_sourceON_clicked()
{
    setSourceON(data,ui->sourceON->isChecked());
}

void WaveFactory::on_dcdoubleSpinBox_valueChanged(double arg1)
{
    setDC(data,arg1);
}
