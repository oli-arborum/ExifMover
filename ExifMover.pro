#-------------------------------------------------
#
# Project created by QtCreator 2012-03-24T18:52:56
#
#-------------------------------------------------

QT       += core gui

TARGET = ExifMover
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

unix:!symbian|win32: LIBS += -lexif
