#ifndef OSCILLOGRAPHGROUPBOX_H
#define OSCILLOGRAPHGROUPBOX_H

#include <QGroupBox>
#include <QTimer>
#include <QButtonGroup>

namespace Ui {
class OscillographGroupBox;
}

class ChannelViewer;
class RPClient;

class OscillographGroupBox : public QGroupBox
{
    Q_OBJECT

public:
    explicit OscillographGroupBox(QWidget *parent = 0);
    ~OscillographGroupBox();
    void setRPClient(RPClient *client = 0);
private slots:
    void on_pushButtonAcq_clicked(bool checked);
    void channelControlClicked(int);
    void updateChannelDataSlot(int channel, const QVector<double> & data);
private:
    Ui::OscillographGroupBox *ui;
    ChannelViewer *viewer;
    RPClient *rpClient;
    QButtonGroup channelControl;
};

#endif // OSCILLOGRAPHGROUPBOX_H
