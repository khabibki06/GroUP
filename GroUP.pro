#-------------------------------------------------
#
# Project created by QtCreator 2019-04-18T16:47:19
#
#-------------------------------------------------

QT       += core gui
QT       += printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GroUP
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
        group.cpp \
    formplot.cpp \
    qcustomplot.cpp \
    packmolgui.cpp \
    gromacsprep.cpp \
    gromacsup.cpp \
    gromacsanalysis.cpp \
    formsetting.cpp \
    formversion.cpp \
    topologygenerator.cpp \
    texteditor.cpp \
    source/dataplot.cpp

HEADERS  += group.h \
    formplot.h \
    qcustomplot.h \
    packmolgui.h \
    gromacsprep.h \
    gromacsup.h \
    gromacsanalysis.h \
    formsetting.h \
    formversion.h \
    topologygenerator.h \
    texteditor.h \
    source/dataplot.h

FORMS    += group.ui \
    formplot.ui \
    packmolgui.ui \
    gromacsprep.ui \
    gromacsup.ui \
    gromacsanalysis.ui \
    formsetting.ui \
    formversion.ui \
    topologygenerator.ui \
    texteditor.ui

win32:RC_ICONS += group.ico

