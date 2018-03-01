#ifndef RPCLIENT_H
#define RPCLIENT_H

#include <QTcpSocket>
#include <QThread>
#include <QMutex>
#include <QList>
#include <QTimer>
#include "typedef.h"
#include <deque>
using namespace std;

class RPClient: public QTcpSocket
{
    Q_OBJECT
protected:    
    int channelMask;
    deque<QString> queryStack;
    QVector<double> dataBuffer;
    QByteArray  responseBuffer;
    QString hostname;
    bool writeCommand(const QString & cmd);
    bool acquisitionState;
    bool getTriggerStatus(void);
    int  sendChannelDataRequest(void);
    bool sendChannelDataRequest(int channel);
    int  decodeChannelDataResponse(QByteArray & response, QVector<double> & data );
protected slots:
    void readResponseSlot(void);
public:
    RPClient(const QString & name, QObject *parent = Q_NULLPTR);
    virtual ~RPClient(void);
    bool connect(int timeout = 500);
    const QString & getHostName(void)
    {
        return hostname;
    }
    bool getLedState(int led);
    bool getLedState(void);
    bool setLedState(int led, bool state);
    //
    bool resetGenerator(void);
    bool getGeneratorChannelState(int channel);
    bool setGeneratorChannelState(int channel, bool state);
    bool getGeneratorFunction(int channel);
    bool setGeneratorFunction(int channel, const QString &func);
    bool getGeneratorFrequency(int channel);
    bool setGeneratorFrequency(int channel, const QString &freq);
    bool getGeneratorAmplitude(int channel);
    bool setGeneratorAmplitude(int channel, const QString &ampl);
    bool getGeneratorPhase(int channel);
    bool setGeneratorPhase(int channel, const QString &phase);
    bool getGeneratorDutyCycle(int channel);
    bool setGeneratorDutyCycle(int channel, const QString &dcycle);
    //
    bool resetOscilloscope(void);
    bool setTriggerDelay(int delay);
    bool setTriggerLevel(int level);
    bool setTriggerSource(const char *source);
    bool startAcquisition(void);
    bool stopAcquisition(void);
    //
    void enableChannel(int channel, bool enable)
    {
        if( (channel < 1) || (channel > 2) )
            return;
        if( enable )
            SETBIT(channelMask,channel-1);
        else
            CLRBIT(channelMask,channel-1);
    }
    //
    bool channelEnabled(int channel)
    {
        if( (channel < 1) || (channel > 2) )
            return false;
        return TESTBIT(channelMask,channel-1);
    }
signals:
    void updateChannelData(int channel, const QVector<double> & data);
    void responseSignal(const QString & cmd, const QByteArray & response);
    void replotSignal(void);
};

#endif // RPCLIENT_H
