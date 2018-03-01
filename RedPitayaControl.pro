#-------------------------------------------------
#
# Project created by QtCreator 2018-02-20T19:04:22
#
#-------------------------------------------------

QT       += core gui network

INCLUDEPATH += /usr/include/qwt
LIBS += -lqwt-qt5

#INCLUDEPATH += /usr/local/qwt-6.1.3/include
#LIBS += -L /usr/local/qwt-6.1.3/lib -lqwt



greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = RedPitayaControl
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        redpitayacontroldialog.cpp \
    rpclient.cpp \
    generatorgroupbox.cpp \
    oscillographgroupbox.cpp \
    channelviewer.cpp \
    utils.cxx \
    ledcontrolgroupbox.cpp

HEADERS  += redpitayacontroldialog.h \
    rpclient.h \
    generatorgroupbox.h \
    oscillographgroupbox.h \
    channelviewer.h \
    typedef.h \
    utils.h \
    ledcontrolgroupbox.h

FORMS    += redpitayacontroldialog.ui \ 
    generatorgroupbox.ui \
    oscillographgroupbox.ui \
    ledcontrolgroupbox.ui

RESOURCES += \
    resource.qrc
