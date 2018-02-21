#ifndef OSCILLOGRAPHGROUPBOX_H
#define OSCILLOGRAPHGROUPBOX_H

#include <QGroupBox>
#include <QTimer>

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
    void timeoutSlot(void);
private:
    Ui::OscillographGroupBox *ui;
    ChannelViewer *viewer;
    RPClient *rpClient;
    QTimer timer;
};

#endif // OSCILLOGRAPHGROUPBOX_H
