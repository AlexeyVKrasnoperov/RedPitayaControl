#ifndef GENERATORGROUPBOX_H
#define GENERATORGROUPBOX_H

#include <QGroupBox>

namespace Ui {
class GeneratorGroupBox;
}

class RPClient;

class GeneratorGroupBox : public QGroupBox
{
    Q_OBJECT

public:
    explicit GeneratorGroupBox(QWidget *parent = 0);
    ~GeneratorGroupBox();    
    void setRPClient(RPClient *client = 0);
private slots:
    void on_comboBoxChannel_currentIndexChanged(int index);

    void on_checkBoxChannelEnabled_clicked(bool checked);

    void on_comboBoxForm_currentIndexChanged(const QString &arg1);

    void on_spinBoxFreq_valueChanged(int arg1);

    void on_doubleSpinBoxAmpl_valueChanged(double arg1);

    void on_spinBoxPhase_valueChanged(int arg1);

    void on_spinBoxDutyCycle_valueChanged(int arg1);

    void responseSlot(const QString & cmd, const QByteArray & response);

private:
    Ui::GeneratorGroupBox *ui;
    RPClient *rpClient;
    void getChannelSettings(int channel);

};

#endif // GENERATORGROUPBOX_H
