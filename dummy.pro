#-------------------------------------------------
#
# Project created by QtCreator 2015-12-09T00:38:29
#
#-------------------------------------------------

QT       += core gui
QT += webenginewidgets
QT += webengine

# QtWebEngine - Ubuntu package is qml-module-qtwebengine
# Library is at /usr/lib/x86_64-linux-gnu/qt5/qml/QtWebEngine on my system

LIBS += -L /usr/lib/x86_64-linux-gnu/qt5/qml/QtWebEngine


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = dummyx
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    oauth2.cpp \
    logindialog.cpp

HEADERS  += mainwindow.h \
    oauth2.h \
    logindialog.h

FORMS    += mainwindow.ui \
    logindialog.ui
