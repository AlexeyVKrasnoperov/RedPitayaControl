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
    timer.setSingleShot(true);
    timer.setInterval(50);
    connect(&timer,SIGNAL(timeout()),this,SLOT(timeoutSlot()));
    //
    for( int i = 0; i < 2; i++)
    {
        channelControl.addButton(new QCheckBox(tr("ch %1").arg(i+1),this),i+1);
        ui->verticalLayoutControl->insertWidget(i,channelControl.button(i+1));
    }
    channelControl.setExclusive(false);
    connect(&channelControl,SIGNAL(buttonClicked(int)),SLOT(channelControlClicked(int)));
    //
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
        connect(rpClient,SIGNAL(updateChannelData(int,QVector<double>)),this,SLOT(updateChannelDataSlot(int,QVector<double>)));
    }
}

void OscillographGroupBox::on_pushButtonAcq_clicked(bool checked)
{
    if( rpClient == 0 )
        return;
    if( checked )
    {
        // timer.start();
        ui->pushButtonAcq->setIcon(QIcon(":/Icons/CrystalClear/media-playback-stop-7.png"));
        rpClient->startAcquisition();
    }
    else
    {
        ui->pushButtonAcq->setIcon(QIcon(":/Icons/CrystalClear/media-playback-start-7.png"));
        rpClient->stopAcquisition();
    }
}

void OscillographGroupBox::timeoutSlot()
{
    const int sz = 512;
    static QVector<double> xData;
    static QVector<double> yData[2];
    if( xData.size() < sz )
    {
        xData.resize(sz);
        for(int i = 0; i < sz; i++)
            xData[i] = i*16;
    }
    for(int i = 0; i < 2; i++)
    {
        if( yData[i].size() < sz )
            yData[i].resize(sz);
        for(int j = 0; j < sz; j++)
            yData[i][j] = qrand()%100;
        QAbstractButton *b = channelControl.button(i+1);
        if( b == 0 )
            continue;
        if( ! b->isChecked() )
            continue;
        viewer->setChannelData(i+1,xData,yData[i]);
    }
    viewer->replot();
    if( ui->pushButtonAcq->isChecked() )
        timer.start();
}

void OscillographGroupBox::channelControlClicked(int id)
{
    QAbstractButton *b = channelControl.button(id);
    if( b == 0 )
        return;
    bool checked = b->isChecked();
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
    viewer->replot();
}
