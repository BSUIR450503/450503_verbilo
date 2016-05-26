#-------------------------------------------------
#
# Project created by QtCreator 2015-12-01T19:01:48
#
#-------------------------------------------------

QT       += core gui multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MrChord
TEMPLATE = app


SOURCES += main.cpp\
    filemanage.cpp \
    mainwindow.cpp \
    addchord.cpp \
    delchord.cpp \
    tunerwindow.cpp \
    metronome.cpp

HEADERS  += \
    filemanage.h \
    mainwindow.h \
    addchord.h \
    delchord.h \
    tunerwindow.h \
    metronome.h

FORMS    += mainwindow.ui \
    addchord.ui \
    delchord.ui \
    tunerwindow.ui \
    metronome.ui
