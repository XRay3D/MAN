#-------------------------------------------------
#
# Project created by QtCreator 2016-07-05T13:05:30
#
#-------------------------------------------------

QT       += core gui serialport sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MAN
TEMPLATE = app


SOURCES += main.cpp\
		mainwindow.cpp \
	man.cpp \
	agilent.cpp \
	elemer.cpp \
	trans.cpp \
    trans_database.cpp

HEADERS  += mainwindow.h \
	man.h \
	agilent.h \
	elemer.h \
	trans.h \
    trans_database.h

FORMS    += mainwindow.ui \
    trans_database.ui
