#-------------------------------------------------
#
# Project created by QtCreator 2014-01-25T21:10:50
#
#-------------------------------------------------

QT       += core gui widgets printsupport

TARGET = polpcore
TEMPLATE = lib

DEFINES += POLPCORE_LIBRARY

unix {
    target.path = /usr/lib
    INSTALLS += target
}

HEADERS += \
    data.h \
    plugins.h \
    simpleanalyser.h \
    simpleview.h \
    simpledevice.h\
    pluginmanager.h \
    project.h \
    projectmanager.h \
    simulationcontrolpane.h

SOURCES += \
    data.cpp \
    simpleanalyser.cpp \
    simpleview.cpp \
    simpledevice.cpp\
    pluginmanager.cpp \
    project.cpp \
    projectmanager.cpp \
    simulationcontrolpane.cpp

FORMS += \
    simpleviewform.ui \
    simulationcontrolpane.ui

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../qcustomplot/release/ -lqcustomplotd1
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../qcustomplot/debug/ -lqcustomplotd1
else:unix: LIBS += -L$$OUT_PWD/../qcustomplot/ -lqcustomplotd

INCLUDEPATH += $$PWD/../qcustomplot
DEPENDPATH += $$PWD/../qcustomplot


macx {
    corelib.target = libqcustomplotd.1.dylib
    corelib.commands = install_name_tool -change libqcustomplotd.1.dylib  $$OUT_PWD/../qcustomplot/libqcustomplotd.1.dylib $$OUT_PWD/libpolpcore.1.0.0.dylib
    QMAKE_POST_LINK+= $$corelib.commands
}

win32{
   # DESTDIR  = OUT_PWD/../../polpcontrol/debug
}
