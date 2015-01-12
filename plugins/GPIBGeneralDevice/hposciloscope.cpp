
#include "ui_OsciloscopeControlPane.h"
#include "hposciloscope.h"
#include "gpibbus.h"

HPOsciloscope::HPOsciloscope(QWidget *parent):QWidget(parent),ui(new Ui::OsciloscopeControlPane)
{
    ui->setupUi(this);
}

QStringList HPOsciloscope::devices()
{
    return GPIBBus::instance()->findDev("HEWLETT-PACKARD,548");
}

QWidget *HPOsciloscope::controlPane()
{
    return this;
}

int HPOsciloscope::loadData(Data *data){
    this->data = data;
    QVariant tmp = "Voltage (V)";

    data->setParameter("view_0_y", tmp);
    tmp = "Time(s)";
    data->setParameter("view_0_x",tmp);
    tmp = qVariantFromValue(QColor(Qt::black));
    data->setParameter("view.CHANNEL1.color",tmp);
    tmp = qVariantFromValue(QColor(Qt::red));
    data->setParameter("view.CHANNEL2.color",tmp);
    tmp = qVariantFromValue(QColor(Qt::blue));
    data->setParameter("view.CHANNEL3.color",tmp);
    tmp = qVariantFromValue(QColor(Qt::green));
    data->setParameter("view.CHANNEL4.color",tmp);
    ui->label->setText("Ready");
    //ui->progressBar->setVisible(false);

    ui->checkBox->setChecked(data->parameter("CH1_checked").isValid()&&data->parameter("CH1_checked").toBool());
    ui->checkBox_2->setChecked(data->parameter("CH2_checked").isValid()&&data->parameter("CH2_checked").toBool());
    ui->checkBox_3->setChecked(data->parameter("CH3_checked").isValid()&&data->parameter("CH3_checked").toBool());
    ui->checkBox_4->setChecked(data->parameter("CH4_checked").isValid()&&data->parameter("CH4_checked").toBool());
    return 0;
}

QString HPOsciloscope::error()
{
    return error_message;
}

QString HPOsciloscope::preferedView()
{
    return "Simple View";
}

QString HPOsciloscope::deviceClass()
{
    return "Osciloscope HP 548xx";
}

void HPOsciloscope::on_pushButton_clicked(){
    //Read data

    unsigned long int retCount;
    QCheckBox * boxes[4] = {ui->checkBox,ui->checkBox_2,ui->checkBox_3,ui->checkBox_4};
    QVariant vi = data->parameter("gpib_vi");
    GPIBBus *bus = GPIBBus::instance();
    QVariant devId = data->parameter("deviceID");
    vi =bus->openDev(devId);
    data->setParameter("gpib_vi",vi);
    QString err = bus->gpib_query(vi,":SYSTEM:ERROR? STRING\n");
    bus->gpib_command(vi,"*CLS\n");
    qDebug(err.toLocal8Bit().data());
    Data2D* data = (Data2D*)this->data;
    data->startEdit();
    //ui->progressBar->setVisible(true);
    for(int i = 1; i<=4;i++){
        QString chstring =QString("CHANNEL%1").arg(i);
        if(boxes[i-1]->isChecked()){
            ui->label->setText("Reading "+chstring+" ...");
            ui->progressBar->setValue(0);
            bus->gpib_command(vi,":WAVEFORM:SOURCE "+chstring+"\n");
            bus->gpib_command(vi,":WAVeform:BYT LSBF\n");
            bus->gpib_command(vi,":WAVEFORM:FORMAT WORD\n");
            bus->gpib_command(vi,"::ACQuire:POINts:AUTO ON\n");
            QString res =bus->gpib_query(vi,":WAVEFORM:PREamble?\n");
            QStringList wfpreamp = res.split(",");
            if(wfpreamp.size()<25){
                char tmp[1000];
                char * buff = tmp;
                unsigned long ret;
                bus->readBlock(vi,buff,1000,&ret);
                QString add = buff;
                res.append(add);
                wfpreamp = res.split(",");
                qDebug(res.toLocal8Bit().data());
                if(wfpreamp.size()<25){
                    ui->label->setText("Failed to read data from the device");
                    break;
                }
            }
            if(wfpreamp.size()>5){
                ui->progressBar->setValue(25);
                //boxes[i-1]->setText(wfpreamp[6]);
                double xinc,xorig,xref, yinc,yorig,yref;
                xinc = wfpreamp[4].toDouble();
                yinc =wfpreamp[7].toDouble();
                xorig = wfpreamp[5].toDouble();
                yorig =wfpreamp[8].toDouble();
                xref = wfpreamp[6].toDouble();
                yref =wfpreamp[9].toDouble();
                unsigned int points = wfpreamp[2].toInt();
                char databuff_[6+points*2];

                ui->progressBar->setValue(50);
                bus->queryBlock(vi, ":WAVEFORM:DATA?\n",databuff_,6+points*2,&retCount);
                short * tmpdata=(short*)((char*)databuff_ + 6);
                for(int j=0; j<points;j++){
                    data->addPoint(chstring,(j*1.0-xref)*xinc+xorig,(tmpdata[j]*1.0-yref)*yinc +yorig); //TODO::ADDconsts
                }
            }else{
                boxes[i-1]->setText(chstring);
            }
            ui->progressBar->setValue(100);
        }else{
            boxes[i-1]->setText(chstring);
        }
         ui->label->setText("Done");
    }
    data->stopEdit();
    bus->closeDev(vi);
}

void HPOsciloscope::on_checkBox_stateChanged(int arg1){
    if(data!=NULL){
        data->setParameter("CH1_checked",ui->checkBox->isChecked());
    }
}

void HPOsciloscope::on_checkBox_2_stateChanged(int arg1){
    if(data!=NULL){
        data->setParameter("CH2_checked",ui->checkBox_2->isChecked());
    }
}

void HPOsciloscope::on_checkBox_3_stateChanged(int arg1){
    if(data!=NULL){
        data->setParameter("CH3_checked",ui->checkBox_3->isChecked());
    }
}

void HPOsciloscope::on_checkBox_4_stateChanged(int arg1){
    if(data!=NULL){
        data->setParameter("CH4_checked",ui->checkBox_4->isChecked());
    }
}
