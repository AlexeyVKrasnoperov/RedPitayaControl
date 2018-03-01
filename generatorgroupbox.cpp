#include "generatorgroupbox.h"
#include "ui_generatorgroupbox.h"
#include "rpclient.h"
#include <QStringList>

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
    connect(rpClient,SIGNAL(responseSignal(QString,QByteArray)),this,SLOT(responseSlot(QString,QByteArray)));
    int idx = ui->comboBoxChannel->currentIndex();
    getChannelSettings(idx+1);
}

void GeneratorGroupBox::responseSlot(const QString & cmd, const QByteArray & response)
{
    QStringList cmdList = cmd.split(":");
    if( cmdList.size() < 2 )
        return;
    if( ! cmdList.front().startsWith("OUTPUT") && ! cmdList.front().startsWith("SOUR") )
        return;
    qDebug() << cmd << response;
    bool ok = false;
    int ch = cmdList.front().right(1).toInt(&ok);
    if( ! ok )
        return;
    if( ch != (ui->comboBoxChannel->currentIndex() + 1) )
        return;
    if( cmdList[1] == "STATE?" )
    {
        bool enabled = (response.toInt(&ok)==1);
        if( ok )
            ui->checkBoxChannelEnabled->setChecked(enabled);
    }
    else if( cmdList[1] == "FUNC?" )
    {
        int idx = ui->comboBoxForm->findText(response);
        if( idx != -1 )
        {
            QSignalBlocker b(ui->comboBoxForm);
            ui->comboBoxForm->setCurrentIndex(idx);
        }
    }
    else if( (cmdList.size()==3) && (cmdList[1] == "FREQ") &&  (cmdList[2] == "FIX?") ) // "SOUR%d:FREQ:FIX?"
    {
        int freq = response.toInt(&ok);
        if( ok )
        {
            QSignalBlocker b(ui->spinBoxFreq);
            ui->spinBoxFreq->setValue(freq);
        }
    }
    else if( cmdList[1] == "PHAS?" )
    {
        int phas = response.toInt(&ok);
        if( ok )
        {
            QSignalBlocker b(ui->spinBoxPhase);
            ui->spinBoxPhase->setValue(phas);
        }
    }
    else if( cmdList[1] == "DCYC?" )
    {
        double d = response.toDouble(&ok);
        if( ok )
        {
            QSignalBlocker b(ui->spinBoxDutyCycle);
            ui->spinBoxDutyCycle->setValue(100*d);
        }
    }
    else if( cmdList[1] == "VOLT?" )
    {
        double d = response.toDouble(&ok);
        if( ok )
        {
            QSignalBlocker b(ui->doubleSpinBoxAmpl);
            ui->doubleSpinBoxAmpl->setValue(d);
        }
    }
}


void GeneratorGroupBox::getChannelSettings(int channel)
{
    if( rpClient == 0 )
        return;
    if( ! rpClient->getGeneratorChannelState(channel) )
        return;
    if( ! rpClient->getGeneratorFunction(channel) )
        return;
    if( ! rpClient->getGeneratorFrequency(channel) )
        return;
    if( ! rpClient->getGeneratorDutyCycle(channel) )
        return;
    if( ! rpClient->getGeneratorAmplitude(channel) )
        return;
    if( ! rpClient->getGeneratorPhase(channel) )
        return;
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
    int idx = ui->comboBoxChannel->currentIndex();
    rpClient->setGeneratorFunction(idx+1,arg1);
}

void GeneratorGroupBox::on_spinBoxFreq_valueChanged(int arg1)
{
    if( rpClient == 0 )
        return;
    int idx = ui->comboBoxChannel->currentIndex();
    rpClient->setGeneratorFrequency(idx+1,QString::number(arg1));
}

void GeneratorGroupBox::on_doubleSpinBoxAmpl_valueChanged(double arg1)
{
    if( rpClient == 0 )
        return;
    int idx = ui->comboBoxChannel->currentIndex();
    rpClient->setGeneratorAmplitude(idx+1,QString::number(arg1));
}

void GeneratorGroupBox::on_spinBoxPhase_valueChanged(int arg1)
{
    if( rpClient == 0 )
        return;
    int idx = ui->comboBoxChannel->currentIndex();
    rpClient->setGeneratorPhase(idx+1,QString::number(arg1));
}


void GeneratorGroupBox::on_spinBoxDutyCycle_valueChanged(int arg1)
{
    if( rpClient == 0 )
        return;
    int idx = ui->comboBoxChannel->currentIndex();
    rpClient->setGeneratorDutyCycle(idx+1,QString::number(0.01*arg1));
}
