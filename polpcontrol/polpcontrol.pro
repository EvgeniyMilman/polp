#-------------------------------------------------
#
# Project created by QtCreator 2014-01-25T21:08:52
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = polpcontrol
TEMPLATE = app

CONFIG -= app_bundle

SOURCES += main.cpp\
        mainwindow.cpp \
    pluginmanager.cpp \
    project.cpp \
    projectmanager.cpp

HEADERS  += mainwindow.h \
    pluginmanager.h \
    project.h \
    projectmanager.h

FORMS    += mainwindow.ui

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../polpcore/release/ -lpolpcore
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../polpcore/debug/ -lpolpcore
else:unix: LIBS += -L$$OUT_PWD/../polpcore/ -lpolpcore

INCLUDEPATH += $$PWD/../polpcore
DEPENDPATH += $$PWD/../polpcore
