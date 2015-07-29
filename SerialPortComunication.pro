#-------------------------------------------------
#
# Project created by QtCreator 2015-06-02T20:21:27
#
#-------------------------------------------------

QT       += core gui
QT       += serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SerialPortComunication
TEMPLATE = app


SOURCES += main.cpp\
        MainWindow.cpp \
    SerialPortThread.cpp \
    InputPlainTextEdit.cpp

HEADERS  += MainWindow.h \
    SerialPortThread.h \
    InputPlainTextEdit.h

FORMS    += mainwindow.ui
RC_FILE = icon.rc

RESOURCES += \
    icon.qrc
