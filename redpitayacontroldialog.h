#ifndef REDPITAYACONTROLDIALOG_H
#define REDPITAYACONTROLDIALOG_H

#include <QDialog>
#include <QButtonGroup>
#include <QVector>

namespace Ui {
class RedPitayaControlDialog;
}

class RPClient;
class QRadioButton;
class QButtonGroup;
class GeneratorGroupBox;
class OscillographGroupBox;
class QPushButton;

class RedPitayaControlDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RedPitayaControlDialog(QWidget *parent = 0);
    ~RedPitayaControlDialog();

private slots:

    void on_comboBoxHostName_currentIndexChanged(const QString &arg1);

    void ledButtonClickedSlot(int id);

private:
    Ui::RedPitayaControlDialog *ui;
    RPClient *rpClient;
    GeneratorGroupBox *generator;
    OscillographGroupBox *oscillograph;
    void updateStatus(void);
    void closeConnection(void);
    QButtonGroup ledButtons;
};


#endif // REDPITAYACONTROLDIALOG_H
