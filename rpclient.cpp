#include "rpclient.h"
#include <unistd.h>

RPClient::RPClient(const QString & name, QObject *parent):QTcpSocket(parent),
    hostname(name)
{
    timeout = 500;
}

RPClient::~RPClient()
{
    if( isOpen() )
    {
        resetGenerator();
        close();
    }
}

bool RPClient::connect(void)
{
    connectToHost(hostname,5000);
    return waitForConnected(timeout);
}

bool RPClient::writeCommand(const QString & cmd)
{
    if( ! isOpen() )
        return false;
    QByteArray array(cmd.toLatin1());
    array += "\r\n";
    write(array);
    return waitForBytesWritten(timeout);
}

bool RPClient::readResponse(QByteArray & response)
{
    response.clear();
    if( ! isOpen() )
        return false;
    for( int cnt = 0; cnt < timeout; cnt++ )
    {
        if( ! waitForReadyRead(1) )
            continue;
        response += readAll();
        if( response.endsWith("\r\n") )
            break;
    }
    if( response.isEmpty() )
        return false;
    qDebug() << response;
    if( response.endsWith("\r\n") )
        response.remove(1,2);
    return !response.isEmpty();
}


bool RPClient::getLedState(int led, unsigned short & state)
{
    state = 0;
    if( (led < 0) || (led > 7) )
        return false;
    if( ! writeCommand(QString("DIG:PIN? LED%1").arg(led)) )
        return false;
    QByteArray response;
    if( ! readResponse(response) )
        return false;
    if( response.size() != 1 )
        return false;
    state = response.toShort();
    return (state <=1);
}

bool RPClient::getLedState(unsigned short & states)
{
    states = 0;
    for(int i = 0; i < 8; i++)
    {
        unsigned short state = 0;
        if( ! getLedState(i,state) )
            return false;
        if( state == 1 )
            states |= (0x01 << i);
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

bool RPClient::getGeneratorChannelState(int channel, unsigned short & state)
{
    state = 0;
    if( (channel < 1) || (channel > 2) )
        return false;
    if( ! writeCommand(QString("OUTPUT%1:STATE?").arg(channel)) )
        return false;
    QByteArray response;
    if( ! readResponse(response) )
        return false;
    if( response.size() != 1 )
        return false;
    state = response.toShort();
    return (state <=1);
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

bool RPClient::getGeneratorFunction(int channel, QString &func)
{
    func.clear();
    if( (channel < 1) || (channel > 2) )
        return false;
    if( ! writeCommand(QString("SOUR%1:FUNC?").arg(channel)) )
        return false;
    QByteArray response;
    if( ! readResponse(response) )
        return false;
    func = response;
    qDebug() << func;
    return !func.isEmpty();
}


bool RPClient::setGeneratorFunction(int channel, const QString & func)
{
    if( (channel < 1) || (channel > 2) )
        return false;
    return writeCommand(QString("SOUR%1:FUNC %2").arg(channel).arg(func));
}

bool RPClient::getGeneratorFrequency(int channel, QString &freq)
{
    if( (channel < 1) || (channel > 2) )
        return false;
    if( ! writeCommand(QString("SOUR%1:FREQ:FIX?").arg(channel)) )
        return false;
    QByteArray response;
    if( ! readResponse(response) )
        return false;
    freq = response;
    qDebug() << freq;
    return !freq.isEmpty();
}

bool RPClient::setGeneratorFrequency(int channel, const QString & freq)
{
    if( (channel < 1) || (channel > 2) )
        return false;
    return writeCommand(QString("SOUR%1:FREQ:FIX %2").arg(channel).arg(freq));
}

bool RPClient::getGeneratorAmplitude(int channel, QString &ampl)
{
    if( (channel < 1) || (channel > 2) )
        return false;
    if( ! writeCommand(QString("SOUR%1:VOLT?").arg(channel)) )
        return false;
    QByteArray response;
    if( ! readResponse(response) )
        return false;
    ampl = response;
    qDebug() << ampl;
    return !ampl.isEmpty();
}

bool RPClient::setGeneratorAmplitude(int channel, const QString & ampl)
{
    if( (channel < 1) || (channel > 2) )
        return false;
    return writeCommand(QString("SOUR%1:VOLT %2").arg(channel).arg(ampl));
}

bool RPClient::getGeneratorPhase(int channel, QString &phase)
{
    if( (channel < 1) || (channel > 2) )
        return false;
    if( ! writeCommand(QString("SOUR%1:PHAS?").arg(channel)) )
        return false;
    QByteArray response;
    if( ! readResponse(response) )
        return false;
    phase = response;
    qDebug() << phase;
    return !phase.isEmpty();
}

bool RPClient::setGeneratorPhase(int channel, const QString & phase)
{
    if( (channel < 1) || (channel > 2) )
        return false;
    return writeCommand(QString("SOUR%1:PHAS %2").arg(channel).arg(phase));
}

bool RPClient::getGeneratorDutyCycle(int channel, QString &dcycle)
{
    if( (channel < 1) || (channel > 2) )
        return false;
    if( ! writeCommand(QString("SOUR%1:DCYC?").arg(channel)) )
        return false;
    QByteArray response;
    if( ! readResponse(response) )
        return false;
    dcycle = response;
    qDebug() << dcycle;
    return !dcycle.isEmpty();
}

bool RPClient::setGeneratorDutyCycle(int channel, const QString & dcycle)
{
    if( (channel < 1) || (channel > 2) )
        return false;
    return writeCommand(QString("SOUR%1:DCYC %2").arg(channel).arg(dcycle));
}

