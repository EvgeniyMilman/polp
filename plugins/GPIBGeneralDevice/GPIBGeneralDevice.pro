#-------------------------------------------------
#
# Project created by QtCreator 2014-02-03T04:01:52
#
#-------------------------------------------------

QT       += widgets

TARGET = GPIBGeneralDevice
TEMPLATE = lib

DEFINES += GPIBGENERALDEVICE_LIBRARY

SOURCES += gpibgeneraldevice.cpp \
    gpibgeneralplugincollection.cpp \
    gpibdeviceview.cpp \
    gpibbus.cpp \
    osciloscope.cpp \
    hposciloscope.cpp \
    wavefactory.cpp \
    tmpesrexperiment.cpp

HEADERS += gpibgeneraldevice.h\
        gpibgeneraldevice_global.h \
    gpibgeneralplugincollection.h \
    gpibdeviceview.h \
    gpibbus.h \
    osciloscope.h \
    hposciloscope.h \
    wavefactory.h \
    tmpesrexperiment.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../polpcore/release/ -lpolpcore
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../polpcore/debug/ -lpolpcore
else:unix: LIBS += -L$$OUT_PWD/../../polpcore/ -lpolpcore
INCLUDEPATH += $$PWD/../../polpcore
DEPENDPATH += $$PWD/../../polpcore

macx {
    corelib.target = libpolpcore.1.dylib
    corelib.commands = install_name_tool -change libpolpcore.1.dylib  $$OUT_PWD/../../polpcore/libpolpcore.1.dylib $$OUT_PWD/libGPIBGeneralDevice.1.0.0.dylib;
QMAKE_POST_LINK+= $$corelib.commands
}

macx{
        thisplugun.target = libSMDFileFormat.1.0.0.dylib
        thisplugun.commands = cp $$OUT_PWD/libGPIBGeneralDevice.1.0.0.dylib  $$OUT_PWD/../../polpcontrol/plugins/;
        QMAKE_POST_LINK+= $$thisplugun.commands
}

FORMS += \
    GPIBDeviceView.ui \
    GPIBControlPane.ui \
    OsciloscopeControlPane.ui \
    wavefactory.ui \
    tmpesrexperiment.ui

win32{
    DESTDIR = ./../../polpcontrol/
}
win32{
    LIBS += -L'C://Program Files (x86)/IVI Foundation/VISA/WinNT/lib/msc/' -lvisa32
    INCLUDEPATH += 'C://Program Files (x86)/IVI Foundation/VISA/WinNT/include'
    DEPENDPATH += 'C://Program Files (x86)/IVI Foundation/VISA/WinNT/include'

    PRE_TARGETDEPS += 'C://Program Files (x86)/IVI Foundation/VISA/WinNT/lib/msc/visa32.lib'
}
