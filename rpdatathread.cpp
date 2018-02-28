#include "rpdatathread.h"
#include "rpclient.h"
#include <QString>
#include <QStringList>

RPDataThread::RPDataThread(RPClient *_client, QObject *parent):QThread(parent),client(_client)
{
    breakCycle = false;
    connect(client,SIGNAL(readyRead()),this,SLOT(readSlot()));
    connect(this,SIGNAL(askTriggerStatusSignal()),client,SLOT(askTriggerStatus()));
    connect(this,SIGNAL(writeCommandSignal(QString)),client,SLOT(writeCommand(QString)));
    connect(this,SIGNAL(updateChannelData(int,QVector<double>)),client,SIGNAL(updateChannelData(int,QVector<double>)));
}

RPDataThread::~RPDataThread()
{
    stop();
}

void RPDataThread::start(void)
{
    array.clear();
    QThread::start();
}


void RPDataThread::stop(void)
{
    if( isRunning() )
    {
        breakCycle = true;
        wait();
    }
}

void RPDataThread::readSlot(void)
{
    array += client->readAll();
}
void RPDataThread::run(void)
{
    breakCycle = false;
    array.clear();
    int lastCmd = -1;
    emit askTriggerStatusSignal();
    lastCmd = 0;
    while( !breakCycle )
    {
        if( ! array.endsWith("\r\n") )
        {
            usleep(1);
            continue;
        }
        if( (lastCmd == 0) && array.contains("TD") )
        {
            array.clear();
            lastCmd = 1;
            emit writeCommandSignal("ACQ:SOUR1:DATA?");
            continue;
        }
        //
        if( lastCmd == 1 )
        {
            if( array.startsWith("{") && array.endsWith("}\r\n"))
            {
                FillDataArray(1,array);
                qDebug() << array.size() << " OK " << lastCmd;
                array.clear();
                lastCmd = 2;
                emit updateChannelData(1,channelData[0]);
                emit writeCommandSignal("ACQ:SOUR2:DATA?");
                continue;
            }
        }
        //
        if( lastCmd == 2 )
        {
            if( array.startsWith("{") && array.endsWith("}\r\n"))
            {
                FillDataArray(2,array);
                qDebug() << array.size() << " OK " << lastCmd;
                array.clear();
                emit updateChannelData(2,channelData[1]);
            }
        }
        //
        lastCmd = 0;
        emit askTriggerStatusSignal();
    }
}

int RPDataThread::FillDataArray(int channel, QByteArray & a)
{
    if( a.startsWith("{") )
        a.remove(1,1);
    if( a.endsWith("}\r\n") )
        a.remove(a.length()-3,3);
    QList<QByteArray> l = a.split(',');
    if( channelData[channel-1].size() != l.size() )
        channelData[channel-1].resize(l.size());
    for(int i = 0; i < l.size(); i++)
        channelData[channel-1][i] = l[i].toDouble();
    return channelData[channel-1].size();
}
