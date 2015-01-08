
#include "ui_OsciloscopeControlPane.h"
#include "osciloscope.h"
#include "gpibbus.h"

Osciloscope::Osciloscope(QWidget *parent):QWidget(parent),ui(new Ui::OsciloscopeControlPane)
{
    ui->setupUi(this);
}

QStringList Osciloscope::devices()
{
    return GPIBBus::instance()->findDev("TEKTRONIX");
}

QWidget *Osciloscope::controlPane()
{
    return this;
}

int Osciloscope::loadData(Data *data){
    this->data = data;
    QVariant tmp = "Voltage (V)";

    data->setParameter("view_0_y", tmp);
    tmp = "Time(s)";
    data->setParameter("view_0_x",tmp);
    tmp = qVariantFromValue(QColor(Qt::black));
    data->setParameter("view.CH1.color",tmp);
    tmp = qVariantFromValue(QColor(Qt::red));
    data->setParameter("view.CH2.color",tmp);
    tmp = qVariantFromValue(QColor(Qt::blue));
    data->setParameter("view.CH3.color",tmp);
    tmp = qVariantFromValue(QColor(Qt::green));
    data->setParameter("view.CH4.color",tmp);
    ui->label->setText("Ready");
    //ui->progressBar->setVisible(false);

    ui->checkBox->setChecked(data->parameter("CH1_checked").isValid()&&data->parameter("CH1_checked").toBool());
    ui->checkBox_2->setChecked(data->parameter("CH2_checked").isValid()&&data->parameter("CH2_checked").toBool());
    ui->checkBox_3->setChecked(data->parameter("CH3_checked").isValid()&&data->parameter("CH3_checked").toBool());
    ui->checkBox_4->setChecked(data->parameter("CH4_checked").isValid()&&data->parameter("CH4_checked").toBool());
    return 0;
}

QString Osciloscope::error()
{
    return error_message;
}

QString Osciloscope::preferedView()
{
    return "Simple View";
}

QString Osciloscope::deviceClass()
{
    return "Osciloscope TDS 2xx";
}

void Osciloscope::on_pushButton_clicked(){
    //Read data
    unsigned long int retCount;
    QCheckBox * boxes[4] = {ui->checkBox,ui->checkBox_2,ui->checkBox_3,ui->checkBox_4};
    QVariant vi = data->parameter("gpib_vi");
    GPIBBus *bus = GPIBBus::instance();
    if(vi.isNull()){
        QVariant devId = data->parameter("deviceID");
        vi =bus->openDev(devId);
        data->setParameter("gpib_vi",vi);
    }
    Data2D* data = (Data2D*)this->data;
    data->startEdit();
    //ui->progressBar->setVisible(true);
    for(int i = 1; i<=4;i++){
        QString chstring =QString("CH%1").arg(i);
        if(boxes[i-1]->isChecked()){
            ui->label->setText("Reading "+chstring+" ...");
            ui->progressBar->setValue(0);
            bus->gpib_command(vi,"DATa:SOUrce "+chstring+"\n");
            bus->gpib_command(vi, "DATa:ENCdg RIBinary\n");
            bus->gpib_command(vi, "DATa:WIDth 1\n");
            bus->gpib_command(vi, "DATa:STARt 1\n");
            bus->gpib_command(vi, "DATa:STOP 2500\n");
            QString res =bus->gpib_query(vi,"WFMPre?\n");
            QStringList wfpreamp = res.split(";");
            if(wfpreamp.size()>5 &&wfpreamp.size()<16){
                char tmp[1000];
                char * buff = tmp;
                unsigned long ret;
                bus->readBlock(vi,buff,1000,&ret);
                QString add = buff;
                res.append(add);
                wfpreamp = res.split(";");
                qDebug(res.toLocal8Bit().data());
                if(wfpreamp.size()<16){
                    ui->label->setText("Failed to read data from the device, try to restart the program and the device");
                    return;
                }
            }
            if(wfpreamp.size()>5){
                ui->progressBar->setValue(25);
                boxes[i-1]->setText(wfpreamp[6]);
                double xmult, ymult,offset;
                xmult = wfpreamp[8].toDouble();
                ymult =wfpreamp[12].toDouble();
                offset = wfpreamp[14].toDouble();
                char databuff_[2507];
                ui->progressBar->setValue(50);
                bus->queryBlock(vi, "Curve?\n",databuff_,2507,&retCount);
                for(int j=6; j<2506;j++){
                    data->addPoint(chstring,j*1.0*xmult,(databuff_[j]-offset)*ymult); //TODO::ADDconsts
                }
            }else{
                boxes[i-1]->setText(chstring);
            }
            ui->progressBar->setValue(100);
        }else{
            boxes[i-1]->setText(chstring);
        }
    }
  //  ui->progressBar->setVisible(false);
    ui->label->setText("Done");
    data->stopEdit();
}

void Osciloscope::on_checkBox_stateChanged(int arg1){
    if(data!=NULL){
        data->setParameter("CH1_checked",ui->checkBox->isChecked());
    }
}

void Osciloscope::on_checkBox_2_stateChanged(int arg1){
    if(data!=NULL){
        data->setParameter("CH2_checked",ui->checkBox_2->isChecked());
    }
}

void Osciloscope::on_checkBox_3_stateChanged(int arg1){
    if(data!=NULL){
        data->setParameter("CH3_checked",ui->checkBox_3->isChecked());
    }
}

void Osciloscope::on_checkBox_4_stateChanged(int arg1){
    if(data!=NULL){
        data->setParameter("CH4_checked",ui->checkBox_4->isChecked());
    }
}
