#-------------------------------------------------
#
# Project created by QtCreator 2014-01-29T21:46:42
#
#-------------------------------------------------

QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ISEFileFormat
TEMPLATE = lib

DEFINES += ISEFILEFORMAT_LIBRARY

SOURCES += isefileformat.cpp

HEADERS += isefileformat.h\
        isefileformat_global.h \
    isefileformat_global.h

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
    corelib.commands = install_name_tool -change libpolpcore.1.dylib  $$OUT_PWD/../../polpcore/libpolpcore.1.dylib $$OUT_PWD/libISEFileFormat.1.0.0.dylib;
QMAKE_POST_LINK+= $$corelib.commands
}

macx{
        thisplugun.target = libISEFileFormat.1.0.0.dylib
        thisplugun.commands = cp $$OUT_PWD/libISEFileFormat.1.0.0.dylib  $$OUT_PWD/../../polpcontrol/plugins/;
        QMAKE_POST_LINK+= $$thisplugun.commands
}

win32{
    DESTDIR = $$OUT_PWD/../../polpcontrol/
}
