#include "redpitayacontroldialog.h"
#include "ui_redpitayacontroldialog.h"

#include "rpclient.h"
#include "generatorgroupbox.h"
#include "oscillographgroupbox.h"
#include "ledcontrolgroupbox.h"
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
    ledControl = new LedControlGroupBox(this);
    ui->verticalLayoutControl->insertWidget(2,ledControl);
    //
    oscillograph = new OscillographGroupBox(this);
    ui->horizontalLayoutMain->insertWidget(0,oscillograph);
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
        ledControl->setRPClient(0);
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
    //
    ledControl->setRPClient(rpClient);
    generator->setRPClient(rpClient);
    oscillograph->setRPClient(rpClient);
}

