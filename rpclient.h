#ifndef RPCLIENT_H
#define RPCLIENT_H

#include <QTcpSocket>

class RPClient : public QTcpSocket
{
    Q_OBJECT
protected:
    QString hostname;
    int  timeout;
    bool writeCommand(const QString & cmd);
    bool readResponse(QByteArray & response);
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
};

#endif // RPCLIENT_H
