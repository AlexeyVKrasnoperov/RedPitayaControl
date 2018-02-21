#include "oscillographgroupbox.h"
#include "ui_oscillographgroupbox.h"
#include "channelviewer.h"


OscillographGroupBox::OscillographGroupBox(QWidget *parent) :
    QGroupBox(parent),
    ui(new Ui::OscillographGroupBox)
{
    ui->setupUi(this);
    viewer = new ChannelViewer(this);
    ui->horizontalLayoutMain->insertWidget(0,viewer);
    setRPClient(0);
    timer.setSingleShot(true);
    timer.setInterval(50);
    connect(&timer,SIGNAL(timeout()),this,SLOT(timeoutSlot()));
}

OscillographGroupBox::~OscillographGroupBox()
{
    delete ui;
}

void OscillographGroupBox::setRPClient(RPClient *client)
{
    rpClient = client;
    //    setEnabled(rpClient != 0);
}

void OscillographGroupBox::on_pushButtonAcq_clicked(bool checked)
{
    QString path;
    if( checked )
    {
        timer.start();
        path = ":/Icons/CrystalClear/media-playback-stop-7.png";
    }
    else
        path = ":/Icons/CrystalClear/media-playback-start-7.png";
    //
    ui->pushButtonAcq->setIcon(QIcon(path));
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
        viewer->setChannelData(i+1,xData,yData[i]);
    }
    viewer->replot();
    if( ui->pushButtonAcq->isChecked() )
        timer.start();
}
