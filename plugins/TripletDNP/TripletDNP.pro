#-------------------------------------------------
#
# Project created by QtCreator 2014-02-22T01:33:17
#
#-------------------------------------------------

QT       += widgets

TARGET = TripletDNP
TEMPLATE = lib

DEFINES += TRIPLETDNP_LIBRARY

SOURCES += tripletdnp.cpp \
    frequencycalculator.cpp \
    concentrationcalculator.cpp

HEADERS += tripletdnp.h\
        tripletdnp_global.h \
    frequencycalculator.h \
    concentrationcalculator.h

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
