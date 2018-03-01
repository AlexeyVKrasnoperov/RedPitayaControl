#include "oscillographgroupbox.h"
#include "ui_oscillographgroupbox.h"
#include "channelviewer.h"
#include "rpclient.h"
#include <QCheckBox>

OscillographGroupBox::OscillographGroupBox(QWidget *parent) :
    QGroupBox(parent),
    ui(new Ui::OscillographGroupBox)
{
    ui->setupUi(this);
    viewer = new ChannelViewer(this);
    ui->horizontalLayout->insertWidget(0,viewer);
    setRPClient(0);
    for( int i = 0; i < 2; i++)
    {
        channelControl.addButton(new QCheckBox(tr("ch %1").arg(i+1),this),i+1);
        ui->verticalLayoutControl->insertWidget(i,channelControl.button(i+1));
    }
    channelControl.setExclusive(false);
    connect(&channelControl,SIGNAL(buttonClicked(int)),SLOT(channelControlClicked(int)));
}

OscillographGroupBox::~OscillographGroupBox()
{
    delete ui;
}

void OscillographGroupBox::setRPClient(RPClient *client)
{
    rpClient = client;
    setEnabled(rpClient != 0);
    //
    if( rpClient != 0 )
    {
        rpClient->resetOscilloscope();
        rpClient->setTriggerSource("CH1_PE");
        for(int channel = 1; channel <= 2; channel++)
        {
            QAbstractButton *b = channelControl.button(channel);
            if( b == 0 )
                continue;
            b->setChecked(rpClient->channelEnabled(channel));
        }
        rpClient->setTriggerDelay(0);
        rpClient->setTriggerDecimation(65536);
        rpClient->setTriggerGain(1,0);
        connect(rpClient,SIGNAL(updateChannelData(int,QVector<double>)),this,SLOT(updateChannelDataSlot(int,QVector<double>)));
        connect(rpClient,SIGNAL(replotSignal()),viewer,SLOT(replot()));
    }
}

void OscillographGroupBox::on_pushButtonAcq_clicked(bool checked)
{
    if( rpClient == 0 )
        return;
    if( checked )
    {
        ui->pushButtonAcq->setIcon(QIcon(":/Icons/CrystalClear/media-playback-stop-7.png"));
        rpClient->startAcquisition();
    }
    else
    {
        ui->pushButtonAcq->setIcon(QIcon(":/Icons/CrystalClear/media-playback-start-7.png"));
        rpClient->stopAcquisition();
    }
}

void OscillographGroupBox::channelControlClicked(int id)
{
    QAbstractButton *b = channelControl.button(id);
    if( b == 0 )
        return;
    bool checked = b->isChecked();
    if( rpClient )
        rpClient->enableChannel(id,checked);
    viewer->showChannelCurve(id,checked);
}

void OscillographGroupBox::updateChannelDataSlot(int channel, const QVector<double> & data)
{
    QAbstractButton *b = channelControl.button(channel);
    if( b == 0 )
        return;
    if( ! b->isChecked() )
        return;
    static QVector<double> xData;
    if( xData.size() < data.size() )
    {
        xData.resize(data.size());
        for(int i = 0; i < data.size(); i++)
            xData[i] = i;
    }
    viewer->setChannelData(channel,xData,data);
    //viewer->replot();
}
