
#include "ui_OsciloscopeControlPane.h"
#include "hposciloscope.h"
#include "gpibbus.h"

HPOsciloscope::HPOsciloscope(QWidget *parent):QWidget(parent),ui(new Ui::OsciloscopeControlPane)
{
    ui->setupUi(this);
    connect(this,SIGNAL(labeNewText(QString)),this,SLOT(on_labeNewText(QString)));
    connect(this,SIGNAL(newProgress(int)),this,SLOT(on_newProgress(int)));
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
    tmp = qVariantFromValue(QColor(Qt::magenta));
    data->setParameter("view.FUNCTION1.color",tmp);
    tmp = qVariantFromValue(QColor(Qt::darkYellow));
    data->setParameter("view.FUNCTION2.color",tmp);
    tmp = qVariantFromValue(QColor(Qt::darkGray));
    data->setParameter("view.FUNCTION3.color",tmp);
    tmp = qVariantFromValue(QColor(Qt::darkCyan));
    data->setParameter("view.FUNCTION4.color",tmp);

    ui->label->setText("Ready");
    //ui->progressBar->setVisible(false);

    ui->checkBox->setChecked(data->parameter("CH1_checked").isValid()&&data->parameter("CH1_checked").toBool());
    ui->checkBox_2->setChecked(data->parameter("CH2_checked").isValid()&&data->parameter("CH2_checked").toBool());
    ui->checkBox_3->setChecked(data->parameter("CH3_checked").isValid()&&data->parameter("CH3_checked").toBool());
    ui->checkBox_4->setChecked(data->parameter("CH4_checked").isValid()&&data->parameter("CH4_checked").toBool());
    ui->func1checkBox->setChecked(data->parameter("FUNCTION1_checked").isValid()&&data->parameter("FUNCTION1_checked").toBool());
    ui->func2checkBox->setChecked(data->parameter("FUNCTION2_checked").isValid()&&data->parameter("FUNCTION2_checked").toBool());
    ui->func3checkBox->setChecked(data->parameter("FUNCTION3_checked").isValid()&&data->parameter("FUNCTION3_checked").toBool());
    ui->func4checkBox->setChecked(data->parameter("FUNCTION4_checked").isValid()&&data->parameter("FUNCTION4_checked").toBool());

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

void HPOsciloscope::readData(Data *_data){
    Data2D* data = (Data2D*)_data;
    unsigned long int retCount;
    bool boxes[8] = { data->parameter("CH1_checked").toBool(),
                     data->parameter("CH2_checked").toBool(),
                     data->parameter("CH3_checked").toBool(),
                     data->parameter("CH4_checked").toBool(),
                     data->parameter("FUNCTION1_checked").toBool(),
                     data->parameter("FUNCTION2_checked").toBool(),
                     data->parameter("FUNCTION3_checked").toBool(),
                     data->parameter("FUNCTION4_checked").toBool()};
    QStringList chstrings;
    chstrings << "CHANNEL1" <<"CHANNEL2" <<"CHANNEL3" <<"CHANNEL4" << "FUNCTION1"
                 << "FUNCTION2" <<"FUNCTION3" << "FUNCTION4";
    QVariant vi = data->parameter("gpib_vi");
    GPIBBus *bus = GPIBBus::instance();
    QVariant devId = data->parameter("deviceID");
    vi =bus->openDev(devId);
    data->setParameter("gpib_vi",vi);
    QString err = bus->gpib_query(vi,":SYSTEM:ERROR? STRING\n");
    bus->gpib_command(vi,"*CLS\n");
    qDebug(err.toLocal8Bit().data());
    data->startEdit();
    for(int i = 1; i<=8;i++){
        QString chstring =chstrings[i-1];
        if(boxes[i-1]){
            emit labeNewText("Reading "+chstring+" ...");
            emit newProgress(0);
            bus->gpib_command(vi,":WAVEFORM:SOURCE "+chstring+"\n");
            emit newProgress(10);
            bus->gpib_command(vi,":WAVeform:BYT LSBF\n");
            emit newProgress(20);
            bus->gpib_command(vi,":WAVEFORM:FORMAT WORD\n");
            emit newProgress(30);
            bus->gpib_command(vi,"::ACQuire:POINts:AUTO ON\n");
            emit newProgress(40);
            QString res =bus->gpib_query(vi,":WAVEFORM:PREamble?\n");
            emit newProgress(50);
            QStringList wfpreamp = res.split(",");
            if(wfpreamp.size()<25){
                emit newProgress(60);
                char tmp[1000];
                char * buff = tmp;
                unsigned long ret;
                bus->readBlock(vi,buff,1000,&ret);
                QString add = buff;
                res.append(add);
                wfpreamp = res.split(",");
                qDebug(res.toLocal8Bit().data());
                if(wfpreamp.size()<25){
                    emit labeNewText("Failed to read data from the device");
                    break;
                }
            }
            if(wfpreamp.size()>5){
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

               emit newProgress(70);
                bus->queryBlock(vi, ":WAVEFORM:DATA?\n",databuff_,6+points*2,&retCount);
                short * tmpdata=(short*)((char*)databuff_ + 6);
                for(int j=0; j<points;j++){
                    data->addPoint(chstring,(j*1.0-xref)*xinc+xorig,(tmpdata[j]*1.0-yref)*yinc +yorig); //TODO::ADDconsts
                }
            }else{
               // boxes[i-1]->setText(chstring);
            }
            emit newProgress(100);
        }else{
           // boxes[i-1]->setText(chstring);
        }
        emit labeNewText("Done");
    }
    data->stopEdit();
    bus->closeDev(vi);
}

void HPOsciloscope::on_pushButton_clicked(){
    readData(data);
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


void HPOsciloscope::on_func1checkBox_stateChanged(int arg1){
    if(data!=NULL){
        data->setParameter("FUNCTION1_checked",ui->func1checkBox->isChecked());
    }
}

void HPOsciloscope::on_func2checkBox_stateChanged(int arg1){
    if(data!=NULL){
        data->setParameter("FUNCTION2_checked",ui->func2checkBox->isChecked());
    }
}
void HPOsciloscope::on_func3checkBox_stateChanged(int arg1){
    if(data!=NULL){
        data->setParameter("FUNCTION3_checked",ui->func3checkBox->isChecked());
    }
}
void HPOsciloscope::on_func4checkBox_stateChanged(int arg1){
    if(data!=NULL){
        data->setParameter("FUNCTION4_checked",ui->func4checkBox->isChecked());
    }
}

void HPOsciloscope::on_labeNewText(QString text)
{
    ui->label->setText(text);
}

void HPOsciloscope::on_newProgress(int pr)
{
    ui->progressBar->setValue(pr);
}
