#-------------------------------------------------
#
# Project created by QtCreator 2014-02-22T00:43:31
#
#-------------------------------------------------

QT       += widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += printsupport

TARGET = NMRAnalyser
TEMPLATE = lib

DEFINES += NMRANALYSER_LIBRARY

SOURCES += nmranalyser.cpp

HEADERS += nmranalyser.h\
        nmranalyser_global.h

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
    corelib.commands = install_name_tool -change libpolpcore.1.dylib  $$OUT_PWD/../../polpcore/libpolpcore.1.dylib $$OUT_PWD/$(TARGET);
QMAKE_POST_LINK+= $$corelib.commands
}

macx{
        thisplugun.target = $(TARGET)
        thisplugun.commands = cp $$OUT_PWD/$$thisplugun.target   $$OUT_PWD/../../polpcontrol/plugins/;
        QMAKE_POST_LINK+= $$thisplugun.commands
}
