#include "redpitayacontroldialog.h"
#include <QApplication>

// Test

int main(int argc, char *argv[])
{
    QCoreApplication::setApplicationName("RedPitayaControl");
    QCoreApplication::setOrganizationName("JINR");
    QApplication a(argc, argv);
    RedPitayaControlDialog w;
    w.show();
    return a.exec();
}
