#include "rpclient.h"
#include <unistd.h>
#include <QDateTime>

RPClient::RPClient(const QString & name, QObject *parent):QTcpSocket(parent),
    hostname(name)
{
    channelMask = 3;
    acquisitionState = false;
    QObject::connect(this,SIGNAL(readyRead()),this,SLOT(readResponseSlot()));
}

RPClient::~RPClient()
{
    if( isOpen() )
    {
        resetGenerator();
        close();
    }
}

bool RPClient::connect(int timeout)
{
    connectToHost(hostname,5000);
    return waitForConnected(timeout);
}

void RPClient::readResponseSlot(void)
{
    responseBuffer += readAll();
    int idx = responseBuffer.indexOf("\r\n");
    while(idx != -1)
    {
        if( ! queryStack.empty() )
        {
            const QString & query = queryStack.front();
            QByteArray response = responseBuffer.left(idx);
            if( query == "ACQ:TRIG:STAT?" )
            {
                // qDebug() << QDateTime::currentDateTime() << response;
                if( response == "TD" )
                {
                    if( sendChannelDataRequest() == 0 )
                        getTriggerStatus();
                }
                else if( response == "WAIT" )
                    getTriggerStatus();
            }
            else if( query.startsWith("ACQ:SOUR") && query.endsWith(":DATA?") )
            {
                QStringList queryList = query.split(":");
                int ch = -1;
                if( queryList.size() == 3 )
                {
                    bool ok = false;
                    ch = queryList[1].right(1).toInt(&ok);
                    if( ok && ((ch == 1) || (ch == 2)) )
                    {
                        if( decodeChannelDataResponse(response,dataBuffer) > 0 )
                            emit updateChannelData(ch,dataBuffer);
                    }
                }
                if( queryStack.size() == 1 )
                {
                    getTriggerStatus();
                    emit replotSignal();
                }
            }
            else
                emit responseSignal(query,response);
            queryStack.pop_front();
        }
        responseBuffer.remove(0,idx+2);
        idx = responseBuffer.indexOf("\r\n");
    }
}

bool RPClient::writeCommand(const QString & cmd)
{
    if( ! isOpen() )
        return false;
    QByteArray array(cmd.toLatin1());
    array += "\r\n";
    if( cmd.contains('?') )
        queryStack.push_back(cmd);
    return (write(array) == array.size());
}

bool RPClient::getLedState(int led)
{
    if( (led < 0) || (led > 7) )
        return false;
    return writeCommand(QString("DIG:PIN? LED%1").arg(led));
}

bool RPClient::getLedState(void)
{
    for(int i = 0; i < 8; i++)
    {
        if( ! getLedState(i) )
            return false;
    }
    return true;
}

bool RPClient::setLedState(int led, bool state)
{
    if( (led < 0) || (led > 7) )
        return false;
    return writeCommand(QString("DIG:PIN LED%1,%2").arg(led).arg(int(state)));
}

bool RPClient::resetGenerator(void)
{
    return writeCommand("GEN:RST");
}

bool RPClient::getGeneratorChannelState(int channel)
{
    if( (channel < 1) || (channel > 2) )
        return false;
    return writeCommand(QString("OUTPUT%1:STATE?").arg(channel));
}

bool RPClient::setGeneratorChannelState(int channel, bool state)
{
    if( (channel < 1) || (channel > 2) )
        return false;
    QString cmd = QString("OUTPUT%1:STATE ").arg(channel);
    if( state )
        cmd += "ON";
    else
        cmd += "OFF";
    return writeCommand(cmd);
}

bool RPClient::getGeneratorFunction(int channel)
{
    if( (channel < 1) || (channel > 2) )
        return false;
    return writeCommand(QString("SOUR%1:FUNC?").arg(channel));
}


bool RPClient::setGeneratorFunction(int channel, const QString & func)
{
    if( (channel < 1) || (channel > 2) )
        return false;
    return writeCommand(QString("SOUR%1:FUNC %2").arg(channel).arg(func));
}

bool RPClient::getGeneratorFrequency(int channel)
{
    if( (channel < 1) || (channel > 2) )
        return false;
    return writeCommand(QString("SOUR%1:FREQ:FIX?").arg(channel));
}

bool RPClient::setGeneratorFrequency(int channel, const QString & freq)
{
    if( (channel < 1) || (channel > 2) )
        return false;
    return writeCommand(QString("SOUR%1:FREQ:FIX %2").arg(channel).arg(freq));
}

bool RPClient::getGeneratorAmplitude(int channel)
{
    if( (channel < 1) || (channel > 2) )
        return false;
    return writeCommand(QString("SOUR%1:VOLT?").arg(channel));
}

bool RPClient::setGeneratorAmplitude(int channel, const QString & ampl)
{
    if( (channel < 1) || (channel > 2) )
        return false;
    return writeCommand(QString("SOUR%1:VOLT %2").arg(channel).arg(ampl));
}

bool RPClient::getGeneratorPhase(int channel)
{
    if( (channel < 1) || (channel > 2) )
        return false;
    return writeCommand(QString("SOUR%1:PHAS?").arg(channel));
}

bool RPClient::setGeneratorPhase(int channel, const QString & phase)
{
    if( (channel < 1) || (channel > 2) )
        return false;
    return writeCommand(QString("SOUR%1:PHAS %2").arg(channel).arg(phase));
}

bool RPClient::getGeneratorDutyCycle(int channel )
{
    if( (channel < 1) || (channel > 2) )
        return false;
    return writeCommand(QString("SOUR%1:DCYC?").arg(channel));
}

bool RPClient::setGeneratorDutyCycle(int channel, const QString & dcycle)
{
    if( (channel < 1) || (channel > 2) )
        return false;
    return writeCommand(QString("SOUR%1:DCYC %2").arg(channel).arg(dcycle));
}

bool RPClient::resetOscilloscope(void)
{
    if( ! writeCommand("ACQ:RST") )
        return false;
    if( ! writeCommand("ACQ:DEC 1") )
        return false;
    if( ! writeCommand("ACQ:DATA:UNITS RAW") )
        return false;
    return setTriggerLevel(0.5);
}

bool RPClient::setTriggerDelay(int delay)
{
    return writeCommand(QString("ACQ:TRIG:DLY %1").arg(delay));
}

bool RPClient::setTriggerLevel(int level)
{
    return writeCommand(QString("ACQ:TRIG:LEV %1").arg(level));
}

bool RPClient::startAcquisition(void)
{
    if( ! writeCommand("ACQ:START") )
        return false;
    acquisitionState = true;
    return getTriggerStatus();
}

bool RPClient::stopAcquisition(void)
{
    acquisitionState = false;
    return writeCommand("ACQ:STOP");
}

bool RPClient::setTriggerSource(const char *source)
{
    return writeCommand(QString("ACQ:TRIG %1").arg(source));
}

bool RPClient::getTriggerStatus(void)
{
    return (acquisitionState) ? writeCommand("ACQ:TRIG:STAT?") : false;
}

int RPClient::sendChannelDataRequest(void)
{
    int cnt = 0;
    for(int channel = 1; channel <=2; channel++)
    {
        if( channelEnabled(channel) )
            if( sendChannelDataRequest(channel) )
                cnt++;
    }
    return cnt;
}

bool RPClient::sendChannelDataRequest(int channel)
{
    return writeCommand(QString("ACQ:SOUR%1:DATA?").arg(channel));
}

int RPClient::decodeChannelDataResponse(QByteArray & response, QVector<double> & data )
{
    if( response.startsWith("{") && response.endsWith("}") )
    {
        response.remove(1,1);
        response.remove(response.length()-1,1);
        QList<QByteArray> responseList = response.split(',');
        if( data.size() != responseList.size() )
            data.resize(responseList.size());
        for(int i = 0; i < responseList.size(); i++)
            data[i] = responseList[i].toInt();
        return data.size();
    }
    return 0;
}
