#-------------------------------------------------
#
# Project created by QtCreator 2015-04-11T19:18:36
#
#-------------------------------------------------

QT       += core
QT       += gui
QT       += network

CONFIG   += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = JChatClient
TEMPLATE = app


SOURCES += main.cpp \
    mainwindow.cpp \
    loginpage.cpp \
    chatpage.cpp

HEADERS  += \
    mainwindow.h \
    loginpage.h \
    chatpage.h

FORMS += \
    mainwindow.ui \
    loginpage.ui \
    chatpage.ui
