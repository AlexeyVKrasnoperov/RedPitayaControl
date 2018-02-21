#include "generatorgroupbox.h"
#include "ui_generatorgroupbox.h"
#include "rpclient.h"

GeneratorGroupBox::GeneratorGroupBox(QWidget *parent) :
    QGroupBox(parent),
    ui(new Ui::GeneratorGroupBox)
{
    ui->setupUi(this);
    rpClient = 0;
    setRPClient(0);
}

GeneratorGroupBox::~GeneratorGroupBox()
{
    delete ui;
}

void GeneratorGroupBox::setRPClient(RPClient *client)
{
    rpClient = client;
    setEnabled(rpClient != 0);
    if( rpClient == 0 )
    {
        ui->groupBoxChannelSettings->setChecked(false);
        return;
    }
    rpClient->resetGenerator();
    int idx = ui->comboBoxChannel->currentIndex();
    getChannelSettings(idx+1);
}

void GeneratorGroupBox::getChannelSettings(int channel)
{
    if( rpClient == 0 )
        return;
    unsigned short state = false;
    if( rpClient->getGeneratorChannelState(channel,state) )
        ui->checkBoxChannelEnabled->setChecked(state!=0);
    QString func;
    if( rpClient->getGeneratorFunction(channel,func) )
    {
        qDebug() << " Read func" << func;
        //        int idx = ui->comboBoxForm->findText(func);
        //        if( idx != -1 )
        QSignalBlocker b(ui->comboBoxForm);
        ui->comboBoxForm->setCurrentText(func);
    }
}

void GeneratorGroupBox::on_comboBoxChannel_currentIndexChanged(int index)
{
    getChannelSettings(index+1);
}

void GeneratorGroupBox::on_checkBoxChannelEnabled_clicked(bool checked)
{
    if( rpClient == 0 )
        return;
    int idx = ui->comboBoxChannel->currentIndex();
    rpClient->setGeneratorChannelState(idx+1,checked);
}

void GeneratorGroupBox::on_comboBoxForm_currentIndexChanged(const QString &arg1)
{
    if( rpClient == 0 )
        return;
    qDebug() << "Write func" << arg1;
    int idx = ui->comboBoxChannel->currentIndex();
    rpClient->setGeneratorFunction(idx+1,arg1);
}

void GeneratorGroupBox::on_spinBoxFreq_valueChanged(int arg1)
{
 if( rpClient == 0 )
        return;
    int idx = ui->comboBoxChannel->currentIndex();
    QString s = QString::number(arg1);
    rpClient->setGeneratorFrequency(idx+1,s);

  //  qDebug() << arg1;
}

void GeneratorGroupBox::on_doubleSpinBoxAmpl_valueChanged(double arg1)
{
    if( rpClient == 0 )
           return;
       int idx = ui->comboBoxChannel->currentIndex();
       QString s = QString::number(arg1);
       rpClient->setGeneratorAmplitude(idx+1,s);
}

void GeneratorGroupBox::on_spinBoxPhase_valueChanged(int arg1)
{
    if( rpClient == 0 )
           return;
       int idx = ui->comboBoxChannel->currentIndex();
       QString s = QString::number(arg1);
       rpClient->setGeneratorPhase(idx+1,s);
}


void GeneratorGroupBox::on_spinBoxDutyCycle_valueChanged(int arg1)
{
    if( rpClient == 0 )
           return;
       int idx = ui->comboBoxChannel->currentIndex();
       QString s = QString::number(arg1);
       rpClient->setGeneratorDutyCycle(idx+1,s);
}
