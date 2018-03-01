#ifndef RPCLIENT_H
#define RPCLIENT_H

#include <QTcpSocket>
#include <QThread>
#include <QMutex>

class RPDataThread;

class RPClient: public QTcpSocket
{
    Q_OBJECT
protected:
    RPDataThread *dataThread;
    QString hostname;
    int  timeout;    
    bool readResponse(QByteArray & response);
protected slots:
    bool askTriggerStatus(void);
    bool writeCommand(const QString & cmd);
public:
    RPClient(const QString & name, QObject *parent = Q_NULLPTR);
    virtual ~RPClient(void);
    void setTimeout(int t)
    {
        timeout = t;
    }
    bool connect(void);
    const QString & getHostName(void)
    {
        return hostname;
    }
    bool getLedState(int led, unsigned short & state);
    bool getLedState(unsigned short & states);
    bool setLedState(int led, bool state);
    //
    bool resetGenerator(void);
    bool getGeneratorChannelState(int channel, unsigned short & state);
    bool setGeneratorChannelState(int channel, bool state);
    bool getGeneratorFunction(int channel, QString &func);
    bool setGeneratorFunction(int channel, const QString &func);
    bool getGeneratorFrequency(int channel, QString &freq);
    bool setGeneratorFrequency(int channel, const QString &freq);
    bool getGeneratorAmplitude(int channel, QString &ampl);
    bool setGeneratorAmplitude(int channel, const QString &ampl);
    bool getGeneratorPhase(int channel, QString &phase);
    bool setGeneratorPhase(int channel, const QString &phase);
    bool getGeneratorDutyCycle(int channel, QString &dcycle);
    bool setGeneratorDutyCycle(int channel, const QString &dcycle);
    //
    bool resetOscilloscope(void);
    bool setTriggerDelay(int delay);
    bool setTriggerLevel(int level);
    bool setTriggerDecimation(int decimation);
    bool setTriggerGain(int channel, bool value);
    bool setTriggerSource(const char *source);
    bool startAcquisition(void);
    bool stopAcquisition(void);
signals:
    void updateChannelData(int channel, const QVector<double> & data);
};

#endif // RPCLIENT_H

//class RPDataThread : public QThread
//{
//    Q_OBJECT
//protected:
//    bool breakCycle;
//    RPClient *client;
//public:
//    RPDataThread(RPClient *_client,QObject *parent = nullptr);
//    virtual ~RPDataThread(void);
//};

