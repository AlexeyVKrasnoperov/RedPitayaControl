#include "redpitayacontroldialog.h"
#include <QApplication>
#include <QPushButton>

// Test double - 2


int main(int argc, char *argv[])
{
    QCoreApplication::setApplicationName("RedPitayaControl");
    QCoreApplication::setOrganizationName("JINR");
    QApplication a(argc, argv);
    qRegisterMetaType<QVector<double>>("QVector<double>");
    RedPitayaControlDialog w;
    w.show();
    return a.exec();
}
