#-------------------------------------------------
#
# Project created by QtCreator 2015-12-09T00:38:29
#
#-------------------------------------------------

QT       += core gui
QT       += webkit
QT +=webkitwidgets
#QT += KWidgetsAddons
#QT += KIOCore
#QT += KIOFileWidgets
#QT += KNTLM
# in file called  kio/metainfo.yaml in https://github.com/KDE/kio/ it lists libraries for qmake
#libraries:
# - qmake: KIOCore
# - qmake: KIOFileWidgets
# - qmake: KIOWidgets
# - qmake: KNTLM
# Which is how I came up with the above 3 QT += commands
# After adding those three it finds KIO/SimpleJob but on taking them out is still finds it. So I dunno

#KWidgetsAddons supplies KMessageBox http://api.kde.org/frameworks-api/frameworks5-apidocs/kwidgetsaddons/html/index.html
#QT += KIO
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
