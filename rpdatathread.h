#ifndef RPDATATHREAD_H
#define RPDATATHREAD_H

#include <QThread>
#include <QVector>

class RPClient;

class RPDataThread : public QThread
{
    Q_OBJECT
protected:
    bool breakCycle;
    QByteArray array;
    RPClient *client;
    QVector<double> channelData[2];
public:
    RPDataThread(RPClient *_client, QObject *parent = nullptr);
    virtual ~RPDataThread(void);
    virtual void run(void);
    virtual void start(void);
    void stop(void);
    int FillDataArray(int channel, QByteArray & a);
protected slots:
    void readSlot(void);
signals:
    void askTriggerStatusSignal(void);
    void writeCommandSignal(const QString &cmd);
    void updateChannelData(int channel, const QVector<double> & data);
};

#endif // RPDATATHREAD_H
