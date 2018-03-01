#include "ledcontrolgroupbox.h"
#include "ui_ledcontrolgroupbox.h"
#include <QRadioButton>
#include <rpclient.h>

LedControlGroupBox::LedControlGroupBox(QWidget *parent) :
    QGroupBox(parent),
    ui(new Ui::LedControlGroupBox)
{
    ui->setupUi(this);
    ledButtons.setExclusive(false);
    for(int i = 0; i < 8; i++)
    {
        QRadioButton *b = new QRadioButton(QString("LED%1").arg(i),this);
        ui->gridLayout->addWidget(b,i/4,i%4);
        ledButtons.addButton(b,i);
    }
    connect(&ledButtons,SIGNAL(buttonClicked(int)),this,SLOT(ledButtonClickedSlot(int)));
}

LedControlGroupBox::~LedControlGroupBox()
{
    delete ui;
}

void LedControlGroupBox::setRPClient(RPClient *client)
{
    rpClient = client;
    setEnabled(rpClient != 0);
    if( rpClient == 0 )
    {
        foreach (QAbstractButton *b, ledButtons.buttons()) {
            b->setChecked(false);
        }
        return;
    }
    connect(rpClient,SIGNAL(responseSignal(QString,QByteArray)),this,SLOT(responseSlot(QString,QByteArray)));
    // ask initial led state
    rpClient->getLedState();
}

void LedControlGroupBox::ledButtonClickedSlot(int id)
{
    if( rpClient != 0 )
    {
        QAbstractButton *b = ledButtons.button(id);
        bool isChecked = b->isChecked();
        if( ! rpClient->setLedState(id,isChecked) )
            b->setChecked(!isChecked);
    }
}

void LedControlGroupBox::responseSlot(const QString & cmd, const QByteArray & response)
{
    if( ! cmd.contains("DIG:PIN? LED") )
        return;
    bool ok = false;
    int id = cmd.right(1).toInt(&ok);
    if( ! ok )
        return;
    if( id >= 0 && id <= 7)
    {
        QAbstractButton *b = ledButtons.button(id);
        if( b != 0 )
            b->setChecked(response.toInt() == 1);
    }
}
