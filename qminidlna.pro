#-------------------------------------------------
#
# Project created by QtCreator 2018-01-14T22:50:56
#
#-------------------------------------------------

QT       += core gui
QT       += network xml xmlpatterns
QT += svg
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

#PKGCONFIG += openssl

LIBS += -L/usr/lib/ssl -lcrypto

#LIBS += -L./lib/qled -L./lib/build-qledplugin-Qt_5_9_1_in_PATH_qt5-Debug -lqled
#libqledplugin

TARGET = qminidlna
TEMPLATE = app
VERSION = "1.0"

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
    src/server/restserver.cpp \
    src/server/serverrequest.cpp \
    src/core/configurationfile.cpp \
    src/core/minidlna_process.cpp \
    src/core/pidthread.cpp \
    src/core/model/mediafolder.cpp \
    src/server/interface/basicrestresource.cpp \
    src/server/interface/restinterfaces.cpp \
    src/server/interface/restmediafolder.cpp \
    src/server/interface/restminidlna.cpp \
    src/server/interface/restresource.cpp \
    src/view/abstractsettings.cpp \
    src/view/foldereditdialog.cpp \
    src/view/generatecertificatedialog.cpp \
    src/view/mainwidget.cpp \
    src/view/mediafolderswidget.cpp \
    src/main.cpp \
    src/view/qminidlna.cpp \
    src/server/restconnection.cpp \
    src/server/certificategenerator.cpp \
    src/libs/qled/qled.cpp \
    src/view/settingdialog.cpp \
    src/view/settinggeneral.cpp \
    src/view/settingminidlna.cpp \
    src/view/settingmediafolder.cpp \
    src/view/settingconffile.cpp \
    src/view/settingserver.cpp

HEADERS += \
    src/server/restserver.h \
    src/server/serverrequest.h \
    src/core/configurationfile.h \
    src/core/minidlna_process.h \
    src/core/pidthread.h \
    src/core/model/mediafolder.h \
    src/server/interface/basicrestresource.h \
    src/server/interface/restinterfaces.h \
    src/server/interface/restmediafolder.h \
    src/server/interface/restminidlna.h \
    src/server/interface/restresource.h \
    src/view/abstractsettings.h \
    src/view/foldereditdialog.h \
    src/view/generatecertificatedialog.h \
    src/view/mainwidget.h \
    src/view/mediafolderswidget.h \
    src/view/qminidlna.h \
    src/server/restconnection.h \
    src/server/certificategenerator.h \
    src/libs/qled/qled.h \
    src/view/settingdialog.h \
    src/view/settinggeneral.h \
    src/view/settingminidlna.h \
    src/view/settingmediafolder.h \
    src/view/settingconffile.h \
    src/view/settingserver.h

FORMS += \
    src/view/settingdialog.ui \
    src/view/settinggeneral.ui \
    src/view/settingminidlna.ui \
    src/view/settingmediafolder.ui \
    src/view/settingconffile.ui \
    src/view/settingserver.ui

RESOURCES += \
    src/resource.qrc \
    src/libs/qled/qled.qrc

DISTFILES += \
    src/qminidlna.desktop \
    src/resource/qminidlna.png
