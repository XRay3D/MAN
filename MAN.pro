#-------------------------------------------------
#
# Project created by QtCreator 2016-07-05T13:05:30
#
#-------------------------------------------------

QT       += core gui serialport sql widgets charts

include(../../CommonInterfaces/CommonInterfaces.pri)
include(../../Meters/Agilent34401/Agilent34401.pri)
include(../ElemerDevice/ElemerDevice.pri)

DESTDIR = $$_PRO_FILE_PWD_/bin


TARGET = MAN
TEMPLATE = app

#CONFIG += c++17

QMAKE_CXXFLAGS += /std:c++latest
QMAKE_CXXFLAGS += /await
DEFINES += __cpp_lib_coroutine
DEFINES += EL_LOG

INCLUDEPATH += ../../magic_get/include
INCLUDEPATH += C:/local/boost_1_71_0
#INCLUDEPATH += ../Elemer_ASCII/

SOURCES += \
    chartview.cpp \
    hwi/interface.cpp \
    hwi/man.cpp \
    main.cpp\
    mainwindow.cpp \
#    trans.cpp \
#    trans_database.cpp\
    measuremodel.cpp \
    testedmodel.cpp \
    transmodel.cpp


HEADERS += \
    chartview.h \
    hwi/common_interfaces.h \
    hwi/interface.h \
    hwi/man.h \
    magicgetruntime.h \
    mainwindow.h \
#    trans.h \
#    trans_database.h \
    measuremodel.h \
    testedmodel.h \
    transmodel.h


FORMS += mainwindow.ui \
    trans_database.ui
