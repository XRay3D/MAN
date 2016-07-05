#-------------------------------------------------
#
# Project created by QtCreator 2016-07-05T13:05:30
#
#-------------------------------------------------

QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MAN
TEMPLATE = app


SOURCES += main.cpp\
		mainwindow.cpp \
	man.cpp \
	agilent.cpp \
	elemer.cpp \
	trans.cpp

HEADERS  += mainwindow.h \
	man.h \
	agilent.h \
	elemer.h \
	trans.h

FORMS    += mainwindow.ui
