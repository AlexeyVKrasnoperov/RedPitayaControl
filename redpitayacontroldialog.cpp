#include "redpitayacontroldialog.h"
#include "ui_redpitayacontroldialog.h"

#include "rpclient.h"
#include "generatorgroupbox.h"
#include "oscillographgroupbox.h"
#include <QRadioButton>


RedPitayaControlDialog::RedPitayaControlDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RedPitayaControlDialog)
{
    ui->setupUi(this);
    rpClient  = 0;
    generator = new GeneratorGroupBox(this);
    ui->verticalLayoutControl->insertWidget(1,generator);
    //
    oscillograph = new OscillographGroupBox(this);
    ui->horizontalLayoutMain->insertWidget(0,oscillograph);
    //
    ledButtons.setExclusive(false);
    for(int i = 0; i < 8; i++)
    {
        QRadioButton *b = new QRadioButton(QString("LED%1").arg(i),ui->groupBoxLed);
        ui->gridLayoutLed->addWidget(b,i/4,i%4);
        ledButtons.addButton(b,i);
    }
    connect(&ledButtons,SIGNAL(buttonClicked(int)),this,SLOT(ledButtonClickedSlot(int)));
    //
    updateStatus();
    oscillograph->setFocus();
}

RedPitayaControlDialog::~RedPitayaControlDialog()
{
    delete ui;
}

void RedPitayaControlDialog::closeConnection()
{
    if( rpClient != 0 )
    {
        delete rpClient;
        rpClient = 0;
    }
    updateStatus();
}

void RedPitayaControlDialog::updateStatus(void)
{
    if( rpClient == 0 )
    {
        setWindowTitle( tr("Red Pitaya Control") );
        foreach (QAbstractButton *b, ledButtons.buttons()) {
            b->setChecked(false);
        }
        ui->groupBoxLed->setDisabled(true);
        generator->setRPClient(0);
        oscillograph->setRPClient(0);
    }
    else
        setWindowTitle( tr("Red Pitaya Control (%1)").arg(rpClient->getHostName()) );
}

void RedPitayaControlDialog::on_comboBoxHostName_currentIndexChanged(const QString &arg1)
{
    if( arg1.isEmpty() )
    {
        closeConnection();
        return;
    }
    rpClient = new RPClient(arg1);
    if( ! rpClient->connect() )
    {
        closeConnection();
        ui->comboBoxHostName->setCurrentIndex(0);
        return;
    }    
    updateStatus();
    // set initial led state
    unsigned short states = 0;
    if( rpClient->getLedState(states) )
    {
        ui->groupBoxLed->setDisabled(false);
        foreach (QAbstractButton *b, ledButtons.buttons())
            b->setChecked((states >> ledButtons.id(b)) & 0x01);
    }
    //
    generator->setRPClient(rpClient);
    oscillograph->setRPClient(rpClient);
}

void RedPitayaControlDialog::ledButtonClickedSlot(int id)
{
    if( rpClient != 0 )
    {
        QAbstractButton *b = ledButtons.button(id);
        bool isChecked = b->isChecked();
        if( ! rpClient->setLedState(id,isChecked) )
            b->setChecked(!isChecked);
    }
}
