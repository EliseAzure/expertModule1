#-------------------------------------------------
#
# Project created by QtCreator 2015-02-22T17:58:10
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport xml

CONFIG += c++11

TARGET = expertModule1
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    saveload.cpp \
    model.cpp \
    qcustomplot.cpp \
    plot.cpp \
    term.cpp

HEADERS  += mainwindow.h \
    saveload.h \
    model.h \
    qcustomplot.h \
    plot.h \
    term.h

FORMS    += mainwindow.ui
